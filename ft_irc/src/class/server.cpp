/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:59:40 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/09 16:44:03 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

bool Server::_signal = false;

// ################################################################################
// #                       Constructor / Destructor                               #
// ################################################################################

Server::Server(int Port, std::string Password) : _port(Port), _serverFd(-1), _password(Password),
												bot(BOT_NAME)
{
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Password: " << _password << std::endl;
}

Server::Server(const Server &rhs): bot(rhs.bot.get_name())
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
// #                                    SET                                       #
// ################################################################################



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
			_pollFds.erase(_pollFds.begin() + i);
			delete _clients_array[i - 1]; // ajout de cette fonction pour free le client alloue avec new
			_clients_array.erase(_clients_array.begin() + (i - 1));
			break;
		}
		// enlever 1 utilisateurs aux chans etc?
		// std::map<std::string, int> _client;
        // std::vector<std::string> _operator;
        // std::vector<std::string> _invite_name;
		// lier a fonction part 
	}
}

// ################################################################################
// #                            COMMUNICATION CLIENT                              #
// ################################################################################

void Server::SendtoAll(int expFd, char *buffer, int bytes_recv)
{
	for (size_t j = 0; j < _pollFds.size(); j++) {
		int dest_fd = _pollFds[j].fd;
		if (dest_fd != _serverFd && dest_fd != expFd) {
			if (send(dest_fd, buffer, bytes_recv, 0) == -1) {
				std::cout << "Failed to send data to " << dest_fd << std::endl;
			}
		}
	}
}

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

std::string to_string(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
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
		for (int i = 0; i < _clients_array.size(); i++) {
			if (_clients_array[i]->get_socket_fd() == fd) {
				client_actif = _clients_array[i]; 
				break;
			}
		}
		// Vérifier si un client a été trouvé
		if (client_actif != NULL && client_actif->get_checked_pwd() == false) {
			if (parser.get_cmd() == "PASS") {
				Pass passwd(fd, parser.get_value(), client_actif, _password);
				if (passwd.check_pass() == 1)
				    CloseClientSocket(fd);

			}
			else if (parser.get_cmd() == "CAP"){
				continue;
			}
			else {
				send(fd, "Please enter password", 21, 0);
				CloseClientSocket(fd);
			}
		}
		else if (client_actif != NULL && client_actif->get_checked_pwd() == true) {
		
			if (parser.get_cmd() == "PRIVMSG"){
				parser.parse_bot(fd, *client_actif, bot);
				// si le parse_bot == false alors on va effectuer un privmsg classique.
			}
			if (parser.get_cmd() == "JOIN")
				parser.parse_join(_clients_array, fd, *client_actif, _channels_array);
			if (parser.get_cmd() == "QUIT"){
				Quit quit(fd, client_actif, parser.get_value(), _channels_array); // revoir quand on envoie une ref??
				quit.send_quit_msg();
				CloseClientSocket(fd);
			}		
			if (parser.get_cmd() == "PING")
				parser.parse_ping(_clients_array, fd, *client_actif);
			if (parser.get_cmd() == "NICK")
				parser.parse_nick(_clients_array, fd, *client_actif, _channels_array, this);
			if (parser.get_cmd() == "USER")
				parser.parse_user(_clients_array, fd, *client_actif);
			if (parser.get_cmd() == "LIST"){
				// parser.parse_list(server, channel, client_count, topic);
				List list(client_actif, _channels_array, fd);
				list.send_list();
			}
			if (parser.get_cmd() == "PART"){
				parser.parse_part(_clients_array, fd, client_actif, _channels_array);
			}

				
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
			CloseClientSocket(fd);
			std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
		}
	}
	else {
		buffer[bytesRecv] = '\0';
		// DEBUT DE CODE ELOUAN
		std::string message(buffer);
		_partial_message[fd] += message;
		process_message(fd);
	}
		
		std::cout << YELLOW << "-----------------------------------------------------" << WHITE << std::endl;
		//SendtoAll(fd, buffer, bytesRecv);
	}
// }

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
