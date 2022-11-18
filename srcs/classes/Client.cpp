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
const string & Client::get_nick(void) const {
	return (this->_nick);
}
string const & Client::get_user(void) const {
	return (_user);
}
string const & Client::get_mode() const {
	return (_mode);
}
//******************************//
// 		  M E T H O D S 	    //
//******************************//
bool Client::is_registered(string const & pass) {
	return (password_is_set(pass) && nickname_is_set() && username_is_set());
}
bool Client::password_is_set(string const & pass) {
	return (!(_pass.empty()) && _pass == pass);
}
bool Client::nickname_is_set() { return (!(_nick.empty())); }
bool Client::username_is_set() { return (!(_user.empty())); }