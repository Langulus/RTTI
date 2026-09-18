///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Export.hpp"
#include <string>

#if not LANGULUS_FEATURE(MANAGED_REFLECTION)
   #error "This file shouldn't be included if MANAGED_REFLECTION is disabled"
#endif

namespace Langulus::RTTI::Inner
{
   /// Helper function that returns a pointer to a function that is           
   /// guaranteed to be defined in the current translation unit & module.     
   /// It can be used together with `dladdr` to detect the current boundary.  
   LANGULUS(ALWAYS_INLINED)
   static void const* GetBoundaryIdentity() {
      return reinterpret_cast<void const*>(&GetBoundaryIdentity);
   }

   /// Get the name of the shared object/executable that contains the pointer 
   /// at runtime. OS-dependent. Shared objects may be unloaded at runtime.   
   LANGULUS_API(RTTI) LANGULUS(NOINLINE)
   auto GetBoundaryOf(void const*) -> ::std::string;

   /// Get the name of the main module (the one that loads shared objects)    
   /// The main module can't ever be unloaded from memory, unless the         
   /// program closes.                                                        
   LANGULUS_API(RTTI) LANGULUS(NOINLINE)
   auto GetMainBoundary() -> ::std::string;
}

/// Get the name of the shared object/executable that contains the pointer    
/// at runtime. OS-dependent.                                                 
#define LglsBoundary() ::Langulus::RTTI::Inner::GetBoundaryOf(::Langulus::RTTI::Inner::GetBoundaryIdentity())
#define LglsMainBoundary() ::Langulus::RTTI::Inner::GetMainBoundary()