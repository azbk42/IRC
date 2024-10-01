#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>  // Pour utiliser les sockets non-bloquants
#include <vector>
#include <algorithm> // Pour std::remove
#include "client.hpp"
#include "client.cpp"
int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int port = 6667; // Port IRC par défaut
    char buffer[1024];
    std::vector<int> client_sockets; // Stocker les sockets clients
    fd_set read_fds; // Ensemble de file descriptors surveillés
    int max_sd; // Pour garder une trace du descripteur le plus élevé
    std::vector<client> clients;
    // Créer le socket du serveur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    // Configurer l'adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Utiliser IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Lier à toutes les interfaces
    server_addr.sin_port = htons(port); // Convertir le port en ordre réseau


    // to avoid the "Address already in use" error message
    // int opt = 1;
    // if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) == -1) {
    //     perror("setsockopt");
    //     exit(1);
    // }
    // SOL_SOCKET pour le niveau de protocole auquel l'option s'applique: ici le socket
    // SO_REUSEADDR pour permettre la reutilisation de l'adresse locale
    // variable opt definie a 1 pour activer l'option 

    // Lier le socket à l'adresse et au port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // Écouter les connexions entrantes
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    // Mettre le socket du serveur en non-bloquant
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        // Initialiser l'ensemble des file descriptors à surveiller
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        max_sd = server_fd;

        // Ajouter les sockets clients à l'ensemble
        for (int client_socket : client_sockets) {
            if (client_socket > 0) {
                FD_SET(client_socket, &read_fds);
            }
            if (client_socket > max_sd) {
                max_sd = client_socket;
            }
        }

        // Utiliser select pour surveiller les sockets
        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR) {
            perror("select");
            break;
        }

        // Vérifier si une nouvelle connexion est en attente
        if (FD_ISSET(server_fd, &read_fds)) {
            client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
            if (client_fd < 0) {
                perror("accept");
                continue;
            }
            clients.push_back(client("Paul", "localhost", "server", client_fd));
           

            // Ajouter le nouveau client à la liste des sockets clients
            client_sockets.push_back(client_fd);
        }

        // Vérifier l'activité sur les sockets clients
        for (size_t i = 0; i < client_sockets.size(); i++) {
            int client_socket = client_sockets[i];

            if (FD_ISSET(client_socket, &read_fds)) {
                ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
                if (bytes_read <= 0) {
                    // Le client s'est déconnecté
                    std::cout << "Client disconnected: " << client_socket << std::endl;
                    close(client_socket);
                    client_sockets.erase(client_sockets.begin() + i);
                    i--; // Ajuster l'indice après la suppression
                } else {

                    // Lire le message et le transmettre aux autres clients
                    buffer[bytes_read] = '\0'; // Terminer la chaîne
                    
                    std::cout << "Received message: " << buffer << std::endl;

                    std::string user_infos(buffer);
                    if (user_infos.substr(0, 4) == "USER"){

                        std::string::size_type pos = user_infos.find(" ");
                        user_infos = user_infos.substr(pos + 1);
                        clients[0].handle_user_command(user_infos);

                        std::cout << "New client socket: " << clients[0].get_socket_fd() << std::endl;
                        std::cout << "New client real_name: " << clients[0].get_real_name() << std::endl;
                        std::cout << "New client server: " << clients[0].get_server_name() << std::endl;
                        std::cout << "New client nickname: " << clients[0].get_nickname() << std::endl;
                        std::cout << "New client hostname: " << clients[0].get_host_name() << std::endl;
                    }
                    
                    



                    // Réenvoyer le message à tous les autres clients
                    for (int other_client_socket : client_sockets) {
                        if (other_client_socket != client_socket) {
                            send(other_client_socket, buffer, bytes_read, 0);
                        }
                    }
                }
            }
        }
    }

    // Fermer le socket du serveur
    close(server_fd);

    return 0;
}
