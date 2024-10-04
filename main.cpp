/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:14:04 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/04 12:53:27 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <cstdlib> 

int main(int argc, char **argv)
{
	if (argc != 3){
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	
	for (int i = 0; argv[1][i]; i++){
		if (!isdigit(argv[1][i])){
			std::cout << "Error: port must be a number" << std::endl;
			return 1;
		}
	}
	int port = atoi(argv[1]);
	if (port < 1024 || port > 49151){ 	// confirmer 49151, 65535 d'apres beej
		std::cout << "Error: port must be between 1024 and 49151" << std::endl;
		return 1;
	}
	std::cout << "Port: " << port << std::endl;

	// verifier le mot de passe ??

	Server server;
	try{
		server.InitListener(); //-> initialize and get the listener socket
		server.Polling(); //-> accept new clients and receive data
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
		server.CloseServerFd();
	}
	std::cout << RED << "Server <" << server.GetFd() << "> Disconnected" << WHITE << std::endl;

	return 0;
}