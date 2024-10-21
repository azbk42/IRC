/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:42:51 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/21 17:19:40 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode.hpp"
#include <cctype>
#include <map>

// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Mode::Mode(std::vector<Client*> &clients_list, Client &client_actif, std::vector<Channel*> &channels_array, int client_fd, std::string value) : 
	_client_fd(client_fd), _value(value),_client_actif(&client_actif),_clients(clients_list), _channels_array(channels_array), _modes_info("+") {}

Mode::~Mode() {}

// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

void Mode::fill_modes_exec(std::string mode, std::string signe, std::string value)
{
    if (_modes_exec.empty()) {
        _modes_exec.push_back(signe + mode);
    } else {
        unsigned long pos = static_cast<unsigned long>(_modes_exec[0].find(signe));
        if (signe == "+" && pos == std::string::npos) {
            _modes_exec[0] += signe;
        }
        if (signe == "+" && pos != std::string::npos) {
            unsigned long plus_pos = static_cast<unsigned long>(_modes_exec[0].find("-"));
            if (plus_pos != std::string::npos && plus_pos > pos) {
                _modes_exec[0] += signe;
            }
        }
        if (signe == "-" && pos == std::string::npos) {
            _modes_exec[0] += signe;
        }
        if (signe == "-" && pos != std::string::npos) {
            unsigned long neg_pos = static_cast<unsigned long>(_modes_exec[0].find("+"));
            if (neg_pos != std::string::npos && neg_pos > pos) {
                _modes_exec[0] += signe;
            }
        }
        _modes_exec[0] += mode;
    }
    if (mode == "k" || mode == "o" || mode == "l") {
        _modes_exec.push_back(value);
    }
}

void Mode::print_modes(Channel* channel){
	std::string modes;
	for (size_t i = 0; i < _modes_exec.size(); i++)
	{
		if (i != 0)
			modes += " ";
		modes += _modes_exec[i];
	}
	if (modes.empty() == true)
		return;
	std::string chanmode = ":" + std::string(SERVER_NAME) + " 324 " + _client_actif->get_nickname() + "#" + channel->get_name() + " " + channel->get_name() + " " + modes + "\r\n";
	send_message(_client_fd, chanmode);
	channel->send_message_to_all(chanmode, _client_fd);
}

void Mode::mode_info(Channel* channel,std::vector<std::string> &values){	
	// Créer le message RPL_CHANNELMODEIS
	if (channel->get_i() == true || channel->get_t() == true || channel->get_pass() == true || channel->get_limite() != -1) {
		if (channel->get_t() == true)
			_modes_info += "t";
		if (channel->get_i() == true)
			_modes_info += "i";
		if (channel->get_limite() != -1)
			_modes_info += "l";
		if (channel->get_pass() == true)
			_modes_info += "k";
		if (channel->get_limite() != -1)
			_modes_info += " " + int_to_string(channel->get_limite());
		if (channel->get_pass() == true)
			_modes_info += " " + channel->get_password();
	}					
	std::string chanmode = ":" + std::string(SERVER_NAME) + " 324 " + _client_actif->get_nickname() + "#" + channel->get_name() + " " + channel->get_name() + " " + _modes_info + "\r\n";
	send(_client_fd, chanmode.c_str(), chanmode.length(), 0);
	
	// Créer le message RPL_CREATIONTIME
	std::time_t creation_time = channel->get_creation_date();
	std::string creationtime = RPL_CREATIONTIME(std::string(SERVER_NAME), values[0], int_to_string(creation_time));
	send(_client_fd, creationtime.c_str(), creationtime.length(), 0);
}

void Mode::channel_mode(std::vector<std::string> &values){
	if (_channels_array.size() == 0){ //-> if there is no channel in the server
		std::string str = ERR_NOSUCHCHANNEL(std::string(SERVER_NAME), values[0]);
		send_message(_client_fd, str);
		return ;
	}
	for (size_t i = 0; i < _channels_array.size(); i++) { 
		if (_channels_array[i]->get_name() == values[0]){ // -> check if the channel exists
			if (values.size() == 1) {// ->if there is no other value than name of channel send modes message and creation time message
				mode_info(_channels_array[i], values);
				return ;
			}
			else { // -> si plusieurs arguments, verifie que le client est operateur du chan
				if (values[i] == "b")
					return;
				if (_channels_array[i]->is_operator(_client_actif->get_nickname()) == false){
					std::string notope = ERR_CHANOPRIVSNEEDED(std::string(SERVER_NAME), _client_actif->get_nickname(),_channels_array[i]->get_name());
					send_message(_client_fd, notope);
					return ;
				}
				else { //-> si le client est bien operateur, on parse les arguments avant d'executer les modes
					parse_mode(values[1]);
					exec_mode(_channels_array[i], values);
					return ;
				}
			}
		}
	}
	std::string str = ERR_NOSUCHCHANNEL(std::string(SERVER_NAME), values[0]);
	send_message(_client_fd, str);
}

void Mode::init_cmd_mode(){
	std::vector<std::string> values = split_by_space(_value); //-> split value by " " to get a vector of strings
	
	for (size_t i = 0; i < _clients.size(); i++){ 
		if (_clients[i]->get_nickname() == values[0]){ // -> check if the first value is a user
			return ; // user modes are not covered in the subject
		}
	}
	channel_mode(values); //-> if not a user, we treat it as a channel
}
