/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:28:40 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/11 15:38:39 by ctruchot         ###   ########.fr       */
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