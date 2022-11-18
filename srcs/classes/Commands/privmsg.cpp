#include "Cmd.hpp"

void Cmd::privmsg_cmd(vector<string> arg, Client *client, Server *server) 
{
	if(!client->is_registered(server->get_password())){
		server->send_error("451", client->get_nick(), ":No such nick/channel", client->get_fd());
		return;
	}
	if (arg.size() < 2){
		server->send_error("411", client->get_nick(), ":No recipient given", client->get_fd());
		return;
	}
	if (arg.size() < 3){
		server->send_error("412", client->get_nick(), ":No text to send", client->get_fd());
		return;
	}
	if (arg[1][0] == '#'){
		string rep = arg[1];
		rep.erase(rep.begin() + 1);
		arg.insert(arg.begin() + 1, rep);
		arg.erase(arg.begin() + 2);
		
		return;
	}
}
