#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Channel
{
    public:

        Channel(const std::string &name);
        ~Channel();

        void modif_topic(const std::string &topic);
        

    private:

        // variable a false si pas de mdp ou true si yen a un
        bool _pass;
        size_t nb_client;


        const std::string _name;
        std::string _topic;
        std::map<std::string, int> _client;
        std::vector<std::string> _operator;

        std::string _password;

        // -i si le channel est en invite only +i, il faut creer un vector<string> avec les noms autorises
        // -k definir un mot de passe que le client va devoir fournir pour se connecter
        // -l definir ou supprimer la limite dutilisateur
        // -o donner ou retirer les droit d'operateur
        // -t seul les operateur peuvent changer le topic

};

// Un channel est cree avec un nom, si cest le premier client, il devient OP
// si _pass = true on va demander au client un mdp