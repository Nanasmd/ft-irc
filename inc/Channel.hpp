#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Channel
{
    private:
        string      _name;
        string      _topic;
		Mode        _modeChannel;
        bool        _isPrivate;
        string      _password;
        bool        _isLimit;
        unsigned int         _limit;
        bool        _isInviteOnly;
        bool        _topicPrivate;
    public:
		vector<string> ban;
        //vector of client's fd
        vector<Socket> clients;
        vector<Socket> operators;
        vector<Socket> inviteList;

        //canonical form
        Channel();
        Channel(const string&);
        Channel(const string&, const Client& );
        Channel(const Channel&);
        ~Channel();
        Channel &operator=(const Channel &);

		//getter/setter
        const string    &getName() const;
        const string    &getTopic() const;
        void            setTopic(const string&);

        const string    &getPassword() const;
        int             getLimit() const;
        bool            getIsLimit() const;
        bool            getIsPrivate() const;
        bool            getIsInviteOnly() const;
        bool            getTopicPrivate() const;

        void            setPassword(const string);
        void            setPrivate(bool);
        void            setLimit(int);
        void            setIsLimit(bool);
        void            setIsInviteOnly(bool);
        void            setTopicPrivate(bool set);
        //checks
        bool	user_in_chan(const Client&);
        bool	user_is_operator(const Client&);
        bool	user_in_invite_list(const Client&);
        //actions
        void    addClient(const Client &);
        void    addOper(const Client &);
        void    addInvite(const Client &);
        void    removeClient(const Client &);
        void    removeOper(const Client &);
        void    removeInvite(const Client &);
		Mode    getMode() const;
		void    setMode(Mode input);
		void    unSetMode(Mode input);
};



#endif