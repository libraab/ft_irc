#include "../ft_irc.hpp"

int main(int ac, char **av)
{
	Server server;

	if (ac != 3)
		cerr << "Execution error ⛔️" << endl;

	server.set_password(string(av[2]));
	start_server(&server, av[1]);
}