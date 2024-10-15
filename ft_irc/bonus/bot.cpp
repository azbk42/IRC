#include "bot.hpp"


// ################################################################################
// #                                     GET                                      #
// ################################################################################


bool Bot::process_command(const std::string &command)
{
    // Initialisation du vector avec la méthode push_back()
    std::vector<std::string> commands;
    commands.push_back("!help");
    commands.push_back("!heads");
    commands.push_back("!tails");
    commands.push_back("!time");

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

void Bot::join_channel()
{
    std::string join = "JOIN #" + _channel_name + "\r\n";

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
// #                                   INIT BOT                                   #
// ################################################################################

void Bot::authenticate()
{
    std::string cap = "CAP LS\r\n";
    std::string pass = "PASS " + _password + "\r\n";
    std::string nick = "NICK " + _bot_name + "\r\n";
    std::string user = "USER " + _bot_name + " Mrbot localhost :" + "Helpmaster" + "\r\n";

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
    while (true);
    // while (true) {
    //     handle_server_response();
    // }
}

// ################################################################################
// #                       INIT ARRAY OF FUNCTION POINTERS                        #
// ################################################################################

// void Bot::initialize_command_handlers()
// {
//     commandHandlers[0] = &Bot::handle_help;
//     commandHandlers[1] = &Bot::handle_heads;
//     commandHandlers[2] = &Bot::handle_tails;
//     commandHandlers[3] = &Bot::handle_time;
// }

// ################################################################################
// #                             CONSTRUCTOR DESTRUCTOR                           #
// ################################################################################
Bot::Bot(int port,const std::string &password):
        _server_ip("127.0.0.1"), _port(port), _channel_name("Bot"), _bot_name(BOT_NAME), _password(password)
{
    //initialize_command_handlers();
}

Bot::~Bot() {};
