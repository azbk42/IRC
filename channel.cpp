#include "channel.hpp"


void Channel::modif_topic(const std::string &topic)
{
    _topic = topic;
    // envoyer le topic a tous les utilisateurs
}

Channel::Channel(const std::string &name): 
    _name(name), _topic(""), _password(""), _pass(false), nb_client(0)
{

}

Channel::~Channel()
{

}