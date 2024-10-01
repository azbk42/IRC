# ft_irc - 42 Project

## Table of Contents
- [Description](#description)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Commands](#commands)

## Description

`ft_irc` est un projet de l'école 42 qui consiste à créer un serveur IRC (Internet Relay Chat) en C++. Le but est d'apprendre à gérer un serveur multi-client, à manipuler les sockets, à utiliser des protocoles réseau (IRC RFC 1459), et à gérer plusieurs clients en parallèle sans multi-threading.

Le serveur doit gérer les connexions, les canaux, les messages privés, ainsi que les commandes d'administration.

## Features

- Gestion des clients multiples via des sockets non-bloquants (pas de threading).
- Implémentation des commandes IRC de base (JOIN, PART, NICK, PRIVMSG, etc.).
- Gestion des canaux IRC.
- Administration du serveur (KICK, BAN, INVITE, TOPIC, etc.).
- Communication en temps réel entre les utilisateurs.
- Respect de la norme IRC RFC 1459.

## Requirements

Pour compiler et exécuter ce projet, vous avez besoin de :

- Un compilateur **C++** compatible (comme `g++`).
- Un système basé sur **Unix** (Linux ou macOS recommandé).
- **Makefile** pour automatiser la compilation.

## Installation

1. Clonez le dépôt Git sur votre machine locale :
   ```bash
   git clone https://github.com/votre-utilisateur/ft_irc.git
   cd ft_irc
   ```

2. Compilez le projet avec make :
    ```bash
    make
    ```
3. Après compilation, un exécutable nommé ircserv sera disponible.

## Usage

Lancer le serveur IRC

1. Pour démarrer le serveur IRC, exécutez la commande suivante :
    ```bash
    ./ircserv <port> <password>
    ```
    - <port> : Le port sur lequel le serveur écoute.
    - <password> : Le mot de passe pour se connecter au serveur.
2. Exemple:
    ```bash
    ./ircserv 6667 password
    ```
3. Vous pouvez maintenant vous connecter à votre serveur IRC à l'aide d'un  client IRC comme WeeChat, irssi, ou n'importe quel autre client IRC.

## Commands

Cette section décrit les principales commandes implémentées sur le serveur `ft_irc` et comment les utiliser avec le client **irssi**.

### Commandes IRC de base

| Commande      | Description                              | Exemple dans irssi |
|---------------|------------------------------------------|--------------------|
| `PASS`        | Définir le mot de passe de connexion.     | `/pass mypassword`  |
| `NICK`        | Choisir ou modifier un pseudonyme.        | `/nick mynickname`  |
| `USER`        | Enregistrer un nouvel utilisateur.       | Automatique après connexion |
| `JOIN`        | Rejoindre un canal.                      | `/join #channel`    |
| `PART`        | Quitter un canal.                        | `/part #channel`    |
| `PRIVMSG`     | Envoyer un message privé à un utilisateur ou un canal. | `/msg #channel Hello everyone!` ou `/msg nickname Salut !` |
| `QUIT`        | Déconnecter l'utilisateur du serveur.     | `/quit`            |
| `MODE`        | Gérer les modes utilisateur ou canal.     | `/mode #channel +o nickname` |
| `TOPIC`       | Modifier le sujet d'un canal.            | `/topic #channel Nouveau sujet du jour` |
| `KICK`        | Expulser un utilisateur d'un canal.      | `/kick #channel nickname` |

### Commandes d'administration

Les commandes suivantes sont réservées aux administrateurs du serveur IRC pour gérer les utilisateurs et les canaux.

| Commande      | Description                              | Exemple dans irssi                      |
|---------------|------------------------------------------|-----------------------------------------|
| `INVITE`      | Inviter un utilisateur dans un canal privé. | `/invite nickname #channel`             |
| `BAN`         | Bannir un utilisateur d'un canal.        | `/mode #channel +b nickname`            |
| `KILL`        | Déconnecter un utilisateur du serveur.   | `/kill nickname`                        |