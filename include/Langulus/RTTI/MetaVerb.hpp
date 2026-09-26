///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Meta.hpp"
#include "DefinitionVerb.hpp"


namespace Langulus::RTTI
{
   namespace Inner
   {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      ///                                                                     
      /// These rely on the definition limits to pack an ID into the smallest 
      /// possible space, but also uses some additional bits to encode some   
      /// often used information about the definition. The handle still has   
      /// to be transformed into a pointer for more advanced uses, but in     
      /// general it is likely to avoid an indirection altogether at the      
      /// cost of a bitwise operation, making it a bit more cache-friendly,   
      /// and worth experimenting with                                        
      
      /// Packing strategy that can't exceed 2^(8*ID_SIZE)-2 possible verbs   
      #pragma pack(push, 1)
      template<unsigned ID_SIZE>
      struct MetaVerbStructured_X8 : MetaPacked<ID_SIZE> {
      private:
         union {
            struct {
               bool reversible   : 1;
               bool contextless  : 1;
            };
            uint8_t all {};
         };
         
      public:
         using CTTI_Abstract = No;
         using Base = MetaPacked<ID_SIZE>;

         constexpr MetaVerbStructured_X8() noexcept = default;
         constexpr MetaVerbStructured_X8(MetaVerbStructured_X8 const&) noexcept = default;
         constexpr MetaVerbStructured_X8(MetaVerbStructured_X8&&) noexcept = default;
         constexpr MetaVerbStructured_X8(nullptr_t) noexcept;
         constexpr MetaVerbStructured_X8(DefinitionVerb const*) noexcept;

         constexpr MetaVerbStructured_X8& operator = (MetaVerbStructured_X8 const&) noexcept = default;
         constexpr MetaVerbStructured_X8& operator = (MetaVerbStructured_X8&&) noexcept = default;
         constexpr MetaVerbStructured_X8& operator = (nullptr_t) noexcept;
         constexpr MetaVerbStructured_X8& operator = (DefinitionVerb const*) noexcept;

         constexpr bool operator == (const MetaVerbStructured_X8&) const noexcept;

         auto GetDefinition() const noexcept -> DefinitionVerb const*;

         auto GetCppName()          const noexcept -> Token;
         auto GetInfo()             const noexcept -> Token;
         auto GetHash()             const noexcept -> Hash;
         auto GetVersionMajor()     const noexcept -> unsigned;
         auto GetVersionMinor()     const noexcept -> unsigned;
         auto GetBoundaries()       const noexcept -> Definition::BoundarySet const&;

         auto GetPositiveName()     const noexcept -> Token;
         auto GetNegativeName()     const noexcept -> Token;
         auto GetPositiveOperator() const noexcept -> Token;
         auto GetNegativeOperator() const noexcept -> Token;
         auto GetPrecedence()       const noexcept -> float;
         auto GetContextless()      const noexcept -> DefinitionVerb::FContextless;

         constexpr bool IsReversible()  const noexcept;
         constexpr bool IsContextless() const noexcept;

         IF_SAFE(explicit operator bool() const noexcept);
      };
      #pragma pack(pop)
      
      static_assert(sizeof(MetaVerbStructured_X8<1>) == 2);
      static_assert(sizeof(MetaVerbStructured_X8<2>) == 3);
      static_assert(sizeof(MetaVerbStructured_X8<3>) == 4);
   #endif

      /// A naked pointer to a definition. Probably the fastest, but most     
      /// memory-inefficient on 64bit systems.                                
      struct MetaVerbNaked : MetaNaked<DefinitionVerb> {
         using CTTI_Abstract = No;
         using Base = MetaNaked;
         using Base::Base;
         using Base::operator =;
         using Base::operator bool;

         auto GetPositiveName()     const noexcept -> Token;
         auto GetNegativeName()     const noexcept -> Token;
         auto GetPositiveOperator() const noexcept -> Token;
         auto GetNegativeOperator() const noexcept -> Token;
         auto GetPrecedence()       const noexcept -> float;
         auto GetContextless()      const noexcept -> DefinitionVerb::FContextless;

         constexpr bool IsReversible()  const noexcept;
         constexpr bool IsContextless() const noexcept;
      };
   }
}

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   #include "MetaVerbStructured.inl"
#endif

#include "MetaVerbNaked.inl"

#if LANGULUS_FEATURE(LOGGING)
namespace fmt
{
   ///                                                                        
   /// Extend FMT to be capable of logging verb types                         
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
   template<unsigned ID_SIZE>
   struct formatter<::Langulus::RTTI::Inner::MetaVerbStructured_X8<ID_SIZE>> {
      using M = ::Langulus::RTTI::Inner::MetaVerbStructured_X8<ID_SIZE>;

      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {return ctx.begin();}

      template<class CONTEXT>
      auto format(M const& c, CONTEXT& ctx) const {
         return format_to(ctx.out(), "{}", c.GetPositiveName());
      }
   };
   #endif
   
   template<>
   struct formatter<::Langulus::RTTI::Inner::MetaVerbNaked> {
      using M = ::Langulus::RTTI::Inner::MetaVerbNaked;

      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {return ctx.begin();}

      template<class CONTEXT>
      auto format(M const& c, CONTEXT& ctx) const {
         return format_to(ctx.out(), "{}", c.GetPositiveName());
      }
   };
}
#endif
