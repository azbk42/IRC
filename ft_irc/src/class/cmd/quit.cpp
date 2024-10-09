/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:02:05 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/09 11:46:40 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quit.hpp"
#include "server.hpp"

Quit::Quit(int fd, Client* client, std::string reason, std::vector<Channel*> channels) : _client_fd(fd), _client(client), _reason(reason), _channels(channels){};
Quit::~Quit() {};

void Quit::send_quit_msg() {
	std::string full_quit_message;
    full_quit_message = _client->get_nickname() + "has quit [QUIT: " + _reason + "]\r\n"; // nick [full] 
    // azbk_ [~ctruchot@bc22-251f-212f-9745-7c26.210.62.ip] has quit [Quit: 
        //   ciao]
	send(_client_fd, full_quit_message.c_str(), full_quit_message.size(), 0);
	for (int i = 0; i < _channels.size(); i++){
        if (_channels[i]->is_in_channel(_client->get_nickname())){
            _channels[i]->send_message_to_all(full_quit_message, _client_fd); // Envoie à tous les autres clients du canal
        }
	}
	
	// The QUIT command is used to terminate a client’s connection to the server. 
	// The server acknowledges this by replying with an ERROR message and closing the connection to the client.


	// If a client connection is closed without the client issuing a QUIT command to the server, 
	//the server MUST distribute a QUIT message to other clients informing them of this, 
	// distributed in the same was an ordinary QUIT message. 
}