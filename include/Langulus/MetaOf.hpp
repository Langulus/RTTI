///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "../../source/Referenced.hpp"
#include "../../source/Meta.inl"
#include "../../source/MetaData.inl"
#include "../../source/MetaTrait.inl"
#include "../../source/MetaVerb.inl"
#include "../../source/MetaConst.inl"
#include "../../source/Tag.hpp"
#include "../../source/Intent.hpp"


namespace Langulus
{

   /// Get the meta definition of a type, deducing whether it's data, verb,   
   /// constant, or trait                                                     
   ///   @tparam T - type to get meta definition of                           
   ///   @return the meta definition of the provided type                     
   template<class T>
   auto MetaOf() {
      if constexpr (CT::Decayed<T> and requires { T::CTTI_Trait; })
         return RTTI::MetaTrait::Of<T>();
      else if constexpr (CT::Decayed<T> and requires { T::CTTI_Constant; })
         return RTTI::MetaConst::Of<T>();
      else if constexpr (CT::Decayed<T> and (
         requires { T::CTTI_Verb;         }
      or requires { T::CTTI_PositiveVerb; }
      or requires { T::CTTI_NegativeVerb; }))
         return RTTI::MetaVerb::Of<T>();
      else
         return RTTI::MetaData::Of<T>();
   }

   /// Data definition retrieval                                              
   /// Some types, like traits/verbs for example, can be represented both as  
   /// DMeta and TMeta/VMeta, and this is useful to state a clear intent      
   ///   @tparam T - type to get meta data definition of                      
   ///   @return the meta definition of the provided type                     
   template<class T>
   auto MetaDataOf() {
      return RTTI::MetaData::Of<T>();
   }

   /// Trait definition retrieval                                             
   /// Some types, like traits for example, can be represented both as DMeta  
   /// and TMeta, and this is useful to state a clear intent                  
   ///   @tparam T - type to get meta trait definition of                     
   ///   @return the meta definition of the provided trait                    
   template<class T>
   auto MetaTraitOf() {
      return RTTI::MetaTrait::Of<T>();
   }

   /// Verb definition retrieval                                              
   /// Some types, like verbs for example, can be represented both as DMeta   
   /// and VMeta, and this is useful to state a clear intent                  
   ///   @tparam T - type to get meta trait definition of                     
   ///   @return the meta definition of the provided verb                     
   template<class T>
   auto MetaVerbOf() {
      return RTTI::MetaVerb::Of<T>();
   }

   /// Constant definition retrieval                                          
   ///   @tparam T - type to get meta constant definition of                  
   ///   @return the meta definition of the provided constant                 
   template<class T>
   auto MetaConstOf() {
      return RTTI::MetaConst::Of<T>();
   }

   /// Convenience operators for getting meta definitions from token          
   LANGULUS(INLINED)
   RTTI::DMeta operator ""_dmeta(const char* token, ::std::size_t size) noexcept {
      return RTTI::GetMetaData(Token {token, size});
   }

   LANGULUS(INLINED)
   RTTI::TMeta operator ""_tmeta(const char* token, ::std::size_t size) noexcept {
      return RTTI::GetMetaTrait(Token {token, size});
   }

   LANGULUS(INLINED)
   RTTI::CMeta operator ""_cmeta(const char* token, ::std::size_t size) noexcept {
      return RTTI::GetMetaConstant(Token {token, size});
   }

   LANGULUS(INLINED)
   RTTI::VMeta operator ""_vmeta(const char* token, ::std::size_t size) noexcept {
      return RTTI::GetMetaVerb(Token {token, size});
   }

   LANGULUS(INLINED)
   RTTI::AMeta operator ""_meta(const char* token, ::std::size_t size) {
      auto& found = RTTI::GetAmbiguousMeta(Token {token, size});
      if (found.size() == 1)
         return *found.begin();
      else
         LANGULUS_THROW(Meta, "Ambiguous meta literal "
            "- use RTTI::GetAmbiguousMeta and process the result yourself");
   }

} // namespace Langulus