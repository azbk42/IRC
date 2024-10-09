#include "nick.hpp"


// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

bool Nick::check_all_errors(const std::string &new_nickname)
{
    // penser a rajouter la verif si un utilisateur porte deja le nickname au debut de la connexion
    std::string server_name = SERVER_NAME;

    if (new_nickname.empty()){
        send(_fd, ERR_NONICKNAMEGIVEN(server_name), strlen(ERR_NONICKNAMEGIVEN(server_name)), 0);
        return false;
    }

    // verif taille du pseudo
    if (new_nickname.size() > 9 || !isalpha(new_nickname[0])) {
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

    // verif si le pseudo est déjà utilisé
    for (int i = 0; i < _clients_list.size(); i++){
        if (_clients_list[i]->get_nickname() == new_nickname){
            send(_fd, ERR_NICKNAMEINUSE(server_name, new_nickname), strlen(ERR_NICKNAMEINUSE(server_name, new_nickname)), 0);
            return false;
        }
    }

    return true;

}

void Nick::send_message_to_all_one_time(const std::string &message, const int i, std::set<int> &clients_already_notified)
{
    // Parcourir tous les clients dans le canal i
    // recuperation de la liste des clients present dans le chan avec leur fd
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

    // changement du pseudo
    std::string old_nickname = _client_actif->get_nickname();

    _client_actif->set_nickname(new_nickname);
    
    // ici cest le message pour envoyer au client emeteur
    std::string confirmation_message = ":" + old_nickname + " NICK :" + new_nickname + "\n";
    send(_fd, confirmation_message.c_str(), confirmation_message.size(), 0);

    // set cest pour avoir une liste ou il ne peut pas avoir de doublon
    // donc je stock ici les fd qui ont deja recu un message pour ne pas leur envoyer deux fois
    std::set<int> clients_already_notified;
    clients_already_notified.insert(_fd);

    // send a tous les gens qui se trouve dans le meme chan
    for (int i = 0; i < _channels_list.size(); i++){
        // pour savoir si le client emeteur est dans le channel
        if (_channels_list[i]->is_in_channel(old_nickname)){
            
            std::string message = NICK_CHANGE(old_nickname, new_nickname);
            // envois le message a tous les clients du chan SAUF si ils ont deja recu un message avant 
            //( qui va etre check avec clients_already_notified)
            send_message_to_all_one_time(message, i, clients_already_notified);

            // je change le nom du client dans la <map> _client du channel
            _channels_list[i]->update_name_client(old_nickname, new_nickname);
        }
    }

    return true;
}

bool Nick::init_cmd_nick(const std::string &new_nickname)
{
    if (check_all_errors(new_nickname) == false){
        std::cerr << RED << "Error in nickname creation" << WHITE << std::endl;
        return false;
    }

    if (modification_actual_nickname(new_nickname) == false ){
        std::cerr << RED << "Error in nickname creation" << WHITE << std::endl;
        return false;
    }

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