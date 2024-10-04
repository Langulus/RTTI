///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "MetaData.hpp"


namespace Langulus::RTTI
{
   
   ///                                                                        
   ///   Meta constant, used to reflect global constants or named values      
   /// in types.                                                              
   ///                                                                        
   struct MetaConst : Meta {
      LANGULUS(NAME) "CMeta";
      LANGULUS_BASES(Meta);

      static constexpr Token DefaultToken = "NoConst";

      DMeta mValueType {};
      const void* mPtrToValue {};

   public:
      MetaConst(const Token& token) : Meta {token} {}

      template<CT::Decayed>
      NOD() static CMeta Of();

      NOD() constexpr bool Is(CMeta) const noexcept;

      Token Kind() const noexcept final {
         return Meta::Constant;
      }

   protected:
      template<CT::Data>
      static consteval Token GetReflectedToken() noexcept;
   };

   template<auto...>
   constexpr auto CreateNamedValueTuple();

} // namespace Langulus::RTTI


/// Define a constant                                                         
///   @param T - the constant, as it appears in namespace Langulus::Constants 
///   @param TOKEN - the token used for referring to the constant             
///   @param VALUE - the value of the constant                                
///   @param INFOSTRING - information about the constant's purpose            
#define LANGULUS_DEFINE_CONSTANT(T, VALUE, TOKEN, INFOSTRING) \
   namespace Langulus::Constants \
   { \
      struct T { \
         LANGULUS(CONSTANT) TOKEN; \
         LANGULUS(INFO) INFOSTRING; \
         static constexpr auto Value = VALUE; \
      }; \
   }
