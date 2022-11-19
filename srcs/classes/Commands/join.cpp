#include "Cmd.hpp"

void Cmd::join_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() == 1) {
        server->send_error("461", client->get_nick(), "Not enough argument given", client->get_fd());
        return;
    }
    else if (arg[1][0] != '#') {
        server->send_error("", client->get_nick(), "Wrong format", client->get_fd());
        return;
    }
    else
        arg[1] = arg[1].substr(1); //remove the '#' in #channel_name
    if (!server->channel_exist(arg[1])) {
        Channel *channel = new Channel(arg[1]);
        server->add_channel_to_server(channel);
        channel->add_user_to_channel(client);
        if (arg.size() > 2)
            channel->set_topic(arg[2]); // prendre tt ce qui reste apres nom du channel
        server->send_reply(client->get_nick(), client->get_user(), "JOIN", arg[1], client->get_fd());
        string to_send = ":localhost 353 " + client->get_user() + " = " + arg[1] + " :@" + client->get_nick() + "\r\n";
        ft_send(client->get_fd(), to_send.c_str());
        server->send_error_with_arg("366", client->get_nick(), arg[1], "End of /NAMES list", client->get_fd());
        server->send_error_with_arg("332", client->get_nick(), arg[1], channel->get_topic(), client->get_fd());
    }
    else {
        Channel *channel = server->get_channel(arg[1]);
        channel->add_user_to_channel(client);
        for (client_map_it it = channel->get_users().begin(); it != channel->get_users().end(); it++) {
            if (it->second->get_fd() != client->get_fd())
                server->send_reply(client->get_nick(), client->get_user(), "JOIN", arg[1], it->second->get_fd()); // dire aux autres qlq a join le channel
            else {
                server->send_reply(client->get_nick(), client->get_user(), "JOIN", arg[1], client->get_fd());
                string to_send = ":localhost 353 " + client->get_user() + " = " + arg[1] + " :" + channel->display_nicknames() + "\r\n"; //rajouter lop
                ft_send(client->get_fd(), to_send.c_str());
                server->send_error_with_arg("366", client->get_nick(), arg[1], "End of /NAMES list", client->get_fd());
                server->send_error_with_arg("332", client->get_nick(), arg[1], channel->get_topic(), client->get_fd());
            }
        }
    }
}