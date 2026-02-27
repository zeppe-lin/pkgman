//! \file signaldispatcher.cpp
//! \brief SignalDispatcher class implementation.
//! \copyright See COPYING for license terms and COPYRIGHT for notices.

#include <cstdlib>
#include <iostream>

#include "signaldispatcher.h"

using namespace std;

SignalDispatcher* SignalDispatcher::m_instance = 0;

SignalDispatcher::SignalDispatcher()
{
}

SignalDispatcher*
SignalDispatcher::instance()
{
  if ( m_instance == 0 )
    m_instance = new SignalDispatcher();

  return m_instance;
}

void
SignalDispatcher::dispatch( int signalNumber )
{
  auto it = instance()->m_signalHandlers.find( signalNumber );
  if ( it != instance()->m_signalHandlers.end() )
  {
    auto res = it->second->handleSignal(signalNumber);
    if (res == SignalHandler::CONTINUE)
      return;
  }
  else
    cerr << "pkgman: caught signal " << signalNumber << endl;

  // Use 128 + signalNumber (conventional shell semantics).
  exit( 128 + signalNumber );
}

void
SignalDispatcher::registerHandler( SignalHandler*  handler,
                                   int             signalNumber )
{
  m_signalHandlers[ signalNumber ] = handler;
}

void
SignalDispatcher::unregisterHandler( int signalNumber )
{
  m_signalHandlers.erase( signalNumber );
}

// vim: sw=2 ts=2 sts=2 et cc=72 tw=70
// End of file.
