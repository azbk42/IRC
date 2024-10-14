#pragma once




// class
#include "server.hpp"
#include "parse.hpp"
#include "client.hpp"
#include "channel.hpp"
// cmd
#include "whois.hpp"
#include "join.hpp"
#include "list.hpp"
#include "msg.hpp"
#include "nick.hpp"
#include "part.hpp"
#include "pass.hpp"
#include "quit.hpp"
#include "kick.hpp"
#include "topic.hpp"
// other
#include "define.hpp"

// ################################################################################
// #                                  Handle_arg                                  #
// ################################################################################

int checkPort(char *argv);
std::string to_uppercase(const std::string &str);
std::vector<std::string> split_by_comma(const std::string &input);
std::string int_to_string(int value);

