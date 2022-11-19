#include "Cmd.hpp"

void Cmd::topic_cmd(vector<string> arg, Client *client, Server *server) {
	string joined = "";
	vector<string>::iterator it = arg.begin() + 2;
	while (it != arg.end()){
		joined += *it;
		joined += " ";
		it++;
	}
	joined.erase(0,1);
    if (arg.size() < 2)
    {
        server->send_error("461", server->client_list[client->get_fd()]->get_nick(), "not enough arg", client->get_fd());
        return ;
    }
	if (arg.size() > 2)
	{
		server->get_channel(arg[1])->set_topic(joined);
		string to_send = ":" + client->get_nick() + "!" + client->get_user() + "@localhost TOPIC #" + arg[1] + " :" + joined + "\r\n";
		ft_send(client->get_fd(), to_send.c_str());
	}
	if (arg.size() == 2)
  		server->send_reply(client->get_nick(), client->get_user(), "TOPIC", joined, client->get_fd());
}