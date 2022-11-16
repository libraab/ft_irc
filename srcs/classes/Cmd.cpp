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
        server->send_error("461", client->get_nick(), "Not enough argument given", client->get_fd());
        return;
    }
    else if (arg[1][0] != '#') {
        server->send_error("", client->get_nick(), "Wrong format", client->get_fd());
        return;
    }
    else
        arg[1] = arg[1].substr(1); //remove the '#' in #channel_name
    if (!server->channel_exist(arg[1]))
    {
        Channel *channel = new Channel(arg[1]);
        channel->add_user_to_channel(client);
        if (arg.size() > 2)
            channel->set_topic(arg[2]); // prendre tt ce qui reste apres nom du channel
        server->add_channel_to_server(channel);
        server->send_reply(client->get_nick(), client->get_user(), "JOIN", arg[1], client->get_fd());
        string to_send = ":localhost 353 " + client->get_user() + " = " + arg[1] + " :@" + client->get_nick() + "\r\n";
        ft_send(client->get_fd(), to_send.c_str());
        server->send_error_with_arg("366", client->get_nick(), arg[1], "End of /NAMES list", client->get_fd());
        server->send_error_with_arg("332", client->get_nick(), arg[1], channel->get_topic(), client->get_fd());
    }
    else {
        Channel *channel = server->get_channel_ptr_by_channel_name(arg[1]);
        channel->add_user_to_channel(client);
        for (map<int, Client *>::iterator it = server->client.begin(); it != server->client.end(); it++) {
            if (it->second->get_fd() != client->get_fd())
                server->send_reply(client->get_nick(), client->get_user(), "JOIN", arg[1], it->second->get_fd()); // dire aux autres qlq a join le channel
            else {
                server->send_reply(client->get_nick(), client->get_user(), "JOIN", arg[1], client->get_fd());
                string to_send = ":localhost 353 " + client->get_user() + " = " + arg[1] + " :" + channel->display_nicknames() + "\r\n"; //rajouter lop
                ft_send(client->get_fd(), to_send.c_str());
                server->send_error_with_arg("366", client->get_nick(), arg[1], "End of /NAMES list", client->get_fd());
                server->send_error_with_arg("332", client->get_nick(), arg[1], channel->get_topic(), client->get_fd());
            }
        }
    }
}
void Cmd::quit_cmd(vector<string> arg, Client *client, Server *server) {
    server->client_disconnect(client->get_fd());
    server->get_channel_ptr_by_channel_name(arg[1])->delete_user_from_channel(client->get_fd());
    // TO DO 
    // si client appartient a un channel
    // ---> verifier si le channel est vide et l'effacer
}
void Cmd::part_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() == 1) {
        server->send_error("461", client->get_nick(), "Not enough argument given", client->get_fd());
        return;
    }
    else if (arg[1][0] != '#') {
        server->send_error("", client->get_nick(), "Wrong format", client->get_fd());
        return;
    }
    else
        arg[1] = arg[1].substr(1); //remove the '#' in #channel_name
    if (server->channel_exist(arg[1])) {
        if (server->get_channel_ptr_by_channel_name(arg[1])->client_is_in_channel(client->get_fd())) {
            server->get_channel_ptr_by_channel_name(arg[1])->delete_user_from_channel(client->get_fd());
            for (map<int, Client *>::iterator it = server->client.begin(); it != server->client.end(); it++) {

            }
        }
        else
            server->send_error_with_arg("442", client->get_nick(), arg[1], "You're not on that channel", client->get_fd());
    }
    else
        server->send_error_with_arg("403", client->get_nick(), arg[1], "No such channel", client->get_fd());
    // TO DO 
    // ---> verifier si le channel est vide et l'effacer
}
void Cmd::privmsg_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() << " msg" << arg[1] << endl;
    (void)server;
}
void Cmd::notice_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " msg no error" << arg[1] << endl;
    (void)server;
}
void Cmd::mode_cmd(vector<string> arg, Client *client, Server *server) {
    cout << "client " << client->get_fd() <<  " mode " << arg[1] << endl;
    (void)server;
}
void Cmd::topic_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() < 2)
    {
        server->send_error("461", server->client[client->get_fd()]->get_nick(), "not enough arg", client->get_fd());
        return ;
    }
    server->send_reply(client->get_nick(), client->get_user(), "TOPIC", arg[1], client->get_fd());
}
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
//--------------------------------------------------------------------
void Cmd::exec_command(string buf, Client *client, Server *server) {
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
//--------------------------------------------------------------------