#include "Cmd.hpp"

//******************************//
//   CONSTRUCTOR / DESTRUCTOR	//
//******************************//
Cmd::Cmd(void) {
    return;
}
Cmd::~Cmd(void) {
    return;
}
//******************************//
// 		  M E T H O D S 	    //
//******************************//
void Cmd::exec_command(string buf, Client *client, Server *server) {
    if (buf.find("\r\n") != string::npos)
        buf = buf.substr(0, buf.length() - 1);
    buf = buf.substr(0, buf.length() - 1);
    vector<string> cmd = split(buf, ' ');

    if (cmd[0] == "JOIN")
        join_cmd(cmd, client, server);
    else if (cmd[0] == "QUIT")
        quit_cmd(client, server);
    else if (cmd[0] == "PART") 
        part_cmd(cmd, client, server);
    else if (cmd[0] == "PRIVMSG")
        privmsg_cmd(cmd, client, server);
    else if (cmd[0] == "NOTICE")
        notice_cmd(cmd, client, server);
    else if (cmd[0] == "TOPIC")
        topic_cmd(cmd, client, server);
    else if (cmd[0] == "MODE")
        mode_cmd(cmd, client, server);
    else if (cmd[0] == "NICK")
        nick_cmd(cmd, client, server);
    else if (cmd[0] == "LIST")
        list_cmd(cmd, client, server);
    else if (cmd[0] == "OPER")
        oper_cmd(cmd, client, server);
    else if (cmd[0] == "KILL")
        kill_cmd(cmd, client, server);
    else if (cmd[0] == "PING")
        ft_send(client->get_fd(), ":localhost PONG :localhost\r\n");
    else
        ft_send(client->get_fd(), "Error: command unknown\r\n");
}