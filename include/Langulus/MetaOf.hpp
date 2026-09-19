///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include <Langulus/CT/ReflectAs.hpp>
#include "RTTI/MetaData.hpp"
#include "RTTI/MetaTag.hpp"
#include "RTTI/MetaConst.hpp"
#include "RTTI/MetaVerb.hpp"


namespace Langulus
{
   /// Get the meta definition of a type, deducing whether it's a data, verb, 
   /// or tag. Note: anything can be data, so meta-data is given only if      
   /// not evaluated to be a tag or verb, which might not be desired. Use one 
   /// of the alternatives below to explicitly state your intent.             
   ///   @tparam T type to get meta definition of                             
   ///   @return the meta definition of the provided type                     
   template<class T>
   auto MetaOf() {
      using DT = Decay<T>;
      if constexpr (CT::DefineTag<DT>)
         return RTTI::TMeta {RTTI::DefinitionTag::Reflect<DT>()};
      else if constexpr (CT::DefineVerb<DT>)
         return RTTI::VMeta {RTTI::DefinitionVerb::Reflect<DT>()};
      else
         return RTTI::DMeta {RTTI::DefinitionData::Reflect<Deref<T>>()};
   }

   /// Get the meta definition of a constant, like an enum                    
   ///   @tparam E constant to get meta definition of                         
   ///   @return the meta definition of the provided constant                 
   template<auto E>
   RTTI::CMeta MetaOf() {
      return RTTI::DefinitionConst::Reflect<E>();
   }

   /// Data definition retrieval.                                             
   /// Some types, like tags/verbs for example, can be represented both as    
   /// DMeta and TMeta/VMeta, and this is useful to state a clear intent.     
   ///   @tparam T type to get data definition from                           
   ///   @return the definition                                               
   template<class T>
   RTTI::DMeta MetaDataOf() {
      return RTTI::DefinitionData::Reflect<CT::ReflectedAs<ShedDeref<T>>>();
   }

   /// Tag definition retrieval.                                              
   /// Some types, like tags for example, can be represented both as DMeta    
   /// and TMeta, and this is useful to state a clear intent.                 
   ///   @tparam T type to get tag definition from                            
   ///   @return the definition                                               
   template<class T>
   RTTI::TMeta MetaTagOf() {
      return RTTI::DefinitionTag::Reflect<Decay<T>>();
   }

   /// Verb definition retrieval.                                             
   /// Some types, like verbs for example, can be represented both as DMeta   
   /// and VMeta, and this is useful to state a clear intent.                 
   ///   @tparam T type to get verb definition from                           
   ///   @return the definition                                               
   template<class T>
   RTTI::VMeta MetaVerbOf() {
      return RTTI::DefinitionVerb::Reflect<Decay<T>>();
   }

   /// Constant definition retrieval                                          
   ///   @tparam E constant to get definition from                            
   ///   @return the definition                                               
   template<auto E>
   RTTI::CMeta MetaConstOf() {
      return RTTI::DefinitionConst::Reflect<E>();
   }

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   /// Convenience literals for getting meta definitions from token...        
   LANGULUS(INLINED)
   RTTI::DMeta operator ""_dmeta(const char* token, size_t size) noexcept {
      return RTTI::Registry::GetMetaDataByToken(Token {token, size});
   }

   LANGULUS(INLINED)
   RTTI::TMeta operator ""_tmeta(const char* token, size_t size) noexcept {
      return RTTI::Registry::GetMetaTagByToken(Token {token, size});
   }

   LANGULUS(INLINED)
   RTTI::CMeta operator ""_cmeta(const char* token, size_t size) noexcept {
      return RTTI::Registry::GetMetaConstByToken(Token {token, size});
   }

   LANGULUS(INLINED)
   RTTI::VMeta operator ""_vmeta(const char* token, size_t size) noexcept {
      return RTTI::Registry::GetMetaVerbByToken(Token {token, size});
   }
   
   /// ... as well as getting them manually                                   
   LANGULUS(INLINED)
   RTTI::DMeta MetaDataOf(const Token& token) noexcept {
      return RTTI::Registry::GetMetaDataByToken(token);
   }

   LANGULUS(INLINED)
   RTTI::TMeta MetaTagOf(const Token& token) noexcept {
      return RTTI::Registry::GetMetaTagByToken(token);
   }

   LANGULUS(INLINED)
   RTTI::VMeta MetaVerbOf(const Token& token) noexcept {
      return RTTI::Registry::GetMetaVerbByToken(token);
   }

   LANGULUS(INLINED)
   RTTI::CMeta MetaConstOf(const Token& token) noexcept {
      return RTTI::Registry::GetMetaConstByToken(token);
   }
#endif
}
