/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:14:04 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/04 16:14:26 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <cstdlib> 

int checkPort(char *argv){
	for (int i = 0; argv[i]; i++){
			if (!isdigit(argv[i])){
				std::cout << "Error: port must be a number" << std::endl;
				return -1;
			}
		}
		int Port = atoi(argv);
		if (Port < 1024 || Port > 49151){ 	// confirmer 49151, 65535 d'apres beej
			std::cout << "Error: Port must be between 1024 and 49151" << std::endl;
			return -1;
		}
	return (Port);
}

int main(int argc, char **argv)
{
	if (argc != 3){
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	
	int Port = checkPort(argv[1]);
	if (Port == -1)
		return 1;
	std::string Password = argv[2];
	
	Server server(Port, Password);
	signal(SIGINT, server.Handler_sigint);
	
	try{
		server.InitListener(); //-> initialize and get the listener socket
		server.Polling(); //-> accept new clients and receive data
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}