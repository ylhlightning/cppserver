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

/*********************************************************************************
*
* The server side representation of a client sending diagnostic messages to the log.
*
* An instance of this client ADT is created as the server detects a connect request.
* Upon creation, the instance registers itself at the Reactor. The Reactor will notify 
* this client representation as an incoming diagnostics message is pending. 
* This client representation simply reads the message and prints it to stdout.
**********************************************************************************/

#include "DiagnosticsClient.h"
#include "DiagnosticsServer.h"
#include "EventHandler.h"
#include "Reactor.h"
#include "Error.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_MESSAGE_SIZE 1024

/************************************************************
* Function declarations.
************************************************************/

static Handle acceptClientConnection(int serverHandle);

static Handle getClientSocket(void* instance);

static void handleReadEvent(void* instance);

/************************************************************
* Implementation of the EventHandler interface.
************************************************************/

int Client::clientCounter = 0;

static Handle getClientSocket(void* instance)
{
   Client* client = (Client*) instance;
   return client->getClientSocket();
}

static void handleReadEvent(void* instance)
{
   Client* client = (Client *)instance;
  
   char clientMessage[MAX_MESSAGE_SIZE] = {0};
   char serverMessage[MAX_MESSAGE_SIZE] = "Server : received message from client.\n";

   const ssize_t receiveResult = recv(client->getClientSocket(), clientMessage, sizeof(clientMessage), 0);
   
   if(0 < receiveResult) {
      /* In the real world, we would probably put a protocol on top of TCP/IP in 
      order to be able to restore the message out of the byte stream (it is no 
      guarantee that the complete message is received in one recv(). */
      
      printf("Client: received - %s\n", clientMessage);
      
      ssize_t sendResult = send(client->getClientSocket(), serverMessage,sizeof(clientMessage), 0);

      if(0 < sendResult)
      {
         printf("Server: Answer client - %s\n", serverMessage);
      }
   }

   ServerEventNotifier onClientClosedNotifier = client->getServerEventNotifier();

   onClientClosedNotifier.onClientClosed(onClientClosedNotifier.server, client);
   
}

/************************************************************
* Implementation of the ADT functions of the client.
************************************************************/

/**
* Creates a representation of the client used to send diagnostic messages.
* The given handle must refer to a valid socket signalled for a pending connect request.
* The created client representation registers itself as the Reactor.
*/
Client:: Client()
{
   clientCounter ++;
}

Client:: ~Client()
{
  if(clientCounter > 0)
    clientCounter --;
}

Handle Client:: getClientSocket(void)
{
  return clientSocket;
}

void Client:: setClientSocket(Handle new_clientSocket)
{
  clientSocket = new_clientSocket;
}

EventHandler Client:: getServerEventHandler(void)
{
  return eventHandler;
}

void Client:: setServerEventHandler(EventHandler new_eventHandler)
{
  eventHandler = new_eventHandler;
}

ServerEventNotifier Client:: getServerEventNotifier(void)
{
  return eventNotifier;
}

void Client:: setServerEventNotifier(ServerEventNotifier *new_eventNotifier)
{
  eventNotifier = *new_eventNotifier;
}

Client& Client::operator=(const Client &client)
{
   clientSocket = client.clientSocket;
   eventHandler = client.eventHandler;
   eventNotifier = client.eventNotifier;
   clientCounter = client.clientCounter;
   return *this;
}

Client *createClient(Handle serverHandle, const ServerEventNotifier* eventNotifier, Reactor *reactor)
{
   Client *client = new Client();

   Handle new_clientSocket;
   EventHandler new_eventHandler;
   
   if(NULL != client) {
      new_clientSocket = acceptClientConnection(serverHandle);
      
      /* Successfully created -> register the client at the Reactor. */
      new_eventHandler.instance = client;
      new_eventHandler.getHandle = getClientSocket;
      new_eventHandler.handleEvent = handleReadEvent;

      reactor->Register(&new_eventHandler);
      
      assert(NULL != eventNotifier);
      client->setServerEventNotifier((ServerEventNotifier*)eventNotifier);
      client->setClientSocket(new_clientSocket);
      client->setServerEventHandler(new_eventHandler);
   }
   
   return client;
}


/**
* Unregisters the given client at the Reactor and releases all associated resources.
* After completion of this function, the client-handle is invalid.
*/
void destroyClient(Client *client, Reactor *reactor)
{

   Handle close_clientSocket = client->getClientSocket();
   EventHandler eventHandler = client->getServerEventHandler();
   
   /* Before deleting the client we have to unregister at the Reactor. */
   reactor->Unregister(&eventHandler);
   
   (void) close(close_clientSocket);
   delete client;
   client = NULL;
}

/************************************************************
* Definition of the local utility function.
************************************************************/

static Handle acceptClientConnection(int serverHandle)
{
   struct sockaddr_in clientAddress = {0};
   
   socklen_t addressSize = sizeof clientAddress;

   const Handle clientHandle = accept(serverHandle, (struct sockaddr*) &clientAddress, &addressSize);
   
   if(0 > clientHandle) {
      /* NOTE: In the real world, this function should be more forgiving.
      For example, the client should be allowed to abort the connection request. */
      error("Failed to accept client connection");
   }
   
   (void) printf("Client: New connection created on IP-address %X\n", ntohl(clientAddress.sin_addr.s_addr));
   
   return clientHandle;
}

