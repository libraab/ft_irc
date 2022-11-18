#include "Cmd.hpp"

void Cmd::quit_cmd(Client *client, Server *server) {
    for (size_t i = 0; i < server->get_channels().size() ;i++) {
        client_map_it it = server->get_channels()[i]->get_users().begin();
        for (; it != server->get_channels()[i]->get_users().end(); it++) {
            if (it->second == client) {
                if (server->get_channels()[i]->get_users().size() == 1)
                    server->delete_channel_from_server(server->get_channels()[i]);
            }
        }
    }
    server->client_disconnect(client->get_fd());
}