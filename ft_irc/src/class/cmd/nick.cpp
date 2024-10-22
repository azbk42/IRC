#include "nick.hpp"


// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

bool Nick::check_all_errors(const std::string &new_nickname)
{
    std::string server_name = SERVER_NAME;

    if (new_nickname.empty()){
        send_message(_fd, ERR_NONICKNAMEGIVEN(server_name));
        return false;
    }

    if (new_nickname.size() > 15 || !isalpha(new_nickname[0])) {
        send_message(_fd, ERR_ERRONEUSNICKNAME(server_name, new_nickname));
        return false;
    }

    for (size_t i = 1; i < new_nickname.size(); ++i) {
        if (!isalnum(new_nickname[i]) && new_nickname.find_first_of("-[]\\{}_|") == std::string::npos) {
            send_message(_fd, ERR_ERRONEUSNICKNAME(server_name, new_nickname));
            return false;
        }
    }

    std::string uppercase_nickname = to_uppercase(new_nickname);
    std::string nick_compare;

    for (size_t i = 0; i < _clients_list.size(); i++){
        nick_compare = _clients_list[i]->get_nickname();
        if (to_uppercase(nick_compare) == uppercase_nickname){
            send_message(_fd, ERR_NICKNAMEINUSE(server_name, new_nickname));
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

        if (clients_already_notified.find(client_fd) == clients_already_notified.end()) {
            
            send_message(client_fd, message);

            clients_already_notified.insert(client_fd);
        }
    }
}


bool Nick::modification_actual_nickname(const std::string &new_nickname)
{
    std::string old_nickname = _client_actif->get_nickname();

    _client_actif->set_nickname(new_nickname);
    
    std::string confirmation_message = ":" + old_nickname + " NICK :" + new_nickname + "\n";
    send_message(_fd, confirmation_message);

    std::set<int> clients_already_notified;
    clients_already_notified.insert(_fd);

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