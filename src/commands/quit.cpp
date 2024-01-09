#include "Server.hpp"

void Server::quit( const vector<string>& params, Client &client)
{
	string msg;
	if (!params.empty())
	{
		msg = params[0];
		for (size_t i = 1; i < params.size(); i++)
		{
			msg += " ";
			msg += params[i];
		}
		msg.erase(string::size_type(0), 1);
	}
	string msg1 = ":" + client.getNickname() + \
                    (client.getUsername().empty() ? "" : "!" + client.getUsername()) + \
                    (client.getHostname().empty() ? "" : "@" + client.getHostname()) + \
                    " QUIT :closed connection " + msg + endmsg;
	client.setBuff(client.getBuff() + msg1);
	for (vector<string>::iterator it = client.channelsMember.begin(); it != client.channelsMember.end(); ++it)
	{
		Channel dest = find_channel(*it, client);
		this->notify_chan(dest, msg, "QUIT", client);
	}
	client.isLeaving = true;
	sendMessage(client, client.getBuff());
}
