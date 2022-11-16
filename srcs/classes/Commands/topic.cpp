#include "Cmd.hpp"

void Cmd::topic_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() < 2)
    {
        server->send_error("461", server->client_list[client->get_fd()]->get_nick(), "not enough arg", client->get_fd());
        return ;
    }
    server->send_reply(client->get_nick(), client->get_user(), "TOPIC", arg[1], client->get_fd());
}