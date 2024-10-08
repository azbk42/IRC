#include "client.hpp"
#include <iomanip>

// METHOD
void Client::handle_cmd_user(std::string &user_infos)
{
    
}


// ################################################################################
// #                                HANDLE CMD                                    #
// ################################################################################

void Client::handle_cmd_nick(const std::string &new_nickname, int client_socket)
{
    // LES ERREURS ON ETE SETUP POUR LUI
    std::string old_nick = get_nickname();
    int socket = get_socket_fd();
    set_nickname(new_nickname);
    
    
    //envoyer le message au client comme quoi on a bien changer de pseudo
    send(client_socket, NICK_CHANGE(old_nick, new_nickname), strlen(NICK_CHANGE(old_nick, new_nickname)), 0);

}
bool Client::check_nb_chan()
{
    std::cout << "NB CHAN = " << _nb_chan << std::endl;
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



// ################################################################################
// #                                    SET                                       #
// ################################################################################
// rajouter une gestion d'erreur sur le nickname
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

Client::Client(int socket): _real_name("real_name"), _server_name("server"), _host_name("host"), _socket_fd(socket),\
            _user_setup(false), _away(false), _connected(true), _username("username"), _checked_pwd(false), _nb_chan(0)
{
    //_arrival_time = std::chrono::system_clock::now();

    // format pour imprimer le temps sous la forme:
    // heure darrive: Tue Oct  1 14:26:09 2024
    // std::time_t arrival_time_t = std::chrono::system_clock::to_time_t(_arrival_time);
    // std::cout << "heure darrive: " << std::ctime(&arrival_time_t) << std::endl;
}
Client::Client()
{

}

Client::~Client() 
{

    // code pour donner la difference entre le temps de connexion et le temps de deco
    // std::cout << "Client numero: " << _socket_fd << " id dead" << std::endl;
    // auto end_time = std::chrono::system_clock::now();
    // std::chrono::duration<double> elapsed_seconds = end_time - _arrival_time;

    // double elapsed_time = elapsed_seconds.count();

    // std::cout << "Elapsed time: ";
    // std::cout << std::fixed << std::setprecision(3) << elapsed_time << " s" << std::endl;
}
