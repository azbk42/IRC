/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:43:04 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/14 15:03:55 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

#ifndef MODE_HPP
# define MODE_HPP

class Mode
{
	private:
		int _client_fd;
		std::string _value;
		Client *_client_actif;
		std::vector<Client*> _clients;
		std::vector<Channel*> _channels_array;
		std::map<char, char> _signe;
		std::map<char, int> _ordre;
	
	public:
		Mode(std::vector<Client*> &clients_list, Client* client_actif, std::vector<Channel*> channels_array, int client_fd, std::string value);
		~Mode();

		void init_cmd_mode();
		void channel_mode(std::vector<std::string> values);
		void parse_mode(std::vector<std::string> values);
		void exec_mode(Channel* channel, std::vector<std::string> values);
		void mode_info(Channel* channel, std::vector<std::string> values);

		void i_mode(Channel *channel, char signe);
		void t_mode(Channel *channel, char signe);
		void l_mode(Channel *channel, char signe, std::string value);
		void k_mode(Channel *channel, char signe, std::string value);
		void o_mode(Channel *channel, char signe, std::string user);


		bool isfulldigit(std::string str);



};

#endif
