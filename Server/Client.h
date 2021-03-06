//
// Created by Marcin on 14.05.2019.
//

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H


#include <sys/poll.h>
#include "threads/ClientLogic.h"
#include "threads/MessageSender.h"
#include "threads/ClientInOutAction.h"

/**
 * Client wysyła wiadomości
 * Client odbiera wiadomości
 * Client mówi, czy chce coś odebrać
 * Cleint mówi, czy chce coś wysłać
 * Client trzyma swój file deskryptor
 *
 *
 *
 */
class Client {
private:
    bool connected = true;
    int fd;
    bool wantsToRead = true;
    bool wantsToWrite;
    SynchronizedQueue<std::unique_ptr<Request>> inQueue;
    SynchronizedQueue<std::unique_ptr<Serializable>> outQueue;
    ClientLogic clientLogic;
    ClientInOutAction clientInOutAction;

public:

    Client(int, int);

    void registerActions(pollfd *);

    void stop();

    void receive();

    void send();

    bool isConnected();

    ClientLogic getLogic(){return clientLogic;}

    void disconnect(){clientInOutAction.disconnect();connected=false;}
};


#endif //SERVER_CLIENT_H
