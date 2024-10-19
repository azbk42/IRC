/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:28:40 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/18 15:56:01 by ctruchot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

std::vector<std::string> split_by_comma(const std::string &input)
{
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

std::vector<std::string> split_by_space(const std::string &input)
{
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ' ')) {
        result.push_back(item);
    }
    return result;
}

std::string int_to_string(int value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

int string_to_int(const std::string& str) {
	int result;
    std::stringstream ss(str);
	ss >> result;
    if (ss.fail()) {
        std::cerr << "Erreur de conversion!" << std::endl;
        return -1; 
    }
	return result;
}

long get_digit(std::string str){
	long digit = -1;
	if (str[0] == '+')
		str.erase(0, 1);
	char *end;
	digit = std::strtol(str.c_str(), &end, 10);
	if (digit == 0 && str[0] != '0')
		return -1;
	return digit;
}

void send_message(int fd, const std::string &message)
{
    if (send(fd, message.c_str(), message.size(), 0) == -1){
        std::cerr << RED << "Send error" << std::endl;
        std::cerr << RED << "Client fd = " << WHITE << fd << std::endl;
        std::cerr << RED << "Message: " << WHITE << message << std::endl;
    }
}