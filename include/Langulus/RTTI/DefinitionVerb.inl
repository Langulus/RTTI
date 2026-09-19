///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "DefinitionVerb.hpp"
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
   /// Reflect or return an already reflected verb                            
   ///   @attention when making a shared library and reflecting your verbs    
   ///      at library initialization, it is recommended you mark all other   
   ///      relevant instantiations of this function as extern template, to   
   ///      save on a lot of compiler resources:                              
   ///      https://stackoverflow.com/questions/8130602                       
   ///   @tparam T the decayed verb to reflect                                
   template<CT::Decayed T>
   auto DefinitionVerb::Reflect() -> DefinitionVerb const* {
      static_assert(CT::Complete<T>,
         "Can't reflect incomplete verb - "
         "make sure you have included the corresponding headers "
         "before the point of reflection. "
         "This could also be triggered due to an incomplete member in T");
      static_assert(CT::Reflectable<T>,
         "Can't reflect verb that was explicitly marked unreflectable");
      static_assert(CT::DefineVerb<T>,
         "Type is not reflected as a verb definition");
      static_assert(not CT::DefineTag<T>,
         "Can't reflect a tag as a verb");
      static_assert(not ::std::is_function_v<T>,
         "Can't reflect this function signature as a verb");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get an already existing definition - the verb might  
         // have been reflected previously in another shared library    
         const auto cppname = CppNameOf<T>();
         DefinitionVerb const* meta = Registry::GetMetaVerbByCppName(cppname);
         if (meta and meta->IsInRelevantBoundary())
            return meta;
         
         const auto verbPos = NameOfVerb<T>();
         const auto verbNeg = NameOfVerbReverse<T>();
         const auto opPos   = OperatorOfVerb<T>();
         const auto opNeg   = OperatorOfVerbReverse<T>();
         DefinitionVerb& definition = meta
            ? const_cast<DefinitionVerb&>(*meta)
            : Registry::RegisterVerb(cppname, verbPos, verbNeg, opPos, opNeg);
      #else
         // There's no centralized registry when MANAGED_REFLECTION is  
         // disabled, so all we can do is keep a definition on the stack
         // for each translation unit, and rely on runtime checks to    
         // make sure that definitions match between those.             
         static constinit std::optional<DefinitionVerb> s_definition;
         if (s_definition.has_value())
            return &s_definition.value();

         const auto cppname = CppNameOf<T>();
         DefinitionVerb& definition = s_definition.emplace(cppname);

         definition.mNameOf = Inner::ToLowercase(NameOfVerb<T>());
         LglsAssert(not definition.mNameOf.empty(),
            "Invalid verb token is not allowed - "
            "you have equipped your verb (or its base) with an empty CTTI_DefineVerb. "
            "The verb in question is: ", cppname);

         definition.mNameOfReverse = Inner::ToLowercase(NameOfVerbReverse<T>());
         LglsAssert(definition.mNameOf != definition.mNameOfReverse,
            "Verb can't have the same positive and negative tokens for: ", cppname);

         definition.mOperator = Inner::ToLowercase(OperatorOfVerb<T>());
         definition.mOperatorReverse = Inner::ToLowercase(OperatorOfVerbReverse<T>());
         LglsAssert(definition.mOperator != definition.mOperatorReverse or definition.mOperator.empty(),
            "Verb can't have the same positive and negative operators for: ", cppname);
         LglsAssert(IsASCII(definition.mOperator),
            "Verb positive operator isn't ASCII for: ", cppname);
         LglsAssert(IsASCII(definition.mOperatorReverse),
            "Verb reverse operator isn't ASCII for: ", cppname);

         definition.mOperatorStripped = Inner::StripSpaces(definition.mOperator);
         definition.mOperatorReverseStripped = Inner::StripSpaces(definition.mOperatorReverse);
      #endif

      //                                                                
      // If this is reached, then verb is not defined yet               
      definition.ReflectCommon<T>();

      if constexpr (CT::Complete<CTTI::DefineVerb<T>>)
         definition.mPrecedence = CTTI::DefineVerb<T>::Precedence;
      else
         definition.mPrecedence = T::CTTI_DefineVerb::Precedence;

      if constexpr (requires { FContextless {&T::ExecuteContextless}; })
         definition.mCurrentBoundary.mContextless = &T::ExecuteContextless;

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         if (definition.mOperator.size()) {
            VERBOSE(
               Logger::DarkGreen, "Operator ",
               Inner::IsolateOperator(definition.mOperator),
               " (ID: ", definition.mID, ") ", Logger::Green,
               "registered from ", (Boundary?Boundary:"MAIN")
            );
         }

         if (not definition.mOperatorReverse.empty()) {
            VERBOSE(
               Logger::DarkGreen, "Operator ", 
               Inner::IsolateOperator(definition.mOperatorReverse),
               " (ID: ", definition.mID, ") ", Logger::Green,
               "registered from ", (Boundary?Boundary:"MAIN")
            );
         }

         if (definition.mNameOfReverse.empty()) {
            VERBOSE(
               Logger::DarkGreen, "Verb ", definition.mNameOf,
               " (ID: ", definition.mID, ") ", Logger::Green,
               "registered from ", (Boundary?Boundary:"MAIN")
            );
         }
         else {
            VERBOSE(
               Logger::DarkGreen, "Verb ", definition.mNameOf, "/", definition.mNameOfReverse,
               " (ID: ", definition.mID, ") ", Logger::Green,
               " registered from ", (Boundary?Boundary:"MAIN")
            );
         }
      #else
         if (definition.mOperator.size()) {
            VERBOSE(
               Logger::DarkGreen, "Operator ", Inner::IsolateOperator(definition.mOperator),
               Logger::Green, " registered from ", (Boundary?Boundary:"MAIN")
            );
         }

         if (not definition.mOperatorReverse.empty()) {
            VERBOSE(
               Logger::DarkGreen, "Operator ", Inner::IsolateOperator(definition.mOperatorReverse),
               Logger::Green, " registered from ", (Boundary?Boundary:"MAIN")
            );
         }

         if (definition.mNameOfReverse.empty()) {
            VERBOSE(
               Logger::DarkGreen, "Verb ", definition.mNameOf, Logger::Green,
               " registered from ", (Boundary?Boundary:"MAIN")
            );
         }
         else {
            VERBOSE(
               Logger::DarkGreen, "Verb ", definition.mNameOf, "/", definition.mNameOfReverse,
               Logger::Green, " registered from ", (Boundary?Boundary:"MAIN")
            );
         }
      #endif
      
      return &definition;
   }

   inline DefinitionVerb::~DefinitionVerb() {
      VERBOSE(Logger::Red, "Destroying verb definition: ", Logger::DarkGreen, mNameOf);
   }
}

#undef VERBOSE
