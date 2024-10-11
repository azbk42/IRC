#pragma once


// ################################################################################
// #                              SERVER_CONFIGURATION                            #
// ################################################################################
#define SERVER_NAME "localhost"
#define BOT_NAME "HelpMaster"

#define CLIENT_NOT_FOUND -1
// ################################################################################
// #                                    ERROR IRSSI                               #
// ################################################################################

// ERROR NICK
#define ERR_NICKNAMEINUSE(server, nick) (server + " " + nick + " :Nickname is already in use\r\n").c_str()
#define ERR_ERRONEUSNICKNAME(server, nick) (server + " " + nick + " :Erroneous nickname\r\n").c_str()
#define ERR_NONICKNAMEGIVEN(server) (server + " :No nickname given\r\n").c_str()
// ERROR USER
#define ERR_NEEDMOREPARAMS(command, server) (":" + server_name + " 461 " + server + " " + command + \
                            " :Not enough parameters\r\n").c_str()
#define ERR_ALREADYREGISTERED(server) (":" + server_name + " 462 " + server + " :You may not reregister\r\n").c_str()

// ERROR PASS
#define ERR_PASSWDMISMATCH(server) (":" + server_name + " 464 " + server + " :Password incorrect\r\n").c_str()

// ERROR JOIN
#define ERR_INVITEONLYCHAN(server, channel) (server + " 473 #" + channel + " :Cannot join channel (+i) - Invite only\r\n")
#define ERR_NOSUCHCHANNEL(server, channel) (":" + server + " 403 " + channel + " :No such channel\r\n")
#define ERR_CHANNELNAMETOOLONG(server, channel) (":" + server + " 403 " + channel + " :Channel name is too long (maximum is 50 characters)\r\n")
#define ERR_TOOMANYCHANNELS(server, channel) (":" + server + " 405 " + channel + "r\n")

// ERROR PART
#define ERR_NOTONCHANNEL(server, channel)(":" + server_name + " 442" + channel + " :You're not on that channel\r\n").c_str()
#define ERR_NOSUCHCHANNEL2(server, channel) (":" + server + " 403 " + channel + " :No such channel\r\n").c_str()

// ERROR PRIVMSG
#define ERR_NOSUCHNICK(server, sender, target) (":" + server + " 401 " + sender + " " + target + " :No such nick/channel\r\n")

// ERROR WHOIS
#define ERR_NOSUCHSERVER(server, sender, target_server) (":" + server + " 402 " + sender + " " + target_server + " :No such server\r\n")

// ################################################################################
// #                                    REPLIES IRSSI                               #
// ################################################################################

#define RPL_LIST(server, channel, client_count, topic) (":" + server_name + " 322 " + server + " #" + channel + " " + client_count + " :" + topic + "\r\n").c_str()
#define RPL_LISTEND(server) (":" + server_name + " 323 " + server + " :End of /LIST\r\n").c_str()

#define QUIT_MESSAGE(nickname, username, hostname, message) (":" + nickname + "!" + username + "@" + hostname + " QUIT :" + message + "\r\n").c_str()
#define PART_MESSAGE(nickname, username, hostname, channel, reason) (":" + nickname + "!" + username + "@" + hostname + " PART #" + channel + " :" + reason + "\r\n");

// WHOIS

#define RPL_WHOISUSER(server, requester, target_nick, username, hostname, realname) \
    (":" + server + " 311 " + requester + " " + target_nick + " " + username + " " + hostname + " * :" + realname + "\r\n")

#define RPL_WHOISSERVER(server, requester, target_nick, target_server, server_info) \
    (":" + server + " 312 " + requester + " " + target_nick + " " + target_server + " :" + server_info + "\r\n")

#define RPL_ENDOFWHOIS(server, requester, target_nick) \
    (":" + server + " 318 " + requester + " " + target_nick + " :End of WHOIS list\r\n")


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