#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "bot.hpp"

int checkPort(const char *argv)
{
	for (int i = 0; argv[i]; i++){
        if (!isdigit(argv[i])){
            return -1;
        }
    }
    int Port = atoi(argv);
    if (Port < 1024 || Port > 49151){
        std::cout << "Error: Port must be between 1024 and 49151" << std::endl;
        return -1;
    }
	return (Port);
}

Bot *handle_arg(const char *port, const char *password)
{
    int _port = checkPort(port);
    if (_port == -1) {
        throw std::invalid_argument("Port invalide fourni.");
    }

    std::string _password = password;

    return (new Bot(_port, _password));

}

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cout << "Usage: ./bot <port> <password>" << std::endl;
        return 1;
    }

    Bot* Celestin = NULL;
    try {
        Celestin = handle_arg(av[1], av[2]);

        signal(SIGINT, Bot::Handler_sigint);
        Celestin->connect_to_server();
        Celestin->run();
    }
    catch (const std::exception &e) {
        std::cerr << RED << "Erreur : " << e.what() << RESET << std::endl;

        delete Celestin;
        return 1;
    }
    
    delete Celestin;
    return 0;
}
