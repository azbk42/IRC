/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:05:01 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/21 17:22:36 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_HPP
# define LIST_HPP

#include "channel.hpp"
#include "include.hpp"

class List {
	private:
		int _client_fd;
		std::vector<Channel*> &_channels_array;
		Client *_client;
		std::string _value;
		
		void send_channel(std::string client_name, Channel *channel) const;
		void list_with_args(std::string client_name);
	
	public:
		List(Client &client_actif, std::vector<Channel*> &channels_array, int client_fd, std::string list);
		~List();
		
		void send_list();
};

#endif