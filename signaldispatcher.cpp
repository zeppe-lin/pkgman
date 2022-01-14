//! \file      signaldispatcher.cpp
//! \brief     SignalDispatcher class implementation
//! \copyright See LICENSE file for copyright and license details.

#include <cstdlib>
#include <iostream>

#include "signaldispatcher.h"

using namespace std;

SignalDispatcher* SignalDispatcher::m_instance = 0;

SignalDispatcher::SignalDispatcher()
{
}

SignalDispatcher* SignalDispatcher::instance()
{
  if ( m_instance == 0 )
    m_instance = new SignalDispatcher();

  return m_instance;
}

void SignalDispatcher::dispatch( int signalNumber )
{
  auto it = instance()->m_signalHandlers.find( signalNumber );
  if ( it != instance()->m_signalHandlers.end() )
    it->second->handleSignal( signalNumber );
  else
    cerr << "pkgman: caught signal " << signalNumber << endl;

  exit( signalNumber );
}

void SignalDispatcher::registerHandler( SignalHandler*  handler,
                                        int        signalNumber )
{
  m_signalHandlers[ signalNumber ] = handler;
}

void SignalDispatcher::unregisterHandler( int signalNumber )
{
  m_signalHandlers.erase( signalNumber );
}

// vim:sw=2:ts=2:sts=2:et:cc=72
// End of file
