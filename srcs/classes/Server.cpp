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
const vector<string> &Server::get_channel_names() const {
	return (_channel_names);
}
//******************************//
// 	    F U N C T I O N S	    //
//******************************//
void Server::delete_client(void) {
	vector<int>::iterator it = _clients_fds.begin();
	vector<int>::iterator ite = _clients_fds.end();
	map<int, Client *>::iterator client_it;

	for (; it != ite; it++) {
		client_it = client.find(*it);
		delete client_it->second;
		close(client_it->first);
		client.erase(client_it->first);
	}
	_clients_fds.clear();
}
//----------------------------------------------------------------------------
void Server::server_accept(void) {
	Client 				*user = new Client();
	struct sockaddr_in 	addr;
	socklen_t 			addr_len = sizeof(addr);
	int 				new_fd;

	new_fd = accept(socket_fd, (struct sockaddr *)&addr, &addr_len);
	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	client.insert(pair<int, Client *>(new_fd, user));
	client[new_fd]->set_fd(new_fd);
	cout << "A new client is connected to the server" << endl;
}
//----------------------------------------------------------------------------
void Server::registration_irssi(string buf, Client *client) // registration parsing
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
	if (client->is_registered(_password)) {
		string to_send = ":localhost 001 " + client->get_nick() + "\r\n" + "\"Welcome to the Internet Relay Chat Network " + client->get_nick() + "!" + client->get_user() + "@localhost" + "\"" + "\r\n";
		send(client->get_fd(), to_send.c_str(), to_send.length(), 0);
	}
}
//----------------------------------------------------------------------------
void Server::registration_netcat(string buf, Client *client) // registration parsing
{
	buf.erase(buf.length() - 1);
	vector<string> tab = split(buf, ' ');
	
	if (tab.size() >= 2) {
		if (tab[0] == "PASS") {
			client->set_pass(tab[1]);
			cout << "pass is {" << tab[1] << "}" << endl;
		}
		else if (tab[0] == "NICK" && client->password_is_set(_password)) {
			client->set_nick(tab[1]);
			cout << "nick is {" << tab[1] << "}" << endl;
		}
		else if (tab[0] == "USER" && client->password_is_set(_password)) {
			client->set_user(tab[1]);
			cout << "user is {" << tab[1] << "}" << endl;
		}
	}
	else
		send(client->get_fd(), "Error: Not enough argument\r\n", 26, 0);    
	if (client->is_registered(_password)) {
		string to_send = ":localhost 001 " + client->get_nick() + "\r\n" + "\"Welcome to the Internet Relay Chat Network " + client->get_nick() + "!" + client->get_user() + "@localhost" + "\"" + "\r\n";
		send(client->get_fd(), to_send.c_str(), to_send.length(), 0);
	}
}
//----------------------------------------------------------------------------
void Server::client_send_msg(int client_fd, string buf, Server *server) {
	Cmd *command = new Cmd(buf);
	vector<string> tab;
	if (buf.length() > 510) {
		send(client_fd, "Error: Exceeded the limit (512 char)\r\n", 35, 0);
		return;
	}
	if (buf.find('\n') == string::npos)
		client[client_fd]->set_msg(buf);
	else if (!client[client_fd]->get_msg().empty()) {
		client[client_fd]->set_msg(buf);
		tab.push_back(client[client_fd]->get_msg());
	}
	else
		client[client_fd]->set_msg(buf);
	if (buf.find("\r\n") != string::npos) { 			//   <------------- IRSSI
		if (!client[client_fd]->is_registered(_password))
			registration_irssi(buf, client[client_fd]);
		else 
			command->exec_command(buf, client[client_fd], server);
	}
	else {												//   <------------- NETCAT
		if (!client[client_fd]->is_registered(_password))
			registration_netcat(buf, client[client_fd]);
		else
			command->exec_command(buf, client[client_fd], server);
	}
}
//----------------------------------------------------------------------------
void Server::client_disconnect(int socket_fd) {
	_clients_fds.push_back(socket_fd);
	if (client[socket_fd]->get_user().empty())
		cout << "Client is disconnected" << endl;
	else
		cout << client[socket_fd]->get_nick() + " is disconnected" << endl;
}
//----------------------------------------------------------------------------
Client *Server::get_client_by_nick(string nick) {
	map<int, Client *>::iterator it = client.begin();
	map<int, Client *>::iterator ite = client.end();

	for (;it != ite; it++) {
		if ((*it).second->get_nick() == nick)
			return ((*it).second);
	}
	return (nullptr);
}

//----------------------------------------------------------------------------
void	Server::send_error(string code, string nickname, string to_send, int client_fd) {
	string message = ":localhost " + code + " " + nickname + " :" + to_send + "\r\n";
	send(client_fd, message.c_str(), message.length(), 0);
}
//----------------------------------------------------------------------------
void	Server::send_error_with_arg(string code, string nickname, string arg, string to_send, int client_fd) {

	string message = ":localhost " + code + " " + nickname + " " + arg + " :" + to_send + "\r\n";
	send(client_fd, message.c_str(), message.length(), 0);
}

//--------------------------------------------------------------------
void Server::send_reply(string nickname, string username, string command, string arg, int client_fd)
{
	string	message = ":" +  nickname + "!" + username + "@127.0.0.1 " + command + " :" + arg + "\r\n";
	send(client_fd, message.c_str(), message.length(), 0);
}
//--------------------------------------------------------------------
bool Server::channel_exist(string name) {
	if (find(get_channel_names().begin(), get_channel_names().end(), name) != get_channel_names().end())
	{
		cout << "exist"<< endl;
		return (true);
	}
	
	cout << "does not exist"<< endl;
	return (false);
}

Channel *Server::get_channel_ptr_by_channel_name(string channel_name)
{
	for (map<int, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if(it->second->get_name() == channel_name)
			return (it->second);
	}
	return (nullptr);
}