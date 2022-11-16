#pragma once
#include "../../ft_irc.hpp"
#include "../../libraries.hpp"
#include "Client.hpp"
#include "./Commands/Cmd.hpp"

class Client;
class Channel;
class Server
{
	public:
		Server(void);
		~Server(void);

		void set_password(string password);
		string const &get_password() const;
		vector<Channel *> &get_channels();

		void client_send_msg(int client_d, string buf, Server *server);
		void client_disconnect(int client_fd);
		void registration_irssi(string buf, Client *client);
		void registration_netcat(string buf, Client *client);

		void exec_command(string buf, int client_fd);

		void send_reply(string nickname, string username, string command, string arg, int client_fd);
		void send_errors(Client *client, string code, string arg, string arg2, int error_type);
		void send_error(string code, string nickname, string to_send, int client_fd);
		void send_error_with_arg(string code, string nickname, string arg, string to_send, int client_fd);

		void add_client_to_server();
		void delete_client_from_server();

		void add_channel_to_server(Channel *channel);
		void delete_channel_from_server(Channel *channel);

		Channel *get_channel(string channel_name);
		Client *get_client(string client_name);
		
		bool channel_exist(string channel_name);
		bool client_exist(string client_name);

		int 				socket_fd;
		map<int, Client *> 	client_list;

	private:
		vector<int>			_clients_fds;
		string				_password;
		vector<Channel *>	_channels;
};