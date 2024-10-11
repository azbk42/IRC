/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:22:58 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/09 15:15:40 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pass.hpp"
#include "server.hpp"
#include <iostream>

Pass::Pass(int fd, std::string value, Client *client, std::string server_pwd) : _fd(fd), _enteredPwd(value), _client(client), _serverPwd(server_pwd){}
Pass::~Pass(){}

int Pass::check_pass(){
	std::string server_name = SERVER_NAME;
	if (_enteredPwd != _serverPwd){
        std::cout << RED << "Password incorrect" << std::endl;
		send(_fd, ERR_PASSWDMISMATCH(server_name), strlen(ERR_PASSWDMISMATCH(server_name)), 0);
		return 1;
    }
	std::cout << GREEN << "Password correct" << std::endl;
	_client->set_checked_pwd(true);
	return 0;
}
