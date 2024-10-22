#include "msg.hpp"

// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

Channel* Msg::find_channel_target(const std::string &target)
{
    for (size_t i = 0; i < _channels_list.size(); i++){
        std::string name = to_uppercase(_channels_list[i]->get_name());
        if (name == to_uppercase(target)){
            return _channels_list[i];
        }
    }
    return CHANNEL_NOT_FOUND;
}

int Msg::find_client_target(const std::string &target)
{
    for (size_t i = 0; i < _clients_list.size(); i++){
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
        send_message(_fd, message);
    }
    else{
        std::string privmsg_message = ":" + sender + " PRIVMSG " + target + " :" + privmsg + "\r\n";
        send_message(target_fd, privmsg_message);
    }

}

bool Msg::verif_sender(const std::string &sender, Channel *chan)
{
    std::map<std::string, int> list = chan->get_clients();

    if (list.find(sender) != list.end()) {
        return true;  
    }
    return false;
}

void Msg::send_message_to_channel(const std::string &target, const std::string &privmsg, const std::string &sender, const std::string &server_name)
{
    Channel *chan = NULL;

    chan = find_channel_target(target);
    if (chan != CHANNEL_NOT_FOUND){
        int client_fd = _client_actif->get_socket_fd();
        if (verif_sender(sender, chan) == false){
            std::string error_message = ":" + server_name + " 404 " + sender + " " + target + " :Cannot send to channel\r\n";
            send_message(client_fd, error_message);
            return;
        }
        
        std::string message = ":" + sender + "!" + _client_actif->get_username() + "@" + server_name + " PRIVMSG " + target + " :" + privmsg + "\r\n";

        chan->send_message_to_all(message, client_fd);
    }
}

bool Msg::init_cmd_msg(const std::string &value)
{
    size_t pos = value.find(" ");

    if (pos == std::string::npos) {
        std::string error_message = ERR_NORECIPIENT(std::string(SERVER_NAME), std::string("PRIVMSG"));
        send_message(_client_actif->get_socket_fd(), error_message);
        return false;
    }

    std::string target = value.substr(0, pos);
    std::string privmsg = value.substr(pos + 1);

    if (privmsg[0] != ':') {
        std::string error_message = ERR_NOTEXTTOSEND(std::string(SERVER_NAME));
        send_message(_client_actif->get_socket_fd(), error_message);
        return false;
    }

    privmsg = privmsg.substr(1);

    if (privmsg.empty()) {
        std::string error_message = ERR_NOTEXTTOSEND(std::string(SERVER_NAME));
        send_message(_client_actif->get_socket_fd(), error_message);
        return false;
    }

    std::string sender = _client_actif->get_nickname();
    std::string server_name = SERVER_NAME;

    if (privmsg.size() >= 400){
        std::string error_message = ERR_INPUTTOOLONG(server_name, sender);
        send_message(_client_actif->get_socket_fd(), error_message);
        return false;
    }

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
        _clients_list(clients_list), _channels_list(channels_list), _client_actif(&client_actif),_fd(client_fd)
{

}

Msg::~Msg() {} ;