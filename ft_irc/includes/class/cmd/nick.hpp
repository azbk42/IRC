#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include "define.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "parse.hpp"
#include "include.hpp"

class Channel;
class Client;
class Parse;
class server;

class Nick
{
    public:

        Nick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
        ~Nick();
        bool init_cmd_nick(const std::string &new_nickname);

    private:

    std::vector<Client*> &_clients_list;
    std::vector<Channel*> &_channels_list;
    Client *_client_actif;
    int _fd;

    bool check_all_errors(const std::string &new_nickname);
    bool modification_actual_nickname(const std::string &new_nickname);
        
};
