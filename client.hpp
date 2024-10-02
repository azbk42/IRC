#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include "define.hpp"

// real_name
// username
// server
// socket_fd
// liste des canaux
// 
class Channel;
// class client qui va nous servir a savoir toute les informations propre a un client unique
// ou il se trouve, son nom, son nick, si il est abs ou connecte
class Client
{
    public:

        Client(std::string real_name, std::string server, std::string host, int socket);
        ~Client();

        std::string get_real_name() const;
        std::string get_nickname() const;
        std::string get_server_name() const;
        std::string get_host_name() const;
        int get_socket_fd() const;
        bool get_status_away() const;
        bool get_status_connected() const;

        void set_nickname(std::string _nickname);
        void set_real_name(std::string name);
        void set_server(std::string server);
        void set_host(std::string host);
        void set_away(bool status);
        void set_connected(bool status);
        void set_nickname_setup();

        // s'occuper de la commande USER
        void handle_cmd_user(std::string &user_infos);
        // changer le pseudo
        void handle_cmd_nick(const std::string &new_nickname, int client_socket);
        // rejoindre un channel
        void handle_cmd_join(const std::string &channel_name);
        // leave un channel
        void handle_cmd_part(const std::string &channel_name);
        // envoyer un mess prive
        void handle_cmd_privmsg(const std::string &message, const std::string &target);
        // quitter le serveur avec un message
        void handle_cmd_quit(const std::string &message);

    private:
        Client();
        
        //a regarder si cest la premiere fois que ce client envois un message
        //cela voudra dire que cest pour setup ses infos    
        bool _nickname_setup;
        bool _away;
        bool _connected;

        std::string _real_name;
        std::string _nickname;
        std::string _server_name;
        std::string _host_name;
        int _socket_fd;

        std::chrono::time_point<std::chrono::system_clock> _arrival_time;

        const std::vector<Channel*>& get_channels() const;
        // je dois build un vector de channel pour savoir ou le client se trouve
};

