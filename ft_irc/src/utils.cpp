/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:28:40 by ctruchot          #+#    #+#             */
/*   Updated: 2024/10/10 16:30:22 by ctruchot         ###   ########.fr       */
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