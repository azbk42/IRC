/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:59:40 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/17 14:30:12 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

bool Server::_signal = false;

// ################################################################################
// #                       Constructor / Destructor                               #
// ################################################################################

Server::Server(int Port, std::string Password) : _port(Port), _serverFd(-1), _password(Password)
{
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Password: " << _password << std::endl;
}

Server::Server(const Server &rhs)
{
	_port = rhs._port;
	_serverFd = rhs._serverFd;
	_password = rhs._password;
	_pollFds = rhs._pollFds;
	_clients_array = rhs._clients_array;
	_channels_array = rhs._channels_array;
	_partial_message = rhs._partial_message;
	return ;
}

Server::~Server()
{ 
	std::cout << "destructeur called" << std::endl;
	CloseServerFd(); 
}

Server &Server::operator=(const Server &rhs)
{
	if (this != &rhs){
		_port = rhs._port;
		_serverFd = rhs._serverFd;
		_pollFds = rhs._pollFds;
		_clients_array = rhs._clients_array;
	}
	return *this;
}

// ################################################################################
// #                                    GET                                       #
// ################################################################################

int Server::GetFd() const {return _serverFd;}

int Server::GetPort() const {return _port;}

std::string Server::GetPassword() const {return _password;}

// ################################################################################
// #                                  CLOSE FDS                                   #
// ################################################################################

void Server::CloseServerFd()
{
	for (size_t i = _pollFds.size() - 1; i > 0; i--) {
		if (close(_pollFds[i].fd) == -1)
			std::cerr << "Failed to close client socket" << std::endl; // throw?
		_pollFds.erase(_pollFds.begin() + i);
		delete _clients_array[i - 1]; // ajout de cette fonction pour free le client alloue avec new
		_clients_array.erase(_clients_array.begin() + (i - 1));
	}
	for (size_t i = 0; i < _channels_array.size(); i++){
		delete _channels_array[i];
	}

	if (_serverFd != -1){
		if (close(_serverFd) == -1)
			std::cerr << "Failed to close server socket" << std::endl; // throw?
		else
			std::cout << RED << "Server <" << _serverFd << "> Disconnected" << WHITE << std::endl;
	}
}

void Server::CloseClientSocket(int fd)
{
	std::cout << RED <<"Closing client socket" << std::endl;
	if (close(fd) == -1)
		std::cout << "Failed to close client socket" << std::endl;
	for (size_t i = 0; i < _pollFds.size(); i++) {
		if (_pollFds[i].fd == fd){
			_pollFds.erase(_pollFds.begin() + i); //-> erase the client's fd from pollFds
			delete _clients_array[i - 1]; // ajout de cette fonction pour free le client alloue avec new
			_clients_array.erase(_clients_array.begin() + (i - 1)); //->erase the client from _client_array
			break;
		}
	}
}

// ################################################################################
// #                            COMMUNICATION CLIENT                              #
// ################################################################################

void Server::AcceptClient()
{
	struct sockaddr_in client_addr;
	struct pollfd clientPoll;
	socklen_t len = sizeof(client_addr);
	
	int newFd = accept(_serverFd, (sockaddr *)&client_addr, &len);
	if (newFd == -1)
	{
		std::cout << "Failed to accept client" <<std::endl; 
		return ;
	}
	if (fcntl(newFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "Failed to set non-blocking fd for client" <<std::endl;
		close (newFd);
		return ;
	}
	clientPoll.fd = newFd;
	clientPoll.events = POLLIN;
	clientPoll.revents = 0;

	std::cout << GREEN << "Client <" << newFd << "> Connected" << WHITE << std::endl;
	_pollFds.push_back(clientPoll);
	Client* new_client = new Client(newFd);
	_clients_array.push_back(new_client);
}

// ################################################################################
// #                                 PARSING                                      #
// ################################################################################

void Server::find_command(int fd, Client* client_actif, Parse &parser, const std::string &cmd)
{
    if (cmd == "QUIT") {
        Quit quit(fd, client_actif, parser.get_value(), _channels_array);
        quit.send_quit_msg();
        CloseClientSocket(fd);
    }
    else if (cmd == "NICK") {
        parser.parse_nick(_clients_array, fd, *client_actif, _channels_array, this);
    }
    else {
        parser.find_cmd_type(cmd, _clients_array, fd, *client_actif, _channels_array);
    }
}

void Server::check_password(int fd, Client* client_actif, Parse &parser, const std::string &cmd)
{
	if (cmd == "PASS") {
		Pass passwd(fd, parser.get_value(), client_actif, _password);
		if (passwd.check_pass() == 1)
			CloseClientSocket(fd);
	}
	else {
		send_message(fd, "Please enter password");
		CloseClientSocket(fd);
	}
}

void Server::process_message(int fd)
{
	size_t newline_pos;
	while ((newline_pos = _partial_message[fd].find('\n')) != std::string::npos) {
		
		std::string complete_message = _partial_message[fd].substr(0, newline_pos + 1);
		_partial_message[fd].erase(0, newline_pos + 1);

		std::cout << YELLOW << "Client <" << fd << "> Data: " << WHITE << complete_message << std::endl;

		Parse parser(complete_message);
		Client* client_actif = NULL;
		for (size_t i = 0; i < _clients_array.size(); i++) {
			if (_clients_array[i]->get_socket_fd() == fd) {
				client_actif = _clients_array[i]; 
				break;
			}
		}
		std::string cmd = parser.get_cmd();
		// Vérifier si un client a été trouvé
		if (client_actif != NULL && client_actif->get_checked_pwd() == false) {
			check_password(fd, client_actif, parser, cmd);
		}
		else if (client_actif != NULL && client_actif->get_checked_pwd() == true) {
            find_command(fd, client_actif, parser, cmd); // Appel à la nouvelle fonction
		}
		else {
			std::cerr << "Client non trouvé pour le socket " << fd << std::endl;
		}
	}
}

void Server::ReceiveData(int fd)
{
	int bytesRecv;
	char buffer[1024];

	bytesRecv = recv(fd, buffer, sizeof(buffer), 0);
	if (bytesRecv <= 0){
		if (bytesRecv == -1)
			std::cout << "Failed to receive data" << std::endl;
		else if (bytesRecv == 0) {
			Client* client_actif = NULL;
			for (size_t i = 0; i < _clients_array.size(); i++) {
				if (_clients_array[i]->get_socket_fd() == fd) {
					client_actif = _clients_array[i]; 
					break;
				}
			}
			Quit quit(fd, client_actif, "Error", _channels_array);
			quit.send_quit_msg();
			CloseClientSocket(fd);
			std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
		}
	}
	else {
		buffer[bytesRecv] = '\0';
		std::string message(buffer);
		_partial_message[fd] += message;
		process_message(fd);
	}		
	std::cout << YELLOW << "-----------------------------------------------------" << WHITE << std::endl;
}

// ################################################################################
// #                            COMMUNICATION CLIENT                              #
// ################################################################################

void Server::Polling()
{
	while(Server::_signal == false)
	{
		if (poll(_pollFds.data(), _pollFds.size(), -1) == -1 && Server::_signal == false)
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

void Server::InitListener()
{
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

// ################################################################################
// #                                   SIGNAL                                     #
// ################################################################################

void Server::Handler_sigint(int sig)
{
	(void)sig;
	Server::_signal = true;
}
