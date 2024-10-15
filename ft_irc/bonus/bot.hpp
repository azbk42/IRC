#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "client.hpp"

#define BOT_NAME "HelpMaster"
#define RED "\e[1;31m" //-> for red color
#define GREEN "\e[1;32m" //-> for green color


class Bot
{
    public:

        Bot(int port,const std::string &passsword);
        ~Bot();

        void connect_to_server();
        void run();
        
    private:

        int _socket_fd;
        const std::string _server_ip;
        const int _port;
        const std::string _channel_name;
        const std::string _bot_name;
        const std::string _password;

        void authenticate();
        void join_channel();
        void handle_server_response();
        void send_message(const std::string &message);

        // Ajout de la logique des commandes de la classe Bot
        void handle_time();
        void handle_help();
        void handle_heads();
        void handle_tails();

        typedef void (Bot::*CommandHandler)();
        CommandHandler commandHandlers[4];
        void initialize_command_handlers();

        bool process_command(const std::string &command);
        std::string time_spent_on_server() const;

};