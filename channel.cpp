#include "channel.hpp"


void Channel::modif_topic(const std::string &topic)
{
    _topic = topic;
    // envoyer le topic a tous les utilisateurs
}


Channel::Channel()
{

}

Channel::~Channel()
{

}