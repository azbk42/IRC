/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:43:04 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/11 17:07:08 by ctruchot         ###   ########.fr       */
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
	
	public:
		Mode(std::vector<Client*> &clients_list, Client* client_actif, std::vector<Channel*> channels_array, int client_fd, std::string value);
		~Mode();

		void init_cmd_mode();
		void channel_mode(std::vector<std::string> values);
		void user_mode(std::vector<std::string> values);
		std::string format_creation_time(Channel *channel);
		void i_mode(Channel *channel, char signe);
		void o_mode(Channel *channel, char signe, std::string user);
		void l_mode(Channel *channel, std::vector<std::string> values);

		bool isfulldigit(std::string str);



};

#endif
