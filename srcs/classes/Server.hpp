#pragma once
#include "../../ft_irc.hpp"
#include "../../libraries.hpp"
#include "Client.hpp"
#include "Cmd.hpp"

class Client;
class Channel;
class Server
{
	public:
		Server(void);
		~Server(void);

		void set_password(string password);
		string const &get_password() const;

		void delete_client();
		void server_accept();
		void client_send_msg(int client_d, string buf, Server *server);
		void client_disconnect(int client_fd);
		void registration_irssi(string buf, Client *client);
		void registration_netcat(string buf, Client *client);
		void exec_command(string buf, int client_fd);
		void send_reply(string nickname, string username, string command, string arg, int client_fd);
		void send_errors(Client *client, string code, string arg, string arg2, int error_type);
		void send_error(string code, string nickname, string to_send, int client_fd);
		void send_error_with_arg(string code, string nickname, string arg, string to_send, int client_fd);
		Client *get_client_by_nick(string name);
		bool channel_exist(string name);
		Channel *get_channel_ptr_by_channel_name(string channel_name);
		const vector<string> &get_channel_names() const;


		int 				socket_fd;
		map<int, Client *> 	client;
		map<int, Channel *> channels;
		size_t				occ;

	private:
		vector<int> 	_clients_fds;
		vector<string> 	_channel_names;
		string 			_password;
};