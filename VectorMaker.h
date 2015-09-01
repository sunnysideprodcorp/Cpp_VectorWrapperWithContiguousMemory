#include "SmallVector.h"
#include "VectorClasses.h"
#include "ArenaMemMgmtPolicies.h"
#include "SmallVectorCreationPolicy.h"
#include "LoggingPolicy.h"



template <typename T,  std::size_t MAX_BLOCK, std::size_t MAX_INDIV, int ID, template <class> class MemoryPolicy,  template <class Created> class CreationPolicy,   template <class Created> class LoggingPolicy>
class VectorMaker : public CreationPolicy<SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>, public LoggingPolicy<SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>
{
 public:
  static std::shared_ptr<SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>> MakeVectorPointer(){
    std::function<void(std::string)> f = VectorMaker::Print;

    auto& a= ArenaSingleton<Arena<MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>::GetInstance();
    a.print = f;
    
    auto res = std::make_shared<SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>();
    res->print = f;
    return res;
  }

  static SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy> MakeVector(){
    std::function<void(std::string)> f = VectorMaker::Print;

    auto& a= ArenaSingleton<Arena<MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>>::GetInstance();
    a.print = f;

    auto res = SmallVector<T,  MAX_BLOCK, MAX_INDIV, ID, MemoryPolicy>();
    res.print = f;
    return res;
  }
};
