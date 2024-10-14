#pragma once

#include "include.hpp"

class Client;
class Channel;

class Topic
{
    public:

        Topic(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels);
        ~Topic();

        void init_topic(const std::string &value);

    private:

        bool check_channel_name(const std::string &name);
        bool is_in_topic_mode(Channel &chan);
        bool is_in_op_list(const std::vector<std::string> &operator_list);

        Channel* get_channel(const std::string &channel_name);

        std::vector<Client*> &_clients_list;
        std::vector<Channel*> &_channels_list;
        Client *_client_actif;
        int _fd;

};