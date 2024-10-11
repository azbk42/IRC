#pragma once

#include "include.hpp"

class Client;

class Msg
{
    public:

        Msg(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        ~Msg();
        bool init_cmd_msg();

    private:
        


};