#include "Cmd.hpp"

void Cmd::mode_cmd(vector<string> arg, Client *client, Server *server) {
    if(arg[1][0] == '#') { // channel mode
        string to_send = ":localhost 221 " + client->get_nick() + " +nt\r\n";
        send(client->get_fd(), to_send.c_str(), to_send.length(), 0);
        return;
    }   
    else {  // user mode
        if (arg.size() != 3) {
	        server->send_error_with_arg("461", client->get_nick(), arg[0], "Not enough parameters", client->get_fd()); 
            return;
        }
        else {
            Client *target = NULL;
            if (arg[1] == "o") {
                if (arg[2] == "123") {
                    string mode = (client->get_mode().substr(1));
                    client->set_mode("+o" + mode);
                    string to_send = ":localhost 221 " + client->get_nick() + " " + client->get_mode() + "\r\n";
                    send(client->get_fd(), to_send.c_str(), to_send.length(), 0);
                }
                return;
            }
            else if (arg[1] == client->get_nick()) // if client wants to change his own mode
                target = client;
            else {
                if (client->get_mode().find("o") != string::npos) { // if client is op
                    if (server->client_exist(arg[1])) // if target exist
                        target = server->get_client(arg[1]);
                    else {
                        server->send_error_with_arg("401", client->get_nick(), arg[1], "No such nick/channel", client->get_fd());
                        return;
                    }
                }
                else {
                    server->send_error("502", client->get_nick(), "Cant change mode for other users", client->get_fd());
                    return;
                }
            }
            string mode = target->get_mode();
            if (arg[2][0] == '-') {
                target->set_mode("");
            }
            else if (arg[2][0] == '+') {
                target->set_mode(arg[2].erase(0));
            }
            string to_send = ":localhost 221 " + target->get_nick() + " +nt\r\n";
            send(target->get_fd(), to_send.c_str(), to_send.length(), 0);
        }
    }
    return ;
}