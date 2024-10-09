/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:12:03 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/09 16:43:26 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PART_HPP
# define PART_HPP

#include "define.hpp"
#include <iostream>
#include <vector>
#include "channel.hpp"



class Part{
	private:
		int _fd;
		std::string _value;
		Client *_client_actif;
		std::vector<Channel*> &_channels_list; // revoir pourquoi ref

		std::vector<std::string> _split_by_comma(const std::string &input);
		void _check_channel(std::string channel);


	public:
		Part(std::vector<Channel*> &channels, int client_fd, Client *client_actif, std::string value);
		~Part();

		void init_cmd_part();




		
};

#endif