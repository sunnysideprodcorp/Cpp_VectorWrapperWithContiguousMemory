#ifndef SMALL_VECTOR_CREATION_H
#define SMALL_VECTOR_CREATION_H

#include "LoggingPolicy.h"

//Create a vector with the MAX_SIZE reserved from initialization
//Better default when users have mixed-use cases or are unsure
//How/when they will resize or size vectors
template <class U>
struct OpMaxSizeCreator
{
  static U Create()
  {
    U to_return(MAX_SIZE);
    return to_return;
  }

  static std::size_t Size(){
    return MAX_SIZE;
  }
};

//Create a vector with 0 memory reserved from initialization
//Better in cases where users know exactly how many elements they want to reserve
//And will do so themselves immediately after initialization
template <class U>
struct VariableCreator
{
  static U Create()
  {
    U to_return(0);
    return to_return;
  }

  static std::size_t Size(){
    return 0;
  }
};


  #endif  //   SMALL_VECTOR_CREATION_H
