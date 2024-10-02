#include "parse.hpp"
#include <string.h>
#include <stdio.h>

// array of functions pointers
void user_cmd(Client& client, Channel &channel, const std::string &str)
{


}

Command cmd_function[] = 
{
    {"USER", &user_cmd},
    // {"NICK", &nick_cmd},
    // {"Quit", &quit_cmd},
    // {"JOIN", &join_cmd},
    // {"TOPIC", &topic_cmd},
    // {"PART", &part_cmd},
    // {"NAMES", &name_cmd},
    // {"LIST", &list_cmd},
    // {"INVITE", &invite_cmd},
    // {"KICK", &kick_cmd},
    // {"MODE", &mode_cmd},
    
};


// METHOD
void Parse::split_cmd_value(const std::string &full_command)
{
    std::cout << "full command: " << _str << std::endl;

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
        std::cout << "Command = " << _command << std::endl;
        std::cout << "Value = (empty)" << std::endl;
        return;
    }

    _value = full_command.substr(pos + 1);
    std::cout << "Command = " << _command << std::endl;
    std::cout << "Value = " << _value << std::endl;
}


bool Parse::parse_nick(std::vector<Client*> &clients_list, int client_fd)
{
    if (_value.size() < 1 || _value.size() > 9) {
        std::string server_name = SERVER_NAME;  // Remplace par le nom du client
        std::string nick = _value;

        // Envoi de l'erreur ERR_ERRONEUSNICKNAME
        send(client_fd, ERR_ERRONEUSNICKNAME(server_name, nick), strlen(ERR_ERRONEUSNICKNAME(server_name, nick)), 0);
        return false;
    }
    std::cout << "_value = " << _value << std::endl;

    return false;
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



// Constructor Destructor

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