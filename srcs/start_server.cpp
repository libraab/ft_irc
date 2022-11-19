#include "../ft_irc.hpp"

int check_port(char *port_input)
{
	for (int i = 0; port_input[i]; i++)
		if (!isdigit(port_input[i]))
			error("Error: port not digit ⛔️");
	int port = atoi(port_input);
	if (port <= 0 || port > 65535)
		error("Error: port out of range ⛔️");
	return (port);
}

void    print_buf(string const & buf, int client_fd)
{
    if (buf == "PING localhost\r\n" || buf == "\n")
        return;
    if (buf.empty())
    {
        cout << "BUFF EMPTY" << endl;
        return;
    }
    cout << black bg_red << "| CLIENT " << client_fd << " |" << reset << endl;
    cout << buf ;
}

void start_server(Server *server, char *port_input)
{
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	fd_set read_fds;

	char buf[1024];
	int port = check_port(port_input);
	int opt = 1;
	int client_fd;

	if ((server->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		failed_function("socket");
	if (setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		failed_function("setsockopt");
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(server->socket_fd, (struct sockaddr *)&addr, addr_len) < 0)
		failed_function("bind");
	if (listen(server->socket_fd, 2) < 0)
		failed_function("listen");
	cout << bg_black red << "Waiting for connections..." << reset << endl;

	while (true)
	{
		bzero(buf, 1024);
		FD_ZERO(&read_fds);
		FD_SET(server->socket_fd, &read_fds);
		server->delete_client_from_server();
		client_map_it it;
		for (it = server->client_list.begin();
			it != server->client_list.end();
			it++)
		{
			client_fd = it->first;
			FD_SET(client_fd, &read_fds);
		}
		if (select(server->client_list.size() + server->socket_fd + 1,
					&read_fds, NULL, NULL, NULL) < 0)
		{ failed_function("select"); }
		if (FD_ISSET(server->socket_fd, &read_fds))
			server->add_client_to_server();
		for (it = server->client_list.begin();
			it != server->client_list.end();
			it++)
		{
			client_fd = it->first;
			if (FD_ISSET(client_fd, &read_fds)) {
				if (recv(client_fd, buf, 1024, 0)) { 
					print_buf(buf, client_fd);
					server->client_send_msg(client_fd, buf, server);
				}
				else
					server->client_disconnect(client_fd);
			}
		}
	}
}