#pragma once
#include "../../../ft_irc.hpp"
#include "../Channel.hpp"

class Server;
class Channel;

class Cmd
{
    public:
        Cmd(void);
        ~Cmd(void);

        void exec_command(string buf, Client *client, Server *server);
        void join_cmd(vector<string> arg, Client *client, Server *server);
        void quit_cmd(Client *client, Server *server);
        void part_cmd(vector<string> arg, Client *client, Server *server);
        void privmsg_cmd(vector<string> arg, Client *client, Server *server);
        void topic_cmd(vector<string> arg, Client *client, Server *server);
        void notice_cmd(vector<string> arg, Client *client, Server *server);
        void mode_cmd(vector<string> arg, Client *client, Server *server);
        void nick_cmd(vector<string> arg, Client *client, Server *server);
        void oper_cmd(vector<string> arg, Client *client, Server *server);
        void list_cmd(vector<string> arg, Client *client, Server *server);

        //commandes non urgente a faire
        // void oper_cmd(vector<string> arg, Client *client, Server *server);
        // void nick_cmd(vector<string> arg, Client *client, Server *server);
        // void kill_cmd(vector<string> arg, Client *client, Server *server);
};
