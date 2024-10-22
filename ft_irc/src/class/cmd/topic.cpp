#include "topic.hpp"

// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

bool Topic::check_channel_name(const std::string &name)
{
    std::string name_upper = to_uppercase(name);
    
    for (size_t i = 0; i < _channels_list.size(); i++){
        std::string chan_name = to_uppercase(_channels_list[i]->get_name());
        if (chan_name == name_upper){
            return true;
        }
    }
    return false;

}

bool Topic::is_in_op_list(const std::vector<std::string> &operator_list)
{
    std::string client_name = to_uppercase(_client_actif->get_nickname());
    std::string name_operator;
    for (size_t i = 0; i < operator_list.size(); i++){
        name_operator = to_uppercase(operator_list[i]);
        if (name_operator == client_name){
            return true;
        }
    }
    return false;
}

Channel* Topic::get_channel(const std::string &channel_name)
{
    Channel *chan = NULL;
    std::string chan_name_up = to_uppercase(channel_name);
    std::string name_list_up;
    for (size_t i = 0; i < _channels_list.size(); i++){
        name_list_up = to_uppercase(_channels_list[i]->get_name());
        if (name_list_up == chan_name_up){
            chan = _channels_list[i];
            break;
        }       
    }
    return chan;
}

bool Topic::is_in_topic_mode(Channel &chan)
{
    if (chan.get_t() == true){
        std::vector<std::string> operator_list = chan.get_operator();
        if (is_in_op_list(operator_list) == false){
            return false;
        }
    }

    return true;
}

void Topic::init_topic(const std::string &value)
{

    std::string channel_name;
    std::string new_topic;
    (void)_fd;
    (void)_clients_list;

    size_t pos = value.find(" ");
    if (pos != std::string::npos){
        channel_name = value.substr(0, pos);
        new_topic = value.substr(pos + 1);
    }
    else{
        channel_name = value;
        new_topic = "";
    }

    std::string server_name = SERVER_NAME;
    std::string message;

    std::string client_name = _client_actif->get_nickname();
    int client_fd = _client_actif->get_socket_fd();

    if (check_channel_name(channel_name) == false){
        message = ERR_NOSUCHCHANNEL_TOPIC(server_name, client_name, channel_name);
        send_message(client_fd, message);
        return;
    }
    Channel *chan = get_channel(channel_name);
    if (chan != NULL){
        if (is_in_topic_mode(*chan) == false){
            message = ERR_CHANOPRIVSNEEDED(server_name, client_name, channel_name);
            send_message(client_fd, message);
            return;
        }
        else{
            message = ":" + _client_actif->get_nickname() + "!" + _client_actif->get_username() + "@" + _client_actif->get_hostname() + 
                        " TOPIC " + channel_name + " " + new_topic + "\r\n";
            chan->set_topic(new_topic);
            chan->send_message_to_all(message, -1);
        }
    }
}

// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Topic::Topic(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels):
        _clients_list(clients_list), _channels_list(channels),  _client_actif(&client_actif), _fd(client_fd)
{
    

}

Topic::~Topic() {} ;