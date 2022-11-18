#include "Cmd.hpp"

void Cmd::notice_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() == 3) {
        if (server->channel_exist(arg[1])) {
            client_map_it it;
            for (it = server->get_channel(arg[1])->get_users().begin();
                it != server->get_channel(arg[1])->get_users().end();
                it++)
            {
                arg[2] = arg[2].substr(1); // remove ':'
                if (it->first != client->get_fd()) {
                    server->send_reply(
                                        client->get_nick(),
                                        client->get_user(),
                                        arg[1],
                                        arg[2],
                                        it->first
                                    );
                }
            }
        }
        else if (server->client_exist(arg[1])) {
            arg[2] = arg[2].substr(1); // remove ':'
            server->send_reply(
                                client->get_nick(),
                                client->get_user(),
                                arg[1],
                                arg[2],
                                server->get_client(arg[1])->get_fd()
                            );
        }
    }   
}
