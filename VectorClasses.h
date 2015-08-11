#ifndef SHORT_ALLOC_H
#define SHORT_ALLOC_H
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

const int MAX_SIZE = 10;
const int NUM_ELEMENTS = 4;
constexpr int MAX_INDIV = sizeof(int)*MAX_SIZE;
constexpr int MAX_BLOCK = NUM_ELEMENTS*MAX_INDIV;
std::mutex myMutex;

template <std::size_t N, std::size_t MAX_SIZE, int ID, template <class Created> class MemPolicy>
  class arena : public MemPolicy<char>
{
  static const std::size_t alignment = 16;
  char* buf_ = new char[N];
  char* ptr_;
  std::vector<char*> buffers;
    
  bool
    pointer_in_buffer(char* p) noexcept
  {
    bool to_return =  buf_ <= p && p <= buf_ + N;
    return to_return;
  }

 public:
  std::function<void(std::string)> print;
  arena() noexcept : ptr_(buf_) {
    buffers.push_back(buf_);
  }
    
  ~arena() {
    std::cout << "running arena destructor" << std::endl;
    ptr_ = nullptr;
    for(auto buffer: buffers){
      delete buffer;
    }
            
  }

  arena(const arena&) = delete;
  arena& operator=(const arena&) = delete;

  char* allocate(std::size_t n);
  void deallocate(char* p, std::size_t n) noexcept;

  static constexpr std::size_t size() {return N;}
  std::size_t used() const {return static_cast<std::size_t>(ptr_ - buf_);}
  void reset() {ptr_ = buf_;}
};


template <typename T>
class ArenaSingleton
{
 public:
  static T& GetInstance(){
    static T s_instance;
    std::cout << "running singleton get instance" << std::endl;
    return s_instance;
  }

  
};

template <std::size_t N, std::size_t MAX_SIZE, int ID, template <class Created> class MemPolicy>
  char*
  arena<N, MAX_SIZE, ID, MemPolicy>::allocate(std::size_t n)
{
  //  std::cout << "1. CURRENT BUFFER IS " << (void *) buf_ << std::endl;
  //std::cout << "2. CURRENT pointer IS " << (void *) ptr_ << std::endl;
  std::cout << "3. !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!there is this much space left in buffer before allocating current request " << (buf_ + N - ptr_)/4 << std::endl; 

  char* availableSlot = MemPolicy<char>::findMemory(n);
  if(availableSlot){
    std::cout << "using existing memory so # should not go down" << std::endl;
    return availableSlot;
  }
  
  if ((buf_ + N - ptr_) >= n)
    {
      char* r = ptr_;
      ptr_ += n;
      std::cout << "5. there is this much space left now " << (buf_ + N - ptr_)/4 << std::endl; 
      std::thread::id this_id = std::this_thread::get_id();
      std::cout << "ending with " << this_id << std::endl;
      return r;
    }
  else
    {
      std::string to_use = "MAKING A NEW BUFFER  " + std::to_string(n/4) + " was too big for " + std::to_string((buf_ + N - ptr_)/4);
      print(to_use);

      MemPolicy<char>::addMemory(ptr_, (buf_ + N - ptr_ ));
      buf_ = new char[N];
      ptr_ = buf_;
      buffers.push_back(buf_);

      char *r = ptr_;
      ptr_ += n;
      std::cout << "5. there is this much space left now " << (buf_ + N - ptr_)/4 << std::endl; 
      std::thread::id this_id = std::this_thread::get_id();
      std::cout << "ending with " << this_id << std::endl;
      return r;
    }

  return static_cast<char*>(::operator new(n));
  
}

template <std::size_t N, std::size_t MAX_SIZE, int ID, template <class Created> class MemPolicy>
  void
  arena<N, MAX_SIZE, ID, MemPolicy>::deallocate(char* p, std::size_t n) noexcept
{
  std::cout << "running deallocate" << std::endl;
  MemPolicy<char>::addMemory(p, n);
  if (pointer_in_buffer(p))
    {
      if (p + n == ptr_)
	ptr_ = p;
    } 
}

template <class T, std::size_t N, std::size_t MAX_SIZE, int ID, template <class Created> class MemPolicy>
  class short_alloc
{
  arena<N, MAX_SIZE, ID, MemPolicy>& a_;
 public:
  typedef T value_type;
  template <class _Up> struct rebind {typedef short_alloc<_Up, N, MAX_SIZE, ID, MemPolicy> other;};
  short_alloc(arena<N, MAX_SIZE, ID, MemPolicy>& a) noexcept : a_(a) {}

  template <class U>
    short_alloc(const short_alloc<U, N, MAX_SIZE, ID, MemPolicy>& a) noexcept
    : a_(a.a_) {}
  short_alloc(const short_alloc&) = default;
  short_alloc& operator=(const short_alloc&) = delete;

  T* allocate(std::size_t n)
  {
    std::lock_guard<std::mutex> guard(myMutex);
    return reinterpret_cast<T*>(a_.allocate(n*sizeof(T)));

  }
  void deallocate(T* p, std::size_t n) noexcept
  {

    std::lock_guard<std::mutex> guard(myMutex);
    a_.deallocate(reinterpret_cast<char*>(p), n*sizeof(T));

  }
};


#endif  // SHORT_ALLOC_H



