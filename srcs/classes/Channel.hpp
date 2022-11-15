#pragma once
#include "../../libraries.hpp"

class Client;
class Channel
{
    public:
        Channel(string name);
        ~Channel(void);

        void set_topic(string const topic);
        const string &get_topic() const;
        const string &get_name() const;
        const vector<string> &get_nicknames() const;
        const map<int, Client *> &get_users() const;
        void add_nick_to_channel(string nick);
        void delete_nick_from_channel(string nick);

    private:
        string _name;
        string _topic;
        vector<string> _nicknames;
        map<int, Client *> _users;
};