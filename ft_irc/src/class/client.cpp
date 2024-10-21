#include "client.hpp"
#include <iomanip>

// ################################################################################
// #                                    TIME                                      #
// ################################################################################




// ################################################################################
// #                                HANDLE CMD                                    #
// ################################################################################

void Client::handle_cmd_nick(const std::string &new_nickname, int client_socket)
{
    std::string old_nick = get_nickname();
    set_nickname(new_nickname);
    send_message(client_socket, NICK_CHANGE(old_nick, new_nickname));
}
bool Client::check_nb_chan()
{
    if (_nb_chan >= 5)
        return false;
    else
        return true;
}

void Client::add_nb_channel()
{
    _nb_chan += 1;
}
void Client::minus_nb_channel()
{
    if (_nb_chan > 0)
        _nb_chan -= 1;
}

// ################################################################################
// #                                    GET                                       #
// ################################################################################

std::string Client::get_real_name() const {return _real_name;};

std::string Client::get_nickname() const {return _nickname;};

std::string Client::get_username() const {return _username;};

std::string Client::get_server_name() const {return _server_name;};

std::string Client::get_hostname() const {return _host_name;};

int Client::get_socket_fd() const {return _socket_fd;};

bool Client::get_status_away() const {return _away;};

bool Client::get_status_connected() const {return _connected;};

bool Client::get_user_setup() const {return _user_setup;};

bool Client::get_checked_pwd() const {return _checked_pwd;};

bool Client::GetFirstNick() const {return _first_nick;};

time_t Client::get_connection_time() const {return _connection_time;};


// ################################################################################
// #                                    SET                                       #
// ################################################################################

void Client::SetFirstNick() { _first_nick = false;};

void Client::set_nickname(const std::string &nickname)
{
    _nickname = nickname;

    std::cout << MAGENTA << "Nouveau Nick: " << WHITE << _nickname << std::endl;
}

void Client::set_username(const std::string &username)
{
    _username = username;
}

void Client::set_real_name(const std::string &name)
{
    _real_name = name;
}

void Client::set_server(const std::string &server)
{
    _server_name = server;
}

void Client::set_host(const std::string &host)
{
    _host_name = host;
}

void Client::set_away(bool status)
{
    _away = status;
}
void Client::set_connected(bool status)
{
    _connected = status;
}

void Client::set_user_setup()
{
    _user_setup = true;
}

void Client::set_checked_pwd(bool status)
{
	_checked_pwd = status;
}

// ################################################################################
// #                             CONSTRUCTOR DESTRUCTOR                           #
// ################################################################################

Client::Client(int socket): _user_setup(false), _away(false), _connected(true), _first_nick(true),\
            _real_name("real_name"), _server_name("server"), _host_name("host"), \
            _username("username"), _checked_pwd(false), _nb_chan(0),
            _socket_fd(socket), _connection_time(time(0))
{

}

Client::~Client() 
{

}
