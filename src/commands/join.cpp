#include "Server.hpp"
#include <vector>

void	Server::is_valid_chan_name(const vector<string>& params, Client& client)
{
	if (params.empty()) {
		add_rply_from_server(":Not enough parameters", client, "JOIN", ERR_NEEDMOREPARAMS);
		throw invalid_argument("join: Not enough parameters");
	}
	if (params[0].size() > 50) {
		add_rply_from_server(":Channel name is too long", client, "JOIN", ERR_NAMETOOLONG);
		throw invalid_argument("join: channel name too long");
	}
}

void	Server::join( const vector<string>& params, Client& client)
{
	try 
	{
		// parse variable
		vector<string>	chan_names;
		string temp_str = params[0];
		little_split(chan_names, temp_str, ",");
		vector<string>	chan_pass;
		temp_str = params[1];
		little_split(chan_pass, temp_str, ",");

		cout << "+++++++++++++++++++++++" << endl;
		cout << params[0] << endl << endl;
		cout << params[1] << endl;
		cout << "+++++++++++++++++++++++" << endl;
		// iterate threw all argument
		str_iter it_pass = chan_pass.begin();
		for (str_iter it = chan_names.begin(); it != chan_names.end(); ++it) 
		{
			// create channel
			bool chan_exists = false;
			for ( chan_iter it2 = this->chan_vec.begin(); it2 != this->chan_vec.end(); ++it2) 
			{
				if (it2->getName() == *it) 
				{
					chan_exists = true;
					break ;
				}
			}
			if (!chan_exists)
			{
				if((*it)[0] != '#')
					*it = "#" + *it;	
				Channel new_channel =Channel(*it);
				this->chan_vec.push_back(new_channel);
				new_channel.addOper(client);
			}
			// get channel
			Channel *current_chan = NULL;
			for (chan_iter it2 = this->chan_vec.begin(); it2 != this->chan_vec.end(); ++it2) 
			{
				if (it2->getName() == *it) 
				{
					current_chan = &(*it2);
					break;
				}
			}
			// cout << "----------------CHANEL in JOIN--------------------" << endl;
			// cout << "is invite: " << current_chan->getIsInviteOnly() << endl;
			// cout << "is limite: " << current_chan->getIsLimit() << endl;
			// cout << "limite: " << current_chan->getLimit() << endl;
			// cout << "is Private: " << current_chan->getIsPrivate() << endl;
			// cout << "Password: " << current_chan->getPassword() << endl;
			// mode verif
			if (current_chan->getIsInviteOnly() == true && !current_chan->user_in_invite_list(client)) {
				cout << "------------------------------++++" << endl;
				for (vector<Socket>::iterator it = current_chan->operators.begin(); it != current_chan->operators.end(); ++it)
					cout << "in chanel " << *it << endl;
				cout << "current client" << client.getFd() << endl;
				cout << current_chan->getIsInviteOnly() << endl;
				cout << current_chan->user_in_invite_list(client) << endl;
				cout << "------------------------------++++" << endl;
				add_rply_from_server(":Cannot join channel (+i)", client, current_chan->getName(), ERR_INVITEONLYCHAN);
				throw invalid_argument("join: Cannot join channel (+i)");
			}
			if (current_chan->getIsLimit() == true && static_cast<int>(current_chan->getLimit()) >= static_cast<int>(current_chan->clients.size())) {
				add_rply_from_server(":Cannot join channel (+l)", client, current_chan->getName(), ERR_CHANNELISFULL);
				throw invalid_argument("join: Cannot join channel (+l)");
			}
			// join channel
			if (current_chan->user_in_invite_list(client) || !current_chan->getIsPrivate() || (it_pass != chan_pass.end() && *it_pass == current_chan->getPassword()))
			{
				current_chan->addClient(client);
				client.channelsMember.push_back(*it);
				notify_chan((*current_chan), *it, "JOIN", client);
			}
			else {
				cout << "IS PRIVATE: "<< current_chan->getIsPrivate() << endl;
				cout << "PASS WORD: "<< current_chan->getPassword() << endl;
				cout << "ENTR2E: "<< *it_pass << endl;
				add_rply_from_server(":Cannot join channel (+k)", client, current_chan->getName(), ERR_BADCHANNELKEY);
				throw invalid_argument("join: Cannot join channel (+k)");
			}
			//topic
			if (current_chan->getTopic() == "")
				add_rply_from_server(":No topic is set", client, current_chan->getName(), RPL_NOTOPIC);
			else
				add_rply_from_server(current_chan->getTopic(), client, (*current_chan), "", RPL_TOPIC);
			//remove invite
			for (vector<Socket>::iterator itList = current_chan->inviteList.begin(); itList != current_chan->inviteList.begin(); ++itList)
			{
				if (*itList == client.getFd())
					current_chan->removeInvite(client);
			}
			if (it_pass != chan_pass.end())
				it_pass++;
		}
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
	}
}
