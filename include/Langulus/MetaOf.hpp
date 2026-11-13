///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include <Langulus/CT/Retype.hpp>

#include "RTTI/MetaData.hpp"
#include "RTTI/MetaTag.hpp"
#include "RTTI/MetaConst.hpp"
#include "RTTI/MetaVerb.hpp"

#include "RTTI/DefinitionData.inl"
#include "RTTI/DefinitionTag.inl"
#include "RTTI/DefinitionConst.inl"
#include "RTTI/DefinitionVerb.inl"


namespace Langulus
{
   using RTTI::DMeta;
   using RTTI::TMeta;
   using RTTI::VMeta;
   using RTTI::CMeta;

   /// Data definition retrieval.                                             
   /// Some types, like tags/verbs for example, can be represented both as    
   /// DMeta and TMeta/VMeta, and this is useful to state a clear intent.     
   ///   @tparam T type to get data definition from                           
   ///   @return the definition                                               
   template<class T>
   DMeta MetaDataOf() {
      if constexpr (::std::is_void_v<T>)
         return {};
      else {
         using AS = CT::ReflectedAs<Decay<T>>;
         if constexpr (::std::is_void_v<AS> and ::std::is_pointer_v<T>)
            return RTTI::DefinitionData::Reflect<Deref<T>>();
         else {
            static_assert(not ::std::is_void_v<AS>,
               CppNameOf<T>() + " was marked as not reflectable");
            return RTTI::DefinitionData::Reflect<Retype<Deref<T>, AS>>();
         }
      }
   }

   /// Tag definition retrieval.                                              
   /// Some types, like tags for example, can be represented both as DMeta    
   /// and TMeta, and this is useful to state a clear intent.                 
   ///   @tparam T type to get tag definition from                            
   ///   @return the definition                                               
   template<class T>
   TMeta MetaTagOf() {
      return RTTI::DefinitionTag::Reflect<Decay<T>>();
   }

   /// Verb definition retrieval.                                             
   /// Some types, like verbs for example, can be represented both as DMeta   
   /// and VMeta, and this is useful to state a clear intent.                 
   ///   @tparam T type to get verb definition from                           
   ///   @return the definition                                               
   template<class T>
   VMeta MetaVerbOf() {
      return RTTI::DefinitionVerb::Reflect<Decay<T>>();
   }

   /// Constant definition retrieval                                          
   ///   @tparam E constant to get definition from                            
   ///   @return the definition                                               
   template<auto E>
   CMeta MetaConstOf() {
      return RTTI::DefinitionConst::Reflect<E>();
   }

   /// Get the meta definition of a type, deducing whether it's a data, verb, 
   /// or tag. Note: anything can be data, so meta-data is given only if      
   /// not evaluated to be a tag or verb, which might not be desired. Use one 
   /// of the alternatives above to explicitly state your intent.             
   ///   @tparam T type to get meta definition of                             
   ///   @return the meta definition of the provided type                     
   template<class T>
   auto MetaOf() {
      using DT = Decay<T>;
      if constexpr (CT::DefineTag<DT>)
         return MetaTagOf<DT>();
      else if constexpr (CT::DefineVerb<DT>)
         return MetaVerbOf<DT>();
      else
         return MetaDataOf<T>();
   }

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   /// Convenience literals for getting meta definitions from token...        
   LANGULUS(INLINED)
   DMeta operator ""_dmeta(const char* token, size_t size) noexcept {
      return RTTI::Registry::GetMetaDataByToken(Token {token, size});
   }

   LANGULUS(INLINED)
   TMeta operator ""_tmeta(const char* token, size_t size) noexcept {
      return RTTI::Registry::GetMetaTagByToken(Token {token, size});
   }

   LANGULUS(INLINED)
   CMeta operator ""_cmeta(const char* token, size_t size) noexcept {
      return RTTI::Registry::GetMetaConstByToken(Token {token, size});
   }

   LANGULUS(INLINED)
   VMeta operator ""_vmeta(const char* token, size_t size) noexcept {
      return RTTI::Registry::GetMetaVerbByToken(Token {token, size});
   }
   
   /// ... as well as getting them manually                                   
   LANGULUS(INLINED)
   DMeta MetaDataOf(Token const& token) noexcept {
      return RTTI::Registry::GetMetaDataByToken(token);
   }

   LANGULUS(INLINED)
   TMeta MetaTagOf(Token const& token) noexcept {
      return RTTI::Registry::GetMetaTagByToken(token);
   }

   LANGULUS(INLINED)
   VMeta MetaVerbOf(Token const& token) noexcept {
      return RTTI::Registry::GetMetaVerbByToken(token);
   }

   LANGULUS(INLINED)
   CMeta MetaConstOf(Token const& token) noexcept {
      return RTTI::Registry::GetMetaConstByToken(token);
   }
#endif
}
