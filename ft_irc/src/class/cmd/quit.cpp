/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:02:05 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/18 15:12:17 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quit.hpp"
#include "server.hpp"

Quit::Quit(int fd, Client* client, std::string reason, std::vector<Channel*> &channels) : _client_fd(fd), _client(client), _reason(reason), _channels(channels){};
Quit::~Quit() {};

void Quit::send_quit_msg() {
	if (_reason[0] == ':')
		_reason.erase(0, 1); // Supprimer le premier espace

	std::string quit_message = QUIT_MESSAGE(_client->get_nickname(), _client->get_username(), _client->get_hostname(), "Quit: " + _reason);

	for (size_t i = 0; i < _channels.size(); i++){
        if (_channels[i]->is_in_channel(_client->get_nickname())){
            _channels[i]->send_message_to_all(quit_message, _client_fd); // Envoie à tous les autres clients du canal
			// _channels[i]->remove_client(_client->get_nickname(), _client_fd, *_client, _reason); // Supprime le client du canal		}
			if (_channels[i]->remove_client(_client->get_nickname(),*_client) == 1)
			{
				delete _channels[i];
				_channels.erase(_channels.begin() + i);
			}
	}
 	}
}