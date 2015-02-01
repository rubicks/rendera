/* rendera/test/pathexe.C */

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Path.H"

int
main( int, char** )
{
  std::cout
    << std::endl
    << __PRETTY_FUNCTION__
    << " at "
    << __FILE__
    << ":"
    << std::dec << __LINE__
    << std::endl
    << "::Path::executable() == "
    << "\""
    << ::Path::executable()
    << "\""
    << std::endl
    ;

  return EXIT_SUCCESS ;
}
