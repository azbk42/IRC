#pragma once




// class
#include "server.hpp"
#include "parse.hpp"
#include "client.hpp"
#include "channel.hpp"
// cmd
#include "join.hpp"
#include "list.hpp"
#include "msg.hpp"
#include "nick.hpp"
#include "part.hpp"
#include "pass.hpp"
#include "quit.hpp"
// other
#include "define.hpp"

// ################################################################################
// #                                  Handle_arg                                  #
// ################################################################################

int checkPort(char *argv);
std::string to_uppercase(const std::string &str);