///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Assumptions.hpp"


namespace Langulus
{

   ///                                                                        
   ///   A tiny class used as base to referenced types                        
   ///                                                                        
   ///   Provides the interface to be considered CT::Referencable             
   /// The destructor of this type guarantees, that after destruction, the    
   /// member mReferences is zeroed.                                          
   ///                                                                        
   class LANGULUS_API(RTTI) Referenced {
      mutable Count mReferences = 1;

   public:
      LANGULUS(INLINED)
      ~Referenced() {
         LANGULUS_ASSUME(DevAssumes, mReferences <= 1,
            "Leftover references (", mReferences,") on instance destruction. "
            "When inheriting from Referenced, you're supposed to "
            "implement either an appropriate destructor (or surrounding logic) "
            "that makes sure references are reduced down to zero, before "
            "this destructor gets called. This is necessary to make sure "
            "that no leaks happen."
         );

         #if LANGULUS(SAFE)
            if (mReferences == 1) {
               Logger::Warning(
                  "Referenced object destroyed before last "
                  "reference was removed - was it on the stack? "
                  "You can breakpoint here to find out: ", LANGULUS_LOCATION()
               );
            }
         #endif
      }

      LANGULUS(INLINED)
      Count Reference(int x) const IF_UNSAFE(noexcept) {
         LANGULUS_ASSUME(DevAssumes, mReferences or x == 0,
            "Dead instance resurrection/overkill");
         LANGULUS_ASSUME(DevAssumes, x >= 0 or mReferences >= static_cast<Count>(-x),
            "Live instance overkill");
         mReferences += x;
         return mReferences;
      }
   };

} // namespace Langulus