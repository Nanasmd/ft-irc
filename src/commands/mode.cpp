#include "Server.hpp"

string mode_to_str(const Mode &mode)
{
	string result = "+";
	if (mode & a)
		result += "a";
	if (mode & i)
		result += "i";
	if (mode & o)
		result += "o";
	if (mode & w)
		result += "w";
	if (mode & r)
		result += "r";
	if (mode & B)
		result += "B";
	if (mode & p)
		result += "p";
	if (mode & n)
		result += "n";
	return (result);
}

void	Server::handle_user(const vector<string>& params, Client& client, Client &target)
{
	if ( params.size() == 1 )
	{
		this->add_rply_from_server(mode_to_str(client.getMode()), target, "", RPL_UMODEIS);
		return ;
	}
	string input = params[1];
	if (input[0] != '+' && input[0] != '-')
	{
		this->add_rply_from_server(":Please use + or - with mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
		throw invalid_argument("mode: Please use + or - with mode");
	}
	if (input[0] == '+')
	{
		input.erase(0, 1);
		for (string::iterator it = input.begin(); it != input.end(); ++it)
		{
			switch (*it)
			{
				case 'a':
						this->add_rply_from_server(":Please use AWAY to set your mode to away", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					break ;
				case 'o':
					if (!client.isOperator)
						this->add_rply_from_server(":Permission Denied- You're not an IRC operator", client , "MODE", ERR_NOPRIVILEGES);
					else
					{
						target.isOperator = true;
						target.setMode(o);
					}
					break ;
				case 'i':
					target.setMode(i);
					break ;
				case 'w':
					target.setMode(w);
					break ;
				case 'r':
					target.setMode(r);
					break ;
				case 'B':
					if (!client.isOperator)
						this->add_rply_from_server(":Permission Denied- You're not an IRC operator", client, "MODE", ERR_NOPRIVILEGES);
					else
					{
						this->get_botList().push_back(target.getNickname());
						target.setMode(B);
					}
					break ;
				default:
					this->add_rply_from_server(":Please use known mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					break ;
			}
		}
	}
	else
	{
		input.erase(0, 1);
		for (string::iterator it = input.begin(); it != input.end(); ++it)
		{
			switch (*it)
			{
				case 'a':
					this->add_rply_from_server(":Please use AWAY to set your mode to away", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					throw invalid_argument("mode: Please use AWAY to set your mode to away");
				case 'o':
					if (!client.isOperator)
						this->add_rply_from_server(":Permission Denied- You're not an IRC operator", client , "MODE", ERR_NOPRIVILEGES);
					else
					{
						target.isOperator = false;
						target.unSetMode(o);
					}
					break ;
				case 'i':
					target.unSetMode(i);
					break ;
				case 'w':
					target.unSetMode(w);
					break ;
				case 'r':
					target.unSetMode(r);
					break ;
				case 'B':
					if (!client.isOperator)
					{
						this->add_rply_from_server(":Permission Denied- You're not an IRC operator", client, "MODE", ERR_NOPRIVILEGES);
					}
					else
					{
						this->get_botList().erase(find(this->get_botList().begin(), this->get_botList().end(), target.getNickname()));
						target.unSetMode(B);
					}
					break ;
				default:
					this->add_rply_from_server(":Please use known mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
					throw invalid_argument("mode: Please use known mode");
			}
        }
    }
	this->add_rply_from_server(mode_to_str(target.getMode()), target, "", RPL_UMODEIS);
}

void	Server::imode(Channel &dest, char sign)
{
	if (sign == '+')
		dest.setIsInviteOnly(true);
	else
		dest.setIsInviteOnly(false);
}

void	Server::tmode(Channel &dest, char sign)
{
	if (sign == '+')
		dest.setTopicPrivate(true);
	else
		dest.setTopicPrivate(false);
}

static bool is_printable(const std::string& str) {
    for (std::size_t i = 0; i < str.length(); ++i) {
        if (!(std::isdigit(static_cast<unsigned char>(str[i])) || std::isalpha(static_cast<unsigned char>(str[i])))) {
            return false;
        }
    }
    return true;
}

void	Server::kmode(Channel &dest, char sign, string param, Client& client)
{
	if (sign == '+')
	{
		if (is_printable(param))
		{
			dest.setPrivate(true);
			dest.setPassword(param);
		}
		else
			this->add_rply_from_server(":Please use alnum parameter with l mode", client, dest, "MODE", ERR_KEYSET);
	}
	else
		dest.setPrivate(false);
}

void	Server::omode(Channel &dest, char sign, string param, Client& client)
{
	cout << "--------------OPERATOR VECTOR BEFORE-------------------" << endl;
	for (vector<Socket>::iterator it = dest.operators.begin(); it != dest.operators.end(); ++it)
		cout << *it << endl;
	try {
		Client &clientToChange= find_user(param, client, "MODE");
		if (sign == '+')
			dest.addOper(clientToChange);
		else
			dest.removeOper(clientToChange);
	cout << "--------------OPERATOR VECTOR AFTER-------------------" << endl;
		for (vector<Socket>::iterator it = dest.operators.begin(); it != dest.operators.end(); ++it)
			cout << *it << endl;
	}
	catch ( exception& x)
	{
		cout << RED << x.what() << RESET << endl;
		return ;
	}

}

static bool is_num(const std::string& str) {
    for (std::size_t i = 0; i < str.length(); ++i) {
        // Utilisez isdigit et isalpha pour vérifier si un caractère est alphanumérique
        if (!(std::isdigit(static_cast<unsigned char>(str[i])))) {
            return false;
        }
    }
    return true;
}

void	Server::lmode(Channel &dest, char sign, string param, Client& client)
{
	if (sign == '+')
	{
		if (is_num(param))
		{
			dest.setIsLimit(true);
			dest.setLimit(std::atoi(param.c_str()));
		}
		else
			this->add_rply_from_server(":Please use digit parameter with l mode", client, dest, "MODE", ERR_KEYSET);
	}
	else
		dest.setIsLimit(false);
}

void    Server::handle_channel(const vector<string>& params, Client& client, Channel &dest)
{
	cout << "----------------CHANEL BEFORE--------------------" << endl;
	cout << "is invite: " << dest.getIsInviteOnly() << endl;
	cout << "is limite: " << dest.getIsLimit() << endl;
	cout << "limite: " << dest.getLimit() << endl;
	cout << "is Private: " << dest.getIsPrivate() << endl;
	cout << "Password: " << dest.getPassword() << endl;
	if (params.size() == 1)
	{
		this->add_rply_from_server(mode_to_str(dest.getMode()), client, dest, "", RPL_CHANNELMODEIS);
		return ;
	}
	string input = params[1];
	string temp_str = params[2];
	vector<string>	param_mode;
	little_split(param_mode, temp_str, "*");
	str_iter it_param = param_mode.begin();
	if (input[0] != '+' && input[0] != '-')
	{
		this->add_rply_from_server(":Please use + or - with mode", client, dest, "MODE", ERR_UNKNOWNMODE);
		throw invalid_argument("mode: Please use + or - with mode");
	}
	char	sign = input[0];
	input.erase(0, 1);
	for (string::iterator it = input.begin(); it != input.end(); ++it)
	{
		cout << *it << endl;
		cout << *it_param << endl;
		switch (*it)
		{
			case 'i':
				imode(dest, sign);
				break ;
			case 't':
				tmode(dest, sign);
				break ;
			case 'k':
				kmode(dest, sign, *it_param, client);
				break ;
			case 'o':
				omode(dest, sign, *it_param, client);
				break ;
			case 'l':
				lmode(dest, sign, *it_param, client);
				break ;
			default:
				this->add_rply_from_server(":Please use known mode", client, dest, "MODE", ERR_UNKNOWNMODE);
				throw invalid_argument("mode: Please use known mode");
				break ;
		}
		it_param++;
	}
	this->add_rply_from_server(mode_to_str(dest.getMode()), client, dest, "", RPL_CHANNELMODEIS);
	cout << "----------------CHANEL AFTER--------------------" << endl;
	cout << "is invite: " << dest.getIsInviteOnly() << endl;
	cout << "is limite: " << dest.getIsLimit() << endl;
	cout << "limite: " << dest.getLimit() << endl;
	cout << "is Private: " << dest.getIsPrivate() << endl;
	cout << "Password: " << dest.getPassword() << endl;
}

void	Server::mode( const vector<string>& params, Client &client)
{
	cout << "1" << endl;
	try
	{
		cout << "2" << endl;
		if (params.empty())
		{
			add_rply_from_server(":Not enough parameters", client , "MODE", ERR_NEEDMOREPARAMS);
			throw invalid_argument("mode: Not enough parameters");
		}
		else if (params[0][0] == '#')
		{
			Channel &dest = find_channel(params[0], client);
			if (dest.user_in_chan(client) || client.isOperator)
				handle_channel(params, client, dest);
			else
				add_rply_from_server(":" + dest.getName() + " :Cannot send to channel", client, "", ERR_CANNOTSENDTOCHAN);
		}
		else
		{
			if (client.getNickname() != params[0] && !client.isOperator)
			{
				add_rply_from_server(":Cannot change mode for other users", client, "MODE", ERR_USERSDONTMATCH);
				throw invalid_argument("mode: Cannot change mode for other users");
			}
			Client &target = this->find_user(params[0], client, "MODE");
			handle_user(params, client, target);
		}
	}
	catch ( exception& x)
	{
		cout << RED << x.what() << RESET << endl;
		return ;
	}
}
