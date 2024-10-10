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
        
        
        void process_command(const std::string &command, Client &client_actif);
    private:

    const std::string _name;

    void handle_time(Client &client_actif);
    void handle_help(Client &client_actif);
    void handle_heads(Client &client_actif);
    void handle_tails(Client &client_actif);

    std::string time_spent_on_server(Client &client_actif) const;

    typedef void (Bot::*CommandHandler)(Client &client_actif);
    CommandHandler commandHandlers[4];
    void initialize_command_handlers();

};

