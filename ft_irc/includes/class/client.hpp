#pragma once

#include <iostream>
#include <string>
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

        Client(int socket);
        ~Client();

        std::string get_real_name() const;
        std::string get_nickname() const;
        std::string get_server_name() const;
        std::string get_hostname() const;
        std::string get_username() const;

        int get_socket_fd() const;
        bool get_status_away() const;
        bool get_status_connected() const;
        bool get_user_setup() const;
		bool get_checked_pwd() const;

        void set_nickname(const std::string &nickname);
        void set_username(const std::string &username);
        void set_real_name(const std::string &name);
        void set_server(const std::string &server);
        void set_host(const std::string &host);
        void set_away(bool status);
        void set_connected(bool status);
        void set_user_setup();
		void set_checked_pwd(bool status);
        
        void add_nb_channel();
        void minus_nb_channel();
        bool check_nb_chan();

        // s'occuper de la commande USER
        void handle_cmd_user(std::string &user_infos);
        // changer le pseudo
        void handle_cmd_nick(const std::string &new_nickname, int client_socket);
        // rejoindre un channel

    private:
        
        Client();
        
        //a regarder si cest la premiere fois que ce client envois un message
        //cela voudra dire que cest pour setup ses infos    
        bool _user_setup;
        bool _away;
        bool _connected;

        std::string _real_name;
        std::string _nickname;
        std::string _username;
        std::string _server_name;
        std::string _host_name;
        int _socket_fd;
		bool _checked_pwd;
        int _nb_chan;

        //std::chrono::time_point<std::chrono::system_clock> _arrival_time;

        const std::vector<Channel*>& get_channels() const;
        // je dois build un vector de channel pour savoir ou le client se trouve
};

