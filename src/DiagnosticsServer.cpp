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

/******************************************************************************
* A simulation of a diagnostics server.
*
* A client that wants to log a diagnostic connects to this server using TCP/IP.
* The server gets notified about a connecting client through the Reactor.
* Upon such a notification, the server creates a client representation.
*******************************************************************************/

#include "DiagnosticsServer.h"
#include "DiagnosticsClient.h"
#include "EventHandler.h"
#include "ServerEventNotifier.h"
#include "Reactor.h"

#include "Error.h"
#include "TcpServer.h"

#include <stdlib.h>
#include <stdio.h>


/************************************************************
* Function declarations.
************************************************************/

static void deleteAllClients(Server* server);

static int findFreeClientSlot(Server* server);

static int findMatchingClientSlot(Server* server, Client* client);

static int matchControlledClientByPointer(Server* server,Client* clientToMatch);

static Handle getServerSocket(void* instance);

static void handleConnectRequest(void* instance);

static void onClientClosed(void* server,
                           void* closedClient);

Server * Server::m_server = NULL;


/************************************************************
* Implementation of the EventHandler interface.
************************************************************/

Server:: Server(int num)
{
  /* All connected clients able to send diagnostics messages. */

  clients = new Client*[num];

  for(int i = 0; i < num; i++)
  {
    clients[i] = NULL;
  }

  cout << "create a new server.\n" << endl;
  
}

Server:: ~Server()
{
  /* All connected clients able to send diagnostics messages. */
  delete m_server;
}

Server* Server:: getInstance(void)
{
  if(m_server == NULL)
  {
    m_server = new Server(MAX_NO_OF_CLIENTS);
  }
  
  return m_server;
}

Handle Server:: getServerListeningSocket(void)
{
  return listeningSocket;
}

void Server:: setServerListeningSocket(Handle new_listeningSocket)
{
  listeningSocket = new_listeningSocket;
}

Reactor *Server:: getServerReactor(void)
{
  return reactor;
}

void Server:: setServerReactor(Reactor* new_reactor)
{
  reactor = new_reactor;
}


EventHandler Server:: getServerEventHandler(void)
{
  return eventHandler;
}

void Server:: setServerEventHandler(EventHandler new_eventHandler)
{
  eventHandler = new_eventHandler;
}

Client *Server:: getClient(const int slot)
{
  return clients[slot];
}

void Server:: addNewClient(const int freeSlot, Client *new_client)
{
  clients[freeSlot] = new_client;
}

void Server:: clearClient(const int slot)
{
  clients[slot] = NULL;
}

static Handle getServerSocket(void* instance)
{
   Server* server = (Server*)instance;
   return server->getServerListeningSocket();
}

static void handleConnectRequest(void* instance)
{
   Server *server = (Server *)instance;
   Handle new_listeningSocket = server->getServerListeningSocket();
   Reactor *reactor = server->getServerReactor();
   Client *new_client;
  
   const int freeSlot = findFreeClientSlot(server);
   
   if(0 <= freeSlot) {
      /* Define a callback for events requiring the actions of the server (for example 
         a closed connection). */
      ServerEventNotifier eventNotifier = {0};
      eventNotifier.server = server;
      eventNotifier.onClientClosed = onClientClosed;
      
      new_client = createClient(new_listeningSocket, &eventNotifier, reactor);
      server->addNewClient(freeSlot, new_client);
        
      (void) printf("Server: Incoming connect request accepted\n");
   }
   else {
      (void) printf("Server: Not space for more clients\n");
   }
}

/************************************************************
* Implementation of the ServerEventNotifier interface. 
************************************************************/

/**
* This function is invoked as a callback from the client representation 
* in case it detects a disconnect on TCP level.
*/
static void onClientClosed(void* server,
                           void* closedClient)
{
   Server* serverInstance = (Server*)server;
   Client* clientInstance = (Client*)closedClient;
   Reactor* reactor = serverInstance->getServerReactor();
   
   const int clientSlot = findMatchingClientSlot(serverInstance, clientInstance);
   
   if(0 > clientSlot) {
      error("Phantom client detected");
   }
   
   destroyClient(clientInstance, reactor);

   serverInstance->clearClient(clientSlot);
}

/************************************************************
* Implementation of the ADT functions of the server. 
************************************************************/

/**
* Creates a server listening for connect requests on the given port.
* The server registers itself at the Reactor upon creation.
*/
Server *createServer(unsigned int tcpPort)
{
   Server *newServer = Server::getInstance();
   Handle new_listeningSocket;
   EventHandler eventHandler;
   Reactor *reactor = Reactor::GetReactorInstance();

   if(NULL != newServer) {
      int i = 0;
     
      new_listeningSocket = createServerSocket(tcpPort);
      newServer->setServerListeningSocket(new_listeningSocket);

      /* Successfully created -> register the listening socket. */
      eventHandler.instance = newServer;
      eventHandler.getHandle = getServerSocket;
      eventHandler.handleEvent = handleConnectRequest;

      reactor->Register(&eventHandler);

      newServer->setServerReactor(reactor);
      newServer->setServerEventHandler(eventHandler);
      
   }
   
   return newServer;
}

/**
* Unregisters at the Reactor and deletes all connected clients 
* before the server itself is disposed.
* After completion of this function, the server-handle is invalid.
*/
void destroyServer(Server* server)
{
   Reactor *reactor = server->getServerReactor();
   deleteAllClients(server);

   EventHandler event_handler = server->getServerEventHandler();
   Handle listeningSocket = server->getServerListeningSocket();
   
   /* Before deleting the server we have to unregister at the Reactor. */
   reactor->Unregister(&event_handler);

   disposeServerSocket(listeningSocket);

   delete server;
}

/************************************************************
* Definition of the local utility functions.
************************************************************/

static void deleteAllClients(Server *server)
{
   int i = 0;

   Client *client;
   Reactor *reactor = server->getServerReactor();
      
   for(i = 0; i < MAX_NO_OF_CLIENTS; ++i) {

      client = server->getClient(i);
      
      if(NULL != client) {
         destroyClient(client, reactor);
      }
   }
}

/**
* Returns the index where a client matching the given pointer is found.
* Returns -1 if no match was found. 
*/
int matchControlledClientByPointer(Server * server, Client * clientToMatch)
{
   int clientSlot = -1;
   int slotFound = 0;
   int i = 0;
      
   for(i = 0; (i < MAX_NO_OF_CLIENTS) && (0 == slotFound); ++i) {
      
      if(clientToMatch == server->getClient(i)) {
         clientSlot = i;
         slotFound = 1;
      }
   }
   
   return clientSlot;
}

int findFreeClientSlot(Server* server)
{
   return matchControlledClientByPointer(server, NULL);
}

int findMatchingClientSlot(Server * server, Client * client)
{  
   return matchControlledClientByPointer(server, client);
}


