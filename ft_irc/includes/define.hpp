#pragma once


// ################################################################################
// #                              SERVER_CONFIGURATION                            #
// ################################################################################
#define SERVER_NAME "localhost"

// ################################################################################
// #                                    ERROR IRSSI                               #
// ################################################################################

// ERROR NICK
#define ERR_NICKNAMEINUSE(client, nick) (client + " " + nick + " :Nickname is already in use\r\n").c_str()
#define ERR_ERRONEUSNICKNAME(client, nick) (client + " " + nick + " :Erroneous nickname\r\n").c_str()
#define ERR_NONICKNAMEGIVEN(client) (client + " :No nickname given\r\n").c_str()
// ERROR USER
#define ERR_NEEDMOREPARAMS(command, client) (":" + server_name + " 461 " + client + " " + command + \
                            " :Not enough parameters\r\n").c_str()
#define ERR_ALREADYREGISTERED(client) (":" + server_name + " 462 " + client + " :You may not reregister\r\n").c_str()

// ERROR PASS
#define ERR_PASSWDMISMATCH(client) (":" + server_name + " 464 " + client + " :Password incorrect\r\n").c_str()

// ERROR JOIN
#define ERR_INVITEONLYCHAN(client, channel) (client + " 473 #" + channel + " :Cannot join channel (+i) - Invite only\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) (":" + client + " 403 " + channel + " :No such channel\r\n")
#define ERR_CHANNELNAMETOOLONG(client, channel) (":" + client + " 403 " + channel + " :Channel name is too long (maximum is 50 characters)\r\n")



// ################################################################################
// #                                    VALID IRSSI                               #
// ################################################################################

// ACCEPT
#define NICK_CHANGE(old_nick, new_nick) (":" + old_nick + " NICK " + new_nick + "\r\n").c_str()


// ################################################################################
// #                                    COLORS                                    #
// ################################################################################

#define RED "\e[1;31m" //-> for red color
#define WHITE "\e[0;37m" //-> for white color
#define GREEN "\e[1;32m" //-> for green color
#define YELLOW "\e[1;33m" //-> for yellow color
#define CYAN "\e[1;36m"   // Couleur cyan vive
#define MAGENTA "\e[1;35m" // Couleur magenta vive
#define BLUE "\e[1;34m"    // Couleur bleue vive