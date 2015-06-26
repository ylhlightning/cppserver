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

#ifndef DIAGNOSTICS_CLIENT_H
#define DIAGNOSTICS_CLIENT_H

#include "Handle.h"
#include "ServerEventNotifier.h"
#include "EventHandler.h"
#include "Reactor.h"

#include <iostream>

class Client
{
public:

   Client();
   ~Client();

   Handle getClientSocket(void);
   void setClientSocket(Handle clientSocket);
   
   EventHandler getServerEventHandler(void);
   void setServerEventHandler(EventHandler new_eventHandler);

   ServerEventNotifier getServerEventNotifier(void);
   void setServerEventNotifier(ServerEventNotifier *new_eventNotifier);

   Client& operator=(const Client &client);
   
private:

   Handle clientSocket;
   EventHandler eventHandler;
   ServerEventNotifier eventNotifier;
   static int clientCounter;
}; 


/**
* Creates a representation of the client used to send diagnostic messages.
* The given handle must refer to a valid socket signalled for a pending connect request.
*/
Client* createClient(Handle serverHandle, const ServerEventNotifier* eventNotifier, Reactor *reactor);

/**
* Unregisters the given client at the Reactor and releases all associated resources.
* After completion of this function, the client-handle is invalid.
*/
void destroyClient(Client* client, Reactor *reactor);

#endif
