/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:04:57 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/17 14:43:31 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.hpp"
#include "include.hpp"
#include <sstream> // Pour std::ostringstream

// ################################################################################
// #                         Constructor / Destructor                             #
// ################################################################################

List::List(Client &client_actif, std::vector<Channel*> &channels_array, int client_fd, std::string value): _client_fd(client_fd), _channels_array(channels_array),_client(&client_actif) , _value(value){};
List::~List() {}

// ################################################################################
// #                                   Methods                                    #
// ################################################################################

void List::list_with_args(std::string client_name, std::string server_name){
	std::string msg_list = "Channel Users Name\r\n";
	send(_client_fd, msg_list.c_str(), msg_list.length(), 0);

	// separer selon coma
	std::vector<std::string> channels_listed = split_by_comma(_value);

	// checker pour chq value
	for (size_t j = 0; j < channels_listed.size(); j++){
		if (channels_listed[j][0] == '#'){
			for (size_t i = 0; i < _channels_array.size(); i++){
				if (_channels_array[i]->get_name() == channels_listed[j]){
					send_channel(client_name, server_name, _channels_array[i]);
				}
			}
		}
		else {
			std::string bad_arg = "Bad list syntax, type /quote list ? or /raw list ?\r\n";
			send(_client_fd, bad_arg.c_str(), bad_arg.length(), 0);
			break;
		}
	}
	send(_client_fd, RPL_LISTEND(client_name), strlen(RPL_LISTEND(client_name)), 0);
}

void List::send_channel(std::string client_name, std::string server_name, Channel *channel) const{
	std::string channel_name = channel->get_name();
	channel_name.erase(0, 1);

	int client_nb = channel->get_nb_client();
	std::string client_count = int_to_string(client_nb);
	std::string topic = channel->get_topic();
	send(_client_fd, RPL_LIST(client_name, channel_name, client_count, topic), 
			strlen(RPL_LIST(client_name, channel_name, client_count, topic)), 0);
}

void List::send_list()
{
	std::string server_name = SERVER_NAME;
	std::string client_name = _client->get_nickname();
	
	if (_value.empty()){
		for (size_t i = 0; i < _channels_array.size(); i++){
			send_channel(client_name, server_name, _channels_array[i]);				
		}
		send(_client_fd, RPL_LISTEND(client_name), strlen(RPL_LISTEND(client_name)), 0);
	}
	else {
		list_with_args(client_name, server_name);
	}
}

