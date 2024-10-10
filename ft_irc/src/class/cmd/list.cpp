/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:04:57 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/10 16:43:03 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.hpp"
#include "include.hpp"
#include <sstream> // Pour std::ostringstream

List::List(Client *client_actif, std::vector<Channel*> channels_array, int client_fd, std::string value): _client(client_actif), _channels_array(channels_array), _client_fd(client_fd), _value(value){};
List::~List() {}
		
// void List::send_list(){
// 	send(_client_fd, "List of channels\r\n", strlen("List of channels\r\n"), 0);
// }

// send(fd, RPL_LIST(server, channel, client_count, topic), strlen(RPL_LIST(server, channel, client_count, topic)), 0);
// 				send(fd, RPL_LISTEND(server_name), strlen(RPL_LISTEND(server_name)), 0);
void List::send_list()
{
	std::string client_name = _client->get_nickname();
	std::string server_name = SERVER_NAME;
	if (_value.empty()){
		for (size_t i = 0; i < _channels_array.size(); i++){
			send_channel(client_name, server_name, _channels_array[i]);
			// std::string channel_name = _channels_array[i]->get_name();
			// int client_nb = _channels_array[i]->get_nb_client();
			// std::ostringstream oss;
			// oss << client_nb;
			// std::string client_count =  oss.str();
			// std::string topic = _channels_array[i]->get_topic();
			// send(fd, RPL_LIST(client_name, channel_name, client_count, topic), 
			// 	strlen(RPL_LIST(client_name, channel_name, client_count, topic)), 0);				
			}
		send(_client_fd, RPL_LISTEND(client_name), strlen(RPL_LISTEND(client_name)), 0);
	}
	else {
		std::string msg_list = "Channel Users Name\r\n";
		send(_client_fd, msg_list.c_str(), msg_list.length(), 0);
		
		// separer selon coma
		std::vector<std::string> channels_listed = split_by_comma(_value);

		// checker pour chq value
		for (size_t j = 0; j < channels_listed.size(); j++){
			if (channels_listed[j][0] == '#'){
				channels_listed[j].erase(0, 1);
				for (size_t i = 0; i < _channels_array.size(); i++){
					if (_channels_array[i]->get_name() == channels_listed[j]){
						send_channel(client_name, server_name, _channels_array[i]);
					}
				}
			}
			else {
				std::string bad_arg = "Bad list syntax, type /quote list ? or /raw list ?\r\n";
				send(_client_fd, bad_arg.c_str(), bad_arg.length(), 0);
			}
		}
		send(_client_fd, RPL_LISTEND(client_name), strlen(RPL_LISTEND(client_name)), 0);
	}
}

std::string List::int_to_string(int value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

void List::send_channel(std::string client_name, std::string server_name, Channel *channel){
	std::string channel_name = channel->get_name();
	int client_nb = channel->get_nb_client();
	// std::ostringstream oss;
   	// oss << client_nb;
	std::string client_count = int_to_string(client_nb);
	std::string topic = channel->get_topic();
	send(_client_fd, RPL_LIST(client_name, channel_name, client_count, topic), 
			strlen(RPL_LIST(client_name, channel_name, client_count, topic)), 0);	
}
