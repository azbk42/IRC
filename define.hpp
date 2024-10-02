#pragma once

#define SERVER_NAME "ircserv"

#define ERR_NICKNAMEINUSE(client, nick) client " " nick " :Nickname is already in use"
#define ERR_ERRONEUSNICKNAME(client, nick) (client + " " + nick + " :Erroneous nickname\r\n").c_str()
#define ERR_NONICKNAMEGIVEN(client) client " :No nickname given"
