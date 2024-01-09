#ifndef FT_IRC_TYPEDEF_HPP
#define FT_IRC_TYPEDEF_HPP

# include <map>
# include <vector>
using namespace std;

class Server;
class Channel;
class Client;

typedef int Socket;
typedef int Epollfd;
typedef uint32_t Mode;

typedef	void (Server::*command_function)( const vector<string>&, Client& );
typedef map<Socket, Client>::iterator	client_iter;
typedef vector<Channel>::iterator chan_iter;
typedef vector<Socket>::iterator cl_iter;
typedef vector<string>::iterator str_iter;
typedef vector<Socket>::iterator vec_sock_iter;

#endif //FT_IRC_TYPEDEF_HPP
