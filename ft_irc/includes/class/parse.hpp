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
class Kick;
class Topic;
class Invite;


class Parse
{
    public:

        Parse(const std::string &str);    
        ~Parse();

        std::string get_cmd() const;
		std::string get_value() const;

        void split_cmd_value(const std::string &_str);
        
        void find_cmd_type(const std::string &cmd, std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);

        void parse_whois(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        void parse_msg(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
        void parse_nick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels, Server* server);
        bool parse_user(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        void parse_ping(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        void parse_join(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
        void parse_kick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
        void parse_topic(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
        void parse_invite(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
    
		void parse_list(int client_fd, Client &client_actif, std::vector<Channel*> &channels);
		void parse_part(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
		void parse_mode(std::vector<Client*> &clients_list, int client_fd, Client &client_actif,  std::vector<Channel*> &channels);

    private:
    
        const std::string _str;
        std::string _command;
        std::string _value;

        bool extract_user_info(const std::string& value, Client& client_actif);

        typedef void (Parse::*f)(std::vector<Client*>&, int, Client&, std::vector<Channel*>&);
        std::map<std::string, f> _command_map;

        void init_command_map();
};