#include "Client.hpp"

//******************************//
//   CONSTRUCTOR / DESTRUCTOR	//
//******************************//
Client::Client(void) {
	return;
}

Client::~Client(void) {
	return;
}

//******************************//
// 		  S E T T E R S		    //
//******************************//
void Client::set_fd(int const &fd) {
	_fd = fd;
}

void Client::set_msg(string const &msg) {
	_msg += msg;
}

void Client::set_pass(string const &pass) {
	_pass = pass;
}

void Client::set_nick(string const &nick) {
	_nick = nick;
}

void Client::set_user(string const &user) {
	_user = user;
}

void Client::set_mode(string const &mode) {
	_mode = mode;
}

//******************************//
// 		  G E T T E R S		    //
//******************************//
int const & Client::get_fd() const {
	return (_fd);
}
string const & Client::get_msg() const {
	return (this->_msg);
}
const string & Client::get_nick(void) const {
	return (this->_nick);
}
string const & Client::get_user(void) const {
	return (_user);
}
string const & Client::get_mode() const {
	return (_mode);
}