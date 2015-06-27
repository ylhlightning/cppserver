/**********************************************************
* 
* @file server_event_notifier.h
* 
* @brief  server event notifier declearation.
* 
*
* @author  Linhu Ying
* @email    ylh1986@hotmail.com
* @date     27/06/2015
*
***********************************************************/


#ifndef __SERVER_EVENT_NOTIFIER_H__
#define __SERVER_EVENT_NOTIFIER_H__

/**************************************************************
* Specifies callbacks from a client to its server.
**************************************************************/

/**
* This function is invoked as a callback in case a disconnect on 
* TCP level is detected.
*/
typedef void (*OnClientClosedFunc)(void* server,
                                   void* closedClient);

typedef struct
{
   /* An instance of the server owning the client.
      This instance shall be passed as an argument to the callbacks. */
   void* server;

   /* Specifies a callback to be used by the client to 
      inform its server about a closed connection. */   
   OnClientClosedFunc onClientClosed;
   
} ServerEventNotifier;

#endif
