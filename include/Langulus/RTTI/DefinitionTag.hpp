///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Definition.hpp"


namespace Langulus::RTTI
{
   ///                                                                        
   /// Tag definition                                                         
   ///                                                                        
   class DefinitionTag final : public Inner::Definition {
   protected:
      friend class Registry;
      friend struct Inner::MetaTagNaked;
      friend struct Inner::MetaTagPacked_16;

   public:
      using CTTI_ReflectAs = void;

      static constexpr Token InvalidName = "notag";

      template<CT::Decayed>
      static auto Reflect() -> DefinitionTag const*;

      DefinitionTag(const Token& cppname) noexcept
         : Definition {cppname} {}

      ~DefinitionTag();
   };
}

#include "DefinitionTag.inl"
