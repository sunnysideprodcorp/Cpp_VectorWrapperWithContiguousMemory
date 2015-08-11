#ifndef ARENA_MEM_POLICY_H
#define ARENA_MEM_POLICY_H

#include <cstddef>
#include <memory>
#include "SmallVector.h"
#include <list>
#include <algorithm>
#include <functional>
#include <string>
 
//This should be tuned to your use case
const double MIN_MEMORY_FRACTION = .5;

//ManageMem manages memory both when it is allocated and deallocated
//this means that if a SmallVector is deallocated or moved
//the memory it had been using will go onto a list to be reallocated
//to SmallVectors of the same type
template <typename T>
struct ManageMem
{
  //track memory size and location
  struct PtrVals{
    T* begin;
    std::size_t size;
  };

  //list of available memory locations by size and starting location
  //this list is kept sorted
  std::list<PtrVals> m_freeMem;

  //memory can be used if it's large enough to accommodate a request
  //but not overly large relative to the request size
  const bool canUseMemory(const PtrVals& x, std::size_t n)
  {	return ( (x.size >= n) && ( x.size*MIN_MEMORY_FRACTION < n ) );}     

  //return a suitable memory location if one is found
  //and update list of free memory locations accordingly
  char* findMemory(std::size_t n){      
    auto it = m_freeMem.begin();
    for(; it!= m_freeMem.end(); it++){
      //if acceptable memory is found, return immediately and update
      //list of available memory held by Arena class of this type
      if(canUseMemory(*it, n)){
	char* to_write = it->begin;
	if(it->size > n){
	  it->begin = it->begin + n;
	  it->size  = it->size - n;
	  if( it->size == 0 ) m_freeMem.erase(it);
	  return to_write;
	}
      }
    }  
    return nullptr;     
  }

  //compare memory locations solely on size available at location
  const static bool memoryIsSmaller(const PtrVals& x, const PtrVals& y) const
  { return x.size < y.size; }
 
  void addMemory(char* p, std::size_t n){

    //find appropriate location to insert memory of size n into list of free memory blocks
    auto insert_it = std::upper_bound(m_freeMem.begin(), m_freeMem.end(), ManageMem::memoryIsSmaller);
				   /*
    auto it = m_freeMem.begin();
    for(; it!= m_freeMem.end(); it++){
      if(memoryIsSmaller(*it, pVal)) break;
    }
    if(it != m_freeMem.end()) ++it;
				   */

    //insert newly availably memory into list of free memory blocks
    PtrVals pVal;
    pVal.begin = p;
    pVal.size = n;
    m_freeMem.insert(insert_it, pVal);
  }
};
    
template <typename T>
struct NoManageMem
{

};

#endif  // ARENA_MEM_POLICY_H
