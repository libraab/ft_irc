#include "Cmd.hpp"

void Cmd::privmsg_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() << " msg" << arg[1] << endl;
    (void)server;
}