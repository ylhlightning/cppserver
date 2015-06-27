/**********************************************************
* 
* @file client.cpp
* 
* @brief  client api declearation.
* 
*
* @author  Linhu Ying
* @email    ylh1986@hotmail.com
* @date     27/06/2015
*
***********************************************************/


#ifndef __CLIENT_H__
#define  __CLIENT_H__

#include "handle.h"
#include "server_event_notifier.h"
#include "event_handler.h"
#include "reactor.h"

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
