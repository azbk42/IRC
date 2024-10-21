/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_parse.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:50:28 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/21 17:18:33 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode.hpp"

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

void send_unknown_char(char value, int fd){
	std::string server_name = SERVER_NAME;

	std::string c(1, value);
	std::string str = ":" + server_name + " 472 " + " :is unknown mode char to me" + c + "\r\n";
	send_message(fd, str);
}

void Mode::parse_mode(std::string value){
	std::string server_name = SERVER_NAME;
	for (size_t i = 0; i < value.size(); i++){
		char signe;
		if (is_covered_mode(value[i]) == true || is_sign(value[i]) == true) { 
			if (is_sign(value[i]) == true){ //-> if there is a sign, all that follows is of that sign
				signe = value[i];
				i++; }
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
							if (_ordre_args.empty())
								_ordre_args[value[i]] = 0;
							else
								_ordre_args[value[i]] = _ordre_args.size();}
					}
				}
				else 
					send_unknown_char(value[i], _client_fd);
			i++;}
		}
		else
			send_unknown_char(value[i], _client_fd);
	}
}