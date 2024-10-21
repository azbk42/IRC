#include "join.hpp"

// ################################################################################
// #                                PRIVATE METHOD                                #
// ################################################################################

std::map<std::string, std::string> Join::_init_channel_map(std::string str, std::vector<std::string> &cannaux)
{
    std::string names;
    std::string passwords;
    std::map<std::string, std::string> channels;
    
    size_t pos = str.find(" ");
    if (pos != std::string::npos) {
        names = str.substr(0, pos);
        passwords = str.substr(pos + 1);
    } else {
        names = str;
    }

    // Separer les noms des canaux des password
    std::vector<std::string> channels_list = split_by_comma(names);
    std::vector<std::string> passwords_list = split_by_comma(passwords);

    for (size_t i = 0; i < channels_list.size(); ++i) {
        cannaux.push_back(channels_list[i]);
        if (i < passwords_list.size()) {
            channels[channels_list[i]] = passwords_list[i];
        } else {
            channels[channels_list[i]] = "";
        }
    }
    
    return channels;
}

bool Join::_check_invalid_char_join(const std::string &chan_name, int client_fd, Client &client_actif)
{
    if (chan_name.length() > 50){
        std::string error_message = ERR_CHANNELNAMETOOLONG(client_actif.get_nickname(), chan_name);
        send_message(client_fd, error_message);
        return false;
    }
    // check si ya un # au debut du nom
    if (chan_name.length() > 0 && chan_name[0] != '#') {
        std::string error_message = ERR_NOSUCHCHANNEL(client_actif.get_nickname(), chan_name);
        send_message(client_fd, error_message);
        return false;
    }
    // check si apres le # il ny a rien
    if (chan_name.length() > 0 && chan_name[0] == '#' && chan_name[1] == '\0') {
        std::string error_message = ERR_NOSUCHCHANNEL(client_actif.get_nickname(), chan_name);
        send_message(client_fd, error_message);
        return false;
    }

    // checi si , ou : ou space
    if (chan_name.find(",") != std::string::npos || chan_name.find(":") != std::string::npos){
        std::string error_message = ERR_NOSUCHCHANNEL(client_actif.get_nickname(), chan_name);
        send_message(client_fd, error_message);
        return false;
    }
    // char non printable
    for (size_t i = 0; i < chan_name.size(); i++) {
        if (chan_name[i] < 32 || chan_name[i] > 127) {
            std::string error_message = ERR_NOSUCHCHANNEL(client_actif.get_nickname(), chan_name);
            send_message(client_fd, error_message);
            return false;
        }
    }
    return true;
}

void Join::creation_channel(std::string channel_name, std::string nickname)
{
    Channel *chan = new Channel(channel_name);
    _channels_list.push_back(chan);
    int len = _channels_list.size() - 1; // -> on doit calculer a chaque fois pour bien utiliser les method du dernier channel de la list ( celui quon vient de creer)
    _channels_list[len]->add_client(nickname, _fd, *_client_actif);
}

bool Join::check_invit_channel(Channel* channel, const std::string& nickname, const std::string& channel_name)
{
    if (channel->get_i() == true){
        if (channel->authorization_check(nickname) == false) {
            std::string error_message = ERR_INVITEONLYCHAN(std::string(SERVER_NAME), nickname ,channel_name);
            send_message(_fd, error_message);
            return false;
        }
    }
    return true;
}

bool Join::_check_channel_access(Channel* channel, const std::string& nickname, const std::string& channel_name, const std::string& password) 
{
    if (channel->get_pass() == true){
        if (channel->get_password() != password) {
            std::string error_message = ":" + std::string(SERVER_NAME) + " 475 " + nickname + " " + channel_name + " :Cannot join channel (+k) - incorrect channel key\r\n";
            send_message(_fd, error_message);
            return false;
        }
    }
    
    return true;
}

bool Join::check_limit_channel(Channel* channel, const std::string &nickname, const std::string &channel_name)
{
    int limit = channel->get_limite(); 
    if (limit != -1){
        if (channel->get_nb_client() >= (size_t)limit){
            std::string error_message = ERR_CHANNELISFULL(std::string(SERVER_NAME), nickname, channel_name);
            send_message(_fd, error_message);
            return false;
        }
    }
    return true;
}

void Join::channel_already_exist(Channel* channel, const std::string &nickname, const std::string &channel_name, const std::string &password)
{
    if (channel->get_pass() == true){
        if (!_check_channel_access(channel, nickname, channel_name, password)){
            return ;
        }
    }
    if (check_invit_channel(channel, nickname, channel_name) == false){
        return ;
    }

    if (check_limit_channel(channel, nickname, channel_name) == false){
        return ;
    }
    channel->add_client(nickname, _fd, *_client_actif);
}

void Join::_process_channel(const std::string &chan_name, const std::string &password)
{
    std::string nickname = _client_actif->get_nickname();

    if (_client_actif->check_nb_chan() == false){
        std::string suppr_channel_message = ERR_TOOMANYCHANNELS(std::string(SERVER_NAME), nickname, chan_name);
        send_message(_fd, suppr_channel_message);
        return ;
    }

    std::string channel_name = chan_name.substr(0);
    // ajout channel existant
    for (size_t i = 0; i < _channels_list.size(); i++){

        if (to_uppercase(_channels_list[i]->get_name()) == to_uppercase(channel_name)){
            channel_already_exist(_channels_list[i], nickname, chan_name, password);
            return;
        }
    }
    // ajout channel non cree
    creation_channel(channel_name, nickname);
}
bool Join::init_cmd_join()
{
    std::map<std::string, std::string> map_channel; 
    std::vector<std::string> cannaux;
    map_channel = _init_channel_map(_value, cannaux);
    
    for (size_t i = 0; i < cannaux.size(); i++) {
        if (_check_invalid_char_join(cannaux[i], _fd, *_client_actif) == false){
            continue;
        }
        _process_channel(cannaux[i], map_channel[cannaux[i]]);
    }

    return true;
}


// ################################################################################
// #                             CONSTRUCTOR DESTRUCTOR                           #
// ################################################################################

Join::Join(int client_fd, Client &client_actif, std::vector<Channel*> &channels, std::string value):
        _channels_list(channels),_value(value) ,_client_actif(&client_actif),_fd(client_fd)
{
       
}

Join::~Join() {};