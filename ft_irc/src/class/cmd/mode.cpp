/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:42:51 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/15 14:09:09 by ctruchot         ###   ########.fr       */
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

void Mode::mode_message(std::string mode, Channel *channel){
	std::string server_name = SERVER_NAME;
	std::string message = ":" + server_name + " 324 " + _client_actif->get_nickname() + " " + channel->get_name() + " " + mode + "\r\n";
	// std::string message2 = " by " + _client_actif->get_nickname() + "\r\n";
	// std::string message = ":" + server_name + " 324 " + _client_actif->get_nickname() + "#" + channel->get_name() + " " + channel->get_name() + " " + mode + " " + "by " + _client_actif->get_nickname() + "\r\n";
	std::map<std::string, int> clients = channel->get_clients();
	for (std::map<std::string, int>::iterator it = clients.begin(); it != clients.end(); ++it) {
        send(it->second, message.c_str(), message.size(), 0);
		// send(it->second, message2.c_str(), message.size(), 0);

    }
	// send(_client_fd, message.c_str(), message.size(), 0);
	// send(_client_fd, message2.c_str(), message.size(), 0);
}

	//  i : Définir/supprimer le canal sur invitation uniquement
void Mode::i_mode(Channel *channel, char signe){
	std::string mode;
	if (signe == '-') {
		channel->set_i('-');
		mode = "-i";
		mode_message(mode, channel);
	}
	if (signe == '+'){
		channel->set_i('+');
		mode = "+i";
		mode_message(mode, channel);
	}
}

	// — t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
void Mode::t_mode(Channel *channel, char signe){
	std::string mode;
	if (signe == '-') {
		channel->set_t('-');
		mode = "-t";
		mode_message(mode, channel);
	}
	if (signe == '+') {
		channel->set_t('+');
		mode = "+t";
		mode_message(mode, channel);
	}
}

	// — k : Définir/supprimer la clé du canal (mot de passe)
void Mode::k_mode(Channel *channel, char signe, std::string value){
	std::string mode;
	if (channel->get_pass() == true && signe == '-') {
		mode = "-k";
		// mode = "-k" + channel->get_password();
		channel->set_pass(false);
		channel->set_password("");
		mode_message(mode, channel);
	}
	if (signe == '+' && value.empty() == false){
		channel->set_pass(true);
		channel->set_password(value);
		mode = "+k " + value;
		mode_message(mode, channel);
	}
}

// — l : Définir/supprimer la limite d’utilisateurs pour le canal
void Mode::l_mode(Channel *channel, char signe, std::string value){
	std::string server_name = SERVER_NAME;
	std::string mode;
	
	if (signe == '-') { // dans ce cas on ignore totalement ce qui vient apres?
		mode = "-l";
		if (channel->get_limite() != -1) // et si y a une limite
			channel->set_limite(-1);
		mode_message(mode, channel);
	}
	if (signe == '+' && value.empty() == true) {
		std::string str = ERR_NEEDMOREPARAMS2("MODE +l", server_name);
		std::cout << str << std::endl;
		send(_client_fd, str.c_str(), str.length(), 0);
	}
	if (signe == '+' && value.empty() == false) {
		long limite = get_digit(value);
		if (limite < 0)
			return ;
		channel->set_limite(limite);									
		mode = "+l " + int_to_string(limite);
		mode_message(mode, channel);
	}
}

// — o : Donner/retirer le privilège de l’opérateur de canal
void Mode::o_mode(Channel *channel, char signe, std::string user){
	std::string server_name = SERVER_NAME;
	std::string mode;
std::cout << "omode" << std::endl;

	if (user.empty() == true)
		return ;
// verifier que lútilisateur existe ?
	else if (channel->is_in_channel(user)) // -> check if user is in the channel
	{
		if (channel->is_operator(user) == true && signe == '-') { // si il est ope mais quon veut le retirer
			std::cout << "remove" << std::endl;
			channel->remove_operator(user);
			mode = "-o " + user;
			mode_message(mode, channel);
		}
		if (channel->is_operator(user) == false && signe == '+') {// si il est pas operateur, mais quón veut lui donner ce role
			std::cout << "add" << std::endl;
			channel->add_operator(user);
			mode = "+o " + user;
			mode_message(mode, channel);
		}
	}
	else {
		std::string str1 =  ERR_NOTONCHANNEL1(server_name, user, channel->get_name()); // yo :No such nick/channel
		std::string str =  ERR_NOTONCHANNEL2(server_name, user, channel->get_name()); // yo #camtrooo They aren't on that channel
		send(_client_fd, str1.c_str(), str1.length(), 0);
		send(_client_fd, str.c_str(), str.length(), 0);
	}
}

long Mode::get_digit(std::string str){
	long digit = -1;
	if (str[0] == '+')
		str.erase(0, 1);
	char *end;
	digit = std::strtol(str.c_str(), &end, 10);
	if (digit == 0 && str[0] != '0')
		return -1;
	return digit;
}

void Mode::exec_mode(Channel* channel, std::vector<std::string> values){
	std::cout << "size signe=" << _signe.size() << std::endl;
	std::cout << "size ordre=" << _ordre.size() << std::endl;

	if (_signe.find('i') != _signe.end()){
		i_mode(channel, _signe['i']);
	}
	if (_signe.find('t') != _signe.end()){
		t_mode(channel, _signe['t']);
	}
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
		int rang = 2 + _ordre['o'];
		if (_signe['o'] == '+'){
			if (values.size() > rang) {
				o_mode(channel, _signe['o'], values[rang]);}
			else {
				o_mode(channel, _signe['o'], "");
			}
		}
		else {
			if (values.size() > rang) {
				o_mode(channel, _signe['o'], values[rang]);}
			else {
				o_mode(channel, _signe['o'], "");
			}
		}
	}
}

size_t Mode::parse_args(std::string value){
	std::cout << "value = " << value << std::endl;
	for (int i = 0; i < value.size(); i++){
		if (value[i] == '+' || value[i] == '-') {
			if ((i + 1)< value.size() && (value[i + 1] == 'i' || value[i + 1] == 'k' 
				|| value[i + 1] == 'l' || value[i + 1] == 'o' || value[i + 1] == 't')){
					if (_signe.find(value[i + 1]) == _signe.end()) { // n'existe pas encore
						_signe[value[i + 1]] = value[i];
						if (((value[i + 1] == 'k'  || value[i + 1] == 'l' || value[i + 1] == 'o')
						&& _signe[value[i + 1]] == '+') || (value[i + 1] == 'o' && _signe[value[i + 1]] == '-')){
							if (_ordre.empty()) {
								_ordre[value[i + 1]] = 0;
							}
							else
								_ordre[value[i + 1]] = _ordre.size();
						}
					}
			}
		}
		else if (value[i] == 'i' || value[i] == 'k' 
			|| value[i] == 'l' || value[i] == 'o' || value[i] == 't') {
				if (_signe.find(value[i]) == _signe.end()) { // n'existe pas encore
					_signe[value[i]] = '+';
					if ((value[i] == 'k'  || value[i] == 'l' || value[i] == 'o')){
						if (_ordre.empty())
							_ordre[value[i]] = 0;
						else
							_ordre[value[i]] = _ordre.size();
					}
				}
		}
		else {
			std::cout << "value=" << value[i] << std::endl;
			std::string str = "Unknown mode character ";
			str += value[i];
			std::cout << "str=" << str << std::endl;
			send(_client_fd, str.c_str(), str.length(), 0);
		}
	}
	return (_ordre.size());
}

void Mode::parse_mode(std::vector<std::string> values){
	int i = 1;
	while (i < values.size()) {
		size_t ordresize = parse_args(values[i]); // -> parse bloc par bloc, mais en sautant les arguments de chaque bloc
		std::cout << "ordresize = " << ordresize << std::endl;
		if (ordresize == 0)
			i++;
		else
			i += ordresize + 1;
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
		else {
			send(_client_fd, ERR_NOSUCHCHANNEL2(server_name, values[0]), strlen(ERR_NOSUCHCHANNEL2(server_name, values[0])), 0);
		}
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
