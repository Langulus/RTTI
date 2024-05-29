///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Reflection.hpp"


namespace Langulus::A
{

   ///                                                                        
   /// The following abstract types are implicitly added as bases             
   /// when reflecting fundamental types. They are linked to their            
   /// corresponding concepts, so you can use them at runtime, to check if    
   /// a type is compatible with the given concept via type->CastsTo          
   ///                                                                        

   /// Check if a type is compatible with CT::Number concept at runtime       
   struct Number {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::Langulus::Real;
      ~Number() = delete;
   };

   /// Check if a type is compatible with CT::Integer concept at runtime      
   struct Integer {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::std::intptr_t;
      LANGULUS_BASES(Number);
      ~Integer() = delete;
   };

   /// Check if a type is compatible with CT::Signed concept at runtime       
   struct Signed {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::Langulus::Real;
      LANGULUS_BASES(Number);
      ~Signed() = delete;
   };

   /// Check if a type is compatible with CT::Unsigned concept at runtime     
   struct Unsigned {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::std::uintptr_t;
      LANGULUS_BASES(Number);
      ~Unsigned() = delete;
   };

   /// Check if a type is compatible with CT::UnsignedInteger concept         
   struct UnsignedInteger {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::std::uintptr_t;
      LANGULUS_BASES(Unsigned, Integer);
      ~UnsignedInteger() = delete;
   };

   /// Check if a type is compatible with CT::Real concept at runtime         
   struct Real {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::Langulus::Real;
      LANGULUS_BASES(Signed);
      ~Real() = delete;
   };

   /// Check if a type is compatible with CT::SignedInteger concept           
   struct SignedInteger {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::std::intptr_t;
      LANGULUS_BASES(Signed, Integer);
      ~SignedInteger() = delete;
   };

   /// Check if a type is compatible with CT::Character concept at runtime    
   struct Char {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) char;
      ~Char() = delete;
   };

   /// Check if a type is compatible with CT::Bool concept at runtime         
   struct Bool {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) bool;
      ~Bool() = delete;
   };

} // namespace Langulus::A
