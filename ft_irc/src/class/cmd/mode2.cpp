/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:54:41 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/18 18:32:55 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode.hpp"
#include <cctype>
#include <map>

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
void Mode::k_mode(Channel *channel, char signe, std::string value){
	if (signe == '-') {
		std::string sign(1, signe);
		if (channel->get_pass() == true)
		{
			fill_modes_exec("k", sign, channel->get_password());
			channel->set_pass(false);
			channel->set_password("");
		}
		else if (channel->get_pass() == false) {
			fill_modes_exec("k", sign, value);
		}
	}
	else if (signe == '+' && value.empty() == false){
		channel->set_pass(true);
		channel->set_password(value);
		std::string sign(1, signe);
		fill_modes_exec("k", sign, value);
	}
}

// — l : Définir/supprimer la limite d’utilisateurs pour le canal
void Mode::l_mode(Channel *channel, char signe, std::string value){
	std::string server_name = SERVER_NAME;
	
	if (signe == '-') { // dans ce cas on ignore totalement ce qui vient apres?
		std::string sign(1, signe);
		fill_modes_exec("l", sign, "");
		if (channel->get_limite() != -1) // et si y a une limite
			channel->set_limite(-1);
	}
	else if (signe == '+' && value.empty() == true) {
		std::string str = ERR_NEEDMOREPARAMS2("MODE +l", server_name);
		send(_client_fd, str.c_str(), str.length(), 0);
		return ;
	}
	else if (signe == '+' && value.empty() == false) {
		long limite = get_digit(value);
		if (limite < 0)
			return ;
		channel->set_limite(limite);									
		std::string sign(1, signe);		
		fill_modes_exec("l", sign, int_to_string(limite));
	}
}

// — o : Donner/retirer le privilège de l’opérateur de canal
void Mode::o_mode(Channel *channel, char signe, std::string user){
	std::string server_name = SERVER_NAME;

	if (user.empty() == true)
		return ;
	// verifier que lútilisateur existe ?
	else if (channel->is_in_channel(user)) // -> check if user is in the channel
	{
		if (channel->is_operator(user) == true && signe == '-') { // si il est ope mais quon veut le retirer
			channel->remove_operator(user);
			std::string sign(1, signe);
			fill_modes_exec("o", sign, user);

			std::string message_op = (":" + std::string("localhost") + " MODE " + channel->get_name() + " -o " + user + "\r\n");
			send_message(_client_fd, message_op);
			channel->send_message_to_all(message_op, _client_fd);
		}
		else if (channel->is_operator(user) == false && signe == '+') {// si il est pas operateur, mais quón veut lui donner ce role
			channel->add_operator(user);
			std::string sign(1, signe);
			fill_modes_exec("o", sign, user);

			std::string message_op = (":" + std::string("localhost") + " MODE " + channel->get_name() + " +o " + user + "\r\n");
			send_message(_client_fd, message_op);
			channel->send_message_to_all(message_op, _client_fd);
		}
	}
	else {
		std::string str1 =  ERR_NOTONCHANNEL1(server_name, user, channel->get_name()); // yo :No such nick/channel
		std::string str =  ERR_NOTONCHANNEL2(server_name, user, channel->get_name()); // yo #camtrooo They aren't on that channel
		send(_client_fd, str1.c_str(), str1.length(), 0);
		send(_client_fd, str.c_str(), str.length(), 0);
	}
}