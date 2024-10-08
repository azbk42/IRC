/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:22:48 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/08 15:50:58 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_HPP
# define PASS_HPP

#include "client.hpp"
#include "server.hpp"

class Pass
{
	private:
		int _fd;
		std::string _enteredPwd;
		Client*_client;
		Server* _server;
		
	public:
		Pass(int fd, std::string value, Client *client, Server* server);
		~Pass();

		void check_pass();
};


#endif