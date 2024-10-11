#pragma once

#include "include.hpp"

class Client;

class Msg
{
    public:

        Msg(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        ~Msg();
        bool init_cmd_msg(const std::string &value);

    private:
        int find_target(const std::string &target);

        std::vector<Client*> &_clients_list;
        Client *_client_actif;
        int _fd;

};