//! \file depresolver.h
//! \brief DepResolver definition.
//! \copyright See COPYING and COPYRIGHT files for corresponding information.

#pragma once

#include <iostream>
#include <list>

using namespace std;


//! \class  DepResolver
//! \brief  A dependency resolver
class DepResolver
{

public:


  //! \brief   Add a dependency
  //!
  //! \param   first   the package with dependency
  //! \param   second  the package which \a first depends on
  void addDependency( ssize_t first, ssize_t second );


  //! \brief   Resolve the dependencies
  //!
  //! \param   result  a list which will be filled with resulting
  //!                  indexes in the correct order
  //!
  //! \return  \a true on success, \a false otherwise
  //!          (cyclic dependencies)
  bool resolve( list< ssize_t >& result );


private:


  //! Sort the dependencies
  bool topSort( list< ssize_t >& result );


  //! The list of dependencies pairs, where
  //!   first   the package with dependency
  //!   second  the package which \a first depends on
  list< pair< ssize_t /*first*/, ssize_t /*second*/ > > m_dependencies;


}; // class DepResolver


// vim: sw=2 ts=2 sts=2 et cc=72 tw=70
// End of file.
