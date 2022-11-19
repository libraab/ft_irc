#include "Cmd.hpp"

bool	is_in_set(char c)
{
	string set = "`|^_-{}[]\\";
	for (int i = 0; i < (int)set.length(); i++)
	{
		if (c == set[i])
			return true;
	}
	return false;
}

bool	check_nickname_validity(string n)
{
	if (n.length() > 9 ||  isalpha(n[0]) == false )
		return false;
	for (int i = 0; i < (int)n.length(); i++)
	{
		if (isalnum(n[i]) == false && is_in_set(n[i]) == false)
			return false;
	}
	return true; 
}

void	Cmd::nick_cmd(vector<string> arg, Client *client, Server *server)
{
	if (arg.size() == 1 ) {
		server->send_error("431", client->get_nick(), "No nickname given", client->get_fd() );
		return ;
	}

	if (arg.size() >= 2) {
        if (server->client_exist(arg[1])) {
            server->send_error_with_arg( "433", client->get_nick(), arg[1], "Nickname is already in use", client->get_fd());
                return ;
        }
        if (!check_nickname_validity(arg[1])) {
            server->send_error_with_arg( "432", client->get_nick(), arg[1], "Erroneus nickname", client->get_fd());
            return ;
        }
		else {
			server->send_reply( client->get_nick(), client->get_user(), "NICK", arg[1], client->get_fd());
			client->set_nick(arg[1]);
		}
	}
} 