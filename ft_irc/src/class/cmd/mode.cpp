/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:42:51 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/21 13:10:48 by ctruchot         ###   ########.fr       */
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

void Mode::exec_mode(Channel* channel, std::vector<std::string> values){
	
	for(std::map<int, char>::iterator it = _ordre_exec.begin(); it != _ordre_exec.end(); ++it)
    {
		if (it->second == 'i')
			i_mode(channel, _signe['i']);
		if (it->second == 't')
			t_mode(channel, _signe['t']);
		if (it->second == 'l'){
			if (_signe['l'] == '+') {
				size_t rang = 2 + _ordre_args['l'];
				if (values.size() > rang)
					l_mode(channel, _signe['l'], values[rang]);
				// else
				// 	l_mode(channel, _signe['l'], "");
			}
			else
				l_mode(channel, _signe['l'], "");
		}
		if (it->second == 'k'){
				size_t rang = 2 + _ordre_args['k'];
				if (values.size() > rang)
					k_mode(channel, _signe['k'], values[rang]);
				// else
				// 	k_mode(channel, _signe['k'], "");
		}
		if (it->second == 'o'){
			size_t rang = 2 + _ordre_args['o'];
				if (values.size() > rang)
					o_mode(channel, _signe['o'], values[rang]);
				else
					o_mode(channel, _signe['o'], "");
   		}
	}
	std::string server_name = SERVER_NAME;
	std::string modes;
	for (size_t i = 0; i < _modes_exec.size(); i++)
	{
		if (i != 0)
			modes += " ";
		modes += _modes_exec[i];
	}
	std::string chanmode = ":" + server_name + " 324 " + _client_actif->get_nickname() + "#" + channel->get_name() + " " + channel->get_name() + " " + modes + "\r\n";
	send_message(_client_fd, chanmode);
	channel->send_message_to_all(chanmode, _client_fd);
}

bool is_sign(char c){
	if (c == '+' || c == '-')
		return true;
	return false;
}

bool is_covered_mode(char c){
	if (c == 'i' || c == 't' || c == 'k' || c == 'l' || c == 'o')
		return true;
	return false;
}

bool is_arg_mode(char c, char signe){
	if ((c == 'k' || c == 'l' || c == 'o') && signe == '+')
		return true;
	if ((c == 'k' || c == 'o') && signe == '-')
		return true;
	return false;
}

void Mode::parse_mode(std::string value){
	std::string server_name = SERVER_NAME;
	for (size_t i = 0; i < value.size(); i++){
		char signe;
		if (is_covered_mode(value[i]) == true || is_sign(value[i]) == true) { 
			if (is_sign(value[i]) == true){ //-> if there is a sign, all that follows is of that sign
				signe = value[i];
				i++;
			}
			else if (is_covered_mode(value[i]) == true)
				signe = '+';
			while ((i) < value.size()){ // until we reach the end of value
				if (is_sign(value[i]) == true) //-> except if we meet another sign
					signe = value[i];
				else if (is_covered_mode(value[i]) == true){ //-> if followed by one of the covered modes
					if (_signe.find(value[i]) == _signe.end()) { //-> if this mode has not yet been parsed
						_signe[value[i]] = signe;
						if (_ordre_exec.empty() == true)
							_ordre_exec[0] = value[i];
						else 
							_ordre_exec[_ordre_exec.size()] = value[i];
						if (is_arg_mode(value[i], signe) == true) { //-> for those combinations, we expect a parameter to follow
							if (_ordre_args.empty()) {
								_ordre_args[value[i]] = 0;
							}
							else
								_ordre_args[value[i]] = _ordre_args.size();
						}
					}
				}
				else {
					std::string c(1, value[i]);
					std::string str = ":" + server_name + " 472 " + " :is unknown mode char to me" + c + "\r\n";
					send_message(_client_fd, str);
				}
			i++;}
		}
		else {
			std::string c(1, value[i]);
			std::string str = ":" + server_name + " 472 " + " :is unknown mode char to me" + c + "\r\n";
			send_message(_client_fd, str);
		}
	}
}

void Mode::mode_info(Channel* channel,std::vector<std::string> &values){
	std::string server_name = SERVER_NAME;
	
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
		if ((channel->get_limite() != -1) || (channel->get_pass() == true)){
			_modes_info += " ";
			if (channel->get_limite() != -1)
				_modes_info += int_to_string(channel->get_limite());
			if (channel->get_pass() == true)
				_modes_info += channel->get_password();
		}	
	}					
	std::string chanmode = ":" + server_name + " 324 " + _client_actif->get_nickname() + "#" + channel->get_name() + " " + channel->get_name() + " " + _modes_info + "\r\n";
	send(_client_fd, chanmode.c_str(), chanmode.length(), 0);
	
	// Créer le message RPL_CREATIONTIME
	std::time_t creation_time = channel->get_creation_date();
	std::string creationtime = RPL_CREATIONTIME(server_name, values[0], int_to_string(creation_time));
	send(_client_fd, creationtime.c_str(), creationtime.length(), 0);
}

void Mode::channel_mode(std::vector<std::string> &values){
	std::string server_name = SERVER_NAME;
	
	if (_channels_array.size() == 0){ //-> if there is no channel
		std::string str = ERR_NOSUCHCHANNEL2(server_name, values[0]);
		send_message(_client_fd, str);
		return ;
	}
	for (size_t i = 0; i < _channels_array.size(); i++) { 
		if (_channels_array[i]->get_name() == values[0]){ // -> check if the channel exists
			if (values.size() == 1) {// ->if there is no other value than name of chan -> Channel #samurai created Mon Sep 30 14:17:56 2024
				mode_info(_channels_array[i], values);
				return ;
			}
			else { // -> si plusieurs arguments, verifie que le client est operateur du chan
				if (_channels_array[i]->is_operator(_client_actif->get_nickname()) == false){
					std::string notope = ERR_CHANOPRIVSNEEDED(server_name, _client_actif->get_nickname(),_channels_array[i]->get_name());
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
	std::string str = ERR_NOSUCHCHANNEL2(server_name, values[0]);
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
