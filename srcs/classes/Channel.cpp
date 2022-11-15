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

void Channel::set_topic(string const topic) {
    _topic = topic;
}
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
void Channel::add_nick_to_channel(string nick) {
    _nicknames.push_back(nick);
}
void Channel::delete_nick_from_channel(string nick) {
    _nicknames.erase(remove(_nicknames.begin(), _nicknames.end(), nick), _nicknames.end());
}