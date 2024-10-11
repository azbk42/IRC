#include "msg.hpp"



// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

int Msg::find_target(const std::string &target)
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

bool Msg::init_cmd_msg(const std::string &value)
{
    size_t pos = value.find(" ");
    std::string target = value.substr(0, pos);
    std::string privmsg = value.substr(pos + 2);

    std::string sender = _client_actif->get_nickname();
    std::string server_name = SERVER_NAME;
    int target_fd;

    std::string message;
    target_fd = find_target(target);

    if (target_fd == CLIENT_NOT_FOUND){
        std::string message = ERR_NOSUCHNICK(server_name, sender, target);
        send(_fd, message.c_str(), message.size(), 0);
        return false;
    }
    else{
        std::string privmsg_message = ":" + sender + " PRIVMSG " + target + " :" + privmsg + "\r\n";
        send(target_fd, privmsg_message.c_str(), privmsg_message.size(), 0);
    }

    return true;
}



// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Msg::Msg(std::vector<Client*> &clients_list, int client_fd, Client &client_actif):
        _clients_list(clients_list), _client_actif(&client_actif), _fd(client_fd)
{

}

Msg::~Msg() {} ;