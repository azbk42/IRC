/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_exec.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:54:41 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/22 18:30:58 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode.hpp"

//  i : Définir/supprimer le canal sur invitation uniquement
void Mode::i_mode(Channel *channel, char signe){
	if (signe == '-')
		channel->set_i('-');
	else if (signe == '+')
		channel->set_i('+');
	std::string sign(1, signe);
	fill_modes_exec("i", sign, "");
}

	// — t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
void Mode::t_mode(Channel *channel, char signe){
	if (signe == '-')
		channel->set_t('-');
	else if (signe == '+')
		channel->set_t('+');
	std::string sign(1, signe);
	fill_modes_exec("t", sign, "");
}

	// — k : Définir/supprimer la clé du canal (mot de passe)
void Mode::k_mode(Channel *channel, char signe, std::vector<std::string> values){
	size_t rang = 2 + _ordre_args['k'];
				
	if (signe == '-') {
		std::string sign(1, signe);
		if (channel->get_pass() == true)
		{
			fill_modes_exec("k", sign, channel->get_password());
			channel->set_pass(false);
			channel->set_password("");
		}
		else if (channel->get_pass() == false) {
			fill_modes_exec("k", sign, values[rang]);
		}
	}
	else if (signe == '+' && values.size() > rang){
		channel->set_pass(true);
		channel->set_password(values[rang]);
		std::string sign(1, signe);
		fill_modes_exec("k", sign, values[rang]);
	}
}

// — l : Définir/supprimer la limite d’utilisateurs pour le canal
void Mode::l_mode(Channel *channel, char signe, std::vector<std::string> values){
	if (signe == '-') {
		std::string sign(1, signe);
		fill_modes_exec("l", sign, "");
		if (channel->get_limite() != -1)
			channel->set_limite(-1);
	}
	if (signe == '+') {
		size_t rang = 2 + _ordre_args['l'];
		if (values.size() > rang){ // s'il y a un parametre
			long limite = get_digit(values[rang]);
			if (limite < 0)
				return ;
			channel->set_limite(limite);									
			std::string sign(1, signe);		
			fill_modes_exec("l", sign, int_to_string(limite));
		}
		else { // il n'y a pas de parametre
			std::string server_name = SERVER_NAME;
			std::string str = ERR_NEEDMOREPARAMS("MODE +l",server_name);
			send_message(_client_fd, str);
			return ;
		}		
	}
}

// — o : Donner/retirer le privilège de l’opérateur de canal
void Mode::o_mode(Channel *channel, char signe, std::vector<std::string> values){
	size_t rang = 2 + _ordre_args['o'];
	if (values.size() <= rang)
		return ;
	else if (channel->is_in_channel(values[rang])) // -> check if user is in the channel
	{
		if (channel->is_operator(values[rang]) == true && signe == '-') { // if is operator
			channel->remove_operator(values[rang]);
			std::string sign(1, signe);
			fill_modes_exec("o", sign, values[rang]);

			std::string message_op = (":" + std::string("localhost") + " MODE " + channel->get_name() + " -o " + values[rang] + "\r\n");
			send_message(_client_fd, message_op);
			channel->send_message_to_all(message_op, _client_fd);
		}
		else if (channel->is_operator(values[rang]) == false && signe == '+') {// if is not yet operator
			channel->add_operator(values[rang]);
			std::string sign(1, signe);
			fill_modes_exec("o", sign, values[rang]);

			std::string message_op = (":" + std::string("localhost") + " MODE " + channel->get_name() + " +o " + values[rang] + "\r\n");
			send_message(_client_fd, message_op);
			channel->send_message_to_all(message_op, _client_fd);
		}
	}
	else {
		std::string str1 =  ERR_NOTONCHANNEL1(std::string(SERVER_NAME), values[rang], channel->get_name()); // "No such nick/channel"
		std::string str =  ERR_NOTONCHANNEL2(std::string(SERVER_NAME), values[rang], channel->get_name()); // "They aren't on that channel"
		send_message(_client_fd, str1);
		send_message(_client_fd, str);
	}
}

void Mode::exec_mode(Channel* channel, std::vector<std::string> values){
	for(std::map<int, char>::iterator it = _ordre_exec.begin(); it != _ordre_exec.end(); ++it)
    {
		if (it->second == 'i')
			i_mode(channel, _signe['i']);
		if (it->second == 't')
			t_mode(channel, _signe['t']);
		if (it->second == 'l')
			l_mode(channel, _signe['l'], values);
		if (it->second == 'k')
			k_mode(channel, _signe['k'], values);
		if (it->second == 'o')
			o_mode(channel, _signe['o'], values);
	}
	print_modes(channel);
}