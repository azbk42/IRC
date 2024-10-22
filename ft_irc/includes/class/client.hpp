#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include "define.hpp"
#include "include.hpp"
 
class Channel;

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
        bool GetFirstNick() const;

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
        void SetFirstNick();
        
        void add_nb_channel();
        void minus_nb_channel();
        bool check_nb_chan();

        time_t get_connection_time() const;

        void handle_cmd_nick(const std::string &new_nickname, int client_socket);

    private:
        
        bool _user_setup;
        bool _away;
        bool _connected;
        bool _first_nick;
        
        std::string _real_name;
        std::string _server_name;
        std::string _host_name;
        std::string _nickname;
        std::string _username;
        
        bool _checked_pwd;
        int _nb_chan;
        int _socket_fd;

        time_t _connection_time;
};

