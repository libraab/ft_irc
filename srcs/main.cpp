#include "../ft_irc.hpp"

int main(int ac, char **av)
{
	Server server;

	if (ac != 3)
	{
		//to do checker le bon range de port 
		cerr << "Execution error ⛔️" << endl;
		return (1);
	}
	server.set_password(string(av[2]));
	start_server(&server, av[1]);
}

// topic a gerer
// PRIVMSG fix : ne prenait en compte que arg[2] .
// ajouter name et liste: facil