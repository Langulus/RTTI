///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Meta.hpp"
#include "DefinitionTag.hpp"


namespace Langulus::RTTI
{
   namespace Inner
   {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      ///                                                                     
      /// Supports up to 65,535 tags                                          
      #pragma pack(push, 1)
      struct MetaTagPacked_16 : MetaPacked<2> {
         using CTTI_Abstract = No;
         using Base = MetaPacked;

         constexpr MetaTagPacked_16() noexcept = default;
         constexpr MetaTagPacked_16(MetaTagPacked_16 const&) noexcept = default;
         constexpr MetaTagPacked_16(MetaTagPacked_16&&) noexcept = default;
         constexpr MetaTagPacked_16(nullptr_t) noexcept;
         constexpr MetaTagPacked_16(DefinitionTag const*) noexcept;

         constexpr MetaTagPacked_16& operator = (MetaTagPacked_16 const&) noexcept = default;
         constexpr MetaTagPacked_16& operator = (MetaTagPacked_16&&) noexcept = default;
         constexpr MetaTagPacked_16& operator = (nullptr_t) noexcept;
         constexpr MetaTagPacked_16& operator = (DefinitionTag const*) noexcept;
         
         constexpr bool operator == (const MetaTagPacked_16&) const noexcept;

         auto GetName()          const noexcept -> Token;
         auto GetInfo()          const noexcept -> Token;
         auto GetCppName()       const noexcept -> Token;
         auto GetHash()          const noexcept -> Hash;
         auto GetVersionMajor()  const noexcept -> unsigned;
         auto GetVersionMinor()  const noexcept -> unsigned;
         auto GetBoundaries()    const noexcept -> Definition::BoundarySet const&;

         IF_SAFE(explicit operator bool() const noexcept);
      };
      #pragma pack(pop)
      
      static_assert(sizeof(MetaTagPacked_16) == 2);
   #endif
      
      ///                                                                     
      /// A naked pointer to a definition. Probably the fastest, but most     
      /// memory-inefficient on 64bit systems.                                
      struct MetaTagNaked : MetaNaked<DefinitionTag> {
         using CTTI_Abstract = No;
         using Base = MetaNaked;
         using Base::Base;
         using Base::operator =;
         using Base::operator bool;
      };
   }
}

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   #include "MetaTagStructured.inl"
#endif

#if LANGULUS_FEATURE(LOGGING)
namespace fmt
{
   ///                                                                        
   /// Extend FMT to be capable of logging tag types                          
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
   template<>
   struct formatter<::Langulus::RTTI::Inner::MetaTagPacked_16> {
      using M = ::Langulus::RTTI::Inner::MetaTagPacked_16;

      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {return ctx.begin();}

      template<class CONTEXT>
      auto format(M const& c, CONTEXT& ctx) const {
         return format_to(ctx.out(), "{}", c.GetName());
      }
   };
   #endif
   
   template<>
   struct formatter<::Langulus::RTTI::Inner::MetaTagNaked> {
      using M = ::Langulus::RTTI::Inner::MetaTagNaked;

      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {return ctx.begin();}

      template<class CONTEXT>
      auto format(M const& c, CONTEXT& ctx) const {
         return format_to(ctx.out(), "{}", c.GetName());
      }
   };
}
#endif
