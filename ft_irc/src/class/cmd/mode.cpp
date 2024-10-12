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
#include <cctype>

Mode::Mode(std::vector<Client*> &clients_list, Client *client_actif, std::vector<Channel*> channels_array, int client_fd, std::string value) : 
	_client_fd(client_fd), _clients(clients_list), _client_actif(client_actif), _channels_array(channels_array), _value(value){}

Mode::~Mode() {}

// ################################################################################

void Mode::i_mode(Channel *channel, char signe){
	//  i : Définir/supprimer le canal sur invitation uniquement
	// pas besoin de parametre
	if (channel->get_i() == true && signe == '-')
		channel->set_i('-');
	if (channel->get_i() == false && signe == '+')
		channel->set_i('+');
	// si pas de changement, pas de message dérreur?
}
// t_mode(Channel *channel){
// 	// — t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux

// }
// k_mode(Channel *channel){
// 	// — k : Définir/supprimer la clé du canal (mot de passe)
// }

void Mode::o_mode(Channel *channel, char signe, std::string user){
// — o : Donner/retirer le privilège de l’opérateur de canal
// verifier que lútilisateur existe
// verifier quíl est bien dans le canal
// verifier ses droits
	if (channel->is_in_channel(user)) // si pas ds le channel, doit lájouter au chan ?? 
	{
		if (channel->is_operator(user) == true && signe == '-') // si il est ope mais quon veut le retirer
			channel->remove_operator(user);
		if (channel->is_operator(user) == false && signe == '+')// si il est pas operateur, mais quón veut lui donner ce role
			channel->add_operator(user);
	} // ajouter un message de confirmation??
 // on ignore les autres cas ??
//   si on a dáutres arg apres??
}
bool Mode::isfulldigit(std::string str){
	for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit(str[i])) {
            return false; // Si un caractère n'est pas un chiffre, retourne faux
        }
	}
	return true;
}

void Mode::l_mode(Channel *channel, std::vector<std::string> values){
// — l : Définir/supprimer la limite d’utilisateurs pour le canal
	if (values[1][0] == '-') { // dans ce cas on ignore totalement ce qui vient apres?
		if (channel->get_limite() != -1) // et si y a une limite
			channel->set_limite(-1);
	}
	if (values[1][0] == '+' && values.size() >= 3){
		if (isfulldigit(values[2]) == true){
			int limite = string_to_int(values[2]);
			channel->set_limite(limite);									
		}
	}
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
			if (_channels_array[i]->get_name() == values[0]){ // verifier si le chan existe
				if (values.size() == 1){ // si pas d'autre argument, Channel #samurai created Mon Sep 30 14:17:56 2024
					send(_client_fd, RPL_CHANNELMODEIS(server_name, values[0]), strlen(RPL_CHANNELMODEIS(server_name, values[0])), 0);
					send(_client_fd, RPL_CREATIONTIME(server_name, values[0], format_creation_time(_channels_array[i])), strlen(RPL_CREATIONTIME(server_name, values[0], format_creation_time(_channels_array[i]))), 0);
				}
				else { // si plusieurs arguments, verifie que le client est operateur du chan
					// std::vector<std::string> operators = _channels_array[i]->get_operators();
					if (_channels_array[i]->is_operator(_client_actif->get_nickname()) == false){
						send(_client_fd, ERR_CHANOPRIVSNEEDED(server_name, _channels_array[i]->get_name()), strlen(ERR_CHANOPRIVSNEEDED(server_name, _channels_array[i]->get_name())), 0);
					}
					else { // si le client est bien operateur, on regarde lárgument suivant
						if (values[1][0] == '+' || values[1][0] == '-') {
							if (values[1][1] == 'i') // faire un switch ?
								i_mode(_channels_array[i], values[1][0]);
							// if (values[1][1] == 't')
							// 	t_mode(_channels_array[i])
							// if (values[1][1] == 'k')
							// 	k_mode(_channels_array[i])
							if (values[1][1] == 'o')
								if (values.size() >= 3)
									o_mode(_channels_array[i], values[1][0], values[2]);
								// si pas dárgument apres +-o, on ignore simplement
							if (values[1][1] == 'l')
								l_mode(_channels_array[i], values);
						}
						// else // quelle erreur si le 2e arg est farfelu?
					}
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
