/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:02:00 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/17 14:49:28 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUIT_HPP
# define QUIT_HPP

#include <iostream>
#include "client.hpp"
#include "server.hpp"

class Quit
{
	private:
		int _client_fd;
		Client* _client;
		std::string _reason;
		std::vector<Channel*> &_channels;
		
	public:
		Quit(int fd, Client* client, std::string reason, std::vector<Channel*> &channels);
		~Quit();

		void send_quit_msg();
};

#endif
