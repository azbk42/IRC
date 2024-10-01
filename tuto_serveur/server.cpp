#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 6667

int create_socket_server()
{
    int fd_socket;
    sockaddr_in add;

    // creation du socket
    // AF_INET pour IPV4
    fd_socket = socket(AF_INET, SOCK_STREAM, 0);

    // setup de ladresse du serveur
    add.sin_addr.s_addr = INADDR_ANY;
    add.sin_port = htons(PORT);
    add.sin_family = AF_INET;

    // bind le socket 
    int b = bind(fd_socket, (struct sockaddr *) &add, sizeof(add));
    if (b != 0){
        std::cerr << "BIND A EU UN PROBLEME" << std::endl;
    }
    std::cout << "b = " << b << std::endl;

    std::cout << "fd = " << fd_socket << std::endl;
    // maintenant on va faire en sorte d'ecouter
    if (listen(fd_socket, 1024) == -1){
        perror("Listen fail");
    }
    return fd_socket;
}



int main()
{
    int server_fd = create_socket_server();
    int max_sd;
    
    int client_socket[10] = {};
    fd_set read_fd;
    struct sockaddr_in add;
    char buffer[1024];

    while (true){

        FD_ZERO(&read_fd);
        FD_SET(server_fd, &read_fd);

        max_sd = server_fd;

        for (int i = 0; i < 10; i++){
            int sd = client_socket[i];

            if (sd > 0){

                FD_SET(sd, &read_fd);
            }

            if (sd > max_sd){
                max_sd = sd;
            }
        }


    }

     

}