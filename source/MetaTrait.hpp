///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Meta.hpp"


namespace Langulus::RTTI
{

   ///                                                                        
   ///   Meta trait                                                           
   ///                                                                        
   struct MetaTrait : Meta {
      friend struct Member;
      LANGULUS(NAME) "TMeta";
      LANGULUS_BASES(Meta);

      static constexpr Token DefaultToken = "NoTrait";

      // A sanitized last token (with a lower first letter)             
      ::std::string mTokenSanitized;
      // Data filter for the trait (optional)                           
      DMeta mDataType {};

   public:
      MetaTrait(const Token& token)
         : Meta {token} {
         mTokenSanitized = ToLastToken(mToken);
         mTokenSanitized[0] = ::std::tolower(mTokenSanitized[0]);
      }

      template<CT::Void>
      NOD() static consteval TMeta Of();
      template<CT::Decayed>
      NOD() static TMeta Of();
      
      NOD() bool Is(TMeta) const noexcept;
      template<CT::Data>
      NOD() bool Is() const;

      Token Kind() const noexcept final {
         return Meta::Trait;
      }

   protected:
      template<CT::Data>
      static consteval Token GetReflectedToken() noexcept;
   };

} // namespace Langulus::RTTI