#include "Cmd.hpp"

void Cmd::privmsg_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() > 2) {
        if (server->channel_exist(arg[1])) {
            if (server->get_channel(arg[1])->client_is_in_channel(client->get_fd())) {
                client_map_it it;
                for (it = server->get_channel(arg[1])->get_users().begin();
                    it != server->get_channel(arg[1])->get_users().end();
                    it++)
                {
                    if (it->first != client->get_fd()) {
                        string to_send = ":" + client->get_nick() + " PRIVMSG #" + arg[1] + " " + arg[2] + "\r\n";
                        ft_send(it->first, to_send.c_str());
                    }
                }
            }
			else
				server->send_error_with_arg("404", client->get_nick(), arg[1], "Cannot send to channel", client->get_fd());
        }
        else if (server->client_exist(arg[1])) {
                string to_send = ":" + client->get_nick() + " PRIVMSG " + arg[1] + " " + arg[2] + "\r\n";
				ft_send(server->get_client(arg[1])->get_fd(), to_send.c_str());
        }
		else
			server->send_error_with_arg("401", client->get_nick(), arg[1], "No such nick/channel", client->get_fd());
    } 
	else if (arg.size() == 1)
		server->send_error("411", client->get_nick(), "No receiver given", client->get_fd());
	else
		server->send_error("407", client->get_nick(), "Duplicate recipients. No message delivered", client->get_fd());
}
