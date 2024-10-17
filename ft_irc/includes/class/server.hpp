/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:59:48 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/17 13:25:18 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

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

#include "channel.hpp"
#include "client.hpp"
#include "parse.hpp"
#include "include.hpp"
#include "pass.hpp"
#include "list.hpp"

class Server //-> class for server
{
	private:
		static bool _signal; //-> signal for sigint
		int _port; //-> server port
		int _serverFd; //-> server socket file descriptor
		bool _first_nick;
		std::string _password; //-> server password
		std::vector<pollfd> _pollFds; //-> vector of structures pollfd
		std::map<int, std::string> _partial_message;
		std::vector<Client*> _clients_array; //-> vector of Clients connected
    	std::vector<Channel*> _channels_array; // vector of Channels created

		void AcceptClient(); //-> accept new clients
		void ReceiveData(int fd); //-> receive new data from clients
		void CloseServerFd(); //-> close the server file descriptor
		void process_message(int fd); // -> check if message end with \n and process command
		
	public:
		Server(int Port, std::string PasSsword);
		Server(const Server &rhs); //-> copy constructor
		~Server(); //-> destructor
		Server &operator=(const Server &rhs); //-> assignation operator
		
		int GetFd() const; //-> getter for fd
		int GetPort() const; //-> getter for port
		std::string GetPassword() const; //-> getter for password
		
		void InitListener(); //-> initialize and get the listener socket
		void Polling(); //-> poll()
				
		void CloseClientSocket(int fd);
		
		static void Handler_sigint(int sig);

};

#endif