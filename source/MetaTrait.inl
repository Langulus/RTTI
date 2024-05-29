///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "MetaTrait.hpp"

#define VERBOSE(...) //Logger::Verbose("RTTI: ", __VA_ARGS__)


namespace Langulus::RTTI
{

   /// Get the reflected token for a type                                     
   /// If type wasn't reflected with LANGULUS(NAME), then the original C++    
   /// name will be used                                                      
   ///   @return the token                                                    
   template<CT::Data T>
   consteval Token MetaTrait::GetReflectedToken() noexcept {
      if constexpr (requires { T::CTTI_Trait; })
         return T::CTTI_Trait;
      else
         return CppNameOf<T>();
   }

   /// Get the meta definition of a void trait                                
   ///   @return always nullptr                                               
   template<CT::Void T>
   consteval TMeta MetaTrait::Of() {
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
      static_assert(sizeof(T) > 0, "Can't reflect an incomplete type");

      constexpr auto token = GetReflectedToken<T>();
      static_assert(token != "", "Invalid trait token is not allowed");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get the definition, type might have been reflected   
         // previously in another library. Unfortunately we can't keep  
         // a static pointer to the meta, because forementioned library 
         // might be reloaded, and thus produce new pointer.            
         TMeta meta = Instance.GetMetaTrait(token, RTTI::Boundary);
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
         meta = Instance.RegisterTrait(token, RTTI::Boundary);
         MetaTrait& generated = const_cast<MetaTrait&>(*meta);
      #else
         meta = ::std::make_unique<MetaTrait>(token);
         MetaTrait& generated = *const_cast<MetaTrait*>(meta.get());
      #endif

      // Type is implicitly reflected, so let's do our best             
      LANGULUS_ASSERT(generated.mToken == token, Meta, "Token not set");
      LANGULUS_ASSERT(generated.mHash == HashOf(token), Meta, "Hash not set");

      if constexpr (requires { T::CTTI_Info; })
         generated.mInfo = T::CTTI_Info;

      generated.mCppName = CppNameOf<T>();

      if constexpr (requires { T::CTTI_VersionMajor; })
         generated.mVersionMajor = T::CTTI_VersionMajor;
      if constexpr (requires { T::CTTI_VersionMinor; })
         generated.mVersionMinor = T::CTTI_VersionMinor;

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

#undef VERBOSE
