#include "whois.hpp"

// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

int Whois::find_target(const std::string &target)
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

void Whois::send_whois_message(const std::string &target)
{
    Client *client = NULL;
    for (int i = 0; i < _clients_list.size(); i++){
        if (to_uppercase(_clients_list[i]->get_nickname()) == to_uppercase(target)){
            client = _clients_list[i];
            break;
        }
    }
    if (client != NULL){
        std::string nickname = client->get_nickname();
        std::string server_name = SERVER_NAME;     
        std::string username = client->get_username();    
        std::string hostname = client->get_hostname(); 
        std::string realname = client->get_real_name();  
        std::string target_server = client->get_server_name(); 
        std::string server_info = "FT_IRC 42 SCHOOL"; 

        // on doit envoye 3 message pour whois
        std::string whois_user = RPL_WHOISUSER(server_name, nickname, target, username, hostname, realname);
        send(_fd, whois_user.c_str(), whois_user.size(), 0);

        std::string whois_server = RPL_WHOISSERVER(server_name, nickname, target, target_server, server_info);
        send(_fd, whois_server.c_str(), whois_server.size(), 0);

        std::string end_of_whois = RPL_ENDOFWHOIS(server_name, nickname, target);
        send(_fd, end_of_whois.c_str(), end_of_whois.size(), 0);
    }
}

bool Whois::process_two_arg(const std::string &value, const std::string &server_name, const std::string &sender)
{
    int pos = value.find(" ");
    std::string server_target = value.substr(0, pos);
    if (server_target != server_name){
        std::string error_message = ERR_NOSUCHSERVER(server_name, sender, server_target);
        send(_fd, error_message.c_str(), error_message.size(), 0);
        return false;
    }
    std::string target = value.substr(pos + 1);
    // if (find_target(target) == CLIENT_NOT_FOUND){
    //     std::string message = ":" + server_name + " 401 " + sender + " " + " :No such nick/channel\r\n";
    //     send(_fd, message.c_str(), message.size(), 0);
    //     return false;
    // }
    // else{
        send_whois_message(target);
    //}
    return true;
}

bool Whois::process_one_arg(const std::string &value, const std::string &server_name, const std::string &sender)
{
    std::string target = value;
    // std::cout << "target = " << target << std::endl;
    // if (find_target(target) == CLIENT_NOT_FOUND){
    //     std::string message = ":" + server_name + " 401 " + sender + " " + " :No such nick/channel\r\n";
    //     send(_fd, message.c_str(), message.size(), 0);
    //     return false;
    // }
    // else{
        send_whois_message(target);
    //}
    return true;
}

bool Whois::init_cmd_msg(const std::string &value)
{
    std::string server_name = SERVER_NAME;
    std::string sender = _client_actif->get_nickname();
    int space_count = std::count(value.begin(), value.end(), ' ');
    if (space_count == 1){
        process_two_arg(value, server_name, sender);
    }
    else{
        process_one_arg(value, server_name, sender);
    }
    return true;
    
}



// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Whois::Whois(std::vector<Client*> &clients_list, int client_fd, Client &client_actif):
        _clients_list(clients_list), _client_actif(&client_actif), _fd(client_fd)
{

}

Whois::~Whois() {} ;