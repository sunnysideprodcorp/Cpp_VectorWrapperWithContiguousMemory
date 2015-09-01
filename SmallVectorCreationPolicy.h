#ifndef SMALL_VECTOR_CREATION_H
#define SMALL_VECTOR_CREATION_H

#include "LoggingPolicy.h"

// Creates a vector with MAX_SIZE reserved
// This is a good default particularly when users expect to add to
// SmallVectors at several points in time rather than a single initialization
template <class U>
struct SizedCreator
{
  static U Create ()
  {
    U to_return(MAX_SIZE);
    return to_return;
  }

  static std::size_t Size () { return MAX_SIZE; }
};

// Creates a vector with 0 memory reserved from initialization
// This is the better option when users know exactly how much
// space to reserve for each SmallVector used and do not anticipate
// many subsequent modifications
template <class U>
struct ZeroCreator
{
  static U Create ()
  {
    U to_return(0);
    return to_return;
  }

  static std::size_t Size () { return 0; }
};


  #endif  //   SMALL_VECTOR_CREATION_H
