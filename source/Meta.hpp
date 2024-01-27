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
#include <Core/Utilities.hpp>
#include <vector>


namespace Langulus::RTTI
{
   
   ///                                                                        
   /// Type interchange format, makes sure to call IsExact on operator ==,    
   /// as well as other handy operations                                      
   ///                                                                        
   struct LANGULUS_API(RTTI) DMeta {
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

   protected:
      friend struct AMeta;
      friend struct Meta;
      friend class Registry;
      friend struct MetaData;
      const MetaData* mMeta {};

   public:
      constexpr DMeta() noexcept = default;
      constexpr DMeta(const MetaData* raw) noexcept
         : mMeta {raw} {}

      constexpr Token GetToken() const noexcept;
      constexpr Hash GetHash() const noexcept;

      const MetaData* operator -> () const noexcept { return mMeta; }
      constexpr explicit operator bool() const noexcept { return mMeta != nullptr; }

      constexpr bool operator == (const DMeta&) const noexcept;
      constexpr bool operator &= (const DMeta&) const noexcept;
      constexpr bool operator |= (const DMeta&) const noexcept;
   };

         
   ///                                                                        
   /// Definition interchange format for verbs                                
   ///                                                                        
   struct LANGULUS_API(RTTI) VMeta {
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

   protected:
      friend struct AMeta;
      friend struct Meta;
      friend class Registry;
      friend struct MetaVerb;
      const MetaVerb* mMeta {};

   public:
      constexpr VMeta() noexcept = default;
      constexpr VMeta(const MetaVerb* raw) noexcept
         : mMeta {raw} {}

      constexpr Token GetToken() const noexcept;
      constexpr Hash GetHash() const noexcept;

      const MetaVerb* operator -> () const noexcept { return mMeta; }
      constexpr explicit operator bool() const noexcept { return mMeta != nullptr; }

      constexpr bool operator == (const VMeta&) const noexcept;
   };
      

   ///                                                                        
   /// Definition interchange format for traits                               
   ///                                                                        
   struct LANGULUS_API(RTTI) TMeta {
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

   protected:
      friend struct AMeta;
      friend struct Meta;
      friend class Registry;
      friend struct MetaTrait;
      const MetaTrait* mMeta {};

   public:
      constexpr TMeta() noexcept = default;
      constexpr TMeta(const MetaTrait* raw) noexcept
         : mMeta {raw} {}

      constexpr Token GetToken() const noexcept;
      constexpr Hash GetHash() const noexcept;

      const MetaTrait* operator -> () const noexcept { return mMeta; }
      constexpr explicit operator bool() const noexcept { return mMeta != nullptr; }

      constexpr bool operator == (const TMeta&) const noexcept;
   };


   ///                                                                        
   /// Definition interchange format for constants                            
   ///                                                                        
   struct LANGULUS_API(RTTI) CMeta {
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

   protected:
      friend struct AMeta;
      friend struct Meta;
      friend class Registry;
      friend struct MetaConst;
      friend struct MetaData;
      const MetaConst* mMeta {};

   public:
      constexpr CMeta() noexcept = default;
      constexpr CMeta(const MetaConst* raw) noexcept
         : mMeta {raw} {}

      constexpr Token GetToken() const noexcept;
      constexpr Hash GetHash() const noexcept;

      const MetaConst* operator -> () const noexcept { return mMeta; }
      constexpr explicit operator bool() const noexcept { return mMeta != nullptr; }

      constexpr bool operator == (const CMeta&) const noexcept;
   };
   

   ///                                                                        
   /// Definition interchange format for any of the above                     
   ///                                                                        
   struct LANGULUS_API(RTTI) AMeta {
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

   protected:
      friend struct Meta;
      friend class Registry;
      const Meta* mMeta {};

   public:
      constexpr AMeta() noexcept = default;
      constexpr AMeta(const Meta* raw) noexcept
         : mMeta {raw} {}
      constexpr AMeta(const DMeta&) noexcept;
      constexpr AMeta(const TMeta&) noexcept;
      constexpr AMeta(const VMeta&) noexcept;
      constexpr AMeta(const CMeta&) noexcept;

      constexpr Token GetToken() const noexcept;
      constexpr Hash GetHash() const noexcept;

      const Meta* operator -> () const noexcept { return mMeta; }
      constexpr explicit operator bool() const noexcept { return mMeta != nullptr; }
      constexpr operator DMeta() const noexcept;
      constexpr operator TMeta() const noexcept;
      constexpr operator CMeta() const noexcept;
      constexpr operator VMeta() const noexcept;

      constexpr bool operator == (const VMeta&) const noexcept;
      constexpr bool operator == (const TMeta&) const noexcept;
      constexpr bool operator == (const CMeta&) const noexcept;

      constexpr bool operator == (const DMeta&) const noexcept;
      constexpr bool operator &= (const DMeta&) const noexcept;
      constexpr bool operator |= (const DMeta&) const noexcept;

      constexpr bool operator == (const AMeta&) const noexcept;
      constexpr bool operator &= (const AMeta&) const noexcept;
      constexpr bool operator |= (const AMeta&) const noexcept;

      template<class T>
      NOD() constexpr T As() const noexcept;

      constexpr Token Kind() const noexcept;
   };


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

      /// Custom operator required, to avoid const members                    
      Meta& operator = (const Meta& rhs) noexcept {
         mInfo = rhs.mInfo;
         mCppName = rhs.mCppName;
         mVersionMajor = rhs.mVersionMajor;
         mVersionMinor = rhs.mVersionMinor;
         IF_LANGULUS_MANAGED_MEMORY(mLibraryName = rhs.mLibraryName);
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
   };

} // namespace Langulus::RTTI

namespace Langulus::CT
{

   /// Concept for meta definitions                                           
   template<class...T>
   concept Meta = ((SameAsOneOf<T,
         RTTI::AMeta, RTTI::DMeta, RTTI::TMeta, RTTI::VMeta, RTTI::CMeta>
      ) and ...);

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
      auto format(T const& value, CONTEXT& ctx) {
         using namespace Langulus;
         auto& denseValue = DenseCast(value);
         for (auto& constant : T::CTTI_NamedValues) {
            if (T {constant.mValue} != denseValue)
               continue;

            return fmt::format_to(ctx.out(), "{}", constant.mToken);
         }

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
      auto format(T const& e, CONTEXT& ctx) {
         return fmt::format_to(ctx.out(), "{}", e.GetToken());
      }
   };

} // namespace fmt