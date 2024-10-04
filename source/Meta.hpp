///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Reflection.hpp"
#include "NameOf.hpp"
#include <Core/Utilities.hpp>
#include <vector>


namespace Langulus::RTTI
{

   ///                                                                        
   ///   Meta                                                                 
   ///                                                                        
   /// Abstract base for meta definitions                                     
   ///                                                                        
   struct Meta {
   public:
      LANGULUS(ABSTRACT) true;
      LANGULUS(UNALLOCATABLE) true;

      Meta() = delete;
      Meta(const Token&);

      /// Virtual destructor required for dynamic_cast                        
      virtual ~Meta() = default;

      virtual Token Kind() const noexcept = 0;

      /// Custom operator required, to avoid const members                    
      Meta& operator = (const Meta& rhs) noexcept {
         mInfo = rhs.mInfo;
         mCppName = rhs.mCppName;
         mVersionMajor = rhs.mVersionMajor;
         mVersionMinor = rhs.mVersionMinor;
         IF_LANGULUS_MANAGED_REFLECTION(mLibraryName = rhs.mLibraryName);
         return *this;
      }

      // Each reflected type has an unique hash                         
      // First for immediate access                                     
      const Hash mHash;
      // Each reflection primitive has a unique token, but that         
      // uniqueness is checked only if MANAGED_REFLECTION feature is    
      // enabled                                                        
      const Token mToken;

      // Each reflection may or may not have some info                  
      Token mInfo = "<no info provided>";
      // Original name of the type                                      
      Token mCppName;
      // Major version                                                  
      Count mVersionMajor = 1;
      // Minor version                                                  
      Count mVersionMinor = 0;

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // The shared library that defined the module, used to unload  
         // definitions when module is unloaded                         
         Token mLibraryName;

         NOD() LANGULUS_API(RTTI)
         Token GetShortestUnambiguousToken() const;
      #endif

      static constexpr Token Data      = "meta data";
      static constexpr Token Trait     = "meta trait";
      static constexpr Token Verb      = "meta verb";
      static constexpr Token Constant  = "meta const";
      static constexpr Token Unknown   = "unknown";
   };
   

   ///                                                                        
   ///   Abstract definition interchange format                               
   ///                                                                        
   /// Can contain DMeta, TMeta, VMeta or CMeta, which are dynamical_cast     
   /// on demand to the appropriate type                                      
   ///                                                                        
   struct AMeta {
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

   protected:
      friend struct Meta;
      friend class  Registry;

      // Pointer to the definition                                      
      const Meta* mMeta {};

   public:
      constexpr AMeta() noexcept = default;
      constexpr AMeta(const AMeta&) noexcept = default;
      constexpr AMeta(AMeta&&) noexcept = default;
      constexpr AMeta(const Meta* meta) noexcept
         : mMeta {meta} {}
      constexpr AMeta(::std::nullptr_t) noexcept {}

      constexpr AMeta& operator = (const AMeta&) noexcept = default;
      constexpr AMeta& operator = (AMeta&&) noexcept = default;

      constexpr Token GetToken() const noexcept;

      constexpr Hash  GetHash() const noexcept {
         return mMeta ? mMeta->mHash : Hash {};
      }

      const Meta* operator -> () const noexcept {
         return mMeta;
      }

      constexpr explicit operator bool() const noexcept {
         return mMeta != nullptr;
      }

      constexpr operator DMeta() const noexcept;
      constexpr operator TMeta() const noexcept;
      constexpr operator CMeta() const noexcept;
      constexpr operator VMeta() const noexcept;

      constexpr bool operator == (const VMeta&) const noexcept;
      constexpr bool operator == (const TMeta&) const noexcept;
      constexpr bool operator == (const CMeta&) const noexcept;

      constexpr bool operator == (const DMeta&) const noexcept;
      constexpr bool operator &  (const DMeta&) const noexcept;
      constexpr bool operator |  (const DMeta&) const noexcept;

      constexpr bool operator == (const AMeta&) const noexcept;
      constexpr bool operator &  (const AMeta&) const noexcept;
      constexpr bool operator |  (const AMeta&) const noexcept;

      template<class T>
      NOD() constexpr T As() const noexcept;

      constexpr Token Kind() const noexcept;
   };


   ///                                                                        
   /// Data type interchange format                                           
   ///                                                                        
   struct DMeta : AMeta {
      LANGULUS_BASES(AMeta);

      using AMeta::AMeta;
      constexpr DMeta(const MetaData*) noexcept;

      constexpr Token GetToken() const noexcept;

      const MetaData* operator -> () const noexcept;
      const MetaData& operator *  () const noexcept;

      constexpr bool operator == (const DMeta&) const noexcept;
      constexpr bool operator |  (const DMeta&) const noexcept;
      constexpr bool operator &  (const DMeta&) const noexcept;

   private:
      using AMeta::operator Langulus::RTTI::CMeta;
      using AMeta::operator Langulus::RTTI::DMeta;
      using AMeta::operator Langulus::RTTI::VMeta;
      using AMeta::operator Langulus::RTTI::TMeta;
   };

         
   ///                                                                        
   /// Verb type interchange format                                           
   ///                                                                        
   struct VMeta : AMeta {
      LANGULUS_BASES(AMeta);

      using AMeta::AMeta;
      constexpr VMeta(const MetaVerb*) noexcept;

      constexpr Token GetToken() const noexcept;

      const MetaVerb* operator -> () const noexcept;
      const MetaVerb& operator *  () const noexcept;

      constexpr bool operator == (const VMeta&) const noexcept;

   private:
      using AMeta::operator Langulus::RTTI::CMeta;
      using AMeta::operator Langulus::RTTI::DMeta;
      using AMeta::operator Langulus::RTTI::VMeta;
      using AMeta::operator Langulus::RTTI::TMeta;
   };
      

   ///                                                                        
   /// Trait type interchange format                                          
   ///                                                                        
   struct TMeta : AMeta {
      LANGULUS_BASES(AMeta);

      using AMeta::AMeta;
      constexpr TMeta(const MetaTrait*) noexcept;

      constexpr Token GetToken() const noexcept;

      const MetaTrait* operator -> () const noexcept;
      const MetaTrait& operator *  () const noexcept;

      constexpr bool operator == (const TMeta&) const noexcept;

   private:
      using AMeta::operator Langulus::RTTI::CMeta;
      using AMeta::operator Langulus::RTTI::DMeta;
      using AMeta::operator Langulus::RTTI::VMeta;
      using AMeta::operator Langulus::RTTI::TMeta;
   };


   ///                                                                        
   /// Definition interchange format for constants                            
   ///                                                                        
   struct CMeta : AMeta {
      LANGULUS_BASES(AMeta);

      using AMeta::AMeta;
      constexpr CMeta(const MetaConst*) noexcept;

      constexpr Token GetToken() const noexcept;

      const MetaConst* operator -> () const noexcept;
      const MetaConst& operator *  () const noexcept;

      constexpr bool operator == (const CMeta&) const noexcept;

   private:
      using AMeta::operator Langulus::RTTI::CMeta;
      using AMeta::operator Langulus::RTTI::DMeta;
      using AMeta::operator Langulus::RTTI::VMeta;
      using AMeta::operator Langulus::RTTI::TMeta;
   };

} // namespace Langulus::RTTI

namespace Langulus::CT
{

   /// Concept for meta type interchangers                                    
   template<class...T>
   concept Meta = (DerivedFrom<T, RTTI::AMeta> and ...);

} // namespace Langulus::CT

namespace fmt
{

   ///                                                                        
   /// Extend FMT to be capable of logging any type with reflected named      
   /// values                                                                 
   ///                                                                        
   template<Langulus::CT::HasNamedValues T>
   struct formatter<T> {
      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {
         return ctx.begin();
      }

      template<class CONTEXT> LANGULUS(INLINED)
      bool langulus_format_inner(T const& lhs, auto const& rhs, CONTEXT& ctx) const {
         using namespace Langulus;
         using D = Deref<decltype(rhs)>;

         if (T {D::Value} != DenseCast(lhs))
            return false;

         fmt::format_to(ctx.out(), "{}::{}",
            NameOf<T>(),
            RTTI::LastCppNameOf<D::Value>()
         );
         return true;
      }

      template<class CONTEXT> LANGULUS(INLINED)
      auto format(T const& value, CONTEXT& ctx) const {
         using namespace Langulus;

         bool found = std::apply([&](auto&&...args) {
            return (... or langulus_format_inner(value, args, ctx));
         }, T::CTTI_NamedValues);

         if (found)
            return ctx.out();
         else
            return fmt::format_to(ctx.out(), "<bad named value>");
      }
   };
   
   ///                                                                        
   /// Extend FMT to be capable of logging any meta definition                
   ///                                                                        
   template<Langulus::CT::Meta T>
   struct formatter<T> {
      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {
         return ctx.begin();
      }

      template<class CONTEXT> LANGULUS(INLINED)
      auto format(T const& e, CONTEXT& ctx) const {
         return fmt::format_to(ctx.out(), "{}", e.GetToken());
      }
   };

} // namespace fmt