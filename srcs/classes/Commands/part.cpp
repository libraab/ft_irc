#include "Cmd.hpp"

void Cmd::part_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() == 1) {
        server->send_error("461", client->get_nick(), "Not enough argument given", client->get_fd());
        return;
    }
    if (server->channel_exist(arg[1])) {
        if (server->get_channel(arg[1])->client_is_in_channel(client->get_fd())) {
            server->get_channel(arg[1])->delete_user_from_channel(client->get_fd());
            for (client_map_it it = server->client_list.begin(); it != server->client_list.end(); it++) {
                if (it->first != client->get_fd())
				    server->send_reply(client->get_nick(), client->get_user(), "PART", arg[1], it->first);
			    else
				    server->send_reply(client->get_nick(), client->get_user(), "PART", arg[1], client->get_fd());
            }
        }
        else
            server->send_error_with_arg("442", client->get_nick(), arg[1], "You're not on that channel", client->get_fd());
    }
    else
        server->send_error_with_arg("403", client->get_nick(), arg[1], "No such channel", client->get_fd());
    if (server->get_channel(arg[1])->get_users().empty())
        server->delete_channel_from_server(server->get_channel(arg[1]));
}