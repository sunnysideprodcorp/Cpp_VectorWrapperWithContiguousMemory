#ifndef SHORT_ALLOC_H
#define SHORT_ALLOC_H

#include "Constants.h"

#include <string>
#include <cstddef>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>
#include <thread>
#include <mutex>

// This code is built on top of Howard Hinnant's stack_alloc class
// https://howardhinnant.github.io/stack_alloc.html

// This mutex is used to control access to memory blocks held by 
std::mutex memoryMutex;

// The Arena class tracks and allocates blocks of memory for custom allocator ShortAlloc
// N and MAX_SIZE are parameters that do not at present enforce rules limiting memory use, but
// could be put to that purpose. MemPolicy determines whether memory will be reused rather than
// simply allocated in contiguous blocks that are just discarded when deallocated
template <std::size_t N, std::size_t MAX_SIZE, int ID, template <class Created> class MemPolicy>
  class Arena : public MemPolicy<char>
{
 public:
  std::function<void(std::string)> print;
  std::mutex m_memoryMutex;
  
  Arena() noexcept : ptr_(buf_) { buffers.push_back(buf_); }
    
  ~Arena() {
    ptr_ = nullptr;
    for(auto buffer: buffers){
      delete buffer;
    }           
  }

  // Arena is managed as a Singleton class, so some default functions are inappropriate 
  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;

  // These are the functions used by the custom allocator to dole out memory
  char* allocate(std::size_t n);
  void deallocate(char* p, std::size_t n) noexcept;

  // These functions allow users to get explicit info about a memory block
  // and minimally manipulate access tot he memory block
  static constexpr std::size_t size() {return N;}
  std::size_t used() const {return static_cast<std::size_t>(ptr_ - buf_);}
  void reset() {ptr_ = buf_;}

 private:
  // Variables to track managed memory
  static const std::size_t alignment = 16;
  char* buf_ = new char[N];
  char* ptr_;
  std::vector<char*> buffers;

  // Safety method to checked returned memory locations
  bool pointer_in_buffer(char* p) noexcept
  {
    bool to_return =  buf_ <= p && p <= buf_ + N;
    return to_return;
  }
};


// Allocates memory in a managed memory block 
template <std::size_t N, std::size_t MAX_SIZE, int ID, template <class Created> class MemPolicy>
  char*
  Arena<N, MAX_SIZE, ID, MemPolicy>::allocate(std::size_t n)
{
  char* availableSlot = MemPolicy<char>::findMemory(n);
  if ( availableSlot ) { return availableSlot; } // Reuse memory if available
  
  if ( (buf_ + N - ptr_) >= n ) {     // Use memory from current buffer if large enough
      char* r = ptr_;
      ptr_ += n;
      return r;
  }  else {                           // Create new buffer if existing buffer not large enough, move old buffer to list of available memory
      MemPolicy<char>::addMemory(ptr_, (buf_ + N - ptr_ ));
      buf_ = new char[N];
      ptr_ = buf_;
      buffers.push_back(buf_);

      char *r = ptr_;
      ptr_ += n;
      return r;
    }
}

// Deallocates memory originally in one of the class's managed memory buffers
// Returns this memory to the list of available memory, so it can be reused
// Given appropriate memory type
template <std::size_t N, std::size_t MAX_SIZE, int ID, template <class Created> class MemPolicy>
  void
  Arena<N, MAX_SIZE, ID, MemPolicy>::deallocate(char* p, std::size_t n) noexcept
{

  MemPolicy<char>::addMemory(p, n);
  if (pointer_in_buffer(p))
    {
      if (p + n == ptr_)
	ptr_ = p;
    } 
}



// So long as Arenas are made via ArenaSingleton, only one Arena for each
// set of template parameters will be made. This ensures all SmallVectors
// with same template parameters are stored in a contiguous memory location
// The int ID template parameter is specifically to ensure that many singletons
// identical except for identity can be made if desired
template <typename T>
class ArenaSingleton
{
 public:
  static T& GetInstance(){
    static T s_instance;
    return s_instance;
  }  
};

template <class T, std::size_t N, std::size_t MAX_SIZE, int ID, template <class Created> class MemPolicy>
  class ShortAlloc
{
 public:
  typedef T value_type;

  template <class _Up> struct rebind {typedef ShortAlloc<_Up, N, MAX_SIZE, ID, MemPolicy> other;};
  ShortAlloc(Arena<N, MAX_SIZE, ID, MemPolicy>& a) noexcept : a_(a) {}

  template <class U>
    ShortAlloc(const ShortAlloc<U, N, MAX_SIZE, ID, MemPolicy>& a) noexcept
    : a_(a.a_) {}

  ShortAlloc(const ShortAlloc&) = default;
  ShortAlloc& operator=(const ShortAlloc&) = default; //delete;

  // The custom allocator's allocate method in turn calls the hidden Arena's allocate method
  T* allocate(std::size_t n)
  {
    std::lock_guard<std::mutex> guard(a_.m_memoryMutex);
    return reinterpret_cast<T*>(a_.allocate(n*sizeof(T)));
  }

  // The custom allocator's deallocate method in turn calls the hidden Arena's deallocate method
  void deallocate(T* p, std::size_t n) noexcept
  {
    std::lock_guard<std::mutex> guard(a_.m_memoryMutex);
    a_.deallocate(reinterpret_cast<char*>(p), n*sizeof(T));
  }

 private:
  Arena<N, MAX_SIZE, ID, MemPolicy>& a_;
};


#endif  // SHORT_ALLOC_H



