#include "parse.hpp"
#include <string.h>
#include <stdio.h>
#include <sstream>

// ################################################################################
// #                                  GET                                         #
// ################################################################################

std::string Parse::get_cmd() const
{
    return _command;
}

// ################################################################################
// #                                   PARSING                                    #
// ################################################################################

bool Parse::parse_nick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif)
{
    std::string server_name = SERVER_NAME;
    std::string nick = _value;

    if (_value.empty()){
        send(client_fd, ERR_NONICKNAMEGIVEN(server_name), strlen(ERR_NONICKNAMEGIVEN(server_name)), 0);
        _value.clear();
        return false;
    }

    if (_value.size() > 9 || !isalpha(_value[0])) {
        // Envoi de l'erreur ERR_ERRONEUSNICKNAME
        send(client_fd, ERR_ERRONEUSNICKNAME(server_name, nick), strlen(ERR_ERRONEUSNICKNAME(server_name, nick)), 0);
        _value.clear();
        return false;
    }

    // Handle invalid character
    for (size_t i = 1; i < _value.size(); ++i) {
        if (!isalnum(_value[i]) && _value.find_first_of("-[]\{}_|") == std::string::npos) {
            send(client_fd, ERR_ERRONEUSNICKNAME(server_name, _value), strlen(ERR_ERRONEUSNICKNAME(server_name, _value)), 0);
            return false;
        }
    }
    // Handle identic nickname
    for (int i = 0; i < clients_list.size(); i++){
        if (clients_list[i]->get_nickname() == _value){
            send(client_fd, ERR_NICKNAMEINUSE(server_name, nick), strlen(ERR_NICKNAMEINUSE(server_name, nick)), 0);
            return false;
        }
    }

    // Tout est ok, on regarde quel client correspond au fd et on le change
    // Il faut maintenant avertir tous les clients des meme channels
    for (int i = 0; i < clients_list.size(); i++){
        if (clients_list[i]->get_socket_fd() == client_fd){
            clients_list[i]->handle_cmd_nick(_value, client_fd);
        }
    }

    return true;
}

bool Parse::check_invalid_char_join(const std::string &chan_name, int client_fd, Client &client_actif)
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
    if (chan_name.find(",") != std::string::npos ||\
        chan_name.find(":") != std::string::npos ||\
        chan_name.find(" ") != std::string::npos) {
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

std::vector<std::string> Parse::split_by_comma(const std::string &input) 
{
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

std::map<std::string, std::string> Parse::init_channel_map(std::map<std::string, std::string> channels, std::string str)
{
    std::string names;
    std::string passwords;

    // Séparer les noms des canaux et les mots de passe par un espace
    size_t pos = str.find(" ");
    if (pos != std::string::npos) {
        names = str.substr(0, pos);           // Partie contenant les noms des canaux
        passwords = str.substr(pos + 1);      // Partie contenant les mots de passe
    } else {
        names = str;                          // Si aucun mot de passe n'est fourni
    }

    // Séparer les noms des canaux
    std::vector<std::string> channels_list = split_by_comma(names);
    std::vector<std::string> passwords_list = split_by_comma(passwords);

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


bool Parse::parse_join(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{
    // std::map<std::string, std::string> chan;

    if (check_invalid_char_join(_value, client_fd, client_actif) == false){
        return false;
    }

    std::string nickname = client_actif.get_nickname();

    std::string channel_name = _value.substr(1);

    // ajout channel existant
    for (int i = 0; i < channels.size(); i++){
        if (channels[i]->get_name() == channel_name){
            // verif si il est autorise
            if (channels[i]->authorization_check(nickname) == false){
                std::string error_message = ERR_INVITEONLYCHAN(nickname, channel_name);
                send(client_fd, error_message.c_str(), error_message.size(), 0);
                return false;
            }
            channels[i]->add_client(nickname, client_fd, client_actif);
            return true;
        }
    }
    // ajout channel non cree
    Channel *chan = new Channel(channel_name);
    channels.push_back(chan);
    int len = channels.size() - 1; // -> on doit calculer a chaque fois pour bien utiliser les method du dernier channel de la list ( celui quon vient de creer)
    channels[len]->add_client(nickname, client_fd, client_actif);

    return true;
}

// bool Parse::parse_pass(std::vector<Client*> &clients_list, int client_fd, Client &client_actif)
// {
//     std::string server_name = SERVER_NAME;
//     if (_value != PASSWORD){
//         send(client_fd, ERR_PASSWDMISMATCH(server_name), strlen(ERR_PASSWDMISMATCH(server_name)), 0);
//         close(client_fd);
//         return false;
//     }
//     return true;
// }

bool Parse::parse_quit(std::vector<Client*> &clients_list, int client_fd, Client &client_actif)
{
    std::string full_quit_message;
    if (_value.empty()){
        std::string full_quit_message = client_actif.get_nickname() + "exited\r\n";
    }
    else{
        std::string full_quit_message = ":" + client_actif.get_nickname() + " QUIT :" + _value + "\r\n";
    }
    send(client_fd, full_quit_message.c_str(), full_quit_message.size(), 0);

    return true;
}


bool Parse::parse_ping(std::vector<Client*> &clients_list, int client_fd, Client &client_actif)
{
    std::string pong = "PONG :" + _value + "\r\n";
    send(client_fd, pong.c_str(), pong.length(), 0);

    return true;
}

bool Parse::parse_user(std::vector<Client*> &clients_list, int client_fd, Client &client_actif)
{
    std::string server_name = SERVER_NAME;

    // Verif if USER is already done
    if (client_actif.get_user_setup() == true){
        send(client_fd, ERR_ALREADYREGISTERED(server_name), strlen(ERR_ALREADYREGISTERED(server_name)), 0);
        return false;
    }
    // Verif nb_parameter
    if (std::count(_value.begin(), _value.end(), ' ') < 3){
        send(client_fd, ERR_NEEDMOREPARAMS("USER", server_name), strlen(ERR_NEEDMOREPARAMS("USER", server_name)), 0);
    }


    // Si tout est bon on recupere les donnees et on informe le client pour valider la connexion
    extract_user_info(_value, client_actif);
    
    std::string welcome_message = ":" + server_name + " 001 " + client_actif.get_nickname() + 
                              " :\x03""04Welcome to the Internet Relay Network " + 
                              client_actif.get_nickname() + "!" + 
                              client_actif.get_username() + "@" + client_actif.get_hostname() + 
                              " - ft_irc 42 Paris\x03""\r\n";

    send(client_fd, welcome_message.c_str(), welcome_message.size(), 0);

    // std::cout << "username: " << client_actif.get_username() << std::endl;
    // std::cout << "hostname: " << client_actif.get_hostname() << std::endl;
    // std::cout << "servername: " << client_actif.get_server_name() << std::endl;
    // std::cout << "realname: " << client_actif.get_real_name() << std::endl;
    return true;
}


// bool Parse::user_cmd(const std::string &str)
// {

//     // PASS : Définit le mot de passe à utiliser lors de la connexion au serveur.
//     // NICK : Change ou définit le pseudonyme de l'utilisateur.
//     // USER : Utilisée lors de la connexion pour spécifier les informations de l'utilisateur (nom, hôte, etc.).
//     // QUIT : Permet à un utilisateur de se déconnecter du serveur.
    
//     return true;

// }
// bool Parse::channel_cmd(const std::string &str)
// {
//     // JOIN : Rejoindre un canal spécifié par son nom (ex: JOIN #channel).
//     // PART : Quitter un canal spécifié.
//     // MODE : Gérer les modes des canaux ou des utilisateurs (ex: mode opérateur, modéré, etc.).
//     // TOPIC : Définir ou récupérer le sujet d'un canal.
//     // NAMES : Lister les utilisateurs dans un canal.
//     // LIST : Lister tous les canaux disponibles sur le serveur.
//     // INVITE : Inviter un utilisateur à rejoindre un canal.

//     return true;
// }

// bool Parse::operator_cmd(const std::string &str)
// {

// //     KICK : Permet à un opérateur de canal d'expulser un client du canal.
// //     INVITE : Utilisée pour inviter un utilisateur spécifique dans un canal.
// //     TOPIC : Permet de modifier ou afficher le sujet (topic) du canal.
// //     MODE : Cette commande permet de changer le mode du canal, avec les sous-options suivantes :

// //     i : Le canal devient sur invitation uniquement.
// //     t : Restreint la modification du topic aux opérateurs de canal.
// //     k : Définit ou supprime une clé (mot de passe) pour accéder au canal.
// //     o : Donne ou retire les privilèges d'opérateur de canal à un utilisateur.
// //     l : Définit ou supprime la limite maximale d'utilisateurs pouvant rejoindre le canal.
//     return true;
// }



// ################################################################################
// #                                SETUP CMD USER                                #
// ################################################################################
void Parse::split_cmd_value(const std::string &full_command)
{
    //std::cout << "full command: " << _str << std::endl;

    // Trouver la position du premier espace
    size_t pos = full_command.find(" ");

    // Si aucun espace n'est trouvé, considérer que la chaîne contient uniquement une commande
    if (pos == std::string::npos) {
        _command = full_command;
        std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);
        _value.clear(); // Pas de valeur associée
        return;
    }

    // Extraire la commande
    _command = full_command.substr(0, pos);
    std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);

    // Extraire la valeur après l'espace, s'il y en a
    if (pos + 1 >= full_command.size()) {
        _value.clear(); // Pas de valeur après l'espace
        std::cout << CYAN << "Command = " << WHITE << _command << std::endl;
        std::cout << GREEN << "Value = (empty)" << WHITE << std::endl;
        return;
    }

    _value = full_command.substr(pos + 1);
    std::cout << CYAN << "Command = " << WHITE << _command << std::endl;
    std::cout << GREEN << "Value = " << WHITE << _value << std::endl;
}


void Parse::extract_user_info(const std::string& value, Client& client_actif)
{
    size_t pos = value.find(":");
    if (pos != std::string::npos) {
        std::string real_name = value.substr(pos + 1);
        client_actif.set_real_name(real_name);
    }

    // Extraire username
    pos = value.find(" ");
    if (pos != std::string::npos) {
        client_actif.set_username(value.substr(0, pos));
        std::string new_str = value.substr(pos + 1);

        // Extraire hostname
        pos = new_str.find(" ");
        if (pos != std::string::npos) {
            client_actif.set_host(new_str.substr(0, pos));
            new_str = new_str.substr(pos + 1);

            // Extraire servername
            pos = new_str.find(" ");
            if (pos != std::string::npos) {
                client_actif.set_server(new_str.substr(0, pos));
            }
        }
    }
}

// ################################################################################
// #                       Constructor / Destructor                               #
// ################################################################################

Parse::Parse(const std::string &str): _str(str)
{
    std::string cleaned_str = _str;
    cleaned_str.erase(std::remove(cleaned_str.begin(), cleaned_str.end(), '\r'), cleaned_str.end());
    cleaned_str.erase(std::remove(cleaned_str.begin(), cleaned_str.end(), '\n'), cleaned_str.end());

    split_cmd_value(cleaned_str);
}

Parse::Parse(): _str("str")
{
    
}


Parse::~Parse()
{

}