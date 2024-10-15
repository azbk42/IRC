/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:02:05 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/15 12:22:10 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quit.hpp"
#include "server.hpp"

Quit::Quit(int fd, Client* client, std::string reason, std::vector<Channel*> channels) : _client_fd(fd), _client(client), _reason(reason), _channels(channels){};
Quit::~Quit() {};

void Quit::send_quit_msg() {
	std::string nickname = _client->get_nickname();
	std::string username = _client->get_username();
	std::string hostname = _client->get_hostname();

	if (_reason[0] == ':')
		_reason.erase(0, 1); // Supprimer le premier espace

	std::string quit_message = QUIT_MESSAGE(nickname, username, hostname, "Quit: " + _reason);

	for (int i = 0; i < _channels.size(); i++){
        if (_channels[i]->is_in_channel(nickname)){
            _channels[i]->send_message_to_all(quit_message, _client_fd); // Envoie à tous les autres clients du canal
			_channels[i]->remove_client(nickname, _client_fd, *_client, _reason); // Supprime le client du canal		}
	}
 	}
	// The QUIT command is used to terminate a client’s connection to the server. 
	// The server acknowledges this by replying with an ERROR message and closing the connection to the client.


	// If a client connection is closed without the client issuing a QUIT command to the server, 
	//the server MUST distribute a QUIT message to other clients informing them of this, 
	// distributed in the same was an ordinary QUIT message. 
}