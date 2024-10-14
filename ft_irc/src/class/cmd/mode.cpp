/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:42:51 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/14 16:14:56 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode.hpp"
#include <cctype>
#include <map>

			// si 2e arg, mais pas en attente d;arg, (pas de koul) le traite comme un nouveau mode



// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Mode::Mode(std::vector<Client*> &clients_list, Client *client_actif, std::vector<Channel*> channels_array, int client_fd, std::string value) : 
	_client_fd(client_fd), _clients(clients_list), _client_actif(client_actif), _channels_array(channels_array), _value(value){}

Mode::~Mode() {}

// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

	//  i : Définir/supprimer le canal sur invitation uniquement
void Mode::i_mode(Channel *channel, char signe){
	if (channel->get_i() == true && signe == '-')
		channel->set_i('-');
	if (channel->get_i() == false && signe == '+')
		channel->set_i('+');
}

	// — t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
void Mode::t_mode(Channel *channel, char signe){
	if (channel->get_t() == true && signe == '-')
		channel->set_t('-');
	if (channel->get_t() == false && signe == '+')
		channel->set_t('+');
	
}

	// — k : Définir/supprimer la clé du canal (mot de passe)
void Mode::k_mode(Channel *channel, char signe, std::string value){
	if (channel->get_pass() == true && signe == '-') {
		channel->set_pass(false);
		channel->set_password(NULL);
	}
	if (signe == '+' && value.empty() == false){
		channel->set_pass(true);
		channel->set_password(value);
	}
}

// — l : Définir/supprimer la limite d’utilisateurs pour le canal
void Mode::l_mode(Channel *channel, char signe, std::string value){
	std::string server_name = SERVER_NAME;

	if (signe == '-') { // dans ce cas on ignore totalement ce qui vient apres?
		if (channel->get_limite() != -1) // et si y a une limite
			channel->set_limite(-1);
	}
	if (signe == '+' && value.empty() == true) {\
		std::string str = ERR_NEEDMOREPARAMS2("MODE +l", server_name);
		std::cout << str << std::endl;
		send(_client_fd, str.c_str(), str.length(), 0);
	}
	if (signe == '+' && value.empty() == false) {
		if (isfulldigit(value) == true){
			int limite = string_to_int(value);
			channel->set_limite(limite);									
		}
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
		if (channel->is_operator(user) == true && signe == '-') // si il est ope mais quon veut le retirer
			channel->remove_operator(user);
		if (channel->is_operator(user) == false && signe == '+')// si il est pas operateur, mais quón veut lui donner ce role
			channel->add_operator(user);
	} // ajouter un message de confirmation a tous ??
	else {
		// yo: No such nick/channel
		std::string str =  ERR_NOTONCHANNEL2(server_name, user, channel->get_name()); // yo #camtrooo They aren't on that channel
		send(_client_fd, str.c_str(), str.length(), 0);
	}
}

bool Mode::isfulldigit(std::string str){
	for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit(str[i])) {
            return false; // Si un caractère n'est pas un chiffre, retourne faux
        }
	}
	return true;
}


void Mode::exec_mode(Channel* channel, std::vector<std::string> values){
	if (_signe.find('i') != _signe.end()){
		i_mode(channel, _signe['i']);
	}
	if (_signe.find('t') != _signe.end())
		t_mode(channel, _signe['t']);
	if (_signe.find('l')!= _signe.end()){
		if (_signe['l'] == '+') {
			int rang = 2 + _ordre['l'];
			if (values.size() > rang)
				l_mode(channel, _signe['l'], values[rang]);
			else
				l_mode(channel, _signe['l'], "");
		}
		else
			l_mode(channel, _signe['l'], "");
	}
	if (_signe.find('k')!= _signe.end()) {
		if (_signe['k'] == '+'){
			int rang = 2 + _ordre['k'];
			if (values.size() > rang) {
				k_mode(channel, _signe['k'], values[rang]);}
			else {
				k_mode(channel, _signe['k'], "");
			}
		}
		else
			k_mode(channel, _signe['k'], "");
	}
	if (_signe.find('o')!= _signe.end()) {
		if (_signe['o'] == '+'){
			int rang = 2 + _ordre['o'];
			if (values.size() > rang) {
				o_mode(channel, _signe['o'], values[rang]);}
			else {
				o_mode(channel, _signe['o'], "");
			}
		}
		else
			o_mode(channel, _signe['o'], "");
	}
	
}

void Mode::parse_mode(std::vector<std::string> values){
	for (int i = 0; i < values[1].size(); i++){
		if (values[1][i] == '+' || values[1][i] == '-') {
			if ((i + 1)< values[1].size() && (values[1][i + 1] == 'i' || values[1][i + 1] == 'k' 
				|| values[1][i + 1] == 'l' || values[1][i + 1] == 'o' || values[1][i + 1] == 't')){
					if (_signe.find(values[1][i + 1]) == _signe.end()) { // n'existe pas encore
						_signe[values[1][i + 1]] = values[1][i];
						if ((values[1][i + 1] == 'k'  || values[1][i + 1] == 'l' || values[1][i + 1] == 'o')
						&& _signe[values[1][i + 1]] == '+'){
							if (_ordre.empty()) {
								_ordre[values[1][i + 1]] = 0;
							}
							else
								_ordre[values[1][i + 1]] = _ordre.size();
						}
					}
			}
		}
		else if (values[1][i] == 'i' || values[1][i] == 'k' 
			|| values[1][i] == 'l' || values[1][i] == 'o' || values[1][i] == 't') {
				if (_signe.find(values[1][i]) == _signe.end()) { // n'existe pas encore
					_signe[values[1][i]] = '+';
					if ((values[1][i] == 'k'  || values[1][i] == 'l' || values[1][i] == 'o')){
						if (_ordre.empty())
							_ordre[values[1][i]] = 0;
						else
							_ordre[values[1][i]] = _ordre.size();
					}
				}
		}
		else {
			std::string str = "Unknown mode character ";
			str += values[1][i];
			send(_client_fd, str.c_str(), str.length(), 0);
		}
	}
}


void Mode::mode_info(Channel* channel,std::vector<std::string> values){
	std::string server_name = SERVER_NAME;
	// Créer le message RPL_CHANNELMODEIS
	std::string modes = "+"; // -> default mode
	if (channel->get_i() == true || channel->get_t() == true || channel->get_pass() == true || channel->get_limite() != -1) {
		if (channel->get_t() == true)
			modes += "t";
		if (channel->get_i() == true)
			modes += "i";
		if (channel->get_limite() != -1)
			modes += "l";
		if (channel->get_pass() == true)
			modes += "k";
		if ((channel->get_limite() != -1) || (channel->get_pass() == true)){
			modes += " ";
			if (channel->get_limite() != -1)
				modes += int_to_string(channel->get_limite());
			if (channel->get_pass() == true)
				modes += channel->get_password();
		}	
	}					
	std::string chanmode = ":" + server_name + " 324 " + _client_actif->get_nickname() + "#" + channel->get_name() + " " + channel->get_name() + " " + modes + "\r\n";
	send(_client_fd, chanmode.c_str(), chanmode.length(), 0);
	
	// Créer le message RPL_CREATIONTIME
	std::time_t creation_time = channel->get_creation_date();
	std::string creation_str = int_to_string(creation_time);
	std::string creationtime = RPL_CREATIONTIME(server_name, values[0], creation_str);
	send(_client_fd, creationtime.c_str(), creationtime.length(), 0);
}
			
void Mode::channel_mode(std::vector<std::string> values){
	std::string server_name = SERVER_NAME;
	
	if (_channels_array.size() == 0){ // -> if there is no channel
		std::string str = ERR_NOSUCHCHANNEL2(server_name, values[0]);
		send(_client_fd, str.c_str(), str.length(), 0);
		return ;
	}
	for (int i = 0; i < _channels_array.size(); i++) { 
		if (_channels_array[i]->get_name() == values[0]){ // -> check if the channel exists
			if (values.size() == 1) // ->if there is no other value than name of chan -> Channel #samurai created Mon Sep 30 14:17:56 2024
				mode_info(_channels_array[i], values);
			else { // -> si plusieurs arguments, verifie que le client est operateur du chan
				if (_channels_array[i]->is_operator(_client_actif->get_nickname()) == false){
					std::string notope = ERR_CHANOPRIVSNEEDED(server_name, _channels_array[i]->get_name());
					send(_client_fd, notope.c_str(), notope.length(), 0);
				}
				else { //-> si le client est bien operateur, on parse les arguments avant d'executer les modes
					parse_mode(values);
					exec_mode(_channels_array[i], values);
				}
			}
		}
		else
			send(_client_fd, ERR_NOSUCHCHANNEL2(server_name, values[0]), strlen(ERR_NOSUCHCHANNEL2(server_name, values[0])), 0);
	}
}

void Mode::init_cmd_mode(){
	std::vector<std::string> values = split_by_space(_value); //-> split value by " " to get a vector of strings
	
	for (int i = 0; i < _clients.size(); i++){ 
		if (_clients[i]->get_nickname() == values[0]){ // -> check if the first value is a user
			return ;
		}
	}
	channel_mode(values); // -> if not a user, we treat it as a channel
}
