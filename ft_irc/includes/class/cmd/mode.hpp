/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:43:04 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/18 18:40:50 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

#ifndef MODE_HPP
# define MODE_HPP

#include <sstream>
#include <vector>


class Mode
{
	private:
		int _client_fd;
		std::string _value;
		Client *_client_actif;
		std::vector<Client*> &_clients;
		std::vector<Channel*> &_channels_array;
		std::map<char, char> _signe;
		std::map<char, int> _ordre_args;
		std::map<int, char> _ordre_exec;
		std::string _modes_info;
		std::vector<std::string> _modes_exec;

		void channel_mode(std::vector<std::string> &values);
		void mode_info(Channel* channel, std::vector<std::string> &values);
		
		void parse_mode(std::string value);

		void exec_mode(Channel* channel, std::vector<std::string> values);
		void mode_message(std::string mode, Channel* channel);

		void i_mode(Channel *channel, char signe);
		void t_mode(Channel *channel, char signe);
		void l_mode(Channel *channel, char signe, std::string value);
		void k_mode(Channel *channel, char signe, std::string value);
		void o_mode(Channel *channel, char signe, std::string user);
		void fill_modes_exec(std::string mode, std::string signe, std::string value);

	public:
		Mode(std::vector<Client*> &clients_list, Client &client_actif, std::vector<Channel*> &channels_array, int client_fd, std::string value);
		~Mode();

		void init_cmd_mode();


};

#endif
