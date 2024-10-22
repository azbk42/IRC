/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:22:58 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/22 18:30:05 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pass.hpp"
#include "server.hpp"

Pass::Pass(int fd, std::string value, Client *client, std::string server_pwd) : 
_fd(fd), _enteredPwd(value), _serverPwd(server_pwd), _client(client){}

Pass::~Pass(){}

int Pass::check_pass(){
	std::string server_name = SERVER_NAME;
	if (_enteredPwd != _serverPwd){
		send_message(_fd, ERR_PASSWDMISMATCH(server_name));
		return 1;
    }
	_client->set_checked_pwd(true);
	return 0;
}
