#include <memory>
#include "SmallVector.h"
#include <list>
#include <algorithm>
#include <functional>

template <typename U, typename T>
struct ManageMem
{
  std::weak_ptr<U> currentVec;
  std::weak_ptr<U> priorVec;
  std::list<PtrVasls> listFree;

  class PtrVals{
    int buffer;
    T* begin;
    T* end;
    size_t size;
  };

  struct memory_is_smaller{
    bool operator()(const PtrVals& x, const PtrVals& y) const
    {
      return x.size < y.size;
    }
  }

  struct memory_is_better{
    bool operator()(const PtrVals& x, const PtrVals& y) const
    {
      return (x.buffer < y.buffer || ((x.buffer == y.buffer) && (x.size > y.size)));
    }     
  }

  struct can_use_pushback{
    bool operator()(const PtrVals& x, std::size_t n) const
    {
      return x.size > n && s.size/2 < n;
    }     
  }

  struct can_use_block{
    bool operator()(const PtrVals& x, std::size_t n) const
    {
      return x.size > n && x.size/4 < n;
    }     
  }
    
    struct update{
      void operator()(PtrVals*p, T*begin){
	for(  std::list<PtrVasls>::iterator it = listFree.begin(); it!= listFree.end(); it++){
	  if(can_use_pusback(it->size, n))){
	  break;
	}	
	if(it!=listFree.end()){

	  
	  
	  
	}
	
	
      }
      
    }
    
  Ptrvals* findM(std::size_t n, listFree){
    for(  std::list<PtrVasls>::iterator it = listFree.begin(); it!= listFree.end(); it++){
      if(can_use_pusback(it->size, n))){
       break;
    }
    if(it!=listFree.end()){
      return *it;
    }
    else{
      return nullptr;
    }
  }
}
				
  
  void clear(Ptrvals* to_remove){
    listVecs.remove(to_remove);
  }
  
};

template <typename U>
struct NoManageMem
{
};

