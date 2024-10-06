#include "channel.hpp"
#include "define.hpp"


// ################################################################################
// #                                   GET                                        #
// ################################################################################
const std::string Channel::get_name() const {return _name_channel;};

std::string Channel::get_topic() const {return _topic;};

std::string Channel::get_password() const {return _password;};

bool Channel::get_pass() const {return _pass;};

// ################################################################################
// #                                                       #
// ################################################################################
void Channel::modif_topic(const std::string &topic)
{
    _topic = topic;
    // envoyer le topic a tous les utilisateurs
}

// ################################################################################
// #                                 WELCOME MESSAGE                              #
// ################################################################################

void Channel::send_welcome_message(const std::string &client, const int fd_client)
{
    // Message de bienvenue
    std::string welcome_msg = ":" + client + " JOIN :" + _name_channel + "\r\n";
    send(fd_client, welcome_msg.c_str(), welcome_msg.length(), 0);

    // Envoyer le sujet du canal s'il y en a un
    if (!_topic.empty()) {
        std::string topic_msg = ":" + std::string("localhost") + " 332 " + client + " " + _name_channel + " :" + _topic + "\r\n";
        send(fd_client, topic_msg.c_str(), topic_msg.length(), 0);
    }

    // Construire la liste des utilisateurs avec les opérateurs marqués par '@'
    std::string name_list_msg = ":" + std::string("localhost") + " 353 " + client + " = " + _name_channel + " :";
    for (std::map<std::string, int>::iterator it = _client.begin(); it != _client.end(); ++it) {
        if (std::find(_operator.begin(), _operator.end(), it->first) != _operator.end()) {
            name_list_msg += "@" + it->first + " ";  // Marquer les opérateurs avec '@'
        } else {
            name_list_msg += it->first + " ";  // Ajouter les utilisateurs normaux
        }
    }
    name_list_msg += "\r\n";
    send(fd_client, name_list_msg.c_str(), name_list_msg.length(), 0);

    // Indiquer la fin de la liste des utilisateurs
    std::string end_of_names_msg = ":" + std::string("localhost") + " 366 " + client + " " + _name_channel + " :End of /NAMES list\r\n";
    send(fd_client, end_of_names_msg.c_str(), end_of_names_msg.length(), 0);
}

// ################################################################################
// #                                                                              #
// ################################################################################

void Channel::add_client(const std::string &name, const int fd_client)
{
    this->_client[name] = fd_client;
    this->_nb_client += 1;
    if (_nb_client == 1){
        _operator.push_back(name);
    }
    

}

// ################################################################################
// #                             CONSTRUCTOR DESTRUCTOR                           #
// ################################################################################

Channel::Channel(std::string &name): 
    _name_channel(name), _topic(""), _password(""), _pass(false), _nb_client(0)
{

}

Channel::~Channel()
{

}