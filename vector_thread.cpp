#include "VectorClasses.h"
#include "SmallVector.h"
#include "ArenaMemMgmtPolicies.h"
#include "SmallVectorCreationPolicy.h"
#include "LoggingPolicy.h"

#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stdlib.h>
#include <thread>
#include <memory>
#include <chrono>

template <typename T,  std::size_t MAX_BLOCK, std::size_t MAX_INDIV, int ID, template <class> class MemoryPolicy,  template <class Created> class CreationPolicy,   template <class Created> class LoggingPolicy>
class VectorMaker : public CreationPolicy<SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>, public LoggingPolicy<SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>
{
 public:
  static std::shared_ptr<SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>> MakeVectorPointer(){
    std::cout << "begin do all" << std::endl;
    std::function<void(std::string)> f = VectorMaker::Print;

    auto res = std::make_shared<SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>();
    res->print = f;

    auto& a= ArenaSingleton<arena<MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>::GetInstance();
    a.print = f;
    std::cout << "end do all" << std::endl;
    return res;
  }

  static SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy> MakeVector(){
    std::cout << "begin do all" << std::endl;
    std::function<void(std::string)> f = VectorMaker::Print;

    auto res = SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>();
    res.print = f;

    auto& a= ArenaSingleton<arena<MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>::GetInstance();
    a.print = f;
    std::cout << "end do all" << std::endl;
    return res;
  }
};

typedef VectorMaker<int,  MAX_BLOCK, MAX_INDIV, 1, ManageMem, VariableCreator, Chatty> VariableVectorMaker12;
typedef SmallVector<int,  MAX_BLOCK, MAX_INDIV, 1, ManageMem> SmallVecType;
std::vector<std::shared_ptr<SmallVecType>> globalVecSmart;
std::vector<SmallVecType> globalVec;

void push_it2(int i){
  auto VV = VariableVectorMaker12::MakeVector();
  VV.vec.reserve(i);
  VV.vec.reserve(2*i);
  VV.vec.reserve(3*i);
  globalVec.push_back(VV);
  std::cout << "end " << i << std::endl;
}

void push_it(int i){
  auto VV = VariableVectorMaker12::MakeVectorPointer();
  auto VV1 = VariableVectorMaker12::MakeVectorPointer();
  globalVecSmart.push_back(VV);
  VV->vec.reserve(i);
  VV->vec.reserve(2*i);
  VV->vec.reserve(3*i);
  std::cout << "end " << i << std::endl;
}


int main()
{
  auto VV = VariableVectorMaker12::MakeVectorPointer();
  globalVecSmart.push_back(VV);

  auto VV2 = VariableVectorMaker12::MakeVectorPointer();
  globalVecSmart.push_back(VV2);
  auto VV3 = VariableVectorMaker12::MakeVectorPointer();
  globalVecSmart.push_back(VV3);
  auto VV4 = VariableVectorMaker12::MakeVectorPointer();
  globalVecSmart.push_back(VV4);
  
  //  push_it(1);
  //push_it(2);
  //push_it(3);
  

  push_it2(1);
  push_it2(2);
  push_it2(3);
  /*
  std::thread t1(push_it, 1);
  t1.join();
  std::thread t2(push_it, 2);
  t2.join();
  std::thread t3(push_it, 3);
  t3.join();
  std::thread t4(push_it, 4);
  t4.join();
  std::thread t5(push_it, 1);
  t5.join();
  */
  std::cout << "end FINAL" <<std::endl;

  return 0;
}