/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:20:06 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/04 16:41:03 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

/* they’re subject to the channel’s specific rules and permissions.
and ensuring the rules are enforced. 
This includes managing operator privileges, 
which allow certain users to kick or ban others, set topics, 
and tweak channel settings.*/

#include <iostream>
#include <vector>
#include "server.hpp"

class Channel {
	private:
		std::string _name;
		std::string _topic;
		int _userLimit;
		std::string _password;
		std::vector<Client> _members;
		std::vector<Client> _operators;

	public:
		Channel();
		Channel(const Channel &rhs);
		~Channel();
		Channel &operator=(const Channel &rhs);
};