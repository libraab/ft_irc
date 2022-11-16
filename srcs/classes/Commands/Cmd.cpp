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
// 	    F U N C T I O N S	    //
//******************************//
//-------------------------------------------------------------------------
// void Cmd::oper_cmd(vector<string> arg, Client *client, Server *server) {
//     cout << "client " << client->get_fd() <<  " oper" << arg[1] << endl;
//     (void)server;
// }
// void Cmd::kill_cmd(vector<string> arg, Client *client, Server *server) {
//     cout << "client " << client->get_fd() <<  " kill" << arg[1] << endl;
//     (void)server;
// }
// void Cmd::list_cmd(vector<string> arg, Client *client, Server *server) {
//     cout << "client " << client->get_fd() <<  " list" << arg[1] << endl;
//     (void)server;
// }

// void Cmd::nick_cmd(vector<string> arg, Client *client, Server *server) {
//     cout << "client " << client->get_fd() <<  " nick" << arg[1] << endl;
//     (void)server;
// }
//-------------------------------------------------------------------------
void Cmd::exec_command(string buf, Client *client, Server *server) {
    if (buf.find("\r\n") != string::npos)
        buf = buf.substr(0, buf.length() - 1);
    buf = buf.substr(0, buf.length() - 1);
    vector<string> cmd = split(buf, ' ');

    if (cmd[0] == "JOIN")
        join_cmd(cmd, client, server);
    else if (cmd[0] == "QUIT")
        quit_cmd(client, server);
    else if (cmd[0] == "PART") // with no arg
        part_cmd(cmd, client, server);
    else if (cmd[0] == "PRIVMSG") // with 1 arg = msg to channel---------with 2 arg = msg to someone --------> error handling
        privmsg_cmd(cmd, client, server);
    else if (cmd[0] == "NOTICE") // with 1 arg = msg to channel---------with 2 arg = msg to someone ---------> no error handling
        notice_cmd(cmd, client, server);
    else if (cmd[0] == "TOPIC") // with arg (change topic)---------- without arg (display topic)
        topic_cmd(cmd, client, server);
    else if (cmd[0] == "MODE")
        mode_cmd(cmd, client, server);
    // else if (cmd[0] == "OPER")
    //     oper_cmd(cmd, client, server);
    // else if (cmd[0] == "LIST")
    //     list_cmd(cmd, client, server);
    // else if (cmd[0] == "KILL")
    //     kill_cmd(cmd, client, server);
    else if (cmd[0] == "PING")
        ft_send(client->get_fd(), ":localhost PONG :localhost\r\n");
    else
        ft_send(client->get_fd(), "Error: command unknown\r\n");
}