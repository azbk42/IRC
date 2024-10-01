#include "client.hpp"

// METHOD

void client::handle_user_command(std::string &user_infos)
{

    std::string::size_type pos = user_infos.find(":");
    if (pos != std::string::npos){
        std::string name = user_infos.substr(pos + 1);
        set_real_name(name);
        set_nickname_setup();

        std::string other_infos = user_infos.substr(0, pos - 1);

        std::string::size_type index = other_infos.find(" ");
        name = other_infos.substr(0, index);
        set_nickname(name);
        other_infos = other_infos.substr(index +1);

        index = other_infos.find(" ");
        name = other_infos.substr(0, index);
        set_host(name);
        name = other_infos.substr(index + 1);
        set_server(name);
    }
    else{
        std::cerr << "You need a nickname" << std::endl;
    }
}


// GETTER
std::string client::get_real_name() const {return _real_name;};

std::string client::get_nickname() const {return _nickname;};

std::string client::get_server_name() const {return _server_name;};

std::string client::get_host_name() const {return _host_name;};

int client::get_socket_fd() const {return _socket_fd;};

bool client::get_status_away() const {return _away;};

bool client::get_status_connected() const {return _connected;};


//SETTER
// rajouter une gestion d'erreur sur le nickname
void client::set_nickname(std::string username)
{
    _nickname = username;
}

void client::set_real_name(std::string name)
{
    _real_name = name;
}

void client::set_server(std::string server)
{
    _server_name = server;
}

void client::set_host(std::string host)
{
    _host_name = host;
}
void client::set_nick()
{
    _nickname_setup = true;
}

void client::set_away(bool status)
{
    _away = status;
}
void client::set_connected(bool status)
{
    _connected = status;
}

void client::set_nickname_setup()
{
    _nickname_setup = true;
}

client::client(std::string real_name, std::string server, std::string host, int socket):
            _real_name(real_name), _server_name(server), _host_name(host), _socket_fd(socket),\
            _nickname_setup(false), _away(false), _connected(true)
{

}
client::client()
{

}

client::~client() 
{

}
