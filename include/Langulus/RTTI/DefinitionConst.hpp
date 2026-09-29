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
   /// A constant value definition                                            
   ///                                                                        
   class DefinitionConst final : public Inner::Definition {
   protected:
      friend class Registry;
      friend struct Inner::MetaConstNaked;
      friend struct Inner::MetaConstPacked_16;

      // The type of the constant                                       
      DefinitionData const* mType = nullptr;
      // A pointer to an instance of the constant on the heap           
      void (*mFillConstant)(void*) = nullptr;

   public:
      using CTTI_ReflectAs = void;

      static constexpr Token InvalidName = "novalue";

      template<class OWNER, class NAMED_VALUE>
      static auto Reflect() -> DefinitionConst const*;
      
      DefinitionConst(const Token& cppname) noexcept
         : Definition {cppname} {}
   };
}

#include "DefinitionConst.inl"
