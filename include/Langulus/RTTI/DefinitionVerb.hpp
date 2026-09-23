///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Definition.hpp"
#include <unordered_set>


namespace Langulus::RTTI
{  
   ///                                                                        
   /// A verb definition                                                      
   ///                                                                        
   class DefinitionVerb final : public Inner::Definition {
   protected:
      friend class Registry;
      friend class DefinitionData;
      friend struct Inner::MetaVerbNaked;
      template<unsigned> friend struct Inner::MetaVerbStructured_X8;

      // Verbs have antonyms, denoted via this 'negative' token         
      // For example, 'destroy' is the reverse of 'create'              
      // This is just syntax sugar - reverse token just does mass *= -1 
      ::std::string mNameOfReverse;

      // Verbs can be tokenized as operators - just syntax sugar        
      ::std::string mOperator;
      ::std::string mOperatorReverse;

      // Operators with stripped spaces around them                     
      Token mOperatorStripped;
      Token mOperatorReverseStripped;

      // Verb's reflected precedence                                    
      Real mPrecedence IF_SAFE(= 0);

      // A set of data types that are capable of doing the verb         
      using AbleList = ::std::unordered_set<DefinitionData const*>;
      AbleList mAble;

      
      //                                                                
      //    These methods are sought in each reflected verb             
      //                                                                
      //    These function pointers will be different for different     
      // libraries. We just collect them all. If a shared object is     
      // unloaded, we simply pick a pointer from another. Once the verb 
      // is reflected from the MainBoundary, the maps are cleared and   
      // only the main code is used, because it is most persistent.     
      using FContextless = bool (*)(Annies::Verb&);

      struct BoundaryDependent {
         // Reflected stateless verb, if available                      
         FContextless mContextless  = nullptr;
      };

      // The currently used boundary                                    
      BoundaryDependent mCurrentBoundary;
      
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // All functions, reflected from all points of view            
         // If this map is empty, then data has been reflected from the 
         // main boundary                                               
         ::std::unordered_map<::std::string, BoundaryDependent> mOtherBoundaries;
      #endif

   public:
      using CTTI_ReflectAs = void;

      static constexpr Token InvalidName = "noverb";

      template<CT::Decayed>
      static auto Reflect() -> DefinitionVerb const*;
      
      DefinitionVerb(const Token& cppname) noexcept
         : Definition {cppname} {}

      ~DefinitionVerb();
   };
}

#include "DefinitionVerb.inl"
