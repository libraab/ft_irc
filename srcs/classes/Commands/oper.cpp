#include "Cmd.hpp"

void Cmd::oper_cmd(vector<string> arg, Client *client, Server *server)
{
	if (arg.size() < 3) {
		server->send_error("461", client->get_nick(), "not enough arg", client->get_fd());
		return ;
	}
	else if (arg.size() != 3 ) {
		server->send_error("", client->get_nick(), "Wrong format", client->get_fd());
		return ;
	}
	else {
        for (client_map_it it = server->client_list.begin(); it != server->client_list.end(); it++) {
		    if (it->second->get_user() == arg[1]) {
                if (arg[2] != server->get_oper_pass()) {
                    server->send_error("464", client->get_nick(), "Password incorrect", client->get_fd());
                    return ;
                }
                string mode = server->get_client(it->second->get_nick())->get_mode();
                if (mode.find('o') != string::npos)
                    return;
                server->get_client(it->second->get_nick())->set_mode("o" + mode);
                string to_send = ":localhost 221 " + client->get_nick() + " +" + client->get_mode() + "\r\n";
                ft_send(client->get_fd(), to_send.c_str());
                return;
            }
        }
        server->send_error_with_arg("401", client->get_nick(), arg[1], "No such nick/channel", client->get_fd());
    }
}