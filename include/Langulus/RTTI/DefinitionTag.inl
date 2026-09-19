///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "DefinitionTag.hpp"
#include <Langulus/CT/ReflectAs.hpp>
#include <Langulus/CT/DefineTag.hpp>
#include <Langulus/CT/DefineVerb.hpp>

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
   /// Reflect or return an already reflected tag                             
   ///   @attention when making a shared library and reflecting your tags     
   ///      at library initialization, it is recommended you mark all other   
   ///      relevant instantiations of this function as extern template, to   
   ///      save on a lot of compiler resources:                              
   ///      https://stackoverflow.com/questions/8130602                       
   ///   @tparam T the decayed trait to reflect                               
   template<CT::Decayed T>
   auto DefinitionTag::Reflect() -> DefinitionTag const* {
      static_assert(CT::Complete<T>,
         "Can't reflect incomplete tag - "
         "make sure you have included the corresponding headers "
         "before the point of reflection. "
         "This could also be triggered due to an incomplete member in T");
      static_assert(CT::Reflectable<T>,
         "Can't reflect tag that was explicitly marked unreflectable");
      static_assert(CT::DefineTag<T>,
         "Type is not reflected as a tag definition");
      static_assert(not CT::DefineVerb<T>,
         "Can't reflect a verb as a tag");
      static_assert(not ::std::is_function_v<T>,
         "Can't reflect this function signature as a tag");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get an already existing definition - the tag might   
         // have been reflected previously in another shared library    
         const auto cppname = CppNameOf<T>();
         DefinitionTag const* meta = Registry::GetMetaTagByCppName(cppname);
         if (meta and meta->IsInRelevantBoundary())
            return meta;

         const auto token = NameOfTag<T>;
         DefinitionTag& definition = meta
            ? const_cast<DefinitionTag&>(*meta)
            : Registry::RegisterTag(cppname, token);
      #else
         // There's no centralized registry when MANAGED_REFLECTION is  
         // disabled, so all we can do is keep a definition on the stack
         // for each translation unit, and rely on runtime checks to    
         // make sure that definitions match between those.             
         static constinit std::optional<DefinitionTag> s_definition;
         if (s_definition.has_value())
            return &s_definition.value();

         const auto cppname = CppNameOf<T>();
         DefinitionTag& definition = s_definition.emplace(cppname);

         definition.mNameOf = Inner::ToLowercase(NameOfTag<T>);
         LglsAssert(not definition.mNameOf.empty(),
            "Invalid tag token is not allowed - "
            "you have equipped your type (or its base) with an empty CTTI_DefineTag. "
            "The type in question is: ", cppname
         );
      #endif


      //                                                                
      // If this is reached, then tag is not defined yet                
      definition.ReflectCommon<T>();

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         VERBOSE(
            Logger::Purple, "Tag ", definition.mNameOf,
            " (ID: ", definition.mID, ") ", Logger::Green,
            "registered from ", (Boundary?Boundary:"MAIN")
         );
      #else
         VERBOSE(
            Logger::Purple, "Tag ", definition.mNameOf,
            Logger::Green, " reflected"
         );
      #endif

      return &definition;
   }

   inline DefinitionTag::~DefinitionTag() {
      VERBOSE(Logger::Red, "Destroying tag definition: ", Logger::Purple, mNameOf);
   }
}

#undef VERBOSE
