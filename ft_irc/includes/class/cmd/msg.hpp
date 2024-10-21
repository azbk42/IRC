#pragma once

#include "include.hpp"

class Client;

class Msg
{
    public:

        Msg(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels_list);
        ~Msg();
        bool init_cmd_msg(const std::string &value);

    private:

        int find_client_target(const std::string &target);
        Channel* find_channel_target(const std::string &target);
        bool verif_sender(const std::string &sender, Channel *chan);

        void send_message_to_client(const std::string &target, const std::string &privmsg, const std::string &sender, const std::string &server_name);
        void send_message_to_channel(const std::string &target, const std::string &privmsg, const std::string &sender, const std::string &server_name);
        
        std::vector<Client*> &_clients_list;
        std::vector<Channel*> &_channels_list;
        Client *_client_actif;
        int _fd;

};