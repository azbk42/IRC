#pragma once

#include <iostream>
#include <string>

// real_name
// username
// server
// socket_fd
// liste des canaux
// 

// class client qui va nous servir a savoir toute les informations propre a un client unique
// ou il se trouve, son nom, son nick, si il est abs ou connecte
class client
{
    public:

        client(std::string real_name, std::string server, std::string host, int socket);
        ~client();

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
        void set_nick();
        void set_away(bool status);
        void set_connected(bool status);
        void set_nickname_setup();

        void handle_user_command(std::string &user_infos);
    private:
        client();
        
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

        // je dois build un vector de channel pour savoir ou le client se trouve
};

