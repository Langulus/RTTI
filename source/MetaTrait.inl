///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "MetaTrait.hpp"

#define VERBOSE(...) //Logger::Verbose("RTTI: ", __VA_ARGS__)


namespace Langulus::RTTI
{

   constexpr Token TMeta::GetToken() const noexcept {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      return mMeta ? mMeta->GetShortestUnambiguousToken() : MetaTrait::DefaultToken;
   #else
      return mMeta ? mMeta->mToken : MetaTrait::DefaultToken;
   #endif
   }

   constexpr Hash TMeta::GetHash() const noexcept {
      return mMeta ? mMeta->mHash : Hash {};
   }

   constexpr TMeta::operator AMeta() const noexcept {
      return mMeta;
   }

   constexpr bool TMeta::operator == (const TMeta& rhs) const noexcept {
      return mMeta == rhs.mMeta or (mMeta and mMeta->Is(rhs));
   }

   /// Get the reflected token for a type                                     
   /// If type wasn't reflected with LANGULUS(NAME), then the original C++    
   /// name will be used                                                      
   ///   @return the token                                                    
   template<CT::Data T>
   constexpr Token MetaTrait::GetReflectedToken() noexcept {
      if constexpr (requires { T::CTTI_Trait; })
         return T::CTTI_Trait;
      else
         return CppNameOf<T>();
   }

   /// Get the meta definition of a void trait                                
   ///   @return always nullptr                                               
   template<CT::Void T>
   constexpr TMeta MetaTrait::Of() {
      return nullptr;
   }

   /// Reflect or return an already reflected type meta trait definition      
   /// Reflection is done only on decayed types to avoid static variable      
   /// duplications                                                           
   ///   @tparam T - the type to reflect (will always be decayed)             
   template<CT::Decayed T>
   LANGULUS(NOINLINE)
   TMeta MetaTrait::Of() {
      // This check is not standard, but doesn't hurt afaik             
      static_assert(sizeof(T) > 0,
         "Can't reflect an incomplete type");
      static_assert(not GetReflectedToken<T>().empty(),
         "Invalid trait token is not allowed");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get the definition, type might have been reflected   
         // previously in another library. Unfortunately we can't keep  
         // a static pointer to the meta, because forementioned library 
         // might be reloaded, and thus produce new pointer.            
         TMeta meta = Instance.GetMetaTrait(GetReflectedToken<T>(), RTTI::Boundary);
         if (meta)
            return meta;
      #else
         // Keep a static meta pointer for each translation unit        
         static constinit ::std::unique_ptr<MetaTrait> meta;
         if (meta)
            return meta.get();
      #endif

      // If this is reached, then trait is not defined yet              
      // We immediately place it in the static here, because the        
      // reflection function might end up forever looping otherwise     
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         meta = Instance.RegisterTrait(GetReflectedToken<T>(), RTTI::Boundary);
         MetaTrait& generated = *const_cast<MetaTrait*>(meta.mMeta);
      #else
         meta = ::std::make_unique<MetaTrait>();
         MetaTrait& generated = *const_cast<MetaTrait*>(meta.get());
      #endif

      // We'll try to explicitly or implicitly reflect it               
      if constexpr (CT::Reflectable<T>) {
         // The trait is explicitly reflected with a custom function    
         // Let's call it...                                            
         generated = T::Reflect();
      }
      else {
         // Type is implicitly reflected, so let's do our best          
         generated.mToken = GetReflectedToken<T>();
         if constexpr (requires { T::CTTI_Info; })
            generated.mInfo = T::CTTI_Info;
         generated.mCppName = CppNameOf<T>();
         generated.mHash = Meta::GenerateHash<T>(GetReflectedToken<T>());
         if constexpr (requires { T::CTTI_VersionMajor; })
            generated.mVersionMajor = T::CTTI_VersionMajor;
         if constexpr (requires { T::CTTI_VersionMinor; })
            generated.mVersionMinor = T::CTTI_VersionMinor;
      }

      IF_LANGULUS_MANAGED_REFLECTION(generated.mLibraryName = RTTI::Boundary);

      VERBOSE("Trait ", Logger::Push, Logger::Purple, generated.mToken,
         Logger::Pop, Logger::Green, " registered (", generated.mLibraryName, ")");
      return &generated;
   }

   /// Check if two meta definitions match exactly                            
   ///   @param other - the trait to compare against                          
   ///   @return true if traits match                                         
   LANGULUS(INLINED)
   bool MetaTrait::Is(TMeta other) const noexcept {
      return other and mHash == other->mHash and mToken == other->mToken;
   }	
   
   /// Check if two meta definitions match exactly                            
   ///   @tparam T - the trait to compare against                             
   ///   @return true if traits match                                         
   template<CT::Data T>
   LANGULUS(INLINED)
   bool MetaTrait::Is() const {
      return Is(MetaTrait::Of<T>());
   }

} // namespace Langulus::RTTI

#ifdef VERBOSE
   #undef VERBOSE
#endif