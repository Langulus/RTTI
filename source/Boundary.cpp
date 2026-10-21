///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/RTTI/Boundary.hpp>
#include <Langulus/Assume.hpp>

#if not LANGULUS_FEATURE(MANAGED_REFLECTION)
   #error "This file shouldn't be built if MANAGED_REFLECTION is disabled"
#endif

#if LANGULUS_OS(WINDOWS)
   #include <Windows.h>
#else
   #include <dlfcn.h>
#endif


/// Get the name of the shared object/executable that contains the pointer    
/// at runtime. OS-dependent. Shared objects may be unloaded at runtime.      
auto Langulus::RTTI::GetBoundaryOf(void const* addr) -> ::std::string {
   #if LANGULUS_OS(WINDOWS)
      HMODULE dli_fbase = NULL;
      LglsAssert(GetModuleHandleEx(
         GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
         (LPCTSTR)addr, 
         &dli_fbase
      ));
   
      char dli_fname[MAX_PATH];
      LglsAssert(GetModuleFileName(dli_fbase, dli_fname, sizeof(dli_fname)));
      return dli_fname;
   #else
      Dl_info info;
      LglsAssert(dladdr(addr, &info),
         "Can't establish boundary");
      return info.dli_fname;
   #endif
}

/// Get the name of the main module (the one that loads shared objects)       
/// The main module can't ever be unloaded from memory, unless the            
/// program closes.                                                           
auto Langulus::RTTI::GetMainBoundary() -> ::std::string {
   LglsDisableWarningPush
   LglsDisableWarning_MicrosoftCast
      return GetBoundaryOf(static_cast<void const*>(&GetMainBoundary));
   LglsDisableWarningPop
}

