/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:04:57 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/22 18:31:07 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.hpp"
#include "include.hpp"

// ################################################################################
// #                         Constructor / Destructor                             #
// ################################################################################

List::List(Client &client_actif, std::vector<Channel*> &channels_array, int client_fd, std::string value): 
_client_fd(client_fd), _channels_array(channels_array),_client(&client_actif) , _value(value){};

List::~List() {}

// ################################################################################
// #                                   Methods                                    #
// ################################################################################

void List::list_with_args(std::string client_name){
	std::string msg_list = "Channel Users Name\r\n";
	send_message(_client_fd, msg_list);

	std::vector<std::string> channels_listed = split_by_comma(_value); //->separe la liste de channels selon coma

	for (size_t j = 0; j < channels_listed.size(); j++){ //->checke pour chq channel liste si # et si le channel existe
		if (channels_listed[j][0] == '#'){
			for (size_t i = 0; i < _channels_array.size(); i++){
				if (to_uppercase(_channels_array[i]->get_name()) == to_uppercase(channels_listed[j]))
					send_channel(client_name, _channels_array[i]);
			}
		}
		else {
			std::string bad_arg = "Bad list syntax, type /quote list ? or /raw list ?\r\n";
			send_message(_client_fd, bad_arg);
			break;
		}
	}
	send_message(_client_fd, RPL_LISTEND(client_name));
}

void List::send_channel(std::string client_name, Channel *channel) const{
	std::string channel_name = channel->get_name();
	channel_name.erase(0, 1);

	int client_nb = channel->get_nb_client();
	std::string client_count = int_to_string(client_nb);
	std::string topic = channel->get_topic();
	send_message(_client_fd, RPL_LIST(client_name, channel_name, client_count, topic));
}

void List::send_list(){
	if (_value.empty()){
		for (size_t i = 0; i < _channels_array.size(); i++){
			send_channel(_client->get_nickname(), _channels_array[i]);				
		}
		send_message(_client_fd, RPL_LISTEND(_client->get_nickname()));
	}
	else
		list_with_args(_client->get_nickname());
}

