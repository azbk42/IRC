#pragma once

#include "server.hpp"
#include "parse.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "define.hpp"


// ################################################################################
// #                                  Handle_arg                                  #
// ################################################################################

int checkPort(char *argv);
std::string to_uppercase(const std::string &str);