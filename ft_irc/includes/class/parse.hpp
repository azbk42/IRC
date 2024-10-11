#pragma once

#include "include.hpp"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>


class Channel;
class Client;
class Server;
class Bot;
class Msg;
class Whois;

class Parse
{
    public:

        Parse(const std::string &str);    
        ~Parse();

        std::string get_cmd() const;
		std::string get_value() const;

        // pour le parsing traiter le 1er mot comme si il etait en majuscule tout le temps
        // function pour separer le premier mot du reste de la phrase (car dans IRC le premier mot est toujours une commande)
        void split_cmd_value(const std::string &_str);
        
        bool parse_whois(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        bool parse_msg(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        bool parse_nick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels, Server* server);
        bool parse_user(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        bool parse_ping(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        bool parse_quit(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        // channel
        bool parse_join(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
		bool parse_part(std::vector<Client*> &clients_list, int client_fd, Client *client_actif, std::vector<Channel*> &channels);
        // bot
        bool parse_bot(int client_fd, Client &client_actif, Bot bot);
    private:
    
        Parse();

        const std::string _str;
        std::string _command;
        std::string _value;

        void extract_user_info(const std::string& value, Client& client_actif);

};