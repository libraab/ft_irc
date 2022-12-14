#pragma once
#include "../../libraries.hpp"

class Client;
typedef map<int, Client *> client_map;
typedef client_map::iterator client_map_it;

class Channel
{
    public:
        Channel(string name);
        ~Channel(void);

        void set_topic(string const topic);

        const string &get_topic() const;
        const string &get_name() const;
        const vector<string> &get_nicknames() const;
        map<int, Client *> &get_users();

        void add_user_to_channel(Client *client);
        void delete_user_from_channel(int client_fd);

        string display_nicknames();
        
        bool client_is_in_channel(int client_fd);

    private:
        string              _name;
        string              _topic;
        map<int, Client *>  _users;
};