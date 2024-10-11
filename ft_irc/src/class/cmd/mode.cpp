/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:42:51 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/11 17:08:13 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode.hpp"



Mode::Mode(std::vector<Client*> &clients_list, std::vector<Channel*> channels_array, int client_fd, std::string value) : 
	_client_fd(client_fd), _clients(clients_list), _channels_array(channels_array), _value(value){}

Mode::~Mode() {}

// ################################################################################

void Mode::user_mode(std::vector<std::string> values){
	
}

std::string Mode::format_creation_time(Channel *channel){
	std::time_t creation_time = channel->get_creation_date();
	char buffer[80];
	struct tm * timeinfo = localtime(&creation_time);
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
	std::string creation_str = buffer;
	return (creation_str);
}

void Mode::channel_mode(std::vector<std::string> values){
	std::string server_name = SERVER_NAME;
	// if (values[0][0] != '#'){
	// 	send(_client_fd, ERR_NOSUCHCHANNEL2(server_name, values[0]), strlen(ERR_NOSUCHCHANNEL2(server_name, values[0])), 0);
	// 	return ;
	// }
	// else {
	// 	values[0].erase(0, 1);
		for (int i = 0; i < _channels_array.size(); i++) {
			if (_channels_array[i]->get_name() == values[0]){
				if (values.size() == 1){ // si pas d'autre argument, Channel #samurai created Mon Sep 30 14:17:56 2024
					std::cout <<"ici" << std::endl;
					send(_client_fd, RPL_CHANNELMODEIS(server_name, values[0]), strlen(RPL_CHANNELMODEIS(server_name, values[0])), 0);
					send(_client_fd, RPL_CREATIONTIME(server_name, values[0], format_creation_time(_channels_array[i])), strlen(RPL_CREATIONTIME(server_name, values[0], format_creation_time(_channels_array[i]))), 0);
				}
			}
			else
				send(_client_fd, ERR_NOSUCHCHANNEL2(server_name, values[0]), strlen(ERR_NOSUCHCHANNEL2(server_name, values[0])), 0);
		}
	
}

void Mode::init_cmd_mode(){
	// split value by " "
	std::vector<std::string> values = split_by_space(_value);
	
	// verifier si la premiere partie de value est un user
	for (int i = 0; i < _clients.size(); i++){
		if (_clients[i]->get_nickname() == values[0]){ // si values[0] est un user
			return ;
		}
	}
	channel_mode(values); // si values[0] n'est pas un user, on le traite comme un channel
	
	
	// si on est en channel
	// si on est en user
}
