#include "msg.hpp"



// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

Channel* Msg::find_channel_target(const std::string &target)
{
    int fd;
    for (int i = 0; i < _channels_list.size(); i++){
        std::string name = to_uppercase(_channels_list[i]->get_name());
        if (name == to_uppercase(target)){
            return _channels_list[i];
        }
    }
    return CHANNEL_NOT_FOUND;
}

int Msg::find_client_target(const std::string &target)
{
    int fd;
    for (int i = 0; i < _clients_list.size(); i++){
        std::string name = to_uppercase(_clients_list[i]->get_nickname());
        if (name == to_uppercase(target)){
            return _clients_list[i]->get_socket_fd();
        }
    }
    return CLIENT_NOT_FOUND;
}

void Msg::send_message_to_client(const std::string &target, const std::string &privmsg, const std::string &sender, const std::string &server_name)
{

    int target_fd;
    std::string message;
    target_fd = find_client_target(target);

    if (target_fd == CLIENT_NOT_FOUND){
        std::string message = ERR_NOSUCHNICK(server_name, sender, target);
        send(_fd, message.c_str(), message.size(), 0);
    }
    else{
        std::string privmsg_message = ":" + sender + " PRIVMSG " + target + " :" + privmsg + "\r\n";
        send(target_fd, privmsg_message.c_str(), privmsg_message.size(), 0);
    }

}
void Msg::send_message_to_channel(const std::string &target, const std::string &privmsg, const std::string &sender, const std::string &server_name)
{
    Channel *chan = NULL;

    chan = find_channel_target(target);
    if (chan != CHANNEL_NOT_FOUND){
        int client_fd = _client_actif->get_socket_fd();

        std::string message = ":" + sender + "!" + _client_actif->get_username() + "@" + server_name + " PRIVMSG " + target + " :" + privmsg + "\r\n";

        chan->send_message_to_all(message, client_fd);
    }
    

}

bool Msg::init_cmd_msg(const std::string &value)
{
    size_t pos = value.find(" ");
    std::string target = value.substr(0, pos);
    std::string privmsg = value.substr(pos + 2);

    std::string sender = _client_actif->get_nickname();
    std::string server_name = SERVER_NAME;

    if (target[0] == '#'){
        send_message_to_channel(target, privmsg, sender, server_name);
    }
    else{
        send_message_to_client(target, privmsg, sender, server_name);
    }
   

    return true;
}



// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Msg::Msg(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels_list):
        _clients_list(clients_list), _client_actif(&client_actif), _fd(client_fd), _channels_list(channels_list)
{

}

Msg::~Msg() {} ;