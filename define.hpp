#pragma once

#define SERVER_NAME "localhost"

// HANDLE ERROR
// ERROR NICK
#define ERR_NICKNAMEINUSE(client, nick) (client + " " + nick + " :Nickname is already in use\r\n").c_str()
#define ERR_ERRONEUSNICKNAME(client, nick) (client + " " + nick + " :Erroneous nickname\r\n").c_str()
#define ERR_NONICKNAMEGIVEN(client) (client + " :No nickname given\r\n").c_str()
// ERROR USER
#define ERR_NEEDMOREPARAMS(command, client) (":" + server_name + " 461 " + client + " " + command + \
                            " :Not enough parameters\r\n").c_str()
#define ERR_ALREADYREGISTERED(client) (":" + server_name + " 462 " + client + " :You may not reregister\r\n").c_str()

// ACCEPT
#define NICK_CHANGE(old_nick, new_nick) (":" + old_nick + " NICK " + new_nick + "\r\n").c_str()
