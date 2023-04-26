///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "MetaTrait.hpp"

namespace Langulus::RTTI
{
   
   /// Get the reflected token for a type                                     
   ///   @return the token                                                    
   template<CT::Data T>
   constexpr Token MetaTrait::GetReflectedToken() noexcept {
      if constexpr (requires { T::CTTI_Trait; })
         return T::CTTI_Trait;
      else
         return NameOf<T>();
   }

   template<CT::Void T>
   constexpr TMeta MetaTrait::Of() requires CT::Decayed<T> {
      return nullptr;
   }

   /// Reflect or return an already reflected type meta trait definition      
   /// Reflection is done only on decayed types to avoid static variable      
   /// duplications                                                           
   ///   @tparam T - the type to reflect (will always be decayed)             
   template<CT::Data T>
   TMeta MetaTrait::Of() requires CT::Decayed<T> {
      // This check is not standard, but doesn't hurt afaik             
      static_assert(sizeof(T) > 0, "Can't reflect an incomplete type");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         static constinit TMeta meta;
      #else
         static constinit ::std::unique_ptr<MetaTrait> meta;
      #endif

      // Never proceed with reflection, if already reflected            
      if (meta) {
         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            return meta;
         #else
            return meta.get();
         #endif
      }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get the definition, type might have been reflected   
         // previously in another translation unit. This is available   
         // only if MANAGED_REFLECTION feature is enabled               
         meta = Database.GetMetaTrait(GetReflectedToken<T>());
         if (meta)
            return meta;
      #endif

      // If this is reached, then trait is not defined yet              
      // We immediately place it in the static here, because the        
      // reflection function might end up forever looping otherwise     
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         meta = Database.RegisterTrait(GetReflectedToken<T>());
      #else
         meta = ::std::make_unique<MetaTrait>();
      #endif

      // We'll try to explicitly or implicitly reflect it               
      if constexpr (CT::Reflectable<T>) {
         // The trait is explicitly reflected with a custom function    
         // Let's call it...                                            
         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            *const_cast<MetaTrait*>(meta) = T::Reflect();
         #else
            *const_cast<MetaTrait*>(meta.get()) = T::Reflect();
         #endif
      }
      else {
         // Type is implicitly reflected, so let's do our best          
         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            MetaTrait& generated = *const_cast<MetaTrait*>(meta);
         #else
            MetaTrait& generated = *const_cast<MetaTrait*>(meta.get());
         #endif

         generated.mToken = GetReflectedToken<T>();
         if constexpr (requires { T::CTTI_Info; })
            generated.mInfo = T::CTTI_Info;
         generated.mCppName = NameOf<T>();
         generated.mHash = Meta::GenerateHash<T>(GetReflectedToken<T>());
         if constexpr (requires { T::CTTI_VersionMajor; })
            generated.mVersionMajor = T::CTTI_VersionMajor;
         if constexpr (requires { T::CTTI_VersionMinor; })
            generated.mVersionMinor = T::CTTI_VersionMinor;
      }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         const_cast<MetaTrait*>(meta)->mLibraryName = RTTI::Library;
         return meta;
      #else
         return meta.get();
      #endif
   }

   /// Check if two meta definitions match exactly                            
   ///   @param other - the trait to compare against                          
   ///   @return true if traits match                                         
   constexpr bool MetaTrait::Is(TMeta other) const noexcept {
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // This function is reduced to a pointer match, if the meta    
         // database is centralized, because it guarantees that         
         // definitions in separate translation units are always the    
         // same instance                                               
         return this == other;
      #else
         return other && mHash == other->mHash && mToken == other->mToken;
      #endif
   }	
   
   /// Check if two meta definitions match exactly                            
   ///   @tparam T - the trait to compare against                             
   ///   @return true if traits match                                         
   template<CT::Data T>
   constexpr bool MetaTrait::Is() const {
      return Is(MetaTrait::Of<T>());
   }
   
   constexpr bool MetaTrait::operator == (const MetaTrait& rhs) const noexcept {
      return Is(&rhs);
   }

} // namespace Langulus::RTTI
