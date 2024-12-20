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
#include "../../source/Arithmetic.hpp"


namespace Langulus
{

   /// Get the meta definition of a type, deducing whether it's data, verb,   
   /// constant, or trait                                                     
   ///   @tparam T - type to get meta definition of                           
   ///   @return the meta definition of the provided type                     
   template<class T>
   NOD() auto MetaOf() {
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
   NOD() auto MetaDataOf() {
      return RTTI::MetaData::Of<T>();
   }

   /// Trait definition retrieval                                             
   /// Some types, like traits for example, can be represented both as DMeta  
   /// and TMeta, and this is useful to state a clear intent                  
   ///   @tparam T - type to get meta trait definition of                     
   ///   @return the meta definition of the provided trait                    
   template<class T>
   NOD() auto MetaTraitOf() {
      return RTTI::MetaTrait::Of<T>();
   }

   /// Verb definition retrieval                                              
   /// Some types, like verbs for example, can be represented both as DMeta   
   /// and VMeta, and this is useful to state a clear intent                  
   ///   @tparam T - type to get meta trait definition of                     
   ///   @return the meta definition of the provided verb                     
   template<class T>
   NOD() auto MetaVerbOf() {
      return RTTI::MetaVerb::Of<T>();
   }

   /// Constant definition retrieval                                          
   ///   @tparam T - type to get meta constant definition of                  
   ///   @return the meta definition of the provided constant                 
   template<class T>
   NOD() auto MetaConstOf() {
      return RTTI::MetaConst::Of<T>();
   }

} // namespace Langulus