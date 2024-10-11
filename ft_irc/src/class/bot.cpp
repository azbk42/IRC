#include "bot.hpp"


// ################################################################################
// #                                     GET                                      #
// ################################################################################

const std::string Bot::get_name() const {return _name;};

// ################################################################################
// #                                    METHOD                                    #
// ################################################################################


void Bot::handle_help(Client &client_actif)
{
    int fd = client_actif.get_socket_fd();
    std::string client_name = client_actif.get_nickname();
    std::string bot_name = BOT_NAME;

    // oblige de faire en vector car on ne peut pas utiliser de \n a cause du protocol irc
    std::vector<std::string> help_lines;
    help_lines.push_back("Available commands:");
    help_lines.push_back("1. NICK <nickname> - Change your nickname.");
    help_lines.push_back("2. JOIN <#channel> - Join a specific channel.");
    help_lines.push_back("3. PART <#channel> - Leave a specific channel.");
    help_lines.push_back("4. TOPIC <#channel> <topic> - Set or view the topic of a channel.");
    help_lines.push_back("5. MODE <#channel> <flags> - Change channel modes (e.g., invite-only, operator).");
    help_lines.push_back("6. KICK <#channel> <user> - Kick a user from a channel (requires operator privileges).");
    help_lines.push_back("7. MSG <nickname> <message> - Send a private message to a user.");
    help_lines.push_back("8. QUIT <message>- Disconnect from the server.");

    // send chaque ligne une par une
    for (size_t i = 0; i < help_lines.size(); ++i) {
        std::string message = ":" + bot_name + "!user@host PRIVMSG " + bot_name + " :" + help_lines[i] + "\r\n";
        send(fd, message.c_str(), message.length(), 0);
    }
}

void Bot::handle_heads(Client &client_actif)
{
    int fd = client_actif.get_socket_fd();
    std::string client_name = client_actif.get_nickname();
    std::string bot_name = get_name();

    srand(time(0));
    bool is_heads = (rand() % 2 == 0);

    std::string result = is_heads ? "Heads" : "Tails";
    std::string outcome = is_heads ? "You win!" : "You lose!";

    std::string message = ":" + bot_name + "!user@host PRIVMSG " + client_name + " :Result: " + result + " - " + outcome + "\r\n";
    send(fd, message.c_str(), message.length(), 0);
}


void Bot::handle_tails(Client &client_actif)
{
    int fd = client_actif.get_socket_fd();
    std::string client_name = client_actif.get_nickname();
    std::string bot_name = get_name();

    srand(time(0));
    bool is_heads = (rand() % 2 == 0);

    std::string result = is_heads ? "Heads" : "Tails";
    std::string outcome = is_heads ? "You lose!" : "You win!";

    std::string message = ":" + bot_name + "!user@host PRIVMSG " + client_name + " :Result: " + result + " - " + outcome + "\r\n";
    send(fd, message.c_str(), message.length(), 0);
}


void Bot::handle_time(Client &client_actif)
{
    std::string time_spent_in_hour = time_spent_on_server(client_actif);
    int fd = client_actif.get_socket_fd();
    std::string client_name = client_actif.get_nickname();
    std::string bot_name = BOT_NAME;

    std::string message = ":" + bot_name + "!user@host PRIVMSG " + bot_name + " :" + "You have been connected for: " + time_spent_in_hour + "\r\n";
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


bool Bot::process_command(const std::string &command, Client &client_actif)
{
   int fd = client_actif.get_socket_fd();
    std::string client_name = client_actif.get_nickname();
    std::string bot_name = get_name();

    std::vector<std::string> commands;
    commands.push_back("!help");
    commands.push_back("!heads");
    commands.push_back("!tails");
    commands.push_back("!time");

    for (size_t i = 0; i < commands.size(); ++i) {
        if (command == commands[i]) {
            (this->*commandHandlers[i])(client_actif);
            return true;
        }
    }

    // si on ne trouve pas de commande correspondante on lui affiche la liste
    std::vector<std::string> error_message_lines;
    error_message_lines.push_back("Unknown command. Available commands are:");
    for (size_t i = 0; i < commands.size(); ++i) {
        error_message_lines.push_back(commands[i]);
    }

    // sens les lignes une par une
    for (size_t i = 0; i < error_message_lines.size(); ++i) {
        std::string message = ":" + bot_name + "!user@host PRIVMSG " + client_name + " :" + error_message_lines[i] + "\r\n";
        send(fd, message.c_str(), message.length(), 0);
    }

    return false;
}

// ################################################################################
// #                       INIT ARRAY OF FUNCTION POINTERS                        #
// ################################################################################

void Bot::initialize_command_handlers()
{
    commandHandlers[0] = &Bot::handle_help;
    commandHandlers[1] = &Bot::handle_heads;
    commandHandlers[2] = &Bot::handle_tails;
    commandHandlers[3] = &Bot::handle_time;
}

// ################################################################################
// #                             CONSTRUCTOR DESTRUCTOR                           #
// ################################################################################
Bot::Bot(const std::string &name): _name(name)
{
    initialize_command_handlers();
}

Bot::~Bot() {};
