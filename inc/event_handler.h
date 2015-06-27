/**********************************************************
* 
* @file event_handler.h
* 
* @brief  event handler declearation.
* 
*
* @author  Linhu Ying
* @email    ylh1986@hotmail.com
* @date     27/06/2015
*
***********************************************************/


#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

#include "handle.h"

/* All interaction from Reactor to an event handler goes 
through function pointers with the following signatures: */
typedef Handle (*getHandleFunc)(void* instance);
typedef void (*handleEventFunc)(void* instance);

typedef struct
{
  void* instance;
  getHandleFunc getHandle;
  handleEventFunc handleEvent;
} EventHandler;

#endif
