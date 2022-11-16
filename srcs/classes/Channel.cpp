#include "Channel.hpp"

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
const vector<string> & Channel::get_nicknames() const {
    return (_nicknames);
}
const map<int, Client *> &Channel::get_users() const {
    return (_users);
}
//******************************//
// 	    F U N C T I O N S	    //
//******************************//
void Channel::add_to_nick_list_in_channel(string nick) {
    _nicknames.push_back(nick);
}
void Channel::delete_nick_from_channel(string nick) {
    _nicknames.erase(remove(_nicknames.begin(), _nicknames.end(), nick), _nicknames.end());
}
string Channel::display_nicknames() const {
    string nicknames;
    for(size_t i = 0; i < _nicknames.size(); i++)
        nicknames += _nicknames[i] + ' ';
    return (nicknames.substr(0, nicknames.length() - 1));// return list des nicknames dans un channel
}