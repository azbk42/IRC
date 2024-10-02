#pragma once


#define ERR_NICKNAMEINUSE(client, nick) client " " nick " :Nickname is already in use"
#define ERR_ERRONEUSNICKNAME(client, nick) client " " nick " :Erroneous nickname"
#define ERR_NONICKNAMEGIVEN(client) client " :No nickname given"
