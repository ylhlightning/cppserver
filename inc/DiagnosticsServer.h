/********************************************************************
* (C) Adam Petersen 2005
*
* This code accompanies the article "Patterns in C, part 5: REACTOR".
* The article is available as PDF from www.adampetersen.se
*
* The code is intended as a illustration of the Reactor at work and 
* is not suitable for use in production code (in order to keep this 
* example as short as possible, the error handling has been strongly 
* simplified).
*
********************************************************************/

#include<iostream>
#include "EventHandler.h"
#include "DiagnosticsClient.h"
#include "Reactor.h"
#include<iostream>
using namespace std;


#ifndef DIAGNOSTICS_SERVER_H
#define DIAGNOSTICS_SERVER_H

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
