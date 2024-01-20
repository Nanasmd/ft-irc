#include "Channel.hpp"
#include "typedef.hpp"

/*-------------------------------CONSTRUCTORS---------------------------------*/

Channel::Channel() {
}

Channel::Channel(const string &name) {
    this->_isPrivate = false;
    this->_isLimit = false;
    this->_isInviteOnly = false;
    this->_topicPrivate = false;
    this->_limit = 0;
    cout << "Creating new channel " << name << endl;
    this->_name = name;
	this->_modeChannel = 0;
}

Channel::Channel(const string &name, const Client& client) {
    this->_isPrivate = false;
    this->_isLimit = false;
    this->_isInviteOnly = false;
    this->_topicPrivate = false;
    this->_limit = 0;
    cout << "Creating new channel " << name << endl;
    this->_name = name;
	this->_modeChannel = 0;
    this->addClient(client);
}

Channel::Channel(const Channel &src) {
    *this = src;
}

/*---------------------------------DESTRUCTOR---------------------------------*/

Channel::~Channel() {
}

/*---------------------------------OPERATORS----------------------------------*/


Channel &Channel::operator=(const Channel &rhs) {
    this->_isPrivate = rhs.getIsPrivate();
    this->_isLimit = rhs.getIsLimit();
    this->_isInviteOnly = rhs.getIsInviteOnly();
    this->_limit = rhs.getLimit();
    this->_name = rhs.getName();
	this->_modeChannel = rhs._modeChannel;
    this->clients = rhs.clients;
    this->operators = rhs.operators;
    this->inviteList = rhs.inviteList;
    this->_password = rhs.getPassword();
    this->_topic = rhs.getTopic();
    return (*this);
}

/*------------------------------MEMBER FUNCTIONS------------------------------*/

const string    &Channel::getName() const {
    return (this->_name);
}

const string    &Channel::getTopic() const {
    return (this->_topic);
}

void    Channel::setTopic(const string &topic) {
    this->_topic = topic;
}

bool	Channel::user_in_chan(const Client& client) {
	cl_iter it = find(this->clients.begin(), this->clients.end(), client.getFd());
	if (it == this->clients.end()) {
		return (false);
	}
	return (true);
}

bool	Channel::user_is_operator(const Client& client) {
	cl_iter it = find(this->operators.begin(), this->operators.end(), client.getFd());
	if (it == this->operators.end()) {
		return (false);
	}
	return (true);
}

bool	Channel::user_in_invite_list(const Client& client) {
	for (vector<Socket>::iterator it = inviteList.begin(); it != inviteList.end(); ++it)
	{
        if (*it == client.getFd())
            return (true);
	}
	return (false);
}

void    Channel::addClient(const Client &client) {
    this->clients.push_back(client.getFd());
}

void    Channel::addOper(const Client &client) {
    this->operators.push_back(client.getFd());
}

void    Channel::addInvite(const Client &client) {
    this->inviteList.push_back(client.getFd());
}

void    Channel::removeClient(const Client &client) {
	this->clients.erase(find(this->clients.begin(), this->clients.end(), client.getFd()));
}

void    Channel::removeOper(const Client &client) {
	this->operators.erase(find(this->operators.begin(), this->operators.end(), client.getFd()));
}

void    Channel::removeInvite(const Client &client) {
	this->inviteList.erase(find(this->inviteList.begin(), this->inviteList.end(), client.getFd()));
}
/*------------------------------        GETTER      ------------------------------*/

Mode    Channel::getMode() const
{
	return (this->_modeChannel);
}

bool    Channel::getIsPrivate() const
{
	return (this->_isPrivate);
}

bool    Channel::getTopicPrivate() const
{
	return (this->_topicPrivate);
}

const string    &Channel::getPassword() const
{
	return (this->_password);
}

bool    Channel::getIsLimit() const
{
	return (this->_isLimit);
}

int    Channel::getLimit() const
{
	return (this->_limit);
}

bool    Channel::getIsInviteOnly() const
{
	return (this->_isInviteOnly);
}

/*------------------------------        SETTER      ------------------------------*/

void    Channel::setPassword(const string newPassword)
{
    this->_password = newPassword;
}

void            Channel::setPrivate(bool set)
{
    this->_isPrivate = set;
}

void            Channel::setTopicPrivate(bool set)
{
    this->_topicPrivate= set;
}

void            Channel::setLimit(int newlimit)
{
    this->_limit = newlimit;
}

void            Channel::setIsLimit(bool set)
{
    this->_isLimit = set;
}

void    Channel::setMode(Mode input)
{
	this->_modeChannel |= input;
}

void    Channel::unSetMode(Mode input)
{
	this->_modeChannel &= ~input;
}

void    Channel::setIsInviteOnly(bool set)
{
    this->_isInviteOnly = set;
}