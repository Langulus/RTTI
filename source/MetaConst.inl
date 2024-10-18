///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "MetaConst.hpp"

#if 0
   #define VERBOSE(...)      Logger::Verbose(__VA_ARGS__)
   #define VERBOSE_TAB(...)  const auto tab = Logger::VerboseTab(__VA_ARGS__)
#else
   #define VERBOSE(...)      LANGULUS(NOOP)
   #define VERBOSE_TAB(...)  LANGULUS(NOOP)
#endif


namespace Langulus::RTTI
{

   /// Get the reflected token for a constant                                 
   /// The type has to be reflected with LANGULUS(CONSTANT) "name"            
   ///   @return the token                                                    
   template<CT::Data T>
   consteval Token MetaConst::GetReflectedToken() noexcept {
      if constexpr (requires { T::CTTI_Constant; })
         return T::CTTI_Constant;
      else
         static_assert(false, "Type is not a constant definition");
   }

   /// Reflect or return an already reflected type meta const definition      
   /// Reflection is done only on decayed types to avoid static variable      
   /// duplications                                                           
   ///   @tparam T - the type to reflect (will always be decayed)             
   template<CT::Decayed T>
   CMeta MetaConst::Of() {
      static_assert(not CT::Function<T>,
         "Can't reflect this function signature as a constant");
      static_assert(CT::Complete<T>,
         "Can't reflect incomplete constant - "
         "make sure you have included the corresponding headers "
         "before the point of reflection. "
         "This could also be triggered due to an incomplete member in T");
      static_assert(CT::Reflectable<T>,
         "Can't reflect constant that was explicitly marked unreflectable");
      static_assert(not requires { T::CTTI_Trait; },
         "Can't reflect trait as constant");
      static_assert(not requires { T::CTTI_Name; },
         "Can't reflect data as constant");
      static_assert(not requires { T::CTTI_Verb; }
      and not requires { T::CTTI_PositiveVerb; }
      and not requires { T::CTTI_NegativeVerb; },
         "Can't reflect verb as constant");

      constexpr auto token = GetReflectedToken<T>();
      static_assert(token != "", "Invalid constant token is not allowed");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get the definition, type might have been reflected   
         // previously in another library. Unfortunately we can't keep  
         // a static pointer to the meta, because forementioned library 
         // might be reloaded, and thus produce new pointer.            
         CMeta meta = Instance.GetMetaConstant(token, RTTI::Boundary);
         if (meta)
            return meta;
      #else
         // Keep a static meta pointer for each translation unit        
         static constinit ::std::unique_ptr<MetaConst> meta;
         if (meta)
            return meta.get();
      #endif

      // If this is reached, then constant is not defined yet           
      // We immediately place it in the static here, because the        
      // reflection function might end up forever looping otherwise     
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         meta = Instance.RegisterConstant(token, RTTI::Boundary);
         MetaConst& generated = const_cast<MetaConst&>(*meta);
      #else
         meta = ::std::make_unique<MetaConst>(token);
         MetaConst& generated = *const_cast<MetaConst*>(meta.get());
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

      // Refer to a local copy of the data                              
      static const auto staticInstance = T::Value;
      generated.mValueType  = MetaData::Of<decltype(T::Value)>();
      generated.mPtrToValue = &staticInstance;

      VERBOSE("Constant ", Logger::PushYellow, generated.mToken,
         Logger::PopGreen, " registered (", generated.mLibraryName, ")");
      return &generated;
   }
      
   /// Check if two meta definitions match exactly                            
   ///   @param other - the type to compare against                           
   ///   @return true if types match                                          
   LANGULUS(INLINED)
   constexpr bool MetaConst::Is(CMeta other) const noexcept {
      return other and mHash == other->mHash and mToken == other->mToken;
   }

} // namespace Langulus::RTTI

#undef VERBOSE
#undef VERBOSE_TAB
