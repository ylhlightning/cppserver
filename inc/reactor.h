/**********************************************************
* 
* @file reactor.h
* 
* @brief  event reactor declearation.
* 
*
* @author  Linhu Ying
* @email    ylh1986@hotmail.com
* @date     27/06/2015
*
***********************************************************/


#ifndef __REACTOR_H__
#define __REACTOR_H__

#include "event_handler.h"

class Reactor
{
  public:
    void Register(EventHandler* handler);
    void Unregister(EventHandler* handler);
    void HandleEvents(void);
    static Reactor *GetReactorInstance();

    Reactor();
    ~Reactor();

  private:

     static EventHandler* handler;
     static Reactor *m_reactor;
};

#endif
