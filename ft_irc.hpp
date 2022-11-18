#pragma once
#include "libraries.hpp"
#include "srcs/classes/Server.hpp"
#include "srcs/classes/Client.hpp"

class Server;

//******************************//
// 		  M E T H O D S 	    //
//******************************//
void start_server(Server *server, char *port);
void exec_server(Server *serv);
void error(string str); // exec error
void failed_function(string function_name);
vector<string> split(string str, char del);
void print_vector(vector<string> vec);
void remove_r(string & str);
void ft_send(int fd, string const & msg);

