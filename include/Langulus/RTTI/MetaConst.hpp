///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Meta.hpp"
#include "DefinitionConst.hpp"


namespace Langulus::RTTI
{
   namespace Inner
   {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      ///                                                                     
      /// Relies on the definition limits to pack an ID into the smallest     
      /// possible space, but also uses some additional bits to encode some   
      /// often used information about the definition. The handle still has   
      /// to be transformed into a pointer for more advanced uses, but in     
      /// general it is likely to avoid an indirection altogether at the      
      /// cost of a bitwise operation, making it a bit more cache-friendly,   
      /// and worth experimenting with                                        
      #pragma pack(push, 1)
      struct MetaConstPacked_16 : MetaPacked<2> {
         using CTTI_Abstract = No;
         using Base = MetaPacked;

         constexpr MetaConstPacked_16() noexcept = default;
         constexpr MetaConstPacked_16(MetaConstPacked_16 const&) noexcept = default;
         constexpr MetaConstPacked_16(MetaConstPacked_16&&) noexcept = default;
         constexpr MetaConstPacked_16(nullptr_t) noexcept;
         constexpr MetaConstPacked_16(DefinitionConst const*) noexcept;

         constexpr MetaConstPacked_16& operator = (MetaConstPacked_16 const&) noexcept = default;
         constexpr MetaConstPacked_16& operator = (MetaConstPacked_16&&) noexcept = default;
         constexpr MetaConstPacked_16& operator = (nullptr_t) noexcept;
         constexpr MetaConstPacked_16& operator = (DefinitionConst const*) noexcept;

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
      
      static_assert(sizeof(MetaConstPacked_16) == 2);
   #endif

      /// A naked pointer to a definition. Probably the fastest, but most     
      /// memory-inefficient on 64bit systems                                 
      struct MetaConstNaked : MetaNaked<DefinitionConst> {
         using CTTI_Abstract = No;
         using MetaNaked::MetaNaked;
         using MetaNaked::operator =;
         using MetaNaked::operator bool;
      };
   }
}

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   #include "MetaConstStructured.inl"
#endif

#if LANGULUS_FEATURE(LOGGING)
namespace fmt
{
   ///                                                                        
   /// Extend FMT to be capable of logging constant types                     
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
   template<>
   struct formatter<::Langulus::RTTI::Inner::MetaConstPacked_16> {
      using M = ::Langulus::RTTI::Inner::MetaConstPacked_16;

      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {return ctx.begin();}

      template<class CONTEXT>
      auto format(M const& c, CONTEXT& ctx) const {
         return format_to(ctx.out(), "{}", c.GetName());
      }
   };
   #endif
   
   template<>
   struct formatter<::Langulus::RTTI::Inner::MetaConstNaked> {
      using M = ::Langulus::RTTI::Inner::MetaConstNaked;

      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {return ctx.begin();}

      template<class CONTEXT>
      auto format(M const& c, CONTEXT& ctx) const {
         return format_to(ctx.out(), "{}", c.GetName());
      }
   };
}
#endif
