#include "parse.hpp"

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

void Parse::catch_first_user_command(const std::string &str)
{
    std::size_t pos = str.find(" ");
    this-> _command = str.substr(0, pos);
    this-> _new_str = str.substr(pos + 1);

    std::cout << "command = " << _command << std::endl;
    std::cout << "new_str = " << _new_str << std::endl;

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
// // INVITE : Utilisée pour inviter un utilisateur spécifique dans un canal.
// // TOPIC : Permet de modifier ou afficher le sujet (topic) du canal.
// // MODE : Cette commande permet de changer le mode du canal, avec les sous-options suivantes :

// //     i : Le canal devient sur invitation uniquement.
// //     t : Restreint la modification du topic aux opérateurs de canal.
// //     k : Définit ou supprime une clé (mot de passe) pour accéder au canal.
// //     o : Donne ou retire les privilèges d'opérateur de canal à un utilisateur.
// //     l : Définit ou supprime la limite maximale d'utilisateurs pouvant rejoindre le canal.

//     return true;
// }



// Constructor Destructor

Parse::Parse(const std::string &str): _str(str), _command("default"), _new_str("default")
{
    catch_first_user_command(_str);
}


Parse::~Parse()
{

}