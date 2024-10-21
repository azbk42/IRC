/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:55:49 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/17 14:35:18 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "part.hpp"

Part::Part(std::vector<Channel*> &channels, int client_fd, Client &client_actif, std::string value) : _fd(client_fd),_value(value),_client_actif(&client_actif) ,_channels_list(channels){}
Part::~Part() {}

void Part::_check_channel(std::string channel, std::string reason){
	std::string server_name = SERVER_NAME;
	for (size_t i = 0 ; i < _channels_list.size(); i++){
		if (_channels_list[i]->get_name() == channel){ // verifier que le channel existe
			if (_channels_list[i]->is_in_channel(_client_actif->get_nickname()) == true){ // verifier que le client est dans le channel
				// message de départ à tous les membres du chan
                std::string part_message = PART_MESSAGE(_client_actif->get_nickname(), _client_actif->get_username(), _client_actif->get_hostname(), channel, reason);
				_channels_list[i]->send_message_to_all(part_message, _fd);

                // message de confirmation au client
                send(_fd, part_message.c_str(), part_message.length(), 0);

                // Supprimer le client du chan
                if (_channels_list[i]->remove_client(_client_actif->get_nickname(),*_client_actif) == 1)
				{
					delete _channels_list[i];
					_channels_list.erase(_channels_list.begin() + i);
				}
				return ;
			}
			else
				send(_fd, ERR_NOTONCHANNEL(server_name, channel), strlen(ERR_NOTONCHANNEL(server_name, channel)), 0);
		}
	} send(_fd, ERR_NOSUCHCHANNEL2(server_name, channel), strlen(ERR_NOSUCHCHANNEL2(server_name, channel)), 0);
}

void Part::init_cmd_part(){
	std::string server_name = SERVER_NAME;
	std::string channels;
	std::string reason;
	size_t pos = _value.find(" :");
    if (pos != std::string::npos) {
        channels = _value.substr(0, pos);// Partie contenant les noms des canaux a quitter
        reason = _value.substr(pos + 1);// Partie contenant la raison
		if (reason.size() > 1)
			reason.erase(0, 1);
    } else {
        channels = _value;// Si aucune raison n'est fournie
    }
	
	std::vector<std::string> channels_part = split_by_comma(channels);
	
	for (size_t i = 0; i < channels_part.size(); i++){
		if ((channels_part.size() > 1) && (channels_part[i][0] != '#')){
			send(_fd, ERR_NOSUCHCHANNEL2(server_name, channels_part[i]), strlen(ERR_NOSUCHCHANNEL2(server_name, channels_part[i])), 0);
			continue;
		}
		else {
			_check_channel(channels_part[i], reason);
		}
	}
}

