#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <vector>
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

        // pour le parsing traiter le 1er mot comme si il etait en majuscule tout le temps
        // function pour separer le premier mot du reste de la phrase (car dans IRC le premier mot est toujours une commande)
        void split_cmd_value(const std::string &_str);
        
        
        bool parse_nick(std::vector<Client*> &clients_list, int client_fd);
        
        bool user_cmd(const std::string &str);
        bool channel_cmd(const std::string &str);
        bool operator_cmd(const std::string &str);

    private:
    
        Parse();

        const std::string _str;
        std::string _command;
        std::string _value;

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