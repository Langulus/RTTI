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
      constexpr operator AMeta() const noexcept;

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
      constexpr operator AMeta() const noexcept;

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
      constexpr operator AMeta() const noexcept;

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
      constexpr operator AMeta() const noexcept;

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

      // Each reflection primitive has a unique token, but that         
      // uniqueness is checked only if MANAGED_REFLECTION feature is    
      // enabled                                                        
      Token mToken;
      // Each reflection may or may not have some info                  
      Token mInfo {"<no info provided>"};
      // Original name of the type                                      
      Token mCppName;
      // Each reflected type has an unique hash                         
      Hash mHash {};
      // Major version                                                  
      Count mVersionMajor {1};
      // Minor version                                                  
      Count mVersionMinor {0};
      // References, increased each time a definition is merged,        
      // and decreases each time a definition is unregistered           
      Count mReferences {1};

      NOD() const Hash& GetHash() const noexcept;

      template<CT::Data T>
      NOD() static constexpr Hash GenerateHash(const Token&) noexcept;

      virtual ~Meta() = default;

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

namespace std
{

   using ::Langulus::RTTI::AMeta;
   using ::Langulus::RTTI::DMeta;
   using ::Langulus::RTTI::TMeta;
   using ::Langulus::RTTI::VMeta;

   template<>
   struct hash<AMeta> {
      size_t operator()(AMeta) const noexcept;
   };

   template<>
   struct hash<DMeta> {
      size_t operator()(DMeta) const noexcept;
   };

   template<>
   struct hash<vector<DMeta>> {
      size_t operator()(const vector<DMeta>&) const noexcept;
   };

   template<>
   struct hash<TMeta> {
      size_t operator()(TMeta) const noexcept;
   };

   template<>
   struct hash<VMeta> {
      size_t operator()(VMeta) const noexcept;
   };

} // namespace std

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

      template<class CONTEXT>
      LANGULUS(INLINED)
      auto format(T const& value, CONTEXT& ctx) {
         using namespace Langulus;
         using DT = Decay<T>;
         auto& denseValue = DenseCast(value);
         for (auto& constant : DT::CTTI_NamedValues) {
            if (DT {constant.mValue} != denseValue)
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