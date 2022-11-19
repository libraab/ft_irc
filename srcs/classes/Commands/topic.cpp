#include "Cmd.hpp"

void Cmd::topic_cmd(vector<string> arg, Client *client, Server *server) {
	if (arg.size() < 2)
	{
		server->send_error("461", server->client_list[client->get_fd()]->get_nick(), "not enough arg", client->get_fd());
		return ;
	}
	if (server->channel_exist(arg[1])) {
		string joined = "";
		vector<string>::iterator it = arg.begin() + 2;
		while (it != arg.end()){
			joined += *it;
			joined += " ";
			it++;
		}
		if (arg.size() == 2)
			server->send_reply(client->get_nick(), client->get_user(), "TOPIC", server->get_channel(arg[1])->get_topic(), client->get_fd());
		if (arg.size() > 2)
		{
			server->get_channel(arg[1])->set_topic(joined);
			for (client_map_it it = server->get_channel(arg[1])->get_users().begin(); it != server->get_channel(arg[1])->get_users().end(); it++) {
				string to_send = ":" + it->second->get_nick() + "!" + it->second->get_user() + "@localhost TOPIC #" + arg[1] + " :" + joined + "\r\n";
				ft_send(it->second->get_fd(), to_send.c_str());
			}
		}
	}
	else
		server->send_error_with_arg("401", client->get_nick(), arg[1], "No such nick/channel", client->get_fd());
}