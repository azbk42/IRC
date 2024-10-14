#pragma once

#include "include.hpp"

class Client;
class Channel;

class Kick
{
    public:

        Kick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
        ~Kick();

        void init_cmd_kick(const std::string &value);

    private:
        
        void process_kick(const std::string &canal_name, const std::string &target_name, const std::string &message);
        void kick_client_from_channel(Channel &Chan, const std::string &target_name, const std::string &message);

        Channel* find_channel(const std::string &canal_name);

        bool check_all_potential_error(const std::string &canal_name, const std::string &target_name);
        bool check_if_client_is_op(Channel &chan);
        bool check_if_target_is_in_chan(Channel &Chan, const std::string &target_name);
        bool check_if_client_is_target(const std::string &target);

        std::vector<Client*> &_clients_list;
        std::vector<Channel*> &_channels_list;
        Client *_client_actif;
        int _fd;



};