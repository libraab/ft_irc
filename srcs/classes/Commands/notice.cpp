#include "Cmd.hpp"

void Cmd::notice_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() == 3) {
        if (server->channel_exist(arg[1])) {
            if (server->get_channel(arg[1])->client_is_in_channel(client->get_fd())) {
                client_map_it it;
                for (it = server->get_channel(arg[1])->get_users().begin();
                    it != server->get_channel(arg[1])->get_users().end();
                    it++)
                {
                    if (it->first != client->get_fd()) {
						string joined = "";
						vector<string>::iterator itt = arg.begin() + 2;
						while (itt != arg.end()){
							joined += *itt;
							joined += " ";
							itt++;
						}
                        string to_send = ":" + client->get_nick() + " PRIVMSG #" + arg[1] + " " + joined + "\r\n";
                        send(it->first, to_send.c_str(), to_send.length(), 0);
                    }
                }
            }
        }
        else if (server->client_exist(arg[1])) {
			string joined = "";
			vector<string>::iterator it = arg.begin() + 2;
			while (it != arg.end()){
				joined += *it;
				joined += " ";
				it++;
			}
            string to_send = ":" + client->get_nick() + " PRIVMSG #" + arg[1] + " " + joined + "\r\n";
			send(server->get_client(arg[1])->get_fd(), to_send.c_str(), to_send.length(), 0);

        }
    }   
}