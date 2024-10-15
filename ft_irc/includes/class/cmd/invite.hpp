#pragma once

#include "include.hpp"

class Client;
class Channel;

class Invite
{
    public:

        Invite(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
        ~Invite();

        void init_cmd_invite(const std::string &value);

    private:
        
        bool check_if_chan_exist(const std::string &chan_name);
        bool check_if_target_exist(const std::string &target);
        bool check_if_client_is_op_and_in_chan(const std::string &chan_name, const std::string &client_nick);
        bool check_if_already_in_invite(const std::string &chan_name, const std::string &client_nick);
        bool check_if_already_in_channel(const std::string &chan_name, const std::string &client_nick);
        Channel* get_chan(const std::string &chan_name);

        std::vector<Client*> &_clients_list;
        std::vector<Channel*> &_channels_list;
        Client *_client_actif;
        int _fd;
        
};