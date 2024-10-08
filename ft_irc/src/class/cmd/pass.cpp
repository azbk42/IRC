/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:22:58 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/08 15:50:52 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pass.hpp"
#include "server.hpp"
#include <iostream>

Pass::Pass(int fd, std::string value, Client *client, Server* server) : _fd(fd), _enteredPwd(value), _client(client), _server(server){}
Pass::~Pass(){}

void Pass::check_pass(){
	std::string server_name = SERVER_NAME;
	if (_enteredPwd != _server->GetPassword()){
        std::cout << RED << "Password incorrect" << std::endl;
		send(_fd, ERR_PASSWDMISMATCH(server_name), strlen(ERR_PASSWDMISMATCH(server_name)), 0);
        _server->CloseClientSocket(_fd);
		return;
    }
	std::cout << GREEN << "Password correct" << std::endl;
	_client->set_checked_pwd(true);
	return;
}
