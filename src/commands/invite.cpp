#include "Server.hpp"
#include <vector>

void	Server::invite( const vector<string>& params, Client& client)
{
	try 
	{
		cout << client.getNickname() << endl;
		cout << "here [0]" << endl;
		// ERR_NOSUCH
		bool is_channel = false;
		bool is_target = false;
		cout << "here [1]" << endl;
		for (vector<Channel>::iterator it = this->chan_vec.begin(); it != this->chan_vec.end(); ++it) {
			if (it->getName() == params[1])
				is_channel = true;
		}
		cout << "here [2]" << endl;
		for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); ++it) {
			if (it->second.getNickname() == params[0])
				is_target = true;
		}
		cout << "here [3]" << endl;
		if (is_channel == false) {
			add_rply_from_server(":No such channel", client, params[1], ERR_NOSUCHCHANNEL);
			throw invalid_argument("invite: No such channel");
		}
		cout << "here [4]" << endl;
		if (is_target == false) {
			add_rply_from_server(":No such nick/channel", client, params[0], ERR_NOSUCHNICK);
			throw invalid_argument(params[0] + ": No such nick/channel");
		}
		cout << "here [5]" << endl;
		// verif
		cout << "channel name: " << params[1] << endl;
		Channel &current_chan = find_channel(params[1],client);
		cout << "--------------INVITE VECTOR BEFORE-------------------" << endl;
		for (vector<Socket>::iterator it = current_chan.inviteList.begin(); it != current_chan.inviteList.end(); ++it)
			cout << "socket: " << *it << endl;
		bool is_target_on_channel = true;
		/*for (chan_iter it = this->chan_vec.begin(); it != this->chan_vec.end(); ++it) {
			if (it->getName() == params[1]) 
				current_chan = &(*it);
		}*/
		// cout << "----------------CHANEL in INVITE--------------------" << endl;
		// cout << "is invite: " << current_chan.getIsInviteOnly() << endl;
		// cout << "is limite: " << current_chan.getIsLimit() << endl;
		// cout << "limite: " << current_chan.getLimit() << endl;
		// cout << "is Private: " << current_chan.getIsPrivate() << endl;
		// cout << "Password: " << current_chan.getPassword() << endl;
		Client &target = find_user(params[0], client, "INVITE");
		cl_iter it = find(current_chan.clients.begin(), current_chan.clients.end(), target.getFd());
		if (it == current_chan.clients.end()) {
			is_target_on_channel = false;
		}
		if (!current_chan.user_in_chan(client)) {
			add_rply_from_server(":You're not on that channel", client, params[1], ERR_NOTONCHANNEL);
			throw invalid_argument(params[1] + " :You're not on that channel");
		}
		cout << "here [6]" << endl;
		if (is_target_on_channel) {
			add_rply_from_server(params[0] + " " + params[1] + ":is already on channel", client, params[0] + " " + params[1], ERR_USERONCHANNEL);
			throw invalid_argument(params[0] + " " + params[1] + " :is already on channel");
		}
		cout << "here [7]" << endl;
		if (current_chan.getIsInviteOnly() && current_chan.user_is_operator(client)) {
			add_rply_from_server(":You're not channel operator", client, params[1], ERR_CHANOPRIVSNEEDED);
			throw invalid_argument(params[1] + " :You're not channel operator");
		}
		cout << "here [8]" << endl;
		// invite
		bool is_already_in_invite_list = true;
		cl_iter it2 = find(current_chan.inviteList.begin(), current_chan.inviteList.end(), client.getFd());
			if (it2 == current_chan.inviteList.end()) {
				is_already_in_invite_list = false;
		}
		if (!is_already_in_invite_list)
			current_chan.addInvite(client);
		add_rply_from_server(params[0], client, params[1], RPL_INVITING);
		cout << "here [9]" << endl;
		cout << "--------------INVITE VECTOR BEFORE-------------------" << endl;
		for (vector<Socket>::iterator it = current_chan.inviteList.begin(); it != current_chan.inviteList.end(); ++it)
			cout << "socket: " << *it << endl;
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
	}
}
