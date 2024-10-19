#include "kick.hpp"


// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

bool Kick::check_if_client_is_op(Channel &chan)
{
    std::string name = _client_actif->get_nickname();
    std::vector<std::string> op = chan.get_operator();
    for (int i = 0; i < op.size(); i++){
        std::string ope = to_uppercase(op[i]);
        if (ope == to_uppercase(name)){
            return true;
        }
    }
    return false;
}

bool Kick::check_if_client_is_target(const std::string &target)
{


    std::string upper_target = to_uppercase(target);
    std::string client_nick = to_uppercase(_client_actif->get_nickname());

    if (client_nick == upper_target){
        return false;
    }
    return true;
}

Channel* Kick::find_channel(const std::string &canal_name)
{
    for (int i = 0; i < _channels_list.size(); i++){
        std::string name_touper = to_uppercase(_channels_list[i]->get_name());
        if ( name_touper == to_uppercase(canal_name)){
            return _channels_list[i];
        }
    }
    return NULL;
}

bool Kick::check_if_target_is_in_chan(Channel &Chan, const std::string &target_name)
{
    std::string upper_target = to_uppercase(target_name);

    std::map<std::string, int> list_client = Chan.get_clients();
    std::map<std::string, int>::iterator it;
    for (it = list_client.begin(); it != list_client.end(); ++it){

        if (to_uppercase(it->first) == upper_target)
            return true;
    }
    return false;
}

void Kick::kick_client_from_channel(Channel &Chan, const std::string &target_name, const std::string &message)
{
    Client *client_target = NULL;
    int fd;

    for (int i = 0; i < _clients_list.size(); i++){
        std::string name_upper = to_uppercase(_clients_list[i]->get_nickname());
        if (name_upper == to_uppercase(target_name)){
            client_target = _clients_list[i];
            break;
        }
    }
    if (client_target != NULL){
        fd = client_target->get_socket_fd();
        Chan.remove_client(target_name, fd, *client_target, message);
        std::string message_to_target = ":" + _client_actif->get_nickname() + "!" + _client_actif->get_username() + "@" + _client_actif->get_hostname() + " KICK " + 
                                           Chan.get_name() + " " + client_target->get_nickname() + " :" + message + "\r\n";

        // send message kick client
        send(fd, message_to_target.c_str(), message_to_target.size(), 0);

        // send message to all except client
        std::string message_to_all = ":" + _client_actif->get_nickname() + "!" + _client_actif->get_username() + "@" + _client_actif->get_hostname() + " KICK " + 
                                        Chan.get_name() + " " + client_target->get_nickname() + " :" + message + "\r\n";
        Chan.send_message_to_all(message_to_all, fd);
    }
}

bool Kick::check_all_potential_error(const std::string &canal_name, const std::string &target_name)
{

    std::string server_name = SERVER_NAME;

    if (check_if_client_is_target(target_name) == false){
        std::cerr << RED << "Can't kick himself" << std::endl;
        std::string error_message = ERR_CANTKICKSELF(server_name, target_name, canal_name);
        send(_fd, error_message.c_str(), error_message.size(), 0);
        return false;
    }
    Channel *Chan;
    Chan = find_channel(canal_name);
    if (Chan == NULL){
        std::cerr << RED << "client doesnt exist" << std::endl;
        std::string error_message = ERR_NOSUCHCHANNEL3(server_name, _client_actif->get_nickname(), canal_name);
        send(_fd, error_message.c_str(), error_message.length(), 0);
        return false;
    }
    if (check_if_client_is_op(*Chan) == false){
        std::cerr << RED << "client not op" << std::endl;
        std::string error_message = ERR_CHANOPRIVSNEEDED(server_name, _client_actif->get_nickname(), canal_name);
        send(_fd, error_message.c_str(), error_message.length(), 0);
        return false;
    }
    if (check_if_target_is_in_chan(*Chan, target_name) == false){
        std::cerr << RED << "not in chan" << std::endl;
        std::string error_message = ERR_USERNOTINCHANNEL(server_name, _client_actif->get_nickname(), target_name, canal_name);
        send(_fd, error_message.c_str(), error_message.length(), 0);
        return false;
    }
    return true;
}

void Kick::process_kick(const std::string &canal_name, const std::string &target_name, const std::string &message)
{
    if (check_all_potential_error(canal_name, target_name) == false){
        return;
    }
    
    Channel *Chan;
    Chan = find_channel(canal_name);
    if (Chan){
        kick_client_from_channel(*Chan, target_name, message);
    }
}

void Kick::init_cmd_kick(const std::string &value)
{
    int pos = value.find(" ");
    std::string canal_name = value.substr(0, pos);
    std::string new_str = value.substr(pos +1);
    pos = new_str.find(" ");
    std::string target_name = new_str.substr(0, pos);
    std::string message = new_str.substr(pos + 2);
    
    process_kick(canal_name, target_name, message);
}

// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Kick::Kick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels):
        _clients_list(clients_list), _channels_list(channels), _client_actif(&client_actif), _fd(client_fd)
{
    

}

Kick::~Kick() {} ;