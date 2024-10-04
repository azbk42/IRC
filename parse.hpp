#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include "define.hpp"
#include "client.hpp"

class Channel;
class Client;

class Parse
{
    public:
        Parse(const std::string &str);
        
        ~Parse();

        std::string get_cmd() const;
        // pour le parsing traiter le 1er mot comme si il etait en majuscule tout le temps
        // function pour separer le premier mot du reste de la phrase (car dans IRC le premier mot est toujours une commande)
        void split_cmd_value(const std::string &_str);
        
        bool parse_nick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        bool parse_user(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        bool parse_ping(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        // parsing du MDP A faire dans la class serveur
        bool parse_pass(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        bool parse_quit(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        // channel
        bool parse_join();

        bool user_cmd(const std::string &str);
        bool channel_cmd(const std::string &str);
        bool operator_cmd(const std::string &str);

    private:
    
        Parse();

        const std::string _str;
        std::string _command;
        std::string _value;

        void extract_user_info(const std::string& value, Client& client_actif);
};

typedef void (*f)(Client&, Channel&, const std::string&);
typedef bool (*f_parse)(int, const std::string&, const std::string&);

struct Command{
    std::string command;
    f func;
};

struct find_parse{
    std::string command;
    f_parse func;
};