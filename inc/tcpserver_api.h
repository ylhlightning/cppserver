 /**********************************************************
 * 
 * @file tcpserver_api.h
 * 
 * @brief tcp server api declearation
 * 
 *
 * @author   Linhu Ying
 * @email    ylh1986@hotmail.com
 * @date     27/06/2015
 *
 ***********************************************************/


#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

int createServerSocket(unsigned int tcpPort);

void disposeServerSocket(int serverSocket);

#endif
