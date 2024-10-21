#include "bot.hpp"


// ################################################################################
// #                                     GET                                      #
// ################################################################################


bool Bot::process_command(const std::string &command)
{
    std::vector<std::string> commands;
    commands.push_back("!help");
    commands.push_back("!heads");
    commands.push_back("!tails");

    for (size_t i = 0; i < commands.size(); ++i) {
        if (command.find(commands[i]) != std::string::npos) {
            (this->*commandHandlers[i])();
            return true;
        }
    }
    return false;
}

// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

void Bot::send_message_error(int fd, const std::string &message)
{
    std::string full_message = "PRIVMSG #Bot :" + message + "\r\n";
    if (send(fd, full_message.c_str(), full_message.size(), 0) == -1){
        std::cerr << RED << "Send error" << std::endl;
        std::cerr << RED << "Client fd = " << WHITE << fd << std::endl;
        std::cerr << RED << "Message: " << WHITE << message << std::endl;
    }
}

void Bot::handle_server_response()
{

    char buffer[1024];

    int bytes = recv(_socket_fd, buffer, sizeof(buffer), 0);
    if (bytes > 0){
        buffer[bytes] = '\0';
        std::string message = buffer;
        std::cout << "message recv: " << message << std::endl;
    
        if (process_command(message)){
            std::cout << "Command: " << message << std::endl;
        }
    }

}

void Bot::join_channel()
{
    std::string join = "JOIN #Bot\r\n";

    if (send(_socket_fd, join.c_str(), join.length(), 0) == -1){
        throw std::runtime_error("Failed to join the channel");
    }

    std::cout << GREEN << " >> Connected to channel: " + _channel_name +"\n" << RESET << std::endl;
}

// ################################################################################
// #                                    CONNECT                                   #
// ################################################################################

void Bot::connect_to_server()
{
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_fd == -1) {
        throw std::runtime_error("Failed to connect to server");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    inet_pton(AF_INET, _server_ip.c_str(), &server_addr.sin_addr);

    if (connect(_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        throw std::runtime_error("Failed to connect to server");
    }
    std::cout << GREEN << " >> Connected to server << \n" << RESET << std::endl;
}

// ################################################################################
// #                                   HANDLE COMMAND                             #
// ################################################################################

void Bot::handle_help() 
{
    // Initialisation du vector avec push_back pour C++98
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

    // Utilisation d'une boucle classique pour parcourir les éléments
    for (size_t i = 0; i < help_lines.size(); ++i) {
        send_message_error(_socket_fd,help_lines[i]);
    }
}

void Bot::handle_heads() 
{
    srand(time(0));
    bool is_heads = (rand() % 2 == 0);
    std::string result = is_heads ? "Heads" : "Tails";
    std::string outcome = is_heads ? "You win!" : "You lose!";
    send_message_error(_socket_fd,"Result: " + result + " - " + outcome);
}

void Bot::handle_tails() 
{
    srand(time(0));
    bool is_heads = (rand() % 2 == 0);
    std::string result = is_heads ? "Heads" : "Tails";
    std::string outcome = is_heads ? "You lose!" : "You win!";
    send_message_error(_socket_fd,"Result: " + result + " - " + outcome);
}


// ################################################################################
// #                                   INIT BOT                                   #
// ################################################################################

void Bot::authenticate()
{
    std::string cap = "CAP LS\r\n";
    std::string pass = "PASS " + _password + "\r\n";
    std::string nick = "NICK " + _bot_name + "\r\n";
    std::string user = "USER " + _bot_name + " Mrbot localhost :" + _bot_name + "\r\n";

    std::string full_message  = pass + nick + user;

    if (send(_socket_fd, full_message.c_str(), full_message.length(), 0) == -1){
        throw std::runtime_error("Failed to authenticate message");
    }

    std::cout << GREEN << " >> authenticate success << \n" << RESET << std::endl;
}

void Bot::run() 
{
    authenticate();

   
    join_channel();

    while (true){
        handle_server_response();
    }
}

// ################################################################################
// #                       INIT ARRAY OF FUNCTION POINTERS                        #
// ################################################################################

void Bot::initialize_command_handlers()
{
    commandHandlers[0] = &Bot::handle_help;
    commandHandlers[1] = &Bot::handle_heads;
    commandHandlers[2] = &Bot::handle_tails;
}

// ################################################################################
// #                             CONSTRUCTOR DESTRUCTOR                           #
// ################################################################################
Bot::Bot(int port,const std::string &password):
        _server_ip("127.0.0.1"), _port(port), _channel_name("Bot"), _bot_name(BOT_NAME), _password(password)
{
    initialize_command_handlers();
}

Bot::~Bot() {};
