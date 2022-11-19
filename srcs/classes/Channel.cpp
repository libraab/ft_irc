#include "Channel.hpp"
#include "Client.hpp"

//******************************//
//   CONSTRUCTOR / DESTRUCTOR	//
//******************************//
Channel::Channel(string name) {
    _name = name;
    _topic = "NO TOPIC";
}
Channel::~Channel(void) {
    return;
}
//******************************//
// 		  S E T T E R S		    //
//******************************//
void Channel::set_topic(string const topic) {
    _topic = topic;
}
//******************************//
// 		  G E T T E R S		    //
//******************************//
const string & Channel::get_topic() const {
    return (_topic);
}
const string & Channel::get_name() const {
    return (_name);
}
map<int, Client *> &Channel::get_users() {
    return (_users);
}
//******************************//
// 		  M E T H O D S 	    //
//******************************//
void Channel::add_user_to_channel(Client *usr) {
    _users.insert(pair<int, Client *>((usr->get_fd()), usr));
    cout << "user was added" << endl;
    for (client_map_it it = _users.begin(); it != _users.end(); it++)
    {
        cout << "key -->" << it->first << endl;
        cout << "value" << it->second->get_nick() << endl;
    }
}
void Channel::delete_user_from_channel(int client_fd) {
    _users.erase(client_fd);
}
string Channel::display_nicknames() {
    string nicknames;
    for (client_map_it it = _users.begin(); it != _users.end(); it++) {
        nicknames += it->second->get_nick() + ' ';
    }
    return (nicknames.substr(0, nicknames.length() - 1));
}
bool Channel::client_is_in_channel(int client_fd) {
     for (client_map_it it = _users.begin(); it != _users.end(); it++) {
        if (it->first == client_fd)
            return (true);
    }
    return (false);
}
