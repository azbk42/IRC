/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:42:51 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/18 12:28:33 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mode.hpp"
#include <cctype>
#include <map>

// si 2e arg, mais pas en attente d;arg, (pas de koul) le traite comme un nouveau mode

// ################################################################################
// #                         constructor / destructor                             #
// ################################################################################

Mode::Mode(std::vector<Client*> &clients_list, Client &client_actif, std::vector<Channel*> &channels_array, int client_fd, std::string value) : 
	_client_fd(client_fd), _clients(clients_list), _client_actif(&client_actif), _channels_array(channels_array), _value(value), _modes_info("+"){}

Mode::~Mode() {}

// ################################################################################
// #                                    METHOD                                    #
// ################################################################################

// void Mode::mode_message(std::string mode, Channel *channel){
// 	std::string server_name = SERVER_NAME;
// 	std::string message = ":" + server_name + " 324 " + _client_actif->get_nickname() + " " + channel->get_name() + " " + mode + "\r\n";
// 	std::map<std::string, int> clients = channel->get_clients();
// 	for (std::map<std::string, int>::iterator it = clients.begin(); it != clients.end(); ++it) {
//         send(it->second, message.c_str(), message.size(), 0);
//     }
// }

void Mode::fill_modes_exec(std::string mode, std::string signe, std::string value){
	std::cout << "mode = " << mode << std::endl;
	std::cout << "signe = " << signe << std::endl;
	std::cout << "value = " << value << std::endl;
	if (_modes_exec.empty() == true){
		_modes_exec.push_back(signe + mode);
	}
	else {
		int pos = _modes_exec[0].find(signe);
		if (signe == "+" && pos == -1) // si on a un + et qu'il n'y en a pas deja un ds _modes_exec[0]
			_modes_exec[0] += signe;
		if (signe == "+" && pos != -1){ // si on a un + et qu'il y en a deja un ds _modes_exec[0]
			if (_modes_exec[0].find("-") != std::string::npos && _modes_exec[0].find("-") > pos) //trouver un - plus proche
			{
				_modes_exec[0] += signe;
			}
		}
		if (signe == "-" && pos == -1)
			_modes_exec[0] += signe;
		if (signe == "-" &&  pos != -1) {// si on a un plus et qu'il y en a deja un ds _modes_exec[0]
			if (_modes_exec[0].find("+") > pos) //trouver un - plus proche
				_modes_exec[0] += signe;
		}
		_modes_exec[0] += mode;
	}
	if (mode == "k" || mode == "o" || mode == "l"){
		_modes_exec.push_back(value);
	}
	for (int i; i < _modes_exec.size(); i++){
		std::cout << _modes_exec[i] << std::endl;
	}
}

//  i : Définir/supprimer le canal sur invitation uniquement
void Mode::i_mode(Channel *channel, char signe){
	if (signe == '-') {
		channel->set_i('-');
	}
	else if (signe == '+'){
		channel->set_i('+');
	}
	std::string sign(1, signe);
	fill_modes_exec("i", sign, "");
}

	// — t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
void Mode::t_mode(Channel *channel, char signe){
	if (signe == '-') {
		channel->set_t('-');
	}
	else if (signe == '+') {
		channel->set_t('+');
	}
	std::string sign(1, signe);
	fill_modes_exec("t", sign, "");
}

	// — k : Définir/supprimer la clé du canal (mot de passe)
void Mode::k_mode(Channel *channel, char signe, std::string value){
	if (channel->get_pass() == true && signe == '-') {
		std::string sign(1, signe);
		fill_modes_exec("k", sign, channel->get_password());
		channel->set_pass(false);
		channel->set_password("");
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
		}
		else if (channel->is_operator(user) == false && signe == '+') {// si il est pas operateur, mais quón veut lui donner ce role
			channel->add_operator(user);
			std::string sign(1, signe);
			fill_modes_exec("o", sign, user);
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
	for(std::map<int, char>::iterator it = _ordre_exec.begin(); it != _ordre_exec.end(); ++it)
    {
		std::cout << it->first << " = " << it->second << std::endl;
		if (it->second == 'i'){
			i_mode(channel, _signe['i']);
		}
		if (it->second == 't'){
			t_mode(channel, _signe['t']);
		}
		if (it->second == 'l'){
			if (_signe['l'] == '+') {
				int rang = 2 + _ordre_args['l'];
				if (values.size() > rang)
					l_mode(channel, _signe['l'], values[rang]);
				else
					l_mode(channel, _signe['l'], "");
			}
			else
				l_mode(channel, _signe['l'], "");
		}
		if (it->second == 'k'){
			if (_signe['k'] == '+') {
				int rang = 2 + _ordre_args['k'];
				if (values.size() > rang) {
					k_mode(channel, _signe['k'], values[rang]);}
				else {
					k_mode(channel, _signe['k'], "");
				}
			}
			else
				k_mode(channel, _signe['k'], "");
		}
		if (it->second == 'o'){
			int rang = 2 + _ordre_args['o'];
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
	std::string server_name = SERVER_NAME;
	std::string modes;
	for (int i = 0; i < _modes_exec.size(); i++)
	{
		if (i != 0)
			modes += " ";
		modes += _modes_exec[i];
	}
	std::string chanmode = ":" + server_name + " 324 " + _client_actif->get_nickname() + "#" + channel->get_name() + " " + channel->get_name() + " " + modes + "\r\n";
	send(_client_fd, chanmode.c_str(), chanmode.length(), 0);
}

size_t Mode::parse_args(std::string value){
	for (int i = 0; i < value.size(); i++){
		if (value[i] == '+' || value[i] == '-') { //-> if there is a sign, all that follows is of that sign
			for (int j = 1; (i + j) < value.size(); j++){
				if (value[i + j] == '+' || value[i + j] == '-')
					break;
				if ((value[i + j] == 'i' || value[i + j] == 'k' 
					|| value[i + j] == 'l' || value[i + j] == 'o' || value[i + j] == 't')){ //-> if followed by one of the covered modes
					if (_signe.find(value[i + j]) == _signe.end()) { //-> if this mode has not yet been parsed
						_signe[value[i + j]] = value[i];
						_ordre_exec[_ordre_exec.size()] = value[i + j];
						if (((value[i + j] == 'k'  || value[i + j] == 'l' || value[i + j] == 'o')
						&& _signe[value[i + j]] == '+') || (value[i + j] == 'o' && _signe[value[i + j]] == '-')){ //-> for those combinations, we expect a parameter to follow
							if (_ordre_args.empty()) {
								_ordre_args[value[i + j]] = 0;
							}
							else
								_ordre_args[value[i + j]] = _ordre_args.size();
						}
					}
				}
			}
		}
		else if (value[i] == 'i' || value[i] == 'k'  //-> if there's no sign
			|| value[i] == 'l' || value[i] == 'o' || value[i] == 't') {
				if (_signe.find(value[i]) == _signe.end()) { //-> if this mode has not yet been parsed
					_signe[value[i]] = '+';
					_ordre_exec[_ordre_exec.size()] = value[i];
					if ((value[i] == 'k'  || value[i] == 'l' || value[i] == 'o')){
						if (_ordre_args.empty())
							_ordre_args[value[i]] = 0;
						else
							_ordre_args[value[i]] = _ordre_args.size();
					}
				}
		}
		else {
			std::string server_name = SERVER_NAME;
			std::string str = ":" + server_name + " 472 " + " :is unknown mode char to me\r\n";
			send(_client_fd, str.c_str(), str.length(), 0);
		}
	}
	return (_ordre_args.size());
}

void Mode::parse_mode(std::vector<std::string> values){
	int i = 1;
	while (i < values.size()) {
		size_t ordresize = parse_args(values[i]); // -> parse bloc par bloc, mais en sautant les arguments de chaque bloc
		if (ordresize == 0)
			i++;
		else
			i += ordresize + 1;
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
		send(_client_fd, str.c_str(), str.length(), 0);
		return ;
	}
	for (int i = 0; i < _channels_array.size(); i++) { 
		if (_channels_array[i]->get_name() == values[0]){ // -> check if the channel exists
			if (values.size() == 1) {// ->if there is no other value than name of chan -> Channel #samurai created Mon Sep 30 14:17:56 2024
				mode_info(_channels_array[i], values);
				return ;
			}
			else { // -> si plusieurs arguments, verifie que le client est operateur du chan
				if (_channels_array[i]->is_operator(_client_actif->get_nickname()) == false){
					std::string notope = ERR_CHANOPRIVSNEEDED(server_name, _client_actif->get_nickname(),_channels_array[i]->get_name());
					send(_client_fd, notope.c_str(), notope.length(), 0);
					return ;
				}
				else { //-> si le client est bien operateur, on parse les arguments avant d'executer les modes
					parse_mode(values);
					exec_mode(_channels_array[i], values);
					return ;
				}
			}
		}
	}
	std::string str = ERR_NOSUCHCHANNEL2(server_name, values[0]);
	send(_client_fd, str.c_str(), str.length(), 0);		
}

void Mode::init_cmd_mode(){
	std::vector<std::string> values = split_by_space(_value); //-> split value by " " to get a vector of strings
	
	for (int i = 0; i < _clients.size(); i++){ 
		if (_clients[i]->get_nickname() == values[0]){ // -> check if the first value is a user
			return ; // user modes are not covered in the subject
		}
	}
	channel_mode(values); //-> if not a user, we treat it as a channel
}
