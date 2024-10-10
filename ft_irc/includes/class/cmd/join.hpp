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

class Join
{
    public:
        Join(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels, std::string value);
        ~Join();
        bool init_cmd_join();

    private:
        std::vector<Client*> &_clients_list;
        std::vector<Channel*> &_channels_list;
        std::string _value;
        Client *_client_actif;
        int _fd;

        std::map<std::string, std::string> _init_channel_map(std::string str);
        // std::vector<std::string> _split_by_comma(const std::string &input);
        bool _check_invalid_char_join(const std::string &chan_name, int client_fd, Client &client_actif);
        bool _process_channel(const std::string &chan_name);
        void creation_channel(std::string channel_name, std::string nickname);
};
