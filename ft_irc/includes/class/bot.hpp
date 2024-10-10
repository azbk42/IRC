#pragma once

#include <string>
#include <iostream>
#include <ctime>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <sys/socket.h>

#include "client.hpp"

class Bot
{
    public:

        Bot(const std::string &name);
        ~Bot();

        const std::string get_name() const;
        void handle_time(Client &client_actif);
        void handle_help(Client &client_actif);
        
        void handle_heads(Client &client_actif);
        void handle_tails(Client &client_actif);
        

    private:

    const std::string _name;

    std::string time_spent_on_server(Client &client_actif) const;

};