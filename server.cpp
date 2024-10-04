/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:59:40 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/04 13:06:23 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() : _port(6667), _serverFd(-1){}

Server::Server(const Server &rhs){
	_port = rhs._port;
	_serverFd = rhs._serverFd;
	_pollFds = rhs._pollFds;
	_clients = rhs._clients;
	return ;
}

Server::~Server(){ CloseServerFd(); }

Server &Server::operator=(const Server &rhs){
	if (this != &rhs){
		_port = rhs._port;
		_serverFd = rhs._serverFd;
		_pollFds = rhs._pollFds;
		_clients = rhs._clients;
	}
	return *this;
}

int Server::GetFd() const {return _serverFd;}

int Server::GetPort() const {return _port;} 

void Server::CloseServerFd() {
	if (_serverFd != -1){
		if (close(_serverFd) == -1)
			std::cerr << "Failed to close server socket" << std::endl; // throw?
		else
			std::cout << RED << "Server <" << _serverFd << "> Disconnected" << WHITE << std::endl;
	}
}

void Server::SendtoAll(int expFd, char *buffer, int bytes_recv){
	for (size_t j = 0; j < _pollFds.size(); j++) {
		int dest_fd = _pollFds[j].fd;
		if (dest_fd != _serverFd && dest_fd != expFd) {
			if (send(dest_fd, buffer, bytes_recv, 0) == -1) {
				std::cout << "Failed to send data to " << dest_fd << std::endl;
			}
		}
	}
}

void Server::AcceptClient(){
	Client newClient;
	struct sockaddr_in client_addr;
	struct pollfd clientPoll;
	socklen_t len = sizeof(client_addr);
	
	int newFd = accept(_serverFd, (sockaddr *)&client_addr, &len);
	if (newFd == -1)
		std::cout << "Failed to accept client" <<std::endl;
	
	if (fcntl(newFd, F_SETFL, O_NONBLOCK) == -1)
		std::cout << "Failed to set non-blocking fd for client" <<std::endl;
	
	clientPoll.fd = newFd;
	clientPoll.events = POLLIN;
	clientPoll.revents = 0;

	std::cout << GREEN << "Client <" << newFd << "> Connected" << WHITE << std::endl;
	_pollFds.push_back(clientPoll);
	newClient.SetFd(newFd);
	_clients.push_back(newClient);
}

void Server::ReceiveData(int fd){
	int bytesRecv;
	char buffer[1024];

	bytesRecv = recv(fd, buffer, sizeof(buffer), 0);
	if (bytesRecv <= 0){
		if (bytesRecv == -1)
			std::cout << "Failed to receive data" << std::endl;
		else if (bytesRecv == 0) {
			std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
			close(fd);
			for (size_t i = 0; i < _pollFds.size() ;i++) {
				if (_pollFds[i].fd == fd){
					_pollFds.erase(_pollFds.begin() + i);
					_clients.erase(_clients.begin() + i);
					break;
				}
			}
		}
	}
	else {
		buffer[bytesRecv] = '\0';
		std::cout << YELLOW << "Client <" << fd << "> Data: " << WHITE << buffer;
		// inclure le parsing ici
		SendtoAll(fd, buffer, bytesRecv);
	}
}

void Server::Polling(){
	for(;;)
	{
		if (poll(_pollFds.data(), _pollFds.size(), -1) == -1)
			throw(std::runtime_error("Failed to poll"));
		// ou if((poll(&_pollFds[0],_pollFds.size(),-1) == -1) ?
		for (size_t i = 0; i < _pollFds.size(); i++){ //-> check all fds
			if (_pollFds[i].revents & POLLIN) { //-> check if there is data to read
				if (_pollFds[i].fd == _serverFd) //-> check if the event is from the server socket
					AcceptClient(); //-> if yes, accept new client
				else
					ReceiveData(_pollFds[i].fd); //-> if not, receive new data from a registered client
			}
		}
	}
}

void Server::InitListener() {
	struct sockaddr_in server_addr;
	struct pollfd serverPoll;
	int opt = 1;
	
	server_addr.sin_family = AF_INET; // -> set the address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // -> bind to all available interfaces
    server_addr.sin_port = htons(_port); // -> convert the port to network byte order (host to network short)

	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
		throw(std::runtime_error("Failed to create socket"));

	if(setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw(std::runtime_error("Failed to set reusable addresses"));	 

	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Failed to set non-blocking fd for server"));
	
	if (bind(_serverFd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		throw(std::runtime_error("Failed to bind socket"));

	if (listen(_serverFd, 20) == -1)
		throw(std::runtime_error("Failed to listen on socket"));
		
	serverPoll.fd = _serverFd;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	_pollFds.push_back(serverPoll);
	std::cout << GREEN << "Server <" << _serverFd << "> Listening" << WHITE << std::endl;
}
