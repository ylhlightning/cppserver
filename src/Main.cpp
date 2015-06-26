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

#include "DiagnosticsServer.h"
#include "ReactorEventLoop.h"
#include "Reactor.h"

#include "Error.h"

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


