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

#ifndef REACTOR_H
#define REACTOR_H

#include "EventHandler.h"

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
