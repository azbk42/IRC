/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:55:49 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/09 16:53:02 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "part.hpp"

Part::Part(std::vector<Channel*> &channels, int client_fd, Client *client_actif, std::string value) : _channels_list(channels), _fd(client_fd), _client_actif(client_actif), _value(value){}
Part::~Part() {}

// ################################################################################
// #                                PRIVATE METHOD                                #
// ################################################################################


std::vector<std::string> Part::_split_by_comma(const std::string &input)
{
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}
void Part::_check_channel(std::string channel){
	channel.erase(0, 1);// supprimer le #
	
	std::string server_name = SERVER_NAME;
	for (int i = 0 ; i < _channels_list.size(); i++){
		if (_channels_list[i]->get_name() == channel){ // verifier que le channel existe
			if (_channels_list[i]->is_in_channel(_client_actif->get_nickname()) == true){ // verifier que le client est dans le channel
				
				_channels_list[i]->remove_client; // supprimer le client du channel
				// envoyer message de depart
			}
			else
				send(_fd, ERR_NOTONCHANNEL(server_name, channel), strlen(ERR_NOTONCHANNEL(server_name, channel)), 0);
		}
		// 	// verifier que le client est dans le channel
		// 	// ERR_NOTONCHANNEL (442) 
		else
			send(_fd, ERR_NOSUCHCHANNEL2(server_name, channel), strlen(ERR_NOSUCHCHANNEL2(server_name, channel)), 0);
	}
	// verifier que le client est dans le channel
	// supprimer le client du channel
	// envoyer message de depart
}

void Part::init_cmd_part(){
	std::vector<std::string> channels_part = _split_by_comma(_value);
	std::string server_name = SERVER_NAME;

	for (int i = 0; i < channels_part.size(); i++){
		if (channels_part[i][0] != '#'){
			// erreur NOSUCHCHANNEL
			send(_fd, ERR_NOSUCHCHANNEL2(server_name, channels_part[i]), strlen(ERR_NOSUCHCHANNEL2(server_name, channels_part[i])), 0);
			continue;
		}
		else {
			_check_channel(channels_part[i]);
		}
	}

}

