#include "Server.hpp"

//******************************//
//   CONSTRUCTOR / DESTRUCTOR	//
//******************************//
Server::Server(void) {
	return;
}
Server::~Server(void) {
	return;
}
//******************************//
// 		  S E T T E R S		    //
//******************************//
void Server::set_password(string password) {
	_password = password;
}
//******************************//
// 		  G E T T E R S		    //
//******************************//
string const &Server::get_password(void) const {
	return (_password);
}
vector<Channel *> &Server::get_channels() {
	return (_channels);
}
//******************************//
// 		  M E T H O D S 	    //
//******************************//
void Server::add_channel_to_server(Channel *channel) {
	_channels.push_back(channel);
}
//------------------------------------------------------------------------------
void Server::delete_channel_from_server(Channel *channel) {
	for (vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		if (*it == channel) {
			_channels.erase(it);
			break;
		}
	}
}
//------------------------------------------------------------------------------
void	Server::send_error(string code, string nickname, string to_send, int client_fd) {
	string message = ":localhost " + code + " " + nickname + " :" + to_send + "\r\n";
	ft_send(client_fd, message.c_str());
}
//------------------------------------------------------------------------------
void	Server::send_error_with_arg(string code, string nickname, string arg, string to_send, int client_fd) {

	string message = ":localhost " + code + " " + nickname + " " + arg + " :" + to_send + "\r\n";
	ft_send(client_fd, message.c_str());
}

//------------------------------------------------------------------------------
void Server::send_reply(string nick, string user, string cmd, string arg, int fd)
{
	string	to_send = ":" +  nick + "!" + user + "@127.0.0.1 " + cmd + " :" + arg + "\r\n";
	ft_send(fd, to_send.c_str());
}
//------------------------------------------------------------------------------
void Server::rpl_welcome(string nick, string user, int fd) {
	string to_send = ":localhost 001 " + nick + "\r\n" 
	+ "\"Welcome to the Internet Relay Chat Network " + nick + "!" + user
	+ "@localhost" + "\"" + "\r\n";
	ft_send(fd, to_send.c_str());
}
//------------------------------------------------------------------------------
Client *Server::get_client(string client_name) {
	for (client_map_it it = client_list.begin(); it != client_list.end(); it++) {
		if ((*it).second->get_nick() == client_name)
			return ((*it).second);
	}
	return (nullptr);
}
//------------------------------------------------------------------------------
Channel *Server::get_channel(string channel_name) {
	for (size_t i = 0; i < get_channels().size(); i++) {
		if(get_channels()[i]->get_name() == channel_name)
			return (get_channels()[i]);
	}
	return (nullptr);
}
//------------------------------------------------------------------------------
bool Server::channel_exist(string channel_name) {
	for (size_t i = 0; i < get_channels().size(); i++) {
		if (get_channels()[i]->get_name() == channel_name) {
			return (true);
		}
	}
	return (false);
}
//------------------------------------------------------------------------------
bool Server::client_exist(string client_name) {
	for (client_map_it it = client_list.begin(); it != client_list.end(); it++) {
		if (it->second->get_nick() == client_name)
			return (true);
	}
	return (false);
}
//------------------------------------------------------------------------------
void Server::delete_client_from_server(void) {
	vector<int>::iterator it = _clients_fds.begin();
	vector<int>::iterator ite = _clients_fds.end();
	map<int, Client *>::iterator client_it;

	for (; it != ite; it++) {
		client_it = client_list.find(*it);
		delete client_it->second;
		close(client_it->first);
		client_list.erase(client_it->first);
	}
	_clients_fds.clear();
}
//------------------------------------------------------------------------------
void Server::add_client_to_server(void) {
	Client 				*user = new Client();
	struct sockaddr_in 	addr;
	socklen_t 			addr_len = sizeof(addr);
	int 				new_fd;

	new_fd = accept(socket_fd, (struct sockaddr *)&addr, &addr_len);
	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	client_list.insert(pair<int, Client *>(new_fd, user));
	client_list[new_fd]->set_fd(new_fd);
	cout << "A new client is connected to the server" << endl;
}
//------------------------------------------------------------------------------
void Server::registration_irssi(string buf, Client *client)
{
	map<string, vector<string> > values;
	vector<string> value = split(buf, '\r');
	string entries [] = {
		"PASS",
		"NICK",
		"USER"
	};
	for (vector<string>::iterator it = value.begin(); it != value.end(); it++) {
		for (int i = 0; i < 3; i++) {
			vector<string> tmp = split(*it, ' ');
			if (tmp[0].size() > 0 && tmp[0][0] == '\n')
				tmp[0] = tmp[0].substr(1);
			if (tmp[0] == entries[i]) {
				tmp.erase(tmp.begin());
				values[entries[i]] = tmp;
				break ;
			}
		}
	}
	for (map<string, vector<string> >::iterator it = values.begin(); it != values.end(); it++) {
		if (it->first == "PASS") {
			if (it->second[0] != get_password())
				return ;
			client->set_pass(it->second[0]);
		}
		else if (it->first == "NICK")
			client->set_nick(it->second[0]);
		else if (it->first == "USER")
			client->set_user(it->second[0]);
	}
	if (client->is_registered(_password))
		rpl_welcome(client->get_nick(), client->get_user(), client->get_fd());
}
//------------------------------------------------------------------------------
void Server::registration_netcat(string buf, Client *client) {
	buf.erase(buf.length() - 1); // efface le dernier char ""
	vector<string> tab = split(buf, ' ');
	if (tab.size() >= 2) {
		if (tab[0] == "PASS") {
			client->set_pass(tab[1]);
		}
		else if (tab[0] == "NICK" && client->password_is_set(_password)) {
			client->set_nick(tab[1]); // to do :verifier si nick est deja pri ou pas 
		}
		else if (tab[0] == "USER" && client->password_is_set(_password)) {
			client->set_user(tab[1]); // Q? si 2 clients peuvent avoir le meme username
		}
	}
	else
		ft_send(client->get_fd(), "Error: Not enough argument\r\n");    
	if (client->is_registered(_password))
		rpl_welcome(client->get_nick(), client->get_user(), client->get_fd());
}
//------------------------------------------------------------------------------
void Server::client_send_msg(int client_fd, string buf, Server *server) {
	vector<string> tab;
	if (buf.length() > 510) {
		ft_send(client_fd, "Error: Exceeded the limit (512 char)\r\n");
		return ;
	}
	if (buf[0] == '\n')
		return ;
	if (buf.find("\r\n") != string::npos) { 	//   <------------- IRSSI
		if (!client_list[client_fd]->is_registered(_password))
			registration_irssi(buf, client_list[client_fd]);
		else {
			Cmd *command = new Cmd;
			command->exec_command(buf, client_list[client_fd], server);
		}
	}
	else {										//   <------------- NETCAT
		if (!client_list[client_fd]->is_registered(_password))
			registration_netcat(buf, client_list[client_fd]);
		else {
			Cmd *command = new Cmd();
			command->exec_command(buf, client_list[client_fd], server);
		}
	}
}
//------------------------------------------------------------------------------
void Server::client_disconnect(int socket_fd) {
	_clients_fds.push_back(socket_fd);
	if (client_list[socket_fd]->get_user().empty())
		cout << "Client is disconnected" << endl;
	else
		cout << client_list[socket_fd]->get_nick() + " is disconnected" << endl;
	//delete_client_from_server();  //<-------------verifier si cette fct marche 
}
//------------------------------------------------------------------------------