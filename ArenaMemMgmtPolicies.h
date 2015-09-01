#ifndef ARENA_MEM_POLICY_H
#define ARENA_MEM_POLICY_H

#include "Constants.h"
#include <cstddef>
#include <memory>
#include "SmallVector.h"
#include <list>
#include <algorithm>
#include <functional>
#include <string>
 

// ManageMem manages memory both when it is allocated and deallocated
// this means that if a SmallVector is deallocated or moved
// the memory it had been using will go onto a list to be reallocated
// other SmallVectors of the same type
template <typename T>
struct ManageMem
{
  // Tracks memory size and location
  struct PtrVals{
    T* begin;
    std::size_t size;
  };

  // Tracks available memory locations
  // This list is kept sorted by size of
  // available memory at a given location
  std::list<PtrVals> m_freeMem;
  
  // Memory can be used if it's large enough to accommodate a request
  // but not so large that it shouldn't be saved
  // MIN_MEMORY_FRACTION should be adjusted for individual application performance
  const bool canUseMemory ( const PtrVals& x, std::size_t n ) { return ( (x.size >= n) && ( x.size*MIN_MEMORY_FRACTION < n ) ); }     

  // Compares PtrVal memory sizes 
  const static bool memoryIsSmaller(const PtrVals& x, const PtrVals& y) { return (x.size < y.size); }
   
  // Returns a suitable memory location to reuse if one is found
  // and updates list of free memory locations accordingly
  char* findMemory(std::size_t n){      
    auto it = m_freeMem.begin();
    for (; it!= m_freeMem.end(); it++ ) {
      // If acceptable memory is found, return immediately after updating list      
      if ( canUseMemory(*it, n) ) {
	char* to_write = it->begin;
	if ( it->size > n ) {
	  it->begin = it->begin + n;
	  it->size  = it->size - n;
	  if ( it->size == 0 ) m_freeMem.erase(it);
	  return to_write;
	}
      }
    }  
    return nullptr;     
  }

  // Adds memory to m_freeMem when deallocate is called from SmallVector's associated allocator
  void addMemory(char* p, std::size_t n){
    PtrVals pVal;
    pVal.begin = p;
    pVal.size = n;

    // m_freeMem is kept sorted in ascending order by memory size
    auto insert_it = std::upper_bound(m_freeMem.begin(), m_freeMem.end(), pVal, ManageMem::memoryIsSmaller);			 
    m_freeMem.insert(insert_it, pVal);
  }
};
    
template <typename T>
struct NoManageMem {
  void findMemory(char* p, std::size_t n){ return nullptr; }
  void addMemory(char* p, std::size_t n){ }
};

#endif  // ARENA_MEM_POLICY_H
