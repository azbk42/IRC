#include "bot.hpp"


// ################################################################################
// #                                     GET                                      #
// ################################################################################

const std::string Bot::get_name() const {return _name;};

// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

void Bot::handle_time(Client &client_actif)
{
    std::string time_spent_in_hour = time_spent_on_server(client_actif);
    int fd = client_actif.get_socket_fd();
    std::string client_name = client_actif.get_nickname();
    std::string bot_name = BOT_NAME;

    std::string message = ":" + bot_name + "!user@host PRIVMSG " + bot_name + " :" + "You have connected for: " + time_spent_in_hour + "\r\n";
    std::cout << MAGENTA << message << WHITE << std::endl;
    send(fd, message.c_str(), message.length(), 0);
}

std::string Bot::time_spent_on_server(Client &client_actif) const 
{
        time_t now = time(0); 
        double secondsSpent = difftime(now, client_actif.get_connection_time()); // Calcul du temps en sec

        // Conversion en H,M,S
        int hours = static_cast<int>(secondsSpent) / 3600;
        int minutes = (static_cast<int>(secondsSpent) % 3600) / 60;
        int seconds = static_cast<int>(secondsSpent) % 60;

        std::ostringstream timeSpent;
        timeSpent << hours << "h " << minutes << "m " << seconds << "s";

        return timeSpent.str();
}



// ################################################################################
// #                             CONSTRUCTOR DESTRUCTOR                           #
// ################################################################################
Bot::Bot(const std::string &name): _name(name)
{

}

Bot::~Bot() {};
