#include "Cmd.hpp"

//******************************//
//   CONSTRUCTOR / DESTRUCTOR	//
//******************************//
Cmd::Cmd(string cmd) {
    command = cmd;
    return;
}
Cmd::~Cmd(void) {
    return;
}
//******************************//
// 	    F U N C T I O N S	    //
//******************************//
void Cmd::join_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() == 1) {
        server->send_error("461", client->get_nick(), "not enough arg", client->get_fd());
    }
    if (!server->channel_exist(arg[1]))
    {
        Channel *channel = new Channel(arg[1]);
        channel->add_nick_to_channel(client->get_nick());
        if (arg.size() > 2)
        {
            cout << "entered here" << endl;
            channel->set_topic(arg[2]);
        }
        server->send_reply(client->get_nick(), client->get_user(), "JOIN", arg[1], client->get_fd());
        string to_send = ":localhost 353 " + client->get_user() + " = " + arg[1] + " :@" + client->get_nick() + "\r\n";
        send(client->get_fd(), to_send.c_str(), to_send.length(), 0);
        server->send_error_with_arg("366", client->get_nick(), arg[1], "End of /NAMES list", client->get_fd());
        server->send_error_with_arg("332", client->get_nick(), arg[1], "No topic set", client->get_fd());
    }
    else {
        server->get_channel_ptr_by_channel_name(arg[1])->add_nick_to_channel(client->get_nick());
        for (map<int, Client *>::iterator it = server->client.begin(); it != server->client.end(); it++) {
            if (it->second->get_fd() != client->get_fd())
                server->send_reply(client->get_nick(), client->get_user(), "JOIN", arg[1], it->second->get_fd()); // dire aux autres qlq a join le channel
            else {
                server->send_reply(client->get_nick(), client->get_user(), "JOIN", arg[1], client->get_fd());
                string to_send = ":localhost 353 " + client->get_user() + " = " + arg[1] + " :@" + client->get_nick() + "\r\n";
                send(client->get_fd(), to_send.c_str(), to_send.length(), 0);
                server->send_error_with_arg("366", client->get_nick(), arg[1], "End of /NAMES list", client->get_fd());
                server->send_error_with_arg("332", client->get_nick(), arg[1], "No topic set", client->get_fd());
            }
        }
    }
}
void Cmd::quit_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " quit" << arg[1] << endl;
    (void)server;
}
void Cmd::part_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " part" << arg[1] << endl;
    (void)server;
}
void Cmd::privmsg_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() << " msg" << arg[1] << endl;
    (void)server;
}
void Cmd::notice_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " msg no error" << arg[1] << endl;
    (void)server;
}
void Cmd::oper_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " oper" << arg[1] << endl;
    (void)server;
}
void Cmd::mode_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " mode " << arg[1] << endl;
    (void)server;
}
void Cmd::kill_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " kill" << arg[1] << endl;
    (void)server;
}
void Cmd::topic_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() < 2)
    {
        server->send_error("461", server->client[client->get_fd()]->get_nick(), "not enough arg", client->get_fd());
        return ;
    }
  //  Channel *chan = server->get_channel_ptr_by_channel_name(arg[1])->set_topic(arg[1]);
    server->send_reply(client->get_nick(), client->get_user(), "TOPIC", arg[1], client->get_fd());
}
void Cmd::list_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " list" << arg[1] << endl;
    (void)server;
}

void Cmd::nick_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " nick" << arg[1] << endl;
    (void)server;
}
//--------------------------------------------------------------------
void Cmd::exec_command(string buf, Client *client, Server *server) {
	cout << "exec command" << endl;
    if (buf.find("\r\n") != string::npos)
        buf = buf.substr(0, buf.length() - 1);
    buf = buf.substr(0, buf.length() - 1);
    vector<string> cmd = split(buf, ' ');

    if (cmd[0] == "JOIN")
        join_cmd(cmd, client, server);
    else if (cmd[0] == "QUIT")
        quit_cmd(cmd, client, server);
    else if (cmd[0] == "PART")
        part_cmd(cmd, client, server);
    else if (cmd[0] == "PRIVMSG")
        privmsg_cmd(cmd, client, server);
    else if (cmd[0] == "NOTICE")
        notice_cmd(cmd, client, server);
    else if (cmd[0] == "TOPIC")
        topic_cmd(cmd, client, server);
    else if (cmd[0] == "LIST")
        list_cmd(cmd, client, server);
    else if (cmd[0] == "OPER")
        oper_cmd(cmd, client, server);
    else if (cmd[0] == "MODE")
        mode_cmd(cmd, client, server);
    else if (cmd[0] == "KILL")
        kill_cmd(cmd, client, server);
    else
        send(client->get_fd(), "Error: command unknown\r\n", 26, 0); 
}
//--------------------------------------------------------------------