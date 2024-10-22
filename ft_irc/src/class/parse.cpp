#include "parse.hpp"
#include "mode.hpp"

// ################################################################################
// #                                  GET                                         #
// ################################################################################

std::string Parse::get_cmd() const
{
    return _command;
}

std::string Parse::get_value() const {
	return _value;
}

// ################################################################################
// #                                   PARSING                                    #
// ################################################################################

void Parse::parse_invite(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{

    Invite inv(clients_list, client_fd, client_actif, channels);
    inv.init_cmd_invite(_value);
}

void Parse::parse_topic(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{
    Topic top(clients_list, client_fd, client_actif, channels);
    top.init_topic(_value);
}

void Parse::parse_whois(std::vector<Client*> &clients_list, int client_fd, Client &client_actif)
{
    Whois who(clients_list, client_fd, client_actif);
    who.init_cmd_msg(_value);
}

void Parse::parse_msg(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{
    Msg privmsg(clients_list, client_fd, client_actif, channels);
    privmsg.init_cmd_msg(_value);       
}

void Parse::parse_nick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels, Server* server)
{
    Nick command(clients_list, client_fd, client_actif, channels);
    command.init_cmd_nick(_value, server);
}

void Parse::parse_kick(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{
    Kick command(clients_list, client_fd, client_actif, channels);
    command.init_cmd_kick(_value);
}

void Parse::parse_join(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{
    (void)(clients_list);
    Join command(client_fd, client_actif, channels, _value);
    command.init_cmd_join();
}

void Parse::parse_ping(std::vector<Client*> &clients_list, int client_fd, Client &client_actif)
{
    (void)clients_list;
    (void)client_actif;

    std::string pong = "PONG "+ std::string(SERVER_NAME) + " :" + _value + "\r\n";
    send_message(client_fd, pong);

}

bool Parse::parse_user(std::vector<Client*> &clients_list, int client_fd, Client &client_actif)
{
    (void)clients_list;
    std::string server_name = SERVER_NAME;

    if (client_actif.get_user_setup() == true){
        send_message(client_fd, ERR_ALREADYREGISTERED(server_name));
        return true;
    }

    if (std::count(_value.begin(), _value.end(), ' ') < 3){
        send_message(client_fd, ERR_NEEDMOREPARAMS("USER", server_name));
        return false;
    }

    // Si tout est bon on recupere les donnees et on informe le client pour valider la connexion
    if (extract_user_info(_value, client_actif) == false){
        send_message(client_fd, ERR_NEEDMOREPARAMS("USER", server_name));
        return false;
    }
    
    std::string welcome_message = ":" + server_name + " 001 " + client_actif.get_nickname() + 
                              " :\x03""04Welcome to the Internet Relay Network " + 
                              client_actif.get_nickname() + "!" + 
                              client_actif.get_username() + "@" + client_actif.get_hostname() + 
                              " - ft_irc 42 Paris\x03""\r\n";

    send_message(client_fd, welcome_message);
    return true;
}

void Parse::parse_list(int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{
	List list(client_actif, channels, client_fd, _value);
	list.send_list();

    return ;
}
void Parse::parse_part(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{
	(void)clients_list;
    Part part(channels, client_fd, client_actif, _value);
	part.init_cmd_part();

    return ;
}

void Parse::parse_mode(std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{
	Mode mode(clients_list, client_actif, channels, client_fd, _value);
	mode.init_cmd_mode();

	return ;
}
// ################################################################################
// #                                    FIND                                      #
// ################################################################################


void Parse::find_cmd_type(const std::string &cmd, std::vector<Client*> &clients_list, int client_fd, Client &client_actif, std::vector<Channel*> &channels)
{
    if (_command_map.find(cmd) != _command_map.end())
    {
        f func = _command_map[cmd];
        (this->*func)(clients_list, client_fd, client_actif, channels); 
    }
    if (cmd == "LIST")
		parse_list(client_fd, client_actif, channels);
    if (cmd == "WHOIS")
        parse_whois(clients_list, client_fd, client_actif);
    if (cmd == "PING")
        parse_ping(clients_list, client_fd, client_actif);
}
// ################################################################################
// #                                SETUP CMD USER                                #
// ################################################################################
void Parse::split_cmd_value(const std::string &full_command)
{
    size_t pos = full_command.find(" ");

    if (pos == std::string::npos) {
        _command = full_command;
        std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);
        _value.clear();
        return;
    }

    _command = full_command.substr(0, pos);
    std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);

    if (pos + 1 >= full_command.size()) {
        _value.clear();
        std::cout << CYAN << "Command = " << WHITE << _command << std::endl;
        std::cout << GREEN << "Value = (empty)" << WHITE << std::endl;
        return;
    }

    _value = full_command.substr(pos + 1);
    std::cout << CYAN << "Command = " << WHITE << _command << std::endl;
    std::cout << GREEN << "Value = " << WHITE << _value << std::endl;
}

bool Parse::extract_user_info(const std::string& value, Client& client_actif)
{
    size_t pos = value.find(":");
    if (pos != std::string::npos) {
        std::string real_name = value.substr(pos + 1);
        client_actif.set_real_name(real_name);
    }
    else
        return false;

    pos = value.find(" ");
    if (pos != std::string::npos) {
        client_actif.set_username(value.substr(0, pos));
        std::string new_str = value.substr(pos + 1);

        pos = new_str.find(" ");
        if (pos != std::string::npos) {
            client_actif.set_host(new_str.substr(0, pos));
            new_str = new_str.substr(pos + 1);

            pos = new_str.find(" ");
            if (pos != std::string::npos) {
                client_actif.set_server(new_str.substr(0, pos));
            }
        }
    }
    return true;
}

void Parse::init_command_map()
{
    _command_map["PRIVMSG"] = &Parse::parse_msg;
    _command_map["INVITE"] = &Parse::parse_invite;
    _command_map["TOPIC"] = &Parse::parse_topic;
    _command_map["KICK"] = &Parse::parse_kick;
    _command_map["JOIN"] = &Parse::parse_join;
    _command_map["PART"] = &Parse::parse_part;
    _command_map["MODE"] = &Parse::parse_mode;
}

// ################################################################################
// #                       Constructor / Destructor                               #
// ################################################################################

Parse::Parse(const std::string &str): _str(str)
{
    std::string cleaned_str = _str;
    cleaned_str.erase(std::remove(cleaned_str.begin(), cleaned_str.end(), '\r'), cleaned_str.end());
    cleaned_str.erase(std::remove(cleaned_str.begin(), cleaned_str.end(), '\n'), cleaned_str.end());

    split_cmd_value(cleaned_str);
    init_command_map();
}

Parse::~Parse()
{

}