#include "Cmd.hpp"

void Cmd::notice_cmd(vector<string> arg, Client *client, Server *server) {
    (void)server;
    (void)client;
    //arg = PRIVMSG tata :hellloo world <----- to channel 
    //arg = PRIVMSG papa :coucou <----- to someone
    if (arg.size() == 3) {
        if (server->channel_exist(arg[1])) {
            for (map<int, Client *>::iterator it = server->get_channel(arg[1])->get_users().begin(); it != server->get_channel(arg[1])->get_users().end(); it++) {
                if (it->first != client->get_fd())
                    server->send_reply(client->get_nick(), client->get_user(), arg[1], arg[2], it->first);
            }
        }
        else if (server->client_exist(arg[1]))
            server->send_reply(client->get_nick(), client->get_user(), arg[1], arg[2], it->first);
    }   
}