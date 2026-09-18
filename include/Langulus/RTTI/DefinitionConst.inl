///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "DefinitionConst.hpp"
#include <Langulus/CT/Info.hpp>
#include <Langulus/CT/Versioned.hpp>
#include <Langulus/Assume.hpp>

#if not LANGULUS_FEATURE(MANAGED_REFLECTION)
   #include <optional>
#endif

#if 0 or LANGULUS_META_VERBOSITY_MASTER_SWITCH()
   #include <Langulus/Logger.hpp>
   #define VERBOSE(...) Logger::Verbose(__VA_ARGS__)
#else
   #define VERBOSE(...)
#endif


namespace Langulus::RTTI
{
   /// Reflect or return an already reflected constant                        
   ///   @attention when making a shared library and reflecting your enums    
   ///      at library initialization, it is recommended you mark all other   
   ///      relevant instantiations of this function as extern template, to   
   ///      save on a lot of compiler resources:                              
   ///      https://stackoverflow.com/questions/8130602                       
   ///   @tparam E the constant to reflect                                    
   template<auto E>
   auto DefinitionConst::Reflect() -> DefinitionConst const* {
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get an already existing definition - the const might 
         // have been reflected previously in another shared library    
         const auto cppname = CppNameOf<E>();
         DefinitionConst const* meta = Registry::GetMetaConstByCppName(cppname);
         if (meta and meta->IsInRelevantBoundary())
            return meta;

         const auto token = NameOf<E, false>();
         DefinitionConst& definition = meta
            ? const_cast<DefinitionConst&>(*meta)
            : Registry::RegisterConst(cppname, token);
      #else
         // There's no centralized registry when MANAGED_REFLECTION is  
         // disabled, so all we can do is keep a definition on the stack
         // for each translation unit, and rely on hashing and runtime  
         // checks to make sure that definitions are the same           
         static constinit std::optional<DefinitionConst> s_definition;
         if (s_definition.has_value())
            return &s_definition.value();

         const auto cppname = CppNameOf<E>();
         DefinitionConst& definition = s_definition.emplace(cppname);

         definition.mNameOf = Inner::NormalizeAtRuntime(NameOf<E, false>());
         LglsAssert(not definition.mNameOf.empty(),
            "Invalid constant token is not allowed - "
            "you have equipped your constant with an empty CTTI::NamedValue. "
            "The constant in question is: ", cppname
         );
         definition.mNameOf[0] = ToUppercase(definition.mNameOf[0]);
      #endif


      //                                                                
      // If this is reached, then constant is not defined yet           
      // Reflected version                                              
      definition.mVersionMajor = VersionOf<E>().Major;
      definition.mVersionMinor = VersionOf<E>().Minor;

      // Save the boundary at time of reflection, but don't even        
      // bother if it is the main one                                   
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         definition.mBoundaries.insert(LglsBoundary());
      #endif

      // Reflected info                                                 
      definition.mInfoOf = InfoOf<E>();
      
      // Refer to a heap copy of the data                               
      using T = decltype(E);
      definition.mType = DefinitionData::Reflect<T>();
      if (not definition.mData) {
         definition.mData = new T {E};
         LglsAssert(definition.mData, "Insufficient memory on reflection");
         definition.mDestroyConstant = [](const void* p) {
            auto pt = static_cast<const T*>(p);
            delete pt;
         };
      }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         VERBOSE(
            Logger::Yellow, "Constant ", definition.mNameOf,
            " (ID: ", definition.mID, ") ", Logger::Green,
            "registered from ", (Boundary?Boundary:"MAIN")
         );
      #else
         VERBOSE(
            Logger::Yellow, "Constant ", definition.mNameOf,
            Logger::Green, " reflected"
         );
      #endif
      
      return &definition;
   }

   inline DefinitionConst::~DefinitionConst() {
      VERBOSE(
         Logger::Red, "Destroying constant definition: ",
         Logger::Yellow, mNameOf
      );
      if (mData) mDestroyConstant(mData);
   }
}

#undef VERBOSE
