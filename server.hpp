/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:59:48 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/04 12:56:05 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector> //-> for vector
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

#define RED "\e[1;31m" //-> for red color
#define WHITE "\e[0;37m" //-> for white color
#define GREEN "\e[1;32m" //-> for green color
#define YELLOW "\e[1;33m" //-> for yellow color

// #include "client.hpp" //-> for client class

class Client //-> class for client
{
	private:
		int _clientFd; //-> client file descriptor
		// std::string _ip; //-> client ip address
	public:
		Client(){}; //-> default constructor
		int GetFd(){return _clientFd;} //-> getter for fd

		void SetFd(int fd){_clientFd = fd;} //-> setter for fd
		// void setIpAdd(std::string ipadd){_ip = ipadd;} //-> setter for ipadd
};

class Server //-> class for server
{
	private:
		int _port; //-> server port
		int _serverFd; //-> server socket file descriptor
		std::vector<pollfd> _pollFds;
		std::vector<Client> _clients;

	public:
		Server(); //-> default constructor
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
};