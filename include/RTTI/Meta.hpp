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
#include "../../source/Tag.hpp"
#include "../../source/Arithmetic.hpp"


namespace Langulus
{

   /// Get the meta definition of a type, deducing it whether it is data,     
   /// verb, constant, or trait                                               
   ///   @tparam T - type to get meta definition of                           
   ///   @return the meta definition of the provided type                     
   template<class T>
   NOD() auto MetaOf() {
      if constexpr (CT::Decayed<T> and requires { T::CTTI_Trait; })
         return RTTI::MetaTrait::Of<T>();
      else if constexpr (CT::Decayed<T>
              and (  requires { T::CTTI_Verb; }
                  or requires { T::CTTI_PositiveVerb; }
                  or requires { T::CTTI_NegativeVerb; }))
         return RTTI::MetaVerb::Of<T>();
      else
         return RTTI::MetaData::Of<T>();
   }

   /// Data definition retrieval                                              
   /// Some types, like traits/verbs for example, can be represented both as  
   /// DMeta and TMeta/VMeta, and this is useful to represent as DMeta only   
   ///   @tparam T - type to get meta data definition of                      
   ///   @return the meta definition of the provided type                     
   template<class T>
   NOD() auto MetaDataOf() {
      return RTTI::MetaData::Of<T>();
   }

   /// Trait definition retrieval                                             
   /// Some types, like traits for example, can be represented both as DMeta  
   /// and TMeta, and this is useful to represent as TMeta only               
   ///   @tparam T - type to get meta trait definition of                     
   ///   @return the meta definition of the provided type                     
   template<class T>
   NOD() auto MetaTraitOf() {
      return RTTI::MetaTrait::Of<T>();
   }

   /// Verb definition retrieval                                              
   /// Some types, like verbs for example, can be represented both as DMeta   
   /// and VMeta, and this is useful to represent as VMeta only               
   ///   @tparam T - type to get meta trait definition of                     
   ///   @return the meta definition of the provided type                     
   template<class T>
   NOD() auto MetaVerbOf() {
      return RTTI::MetaVerb::Of<T>();
   }

} // namespace Langulus