///                                                                           
/// Langulus::Core                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include <Langulus/Core.hpp>
#include <Langulus/Assume.hpp>


namespace Langulus::CTTI
{
   /// Can be used in two ways to satisfy CT::Referenced<T>:                  
   /// @attention T has to posses the referencing interface for this to work. 
   ///  - easiest way to achieve this is to simply inherit Referenced.        
   /// 1. Specialize for T/concept                                            
   /// 2. Add a public `using CTTI_Referenced = Yes<>;` in T                  
   template<class T>
   struct Referenced;
}

LANGULUS_CTTI_CONCEPT_DECVQ(Referenced);

namespace Langulus
{
   ///                                                                        
   /// Base types off this one, to make them CT::Referenced and provide the   
   /// required interface for it                                              
   ///                                                                        
   class Referenced {
      mutable int mReferences = 1;

   public:
      using CTTI_Referenced = Yup;

      constexpr ~Referenced() {
         LglsAssumeDev(mReferences <= 1,
            "Leftover references (", mReferences,") on instance destruction. "
            "When inheriting from Referenced, you're supposed to "
            "implement either an appropriate destructor (or surrounding logic) "
            "that makes sure references are reduced down to zero, before "
            "this destructor gets called. This is necessary to make sure "
            "that no leaks happen."
         );
         LglsAssumeDevWarn(mReferences != 1,
            "Referenced object destroyed before last "
            "reference was removed - was it on the stack?"
         );
      }

      constexpr int GetReferences() const noexcept {
         return mReferences;
      }

      constexpr int Reference(int x) const assumptious {
         LglsAssumeDev(mReferences or x == 0,
            "Dead instance resurrection/overkill: adding ",
            x, " references to ", mReferences);
         LglsAssumeDev(x >= 0 or mReferences >= -x,
            "Live instance overkill: adding ",
            x, " references to ", mReferences);
         
         mReferences += x;
         return mReferences;
      }
   };
}
