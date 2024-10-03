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

#include "parse.hpp"
#include <map>

int prepareSelect(fd_set& read_fds, int server_fd, std::vector<int>& client_sockets) {
    FD_ZERO(&read_fds);
    FD_SET(server_fd, &read_fds);
    int max_sd = server_fd;

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
        perror("select error");
        return -1;
    }

    return activity;
}

void handleNewConnection(int server_fd, std::vector<int>& client_sockets, std::vector<Client*>& clients, struct sockaddr_in& client_addr, socklen_t& client_addr_len) {
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept error");
        return;
    }

    // Créer un nouvel objet Client (tu peux remplacer les valeurs par celles adaptées à ton application)
    Client* new_client = new Client(client_fd);

    // Ajouter le nouveau client à la liste
    clients.push_back(new_client);

    // Ajouter le nouveau socket client à la liste des sockets clients
    client_sockets.push_back(client_fd);
    std::cout << "New connection, socket fd is " << client_fd << std::endl;
}


void processAndBroadcastMessage(int client_socket, std::map<int, std::string>& partial_messages, std::vector<int>& client_sockets, std::vector<Client*> clients) {
    // Check if the message contains a newline character
    size_t newline_pos;
    while ((newline_pos = partial_messages[client_socket].find('\n')) != std::string::npos) {
        // Extract the complete message up to the newline
        std::string complete_message = partial_messages[client_socket].substr(0, newline_pos + 1);

        // Remove the processed message from the buffer
        partial_messages[client_socket].erase(0, newline_pos + 1);

        // Log the complete message
        std::cout << "Received message: " << complete_message << std::endl;

        // ici on va init la classe Parser et separer la Commande (_command) de la Value (_value)
        Parse parser(complete_message);
        // Déclaration de la référence en dehors de la boucle
        Client* client_actif = nullptr;

        // Rechercher le client correspondant au client_socket
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i]->get_socket_fd() == client_socket) {
                client_actif = clients[i];  // Ici, on affecte le pointeur
                break; // Sortir de la boucle une fois trouvé
            }
        }

        // Vérifier si un client a été trouvé
        if (client_actif != nullptr) {
            // PARSER POUR NICK PRESQUE OK JE DOIS FAIRE EN SORTE
            // d'envoyer à tous les clients du même chan
            if (parser.get_cmd() == "NICK")
                parser.parse_nick(clients, client_socket);
            if (parser.get_cmd() == "USER")
                parser.parse_user(clients, client_socket, *client_actif); // Passer par référence
        } else {
            std::cerr << "Client non trouvé pour le socket " << client_socket << std::endl;
        }


        // Broadcast the message to all other clients
        for (std::vector<int>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it) {
            if (*it != client_socket) {
                send(*it, complete_message.c_str(), complete_message.size(), 0);
            }
        }
    }
}


int main() 
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int port = 6667; // Port IRC par défaut
    char buffer[1024];
    std::map<int, std::string> partial_messages;
    std::vector<int> client_sockets; // Stocker les sockets clients
    fd_set read_fds; // Ensemble de file descriptors surveillés
    int max_sd; // Pour garder une trace du descripteur le plus élevé

    std::vector<Client*> clients;
    std::vector<Channel*> channels;
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
    int opt = 1;
    if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) == -1) {
        perror("setsockopt");
        exit(1);
    }
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
   
        int activity = prepareSelect(read_fds, server_fd, client_sockets);
        if (activity < 0) {
            break;  // Si une erreur est rencontrée
        }

        // Check if a new connection is pending
        if (FD_ISSET(server_fd, &read_fds)) {
            handleNewConnection(server_fd, client_sockets, clients, client_addr, client_addr_len);
        }

        // Check activity on client sockets
        for (size_t i = 0; i < client_sockets.size(); i++) {
            int client_socket = client_sockets[i];
            if (FD_ISSET(client_socket, &read_fds)) {
                ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
                if (bytes_read == 0) {
                    // Client disconnected
                    std::cout << "Client disconnected: " << client_socket << std::endl;
                    close(client_socket);
                    client_sockets.erase(client_sockets.begin() + i);
                    i--; // Adjust index after deletion
                } 
                else if (bytes_read < 0) {
                    if (errno != EWOULDBLOCK && errno != EAGAIN) {
                        // Read error, handle it (but don't close the server)
                        perror("read error");
                    }
                } 
                else {

                    buffer[bytes_read] = '\0'; // Null-terminate the string
                    std::string message(buffer);
                    
                    // on ajoute le message par rapport au socket tant quil n'y a pas de \n 
                    partial_messages[client_socket] += message;

                    // ici commence le parsing du message, il va boucler tant quil trouve un \n et traiter les commandes
                    // on va donc proceder a la reception du message, le traiter, et renvoyer les informations demandees a tous les clients concernes.
                    processAndBroadcastMessage(client_socket, partial_messages, client_sockets, clients);
            }
        }
    }
}

// Close the server socket before exiting
close(server_fd);


    return 0;
}

