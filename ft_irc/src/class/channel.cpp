#include "channel.hpp"
#include "define.hpp"

// ################################################################################
// #                                   GET                                        #
// ################################################################################
const std::string Channel::get_name() const {return _name_channel;};

std::string Channel::get_topic() const {return _topic;};

std::string Channel::get_password() const {return _password;};

bool Channel::get_pass() const {return _pass;};

std::map<std::string, int> Channel::get_clients() const {return _client;};

std::vector<std::string> Channel::get_operator() const {return _operator;};

size_t Channel::get_nb_client() const {return _nb_client;};

std::time_t Channel::get_creation_date() const {return _creation_time;};

// std::vector<std::string> Channel::get_operators() const {return _operator;}
bool Channel::get_i() const {return _i;}

bool Channel::get_t() const {return _t;}


int Channel::get_limite() const {return _limite;}

void Channel::set_limite(int x){
    _limite = x;
}
std::vector<std::string> Channel::get_invite_name() const {return _invite_name;};

void Channel::set_i(const char &i)
{
    if (i == '+'){
        _i = true;
    }
    else if (i == '-'){
        _i = false;
    }
}

void Channel::set_t(const char &t)
{
    if (t == '+'){
        _t = true;
    }
    else if (t == '-'){
        _t = false;
    }
}

void Channel::set_pass(bool x) {
    _pass = x;
}

void Channel::set_password(std::string str){
    _password = str;
}

void Channel::set_topic(const std::string &value)
{
    _topic = value;
}
// ################################################################################
// #                                                       #
// ################################################################################

void Channel::add_invite(const std::string &name)
{
    _invite_name.push_back(name);
}

void Channel::minus_invite(const std::string &name)
{
    for (std::vector<std::string>::iterator it = _invite_name.begin(); it != _invite_name.end(); ++it) {
        if (to_uppercase(*it) == to_uppercase(name)) {
            _invite_name.erase(it);
            break;
        }
    }
}

void Channel::modif_topic(const std::string &topic)
{
    _topic = topic;
    // envoyer le topic a tous les utilisateurs

}

void Channel::send_message_to_all(const std::string &message, const int fd_client)
{
    for (std::map<std::string, int>::iterator it = _client.begin(); it != _client.end(); ++it) {
        // on envoie le message a tout le monde sauf le client principal
        if (it->second != fd_client) {
            send_message(it->second, message);
        }
    }
}

// ################################################################################
// #                                 WELCOME MESSAGE                              #
// ################################################################################

void Channel::send_welcome_message(const std::string &client, const int fd_client)
{
    // Message de bienvenue
    std::string welcome_msg = ":" + client + " JOIN :" + _name_channel + "\r\n";
    send_message(fd_client, welcome_msg);

    // Envoyer le sujet du canal s'il y en a un
    if (!_topic.empty()) {
        std::string topic_msg = ":" + std::string("localhost") + " 332 " + client + " " + _name_channel + " :" + _topic + "\r\n";
        send_message(fd_client, topic_msg);
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
    send_message(fd_client, name_list_msg);

    // Indiquer la fin de la liste des utilisateurs
    std::string end_of_names_msg = ":" + std::string("localhost") + " 366 " + client + " " + _name_channel + " :End of /NAMES list\r\n";
    send_message(fd_client, end_of_names_msg);
}

void Channel::send_part_message(const std::string &client, const int fd_client)
{
    // Message de bienvenue
    std::string part_msg = ":" + client + " PART :" + _name_channel + "\r\n";
    send_message(fd_client, part_msg);
}

// ################################################################################
// #                                                                              #
// ################################################################################

bool Channel::is_in_channel(const std::string &name)
{
    if (_client.find(name) != _client.end())
        return true;
    else
        return false;
}

bool Channel::is_operator(const std::string &name)
{
    for (size_t i = 0; i < _operator.size(); i++){
        if (_operator[i] == name)
            return true;
    }
    return false;
}

void Channel::add_operator(const std::string &name)
{
    _operator.push_back(name);
}

void Channel::remove_operator(const std::string &name)
{
    for (size_t i = 0; i < _operator.size(); i++){
        if (_operator[i] == name)
            _operator.erase(_operator.begin() + i);
    }
}

void Channel::remove_invite(const std::string &name)
{
    for (size_t i = 0; i < _invite_name.size(); i++){
        if (_invite_name[i] == name)
            _invite_name.erase(_invite_name.begin() + i);
    }
}

void Channel::add_client(const std::string &name, const int fd_client, Client &client_actif)
{
	this->_client[name] = fd_client;
    this->_nb_client += 1;
    std::string join_message = ":" + name + "!" + client_actif.get_username() + "@" + client_actif.get_hostname() +\
                                " JOIN :" + get_name() + "\r\n";
    send_message_to_all(join_message, fd_client);
    client_actif.add_nb_channel();
    if (_nb_client == 1){
        _operator.push_back(name);
    }
    send_welcome_message(name, fd_client);
}

int Channel::remove_client(const std::string &name, Client &client_actif)
{
    
    if (_nb_client > 1){
		_client.erase(name); // erase client from the channel's client list
		_nb_client -= 1;
		for (size_t i = 0; i < _operator.size(); i++){
			if (_operator[i] == name){ // if operator, remove from operator list
				_operator.erase(_operator.begin() + i);
				break;
			}
		}
		client_actif.minus_nb_channel();
		return 0;
	}
	if (_nb_client == 1){
    	client_actif.minus_nb_channel();
		return 1;
	}

	return 0;
}

bool Channel::authorization_check(const std::string &nickname)
{
    if (_i == true){
        
        for (size_t i = 0; i < _invite_name.size(); i++){
            if (to_uppercase(_invite_name[i]) == to_uppercase(nickname)){
                return true;
            }
        }
        return false;
    }
    return true;

}

void Channel::update_name_client(const std::string &old_nickname, const std::string &new_nickname)
{
    int client_fd = _client[old_nickname];
    _client.erase(old_nickname);
    _client[new_nickname] = client_fd;
}

// ################################################################################
// #                             CONSTRUCTOR DESTRUCTOR                           #
// ################################################################################

Channel::Channel(const std::string &name):  _pass(false), _nb_client(0),\
                _name_channel(name), _topic(""), _password(""),\
                _i(false), _limite(-1), _t(false)
{
	_creation_time = time(NULL);
}

Channel::~Channel()
{

}
