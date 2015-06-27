/**********************************************************
* 
* @file main.cpp
* 
* @brief  a minimal server implementation by using cpp.
* 
*
* @author  Linhu Ying
* @email    ylh1986@hotmail.com
* @date     27/06/2015
*
***********************************************************/

#include "server.h"
#include "reactor_event_loop.h"
#include "reactor.h"

#include "error.h"

#include <stddef.h>


int main(void){
   
   /* Create a server and enter an eternal event loop, watching 
      the Reactor do the rest. */
   
   const unsigned int serverPort = 5000;
   
   Server* server = createServer(serverPort);

   if(NULL == server) {
      error("Failed to create the server");
   }

   Reactor* reactor = server->getServerReactor();

   /* Enter the eternal reactive event loop. */
   for(;;){
      reactor->HandleEvents();
   }
   
   destroyServer(server);
   
}


