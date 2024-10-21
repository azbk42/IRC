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
#include <ctime>

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
        bool is_operator(const std::string &name);

        void add_invite(const std::string &name);
        void minus_invite(const std::string &name);

        bool authorization_check(const std::string &nickname);

        std::vector<std::string> get_operator() const;
        bool get_pass() const;

        const std::string get_name() const;
        std::string get_topic() const;
        std::string get_password() const;
       
        std::map<std::string, int> get_clients() const;
		size_t get_nb_client() const;
		std::time_t get_creation_date() const;
        // std::vector<std::string> get_operators() const;
        bool get_i() const;
		bool get_t() const;

        int get_limite() const;

        std::vector<std::string> get_invite_name() const;

        void set_limite(int x);
        // void set_i(const std::string &i);
        void set_i(const char &i);
		void set_t(const char &t);
		void set_pass(bool x);
		void set_password(std::string str);

        void set_topic(const std::string &value);

		int remove_client(const std::string &name, Client &client_actif);
        void add_operator(const std::string &name);
        void remove_operator(const std::string &name);
		

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
		time_t _creation_time;

		std::map<std::string, bool> _flags;
        bool _i;
        int _limite;
		bool _t;

};