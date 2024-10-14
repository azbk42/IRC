#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include "define.hpp"
#include "client.hpp"
#include "parse.hpp"
#include "server.hpp"

class Channel
{
    public:

        Channel(const std::string &name);
        ~Channel();

        void modif_topic(const std::string &topic);
        void add_client(const std::string &name, const int fd_client, Client &client_actif);
        void send_message_to_all(const std::string &message, const int fd_client);
        void send_welcome_message(const std::string &name, const int fd_client);
		void send_part_message(const std::string &name, const int fd_client);
        bool is_in_channel(const std::string &name);
        void update_name_client(const std::string &old_nickname, const std::string &new_nickname);

        bool authorization_check(const std::string &nickname);

        std::vector<std::string> get_operator() const;
        bool get_pass() const;

        const std::string get_name() const;
        std::string get_topic() const;
        std::string get_password() const;
       
        std::map<std::string, int> get_clients() const;
		size_t get_nb_client() const;

        void set_i(const std::string &i);
        void set_topic(const std::string &value);

		int remove_client(const std::string &name, const int fd_client, Client &client_actif, std::string reason);
		

    private:

        // variable a false si pas de mdp ou true si yen a un
        bool _pass;
        size_t _nb_client;

        const std::string _name_channel;
        std::string _topic;
        std::map<std::string, int> _client;
        std::vector<std::string> _operator;
        std::vector<std::string> _invite_name;
        std::string _password;

        bool _i;
        bool _t;
        // -i si le channel est en invite only +i, il faut creer un vector<string> avec les noms autorises
        // -k definir un mot de passe que le client va devoir fournir pour se connecter
        // -l definir ou supprimer la limite dutilisateur
        // -o donner ou retirer les droit d'operateur
        // -t seul les operateur peuvent changer le topic

};

// Un channel est cree avec un nom, si cest le premier client, il devient OP
// si _pass = true on va demander au client un mdp