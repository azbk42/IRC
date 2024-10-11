#include "join.hpp"

// ################################################################################
// #                                PRIVATE METHOD                                #
// ################################################################################

std::vector<std::string> Join::_split_by_comma(const std::string &input) 
{
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

std::map<std::string, std::string> Join::_init_channel_map(std::string str)
{
    std::string names;
    std::string passwords;
    std::map<std::string, std::string> channels;
    // Séparer les noms des canaux et les mots de passe par un espace
    size_t pos = str.find(" ");
    if (pos != std::string::npos) {
        names = str.substr(0, pos);           // Partie contenant les noms des canaux
        passwords = str.substr(pos + 1);      // Partie contenant les mots de passe
    } else {
        names = str;                          // Si aucun mot de passe n'est fourni
    }

    // Séparer les noms des canaux
    std::vector<std::string> channels_list = _split_by_comma(names);
    std::vector<std::string> passwords_list = _split_by_comma(passwords);

    if (passwords_list.empty()){
        std::cout << "password empty" << std::endl;
    }
    
    // Remplir la map avec les canaux et les mots de passe
    for (size_t i = 0; i < channels_list.size(); ++i) {
        if (i < passwords_list.size()) {
            channels[channels_list[i]] = passwords_list[i];  // Assigner le mot de passe si disponible
        } else {
            channels[channels_list[i]] = "";  // Aucun mot de passe fourni, chaîne vide
        }
    }
    
    return channels;
}

bool Join::_check_invalid_char_join(const std::string &chan_name, int client_fd, Client &client_actif)
{

    if (chan_name.length() > 50){
         std::string error_message = ERR_CHANNELNAMETOOLONG(client_actif.get_nickname(), chan_name);
        send(client_fd, error_message.c_str(), error_message.size(), 0);
        std::cout << "LEN TROP GRANDE" << std::endl;
        return false;
    }
    // check si ya un # au debut du nom
    if (chan_name.length() > 0 && chan_name[0] != '#') {
        std::string error_message = ERR_NOSUCHCHANNEL(client_actif.get_nickname(), chan_name);
        send(client_fd, error_message.c_str(), error_message.size(), 0);
        std::cout << "NO DOLLS" << std::endl;
        return false;
    }
    // check si apres le # il ny a rien
    if (chan_name.length() > 0 && chan_name[0] == '#' && chan_name[1] == '\0') {
        std::string error_message = ERR_NOSUCHCHANNEL(client_actif.get_nickname(), chan_name);
        send(client_fd, error_message.c_str(), error_message.size(), 0);
        std::cout << "BACK 0 FIND" << std::endl;
        return false;
    }

    // checi si , ou : ou space
    if (chan_name.find(",") != std::string::npos || chan_name.find(":") != std::string::npos){
        std::string error_message = ERR_NOSUCHCHANNEL(client_actif.get_nickname(), chan_name);
        send(client_fd, error_message.c_str(), error_message.size(), 0);
        std::cout << "INVALID CHARACTER" << std::endl;
        return false;
    }
    // char non printable
    for (size_t i = 0; i < chan_name.size(); i++) {
        if (chan_name[i] < 32 || chan_name[i] > 127) {
            std::string error_message = ERR_NOSUCHCHANNEL(client_actif.get_nickname(), chan_name);
            send(client_fd, error_message.c_str(), error_message.size(), 0);
            std::cout << "NON-PRINTABLE CHARACTER DETECTED" << std::endl;
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

bool Join::_process_channel(const std::string &chan_name)
{
    std::string nickname = _client_actif->get_nickname();

    if (_client_actif->check_nb_chan() == false){
        std::cout << RED << "ERROR TO MUCH CHAN" << WHITE << std::endl;
        std::string error_message = ERR_TOOMANYCHANNELS(nickname, chan_name);
        send(_fd, error_message.c_str(), error_message.size(), 0);
        return false;
    }

    std::string channel_name = chan_name.substr(1);
    // ajout channel existant
    for (int i = 0; i < _channels_list.size(); i++){
        if (_channels_list[i]->get_name() == channel_name){
            // verif si il est autorise
            if (_channels_list[i]->authorization_check(nickname) == false){
                std::string error_message = ERR_INVITEONLYCHAN(nickname, channel_name);
                send(_fd, error_message.c_str(), error_message.size(), 0);
                return false;
            }
            _channels_list[i]->add_client(nickname, _fd, *_client_actif);
            return true;
        }
    }
    // ajout channel non cree
    creation_channel(channel_name, nickname);

    return true;
}

bool Join::init_cmd_join()
{
    std::map<std::string, std::string> map_channel; 
    map_channel = _init_channel_map(_value);

    for (std::map<std::string, std::string>::const_iterator it = map_channel.begin(); it != map_channel.end(); ++it) {
       //std::cout << "Channel: " << it->first << ", Password: " << (it->second.empty() ? "No password" : it->second) << std::endl;
   
        if (_check_invalid_char_join(it->first, _fd, *_client_actif) == false){
            continue;
        }

        _process_channel(it->first);

    }
    return true;

}


// ################################################################################
// #                             CONSTRUCTOR DESTRUCTOR                           #
// ################################################################################

Join::Join(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels, std::string value):
        _clients_list(clients_list), _channels_list(channels), _client_actif(&client_actif), _fd(client_fd),
        _value(value)

{
       
}

Join::~Join() {};