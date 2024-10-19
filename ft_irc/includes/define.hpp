#pragma once


// ################################################################################
// #                              SERVER_CONFIGURATION                            #
// ################################################################################
#define SERVER_NAME "localhost"

#define CLIENT_NOT_FOUND -1
#define CHANNEL_NOT_FOUND NULL
// ################################################################################
// #                                    ERROR IRSSI                               #
// ################################################################################

// ERROR NICK
#define ERR_NICKNAMEINUSE(server, nick) (server + " " + nick + " :Nickname is already in use\r\n").c_str()
#define ERR_ERRONEUSNICKNAME(server, nick) (server + " " + nick + " :Erroneous nickname\r\n").c_str()
#define ERR_NONICKNAMEGIVEN(server) (server + " :No nickname given\r\n").c_str()
// ERROR USER
#define ERR_NEEDMOREPARAMS(command, server) (":" + server_name + " 461 " + server + " " + command + " :Not enough parameters \r\n").c_str()
#define ERR_ALREADYREGISTERED(server) (":" + server_name + " 462 " + server + " :You may not reregister\r\n").c_str()

// ERROR PASS
#define ERR_PASSWDMISMATCH(server) (":" + server_name + " 464 " + server + " :Password incorrect\r\n").c_str()

// ERROR JOIN
#define ERR_INVITEONLYCHAN(server, client, channel) (":" + server + " 473 "+ client + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_NOSUCHCHANNEL(server, channel) (":" + server + " 403 " + channel + " :No such channel\r\n")
#define ERR_CHANNELNAMETOOLONG(server, channel) (":" + server + " 403 " + channel + " :Channel name is too long (maximum is 50 characters)\r\n")
#define ERR_TOOMANYCHANNELS(server, client, channel) (":" + server + " 405 " + client + " " + channel + " :You have joined too many channels\r\n")
#define ERR_CHANNELISFULL(server,client,channel) (":" + server + " 471 " + client + " " + channel + " :Cannot join channel (channel is full)\r\n")


// ERROR PART
#define ERR_NOTONCHANNEL(server, channel)(":" + server_name + " 442" + channel + " :You're not on that channel\r\n").c_str()
#define ERR_NOSUCHCHANNEL2(server, channel) (":" + server + " 403 " + channel + " :No such channel\r\n").c_str()

// ERROR PRIVMSG
#define ERR_NOSUCHNICK(server, sender, target) (":" + server + " 401 " + sender + " " + target + " :No such nick/channel\r\n")

// ERROR WHOIS
#define ERR_NOSUCHSERVER(server, sender, target_server) (":" + server + " 402 " + sender + " " + target_server + " :No such server\r\n")

// ERROR MODE
#define ERR_CHANOPRIVSNEEDED2(server, channel)(":" + server + " 482 " + " " + channel + " " + "#" + channel +" :You're not channel operator\r\n")
#define ERR_NEEDMOREPARAMS2(command, server) (":" + server_name + " 461 " + server + " " + command + ":Not enough parameters \r\n")
#define ERR_NOTONCHANNEL1(server, user, channel)(":" + server_name + " 442" + channel + " " + user + " " + user + ":" + " :No such nick/channel\r\n").c_str()
#define ERR_NOTONCHANNEL2(server, user, channel)(":" + server_name + " 442" + channel + " " + user + " " + user + " " + "#" + channel + " :They aren't on that channel\r\n").c_str()


// KICK
#define ERR_NOSUCHCHANNEL3(server, sender, channel) (":" + server + " 403 " + sender + " " + channel + " :No such channel\r\n")
#define ERR_CHANOPRIVSNEEDED(server, sender, channel) (":" + server + " 482 " + sender + " " + channel + " :You're not channel operator\r\n")
#define ERR_USERNOTINCHANNEL(server, sender, target, channel) (":" + server + " 441 " + sender + " " + target + " " + channel + " :They aren't on that channel\r\n")
#define ERR_CANTKICKSELF(server, operator_nickname, channel) (":" + server + " 484 " + operator_nickname + " " + channel + " :Cannot kick yourself\r\n")

// PRIVMSG
#define ERR_INPUTTOOLONG(server, client) \
    (":" + server + " 417 " + client + " :Your message is too long (maximum is 400 characters)\r\n")

// TOPIC
#define ERR_NOSUCHCHANNEL_TOPIC(server, nickname, channel) (":" + server + " 403 " + nickname + " " + channel + " :No such channel\r\n")

// INVITE
#define ERR_NOSUCHNICK_INVITE(server, client, target) (":" + server + " 401 " + client + " " + target + " :No such nick/channel\r\n")
#define ERR_NOTONCHANNEL_INVITE(server, client, channel) (":" + server + " 442 " + client + " " + channel + " :You're not on that channel\r\n")
#define ERR_CHANOPRIVSNEEDED_INVITE(server, client, channel) \
    (":" + server + " 482 " + client + " " + channel + " :You're not channel operator\r\n")
#define ERR_USERONCHANNEL(server, client, target, channel) \
    (":" + server + " 443 " + client + " " + target + " " + channel + " :is already on channel\r\n")


// ################################################################################
// #                                    REPLIES IRSSI                               #
// ################################################################################

#define RPL_LIST(server, channel, client_count, topic) (":" + server_name + " 322 " + server + " #" + channel + " " + client_count + " :" + topic + "\r\n").c_str()
#define RPL_LISTEND(server) (":" + server_name + " 323 " + server + " :End of /LIST\r\n").c_str()

#define QUIT_MESSAGE(nickname, username, hostname, message) (":" + nickname + "!" + username + "@" + hostname + " QUIT :" + message + "\r\n").c_str()
#define PART_MESSAGE(nickname, username, hostname, channel, reason) (":" + nickname + "!" + username + "@" + hostname + " PART " + channel + " :" + reason + "\r\n");

#define RPL_CHANNELMODEIS(server, username, channel, modes) (":" + server_name + " 324 " + username + "#" + channel + " " + modes + "\r\n").c_str()
#define RPL_CREATIONTIME(server, channel, creation_date_str) (":" + server_name + " 329 " + server + " " + channel + " " + creation_date_str + "\r\n").c_str()

//   "<client> <channel> <modestring> <mode arguments>..."

// Sent to a client to inform them of the currently-set modes of a channel. 
// <channel> is the name of the channel. 
// <modestring> and <mode arguments> are a mode string and the mode arguments (delimited as separate parameters) as defined in the MODE message description.

// RPL_CREATIONTIME (329)

//   "<client> <channel> <creationtime>"

// Sent to a client to inform them of the creation time of a channel. <channel> is the name of the channel. 
// <creationtime> is a unix timestamp representing when the channel was created on the network.

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
#define RESET "\e[0m"  // Réinitialise les couleurs et attributs
