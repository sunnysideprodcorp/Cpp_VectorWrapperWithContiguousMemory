#include "VectorMaker.h"

#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stdlib.h>
#include <thread>
#include <memory>
#include <chrono>




int main()
{
  // We can make a factory or just typedef a particular kind of SmallVector
  // The factory includes a method to return a shared_ptr rather than just a SmallVector
  typedef VectorMaker<int,  MAX_BLOCK, MAX_INDIV, 1, ManageMem, ZeroCreator, Chatty> VectorMaker1;
  typedef SmallVector<int,  MAX_BLOCK, MAX_INDIV, 1, ManageMem> SmallVecType;

  auto v1 = VectorMaker1::MakeVector();
  v1.reserve(10);
  SmallVecType v2(15);
  // The pointer arithmetic shows that these vectors have been put in the same memory block
  std::cout << "Different should be 10 and is: " << &v2[1] - &v1[1] << std::endl;

  // Here we want to store another set of vectors in a separate block of memory even if all
  // other characteristics of the desired memory blocks are the same. So we use the int ID
  // template parameter to ensure a separate memory manager identity for this typedef
  typedef VectorMaker<int,  MAX_BLOCK, MAX_INDIV, 2, ManageMem, ZeroCreator, Chatty> VectorMaker2;
  auto v3 = VectorMaker2::MakeVector();
  v3.reserve(20);
  // This vector was put in another memory block, because it has id # 2 rather than id # 1
  std::cout << "This result should be large because these vectors are stored in entirely different memory blocks with separate template parameters " << &v1[1] - &v3[1] << std::endl;

  // We can also see that memory gets recycled, so we are keeping vector contiguity even if vectors are sometimes moved or destroyed:
  v1.reserve(15);
  // Now v1 has been moved to after v2, and v1's original memory pushed into list of available memory for its template class type
  std::cout << "Difference should be 15 and is: " << &v1[1] - &v2[1] << std::endl;
  // The above has left the beginning of the first memory block empty, but it won't stay empty if we put a vector there.
  auto v4 = VectorMaker1::MakeVector();
  v4.reserve(6);
  // We expect that v4 was allocated the memory where v1 used to be, so we should get the same value printed below as just above.
  std::cout << "new vec in old place different should be 10 and is: " << &v2[1] - &v4[1] << std::endl;
  // We can even fit in a tiny vector between v4 and v2 because v4 did not use all reusable space...this will, however, be compiler dependent
  auto v5 = VectorMaker1::MakeVector();
  v5.reserve(3);
  std::cout << "New tiny vector minus older vector is: " << &v2[1] - &v5[1] << " and should be 4" << std::endl;
  std::cout << "Difference between two small vectors is: " << &v5[1] - &v4[1] << " and should be 6" << std::endl;
  std::cout << "new vec should be 15 + 4 and is: " << &v1[1] - &v5[1] << std::endl;

}
 
