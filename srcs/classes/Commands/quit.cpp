#include "Cmd.hpp"

void Cmd::quit_cmd(Client *client, Server *server) {
    // verifier si le client est le dernier dans le channel et effacer le channel si oui 
    for (size_t i = 0; i < server->get_channels().size() ;i++) { // traverser mon vector de channel *
        for (map<int, Client *>::iterator it = server->get_channels()[i]->get_users().begin(); it != server->get_channels()[i]->get_users().end(); it++) { // traverser ma map de users dans chaque channel
            if (it->second == client) { // checher le client
                if (server->get_channels()[i]->get_users().size() == 1)
                    server->delete_channel_from_server(server->get_channels()[i]);
            }
        }
    }
    server->client_disconnect(client->get_fd());
}