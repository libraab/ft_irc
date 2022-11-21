#include "Cmd.hpp"

void Cmd::kill_cmd(vector <string> arg, Client *client, Server *server) {
    if (client->get_mode().find('o') == string::npos) {
        string to_send = ":Permission Denied- You're not an IRC operator";
        server->send_error_with_arg("481", client->get_nick(), arg[0], to_send, client->get_fd());
        return;
    }
    if (server->client_exist(arg[1])) {
        for (size_t i = 0; i < server->get_channels().size(); i++) {
            client_map_it it = server->get_channels()[i]->get_users().begin();
            for (; it != server->get_channels()[i]->get_users().end(); it++) {
                if (it->second == server->get_client(arg[1])) {
                    if (server->get_channels()[i]->get_users().size() == 1)
                        server->delete_channel_from_server(server->get_channels()[i]);
                }
            }
        }
        server->client_disconnect(server->get_client(arg[1])->get_fd());
    }
    else {
        server->send_error_with_arg("401", client->get_nick(), arg[1], "No such nick/channel", client->get_fd());
        return;
    }
}