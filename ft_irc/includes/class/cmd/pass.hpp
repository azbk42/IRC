/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:22:48 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/22 18:30:16 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_HPP
# define PASS_HPP

#include "server.hpp"
#include "client.hpp"

#include <iostream>

class Pass
{
	private:
		int _fd;
		std::string _enteredPwd;
		std::string _serverPwd;
		Client *_client;
		
	public:
		Pass(int fd, std::string value, Client *client, std::string server_pwd);
		~Pass();

		int check_pass();
};


#endif