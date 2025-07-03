//! \file signaldispatcher.h
//! \brief SignalHandler and SignalDispatcher classes definition.
//! \copyright See COPYING and COPYRIGHT files for corresponding information.

#pragma once

#include <map>

using namespace std;


//! \class  SignalHandler
//! \brief  Signal handler for the SignalDispatcher class.
//!
//! Implement this class to receive signals.
class SignalHandler
{

public:


  //! Result of a \a handleSignal() call
  enum HandlerResult
  {
    SIGNAL_NOT_HANDLED, //!< Not handled
    EXIT,               //!< Signal handled, exit now
    CONTINUE            //!< Signal handled, don't exit
  };


  virtual HandlerResult handleSignal( int signalNumber ) = 0;

}; // class SignalHandler


//! \class  SignalDispatcher
//! \brief  Dispatch unix signals
//!
//! Dispatches signals.
//! Singleton, use the instance() method to access the instance
//! of this class.
//!
//! Register your SignalHandler to handle signals.
class SignalDispatcher
{

public:


  //! Create the Signal Dispatcher instance
  static SignalDispatcher* instance();

  // XXX: document
  static void dispatch( int signalNumber );

  // XXX: document
  void registerHandler( SignalHandler* handler, int signalNumber );

  // XXX: document
  void unregisterHandler( int signalNumber );

protected:

  // XXX: document
  SignalDispatcher();

private:

  // XXX: document
  static SignalDispatcher* m_instance;

  // XXX: document
  map< int, SignalHandler* > m_signalHandlers;


}; // class SignalDispatcher


// vim: sw=2 ts=2 sts=2 et cc=72 tw=70
// End of file.
