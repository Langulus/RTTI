///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
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
   class Number {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::Langulus::Real;
      ~Number() = delete;
   };

   /// Check if a type is compatible with CT::Integer concept at runtime      
   class Integer {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::std::intptr_t;
      LANGULUS_BASES(Number);
      ~Integer() = delete;
   };

   /// Check if a type is compatible with CT::Signed concept at runtime       
   class Signed {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::Langulus::Real;
      LANGULUS_BASES(Number);
      ~Signed() = delete;
   };

   /// Check if a type is compatible with CT::Unsigned concept at runtime     
   class Unsigned {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::std::uintptr_t;
      LANGULUS_BASES(Number);
      ~Unsigned() = delete;
   };

   /// Check if a type is compatible with CT::UnsignedInteger concept         
   class UnsignedInteger {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::std::uintptr_t;
      LANGULUS_BASES(Unsigned, Integer);
      ~UnsignedInteger() = delete;
   };

   /// Check if a type is compatible with CT::Real concept at runtime         
   class Real {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::Langulus::Real;
      LANGULUS_BASES(Signed);
      ~Real() = delete;
   };

   /// Check if a type is compatible with CT::SignedInteger concept           
   class SignedInteger {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) ::std::intptr_t;
      LANGULUS_BASES(Signed, Integer);
      ~SignedInteger() = delete;
   };

   /// Check if a type is compatible with CT::Character concept at runtime    
   class Text {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) char;
      ~Text() = delete;
   };

   /// Check if a type is compatible with CT::Bool concept at runtime         
   class Bool {
      LANGULUS(ABSTRACT) true;
      LANGULUS(CONCRETE) bool;
      ~Bool() = delete;
   };

} // namespace Langulus::A
