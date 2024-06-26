///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
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

      // Data filter for the trait (optional)                           
      DMeta mDataType {};

   public:
      MetaTrait(const Token& token) : Meta {token} {}

      template<CT::Void>
      NOD() static consteval TMeta Of();
      template<CT::Decayed>
      NOD() static TMeta Of();
      
      NOD() bool Is(TMeta) const noexcept;
      template<CT::Data>
      NOD() bool Is() const;

   protected:
      template<CT::Data>
      static consteval Token GetReflectedToken() noexcept;
   };

} // namespace Langulus::RTTI