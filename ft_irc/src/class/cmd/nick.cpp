#include "nick.hpp"


// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

bool Nick::check_all_errors(const std::string &new_nickname)
{
    std::string server_name = SERVER_NAME;

    if (new_nickname.empty()){
        send(_fd, ERR_NONICKNAMEGIVEN(server_name), strlen(ERR_NONICKNAMEGIVEN(server_name)), 0);
        return false;
    }

    // verif taille du pseudo
    if (new_nickname.size() > 15 || !isalpha(new_nickname[0])) {
        send(_fd, ERR_ERRONEUSNICKNAME(server_name, new_nickname), strlen(ERR_ERRONEUSNICKNAME(server_name, new_nickname)), 0);
        return false;
    }

    // verif char invalid
    for (size_t i = 1; i < new_nickname.size(); ++i) {
        if (!isalnum(new_nickname[i]) && new_nickname.find_first_of("-[]\\{}_|") == std::string::npos) {
            send(_fd, ERR_ERRONEUSNICKNAME(server_name, new_nickname), strlen(ERR_ERRONEUSNICKNAME(server_name, new_nickname)), 0);
            return false;
        }
    }

    std::string uppercase_nickname = to_uppercase(new_nickname);
    std::string nick_compare;

    // verif si le pseudo est déjà utilisé
    for (size_t i = 0; i < _clients_list.size(); i++){
        nick_compare = _clients_list[i]->get_nickname();
        if (to_uppercase(nick_compare) == uppercase_nickname){
            send(_fd, ERR_NICKNAMEINUSE(server_name, new_nickname), strlen(ERR_NICKNAMEINUSE(server_name, new_nickname)), 0);
            return false;
        }
    }

    return true;
}

void Nick::send_message_to_all_one_time(const std::string &message, const int i, std::set<int> &clients_already_notified)
{
    std::map<std::string, int> map = _channels_list[i]->get_clients(); 
    
    std::map<std::string, int>::iterator it;
    
    for (it = map.begin(); it != map.end(); ++it) {
        int client_fd = it->second;

        // verif si le client a deja ete notif
        if (clients_already_notified.find(client_fd) == clients_already_notified.end()) {
            
            send(client_fd, message.c_str(), message.size(), 0);

            // add le client a la liste des clients qui ont deja recu le mess
            clients_already_notified.insert(client_fd);
        }
    }
}


bool Nick::modification_actual_nickname(const std::string &new_nickname)
{
    std::string old_nickname = _client_actif->get_nickname();

    _client_actif->set_nickname(new_nickname);
    
    // send emeteur client
    std::string confirmation_message = ":" + old_nickname + " NICK :" + new_nickname + "\n";
    send(_fd, confirmation_message.c_str(), confirmation_message.size(), 0);

    std::set<int> clients_already_notified;
    clients_already_notified.insert(_fd);

    // send a tous les gens qui se trouve dans le meme chan
    for (size_t i = 0; i < _channels_list.size(); i++){
        if (_channels_list[i]->is_in_channel(old_nickname)){
            
            std::string message = NICK_CHANGE(old_nickname, new_nickname);

            send_message_to_all_one_time(message, i, clients_already_notified);

            _channels_list[i]->update_name_client(old_nickname, new_nickname);
        }
    }

    return true;
}

bool Nick::init_cmd_nick(const std::string &new_nickname, Server *server)
{
    if (check_all_errors(new_nickname) == false || \
        modification_actual_nickname(new_nickname) == false){
        std::cerr << RED << "Error in nickname creation" << WHITE << std::endl;

        if (_client_actif->GetFirstNick() == true)
            server->CloseClientSocket(_fd);
        return false;
    }

    _client_actif->SetFirstNick();
    return true;
}

// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Nick::Nick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels):
        _clients_list(clients_list), _channels_list(channels), _client_actif(&client_actif), _fd(client_fd)
{

}

Nick::~Nick() {} ;