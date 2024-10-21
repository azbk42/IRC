#include "invite.hpp"

// ################################################################################
// #                                    GET                                       #
// ################################################################################

Channel* Invite::get_chan(const std::string &chan_name)
{
    Channel *chan = NULL;

    std::string name_upper = to_uppercase(chan_name);
    std::string chan_name_upp;

    for (size_t i = 0; i < _channels_list.size(); i++){
        chan_name_upp = to_uppercase(_channels_list[i]->get_name());
        if (chan_name_upp == name_upper){
            chan = _channels_list[i];
        }
    }
    return chan;
}

// ################################################################################
// #                                  METHOD                                      #
// ################################################################################

bool Invite::check_if_chan_exist(const std::string &chan_name)
{

    std::string name_upper = to_uppercase(chan_name);
    std::string chan_name_upp;

    for (size_t i = 0; i < _channels_list.size(); i++){
        chan_name_upp = to_uppercase(_channels_list[i]->get_name());
        if (chan_name_upp == name_upper){
            return true;
        }
    }
    return false;
}

bool Invite::check_if_target_exist(const std::string &target)
{
    std::string name_target = to_uppercase(target);
    std::string compare;

    for (size_t i = 0; i < _clients_list.size(); i++){
        compare = to_uppercase(_clients_list[i]->get_nickname());
        if (compare == name_target){
            return true;
        }
    }
    return false;

}

bool Invite::check_if_client_is_op_and_in_chan(const std::string &chan_name, const std::string &client_nick)
{

    Channel *chan = get_chan(chan_name);

    if (chan){
        std::string server_name = SERVER_NAME;
        std::vector<std::string> ope = chan->get_operator();
        std::string client_nick_up = to_uppercase(client_nick);
        if (chan->is_in_channel(client_nick)){
            for (size_t i = 0; i < ope.size(); i++){
                std::string ope_up = to_uppercase(ope[i]);
                if (ope_up == client_nick_up){
                    return true;
                }
            }
            std::string error_message = ERR_CHANOPRIVSNEEDED_INVITE(server_name, client_nick, chan_name);
            send_message(_client_actif->get_socket_fd(), error_message);
        }
        else{
            std::string message = ERR_NOTONCHANNEL_INVITE(server_name, client_nick, chan_name);
            send_message(_client_actif->get_socket_fd(), message);
        }
    }
    return false;
}


bool Invite::check_if_already_in_invite(const std::string &chan_name, const std::string &client_nick)
{
    Channel* chan = get_chan(chan_name);

    if (chan){
        std::string nick_upper = to_uppercase(client_nick);
        std::vector<std::string> invite_array = chan->get_invite_name();
        for (size_t i = 0; i < invite_array.size(); i++){
            std::string name_upper = invite_array[i];
            if (nick_upper == name_upper){
                return false;
            }
        }
    }
    return true;;
}

bool Invite::check_if_already_in_channel(const std::string &chan_name, const std::string &client_nick)
{
    Channel* chan = get_chan(chan_name);

    if (chan){
        if (chan->is_in_channel(client_nick)){
            return false;
        }
    }
    return true;;
}

int Invite::return_fd_target(const std::string &target)
{
    std::string name_target = to_uppercase(target);
    std::string compare;

    for (size_t i = 0; i < _clients_list.size(); i++){
        compare = to_uppercase(_clients_list[i]->get_nickname());
        if (compare == name_target){
            return _clients_list[i]->get_socket_fd();
        }
    }
    return -1;
}

void Invite::add_client_to_invite(const std::string &chan_name, const std::string &client_nick)
{

    Channel *chan = get_chan(chan_name);

    if (chan != NULL){
        chan->add_invite(client_nick);
    }
}

void Invite::send_invite_confirmation(const std::string &chan_name, const std::string &target, const std::string &client_nick)
{
    // ajouter client invite list
    add_client_to_invite(chan_name, target);

    std::string invite_confirm_msg = ":" + std::string(SERVER_NAME) + " 341 " + client_nick + " " + target + " " + chan_name + "\r\n";
    send_message(_fd, invite_confirm_msg);

    int fd_target = return_fd_target(target);

    // send invite message
    std::string invite_msg = ":" + client_nick + "!" + _client_actif->get_username() + "@" + _client_actif->get_hostname() + " INVITE " + target + " :" + chan_name + "\r\n";
    send_message(fd_target, invite_msg);
}

bool Invite::check_invite_conditions(const std::string &chan_name, const std::string &target, const std::string &client_nick, int fd)
{
    std::string server_name = SERVER_NAME;

    if (check_if_chan_exist(chan_name) == false) {
        std::string error_message = ERR_NOSUCHCHANNEL_TOPIC(server_name, client_nick, chan_name);
        send_message(fd, error_message);
        return false;
    }

    if (check_if_target_exist(target) == false) {
        std::string message = ERR_NOSUCHNICK_INVITE(server_name, client_nick, target);
        send_message(fd, message);
        return false;
    }

    if (check_if_client_is_op_and_in_chan(chan_name, client_nick) == false) {
        return false;
    }

    if (check_if_already_in_invite(chan_name, client_nick) == false) {
        return false;
    }

    if (check_if_already_in_channel(chan_name, target) == false) {
        std::string error_message = ERR_USERONCHANNEL(server_name, client_nick, target, chan_name);
        send_message(fd, error_message);
        return false;
    }

    return true;
}


void Invite::init_cmd_invite(const std::string &value)
{
    int pos = value.find(" ");
    std::string target = value.substr(0, pos);
    std::string chan_name = value.substr(pos + 1);

    std::string client_nick = _client_actif->get_nickname();
    int fd = _client_actif->get_socket_fd();

    if (check_invite_conditions(chan_name, target, client_nick, fd) == false) {
        return;
    }
    
    send_invite_confirmation(chan_name, target, client_nick);
}


// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Invite::Invite(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels):
        _clients_list(clients_list), _channels_list(channels), _client_actif(&client_actif), _fd(client_fd)
{
    
}


Invite::~Invite() {} ;