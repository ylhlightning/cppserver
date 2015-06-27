/**********************************************************
* 
* @file server.h
* 
* @brief  server class declearation.
* 
*
* @author  Linhu Ying
* @email    ylh1986@hotmail.com
* @date     27/06/2015
*
***********************************************************/


#include<iostream>
#include "event_handler.h"
#include "client.h"
#include "reactor.h"
#include<iostream>
using namespace std;


#ifndef __SERVER_H__
#define __SERVER_H__

#define MAX_NO_OF_CLIENTS 10

class Server
{
  public:
     Server(int num);
     ~Server();   

    static Server *getInstance(void);

    Handle getServerListeningSocket(void);
    void setServerListeningSocket(Handle new_listeningSocket);

    EventHandler getServerEventHandler(void);
    void setServerEventHandler(EventHandler new_eventHandler);
    
    Reactor *getServerReactor(void);
    void setServerReactor(Reactor *new_reactor);

    void addNewClient(const int freeSlot, Client *new_client);
    void clearClient(const int slot);
    Client* getClient(int slot);
    
  private:
     
    Handle listeningSocket;
    EventHandler eventHandler;
    Client **clients;
    Reactor *reactor;
    static Server *m_server;
};

/**
* Creates a server listening for connect requests on the given port.
* The server registers itself at the Reactor upon creation.
*/
Server* createServer(unsigned int tcpPort);

/**
* Unregisters at the Reactor and deletes all connected clients 
* before the server itself is disposed.
* After completion of this function, the server-handle is invalid.
*/
void destroyServer(Server* server);


#endif
