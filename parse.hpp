#pragma once

#include <iostream>
#include <string>


class Channel;
class Client;

class Parse
{
    public:
        Parse(const std::string &str);
        ~Parse();

        // pour le parsing traiter le 1er mot comme si il etait en majuscule tout le temps
        // function pour separer le premier mot du reste de la phrase (car dans IRC le premier mot est toujours une commande)

        void catch_first_user_command(const std::string &str);

        bool user_cmd(const std::string &str);
        bool channel_cmd(const std::string &str);
        bool operator_cmd(const std::string &str);


    private:

        const std::string _str;
        std::string _command;
        std::string _new_str;
};

typedef void (*f)(Client&, Channel&, const std::string&);

struct Command{
    std::string command;
    f func;
};