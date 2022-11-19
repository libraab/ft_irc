#include "Cmd.hpp"

void Cmd::mode_cmd(vector<string> arg, Client *client, Server *server) {
    if (arg.size() == 1) {
        server->send_error_with_arg("461", client->get_nick(), arg[0], "Not enough parameters", client->get_fd()); 
        return;
    }
    else if(arg.size() == 2) { // channel mode
        if (server->channel_exist(arg[1])) {
            client->set_mode(arg[1]);
            string to_send = ":localhost 221 " + client->get_nick() + " " + client->get_mode() +  "\r\n";
            ft_send(client->get_fd(), to_send.c_str());
        }
        else {
            server->send_error_with_arg("401", client->get_nick(), arg[1], "No such nick/channel", client->get_fd());
            return;
        }
    }   
    else {  // user mode
        if (!server->channel_exist(arg[1]) && !server->client_exist(arg[1])) {
            server->send_error_with_arg("401", client->get_nick(), arg[1], "No such nick/channel", client->get_fd());
            return;
        }
        else if (arg[2].find('o') == string::npos && arg[2].find('i') == string::npos)
            return;
        else if (server->client_exist(arg[1])) {
            if (client->get_nick() != arg[1] && client->get_mode().find('o') == string::npos) {
                server->send_error("502", client->get_nick(), "Cannot change mode for other users", client->get_fd());
                    return;
            }
            else if (client->get_nick() == arg[1] && arg[2].find('o') != string::npos) { // client wants to be oper
                    if (arg.size() > 3 && arg[3] == "oper") {      
                        string mode = client->get_mode();
                        if (mode.find('o') != string::npos)
                            return;
                        client->set_mode("o" + mode);
                        string to_send = ":localhost 221 " + client->get_nick() + " +" + client->get_mode() + "\r\n";
                        ft_send(client->get_fd(), to_send.c_str());
                    }
                    else {
                        string to_send = "Permission Denied\r\n";
                        ft_send(client->get_fd(), to_send.c_str());
                        return;
                    }
            }
            else {
                if (arg[2][0] == '-') {
                    string mode = (server->get_client(arg[1])->get_mode());
                    if (mode.find(arg[2][1]) == string::npos)
                        return; // you cant remove something that is not there
                    server->get_client(arg[1])->set_mode(mode);
                    string to_send = ":localhost 221 " + server->get_client(arg[1])->get_nick() + " -" + arg[2][1] + server->get_client(arg[1])->get_mode() + "\r\n";
                    ft_send(server->get_client(arg[1])->get_fd(), to_send.c_str());
                }
                else if (arg[2][0] == '+') {
                    string mode = (server->get_client(arg[1])->get_mode());
                    if (mode.find(arg[2][1]) != string::npos)
                        return; // why adding something that is already there
                    server->get_client(arg[1])->set_mode("o" + mode);
                    string to_send = ":localhost 221 " + server->get_client(arg[1])->get_nick() + " +" + arg[2][1] + server->get_client(arg[1])->get_mode() + "\r\n";
                    ft_send(server->get_client(arg[1])->get_fd(), to_send.c_str());
                }
            }
        }
    }
}