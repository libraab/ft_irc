#include "Cmd.hpp"

void Cmd::list_cmd(vector<string> arg, Client *client, Server *server) {
    string channel_list;
    for (size_t i = 0; i < server->get_channels().size(); i++) {
        channel_list += "#" + server->get_channels()[i]->get_name() + "\n";
    }
    string to_send = ":" + client->get_nick() + "!" + client->get_user() + "@localhost " + arg[0] + " " +  channel_list + "\r\n";
    ft_send(client->get_fd(), to_send.c_str());
}