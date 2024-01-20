#include "Server.hpp"
#include <ostream>
#include <sstream>

// FINISHED VERSION
void Server::process_input(Socket fd) {
    Client &client = (this->fd_map.find(fd))->second;
    ssize_t byte_count;
    string temp;

    temp.resize(SERVER_LIMITS_SIZE);
    byte_count = recv(fd, (void *)(temp.c_str()), SERVER_LIMITS_SIZE, MSG_DONTWAIT);
    if (byte_count == -1)
        throw runtime_error(string("recv: ") + strerror(errno));
    if (byte_count == 0) {
        this->_disconect_client(fd);
        return;
    }
    temp.resize(byte_count);
    client.cmd_buff += temp;

    if (client.cmd_buff.find("\r\n") == string::npos) {
        return;
    }
    cout << YELLOW << "str received = [" << client.cmd_buff << "]" << RESET << endl;
    cout << YELLOW << byte_count << " bytes RECEIVED" << RESET << endl;

    while (client.cmd_buff.find(endmsg) != string::npos) {
        string tok = client.cmd_buff.substr(0, client.cmd_buff.find(endmsg));
        client.cmd_buff.erase(0, client.cmd_buff.find(endmsg) + 2);

        try {
            if (!client.isRegistered) {
                string command = tok.substr(0, tok.find(" ")); // Extract the command

                if (!client.passwd_provided) {
                    // 20.01 : REVOIR PASS NON RECONNU
                    if (command == "PASS") {
                        parse_command(tok, client);
                        client.passwd_provided = true;
                        sendMessage(client, "Please provide NICK\r\n");
                    } else {
                        sendMessage(client, "Please provide PASS\r\n");
                        return;
                    }
                } else if (!client.nickname_provided) {
                    if (command == "NICK") {
                        parse_command(tok, client);
                        client.nickname_provided = true;
                        sendMessage(client, "Please provide USER\r\n");
                    } else {
                        sendMessage(client, "Please provide NICK\r\n");
                        return;
                    }
                } else if (!client.username_provided) {
                    if (command == "USER") {
                        parse_command(tok, client);
                        client.username_provided = true;
                    } else {
                        sendMessage(client, "Please provide USER\r\n");
                        return;
                    }
                }

                if (client.passwd_provided && client.nickname_provided && client.username_provided) {
                    client.isRegistered = true; // Register the client if all credentials are provided
                    // Handle client registration completion
                }
            } else {
                // Process other commands for registered clients
                parse_command(tok, client);
            }
        } catch (const NicknameInUse& e) {
            cout << RED << "Error: " << e.what() << RESET << endl;
            client.asTriedNickname = true;
            break;
        } catch (const PasswordIncorrect& e) {
            cout << RED << "Error: " << e.what() << RESET << endl;
            sendMessage(client, "Password is incorrect.\r\n");
            client.asTriedNickname = true;
            break;
        }
    }
    client.cmd_buff.clear();
    cout << YELLOW << "sending = [" << client.getBuff() << "]" << RESET << endl;
    flush_all_buffers();
}


void	Server::parse_command( string& input, Client& client ) {
	vector<string>	result = vector<string>();
	size_t			pos;
	string delimiter = " ";
	while ((pos = input.find(delimiter)) != string::npos) {
		result.push_back(input.substr(0, pos));
		input.erase(0, pos + delimiter.length());
	}
	result.push_back(input);
	string cmd = result[0];
	result.erase(result.begin());
	map<string, command_function>::iterator it = this->cmd_map.find(cmd);
	if (it == this->cmd_map.end()) {
		return ;
	}
	if (!client.isRegistered && cmd != "PASS" && cmd != "NICK" && cmd != "USER" && cmd != "QUIT")
	{
		add_rply_from_server(":You have not registered",client, "", ERR_NOTREGISTERED);
		return ;
	}
	(this->*(it->second))(result, client);
}

void Server::flush_all_buffers() {
	for ( client_iter it = this->fd_map.begin(); it != this->fd_map.end(); ++it ) {
		if (it->second.isLeaving)
			continue ;
        ssize_t byte_count = sendMessage(it->second, it->second.getBuff());
        if (byte_count == -1)
        {
			cout << RED << "fd is " << it->first << RESET << endl;
            throw runtime_error(string("send: ") + strerror(errno));
        }
        cout << YELLOW << byte_count << " bytes SENT" << RESET << endl << endl;
		it->second.clearBuff();
	}
}
