/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:59:48 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/04 17:16:06 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <iostream>
#include <vector> //-> for vector
#include <map> //-> for map
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
#include <cstring> //-> for memset()
// #include <netdb.h>
// #include <cstdlib>
// #include <cstdio>

#include "channel.hpp"
#include "client.hpp"
#include "parse.hpp"
#include "include.hpp"

class Server //-> class for server
{
	private:
		static bool _signal; //-> signal for sigint
		int _port; //-> server port
		int _serverFd; //-> server socket file descriptor
		std::string _password; //-> server password
		std::vector<pollfd> _pollFds;
		// std::vector<Channel> _chans;
		
		/* ELOUAN */
		std::map<int, std::string> _partial_message;
		std::vector<Client*> _clients_array;
    	std::vector<Channel*> _channels_array;
		/* ELOUAN */

	public:
		Server(int Port, std::string PasSsword);
		Server(const Server &rhs); //-> copy constructor
		~Server(); //-> destructor
		Server &operator=(const Server &rhs); //-> assignation operator
		
		int GetFd() const; //-> getter for fd
		int GetPort() const; //-> getter for port
		
		void InitListener(); //-> initialize and get the listener socket
		void Polling(); //-> poll()
		void AcceptClient(); //-> accept new clients
		void ReceiveData(int fd); //-> receive new data from clients
		void SendtoAll(int expFd, char *buffer, int bytes_recv); //-> send data to all clients
		void CloseServerFd(); //-> close the server file descriptor
		void CloseClientSocket(int fd);
		
		/* ELOUAN */
		void process_message(int fd); // -> check if message end with \n and process command
		/* ELOUAN */

		static void Handler_sigint(int sig);

};