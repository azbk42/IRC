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

#include "include.hpp"
#include <cstdlib> 

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