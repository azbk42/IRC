#pragma once

#include "include.hpp"

class Client;

class Whois
{
    public:

        Whois(std::vector<Client*> &clients_list, int client_fd, Client &client_actif);
        ~Whois();
        
        bool init_cmd_msg(const std::string &value);

    private:

        int find_target(const std::string &target);
        bool process_two_arg(const std::string &value, const std::string &server_name, const std::string &sender);
        bool process_one_arg(const std::string &value);
        void send_whois_message(const std::string &target);

        std::vector<Client*> &_clients_list;
        Client *_client_actif;
        int _fd;

};