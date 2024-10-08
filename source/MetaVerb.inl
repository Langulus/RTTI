///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "MetaVerb.hpp"

#if 0
   #define VERBOSE(...) Logger::Verbose("RTTI: ", __VA_ARGS__)
#else
   #define VERBOSE(...) LANGULUS(NOOP)
#endif


namespace Langulus::RTTI
{

   /// Isolate and lowercase an operator token                                
   ///   @param token - the operator                                          
   ///   @return the lowercased and isolated operator token                   
   LANGULUS(INLINED)
   Lowercase IsolateOperator(const Token& token) noexcept {
      // Skip skippable at the front and the back of token              
      auto l = token.data();
      auto r = token.data() + token.size();
      while (l < r and *l <= 32)
         ++l;

      while (r > l and *(r-1) <= 32)
         --r;

      // Lowercase the isolated token                                   
      return ToLowercase(token.substr(l - token.data(), r - l));
   }

   /// Get the reflected positive token for a verb                            
   ///   @return the token                                                    
   template<CT::Data T>
   consteval Token MetaVerb::GetReflectedPositiveVerbToken() noexcept {
      if constexpr (requires { T::CTTI_Verb; })
         return T::CTTI_Verb;
      else if constexpr (requires { T::CTTI_PositiveVerb; }) {
         if constexpr (not requires { T::CTTI_NegativeVerb; }) {
            LANGULUS_ERROR(
               "Positive verb defined, but no negative provided - "
               "either define the negative, or use LANGULUS(VERB) "
               "if both tokens are the same");
         }

         return T::CTTI_PositiveVerb;
      }
      else return CppNameOf<T>();
   }

   /// Get the reflected negative token for a verb                            
   ///   @return the token                                                    
   template<CT::Data T>
   consteval Token MetaVerb::GetReflectedNegativeVerbToken() noexcept {
      if constexpr (requires { T::CTTI_Verb; })
         return T::CTTI_Verb;
      else if constexpr (requires { T::CTTI_NegativeVerb; }) {
         if constexpr (not requires { T::CTTI_PositiveVerb; }) {
            LANGULUS_ERROR(
               "Negative verb defined, but no positive provided - "
               "either define the positive, or use LANGULUS(VERB) "
               "if both tokens are the same");
         }

         return T::CTTI_NegativeVerb;
      }
      else return CppNameOf<T>();
   }

   /// Get the reflected positive operator for a verb                         
   ///   @return the token                                                    
   template<CT::Data T>
   consteval Token MetaVerb::GetReflectedPositiveVerbOperator() noexcept {
      if constexpr (requires { T::CTTI_Operator; })
         return T::CTTI_Operator;
      else if constexpr (requires { T::CTTI_PositiveOperator; }) {
         if constexpr (not requires { T::CTTI_NegativeOperator; }) {
            LANGULUS_ERROR(
               "Positive operator defined, but no negative provided - "
               "either define the negative, or use LANGULUS(OPERATOR) "
               "if both operators are the same");
         }

         return T::CTTI_PositiveOperator;
      }
      else return {};
   }

   /// Get the reflected negative operator for a verb                         
   ///   @return the token                                                    
   template<CT::Data T>
   consteval Token MetaVerb::GetReflectedNegativeVerbOperator() noexcept {
      if constexpr (requires { T::CTTI_Operator; })
         return T::CTTI_Operator;
      else if constexpr (requires { T::CTTI_NegativeOperator; }) {
         if constexpr (not requires { T::CTTI_PositiveOperator; }) {
            LANGULUS_ERROR(
               "Negative operator defined, but no positive provided - "
               "either define the positive, or use LANGULUS(OPERATOR) "
               "if both operators are the same");
         }

         return T::CTTI_NegativeOperator;
      }
      else return {};
   }

   /// Get the constexpr hash of a verb                                       
   ///   @return the hash of the type                                         
   template<CT::Data T>
   consteval Hash MetaVerb::GetVerbHash() noexcept {
      const auto name = MetaVerb::GetReflectedPositiveVerbToken<T>();
      return HashBytes(name.data(), static_cast<int>(name.size()));
   }

   /// Meta of a void always returns nullptr                                  
   ///   @return nullptr                                                      
   template<CT::Void T>
   consteval VMeta MetaVerb::Of() {
      return nullptr;
   }

   /// Reflect or return an already reflected verb definition                 
   /// Reflection is done only on decayed types to avoid static variable      
   /// duplications                                                           
   ///   @tparam T - the type to reflect (will always be decayed)             
   template<CT::Decayed T> LANGULUS(NOINLINE)
   VMeta MetaVerb::Of() {
      static_assert(not CT::Function<T>,
         "Can't reflect this function signature as a verb");
      static_assert(CT::Complete<T>,
         "Can't reflect incomplete verb - "
         "make sure you have included the corresponding headers "
         "before the point of reflection. "
         "This could also be triggered due to an incomplete member in T");
      static_assert(CT::Reflectable<T>,
         "Can't reflect verb that was explicitly marked unreflectable");
      static_assert(not requires { T::CTTI_Constant; },
         "Can't reflect constant as verb");
      static_assert(not requires { T::CTTI_Trait; },
         "Can't reflect trait as verb");

      constexpr auto verbPos = GetReflectedPositiveVerbToken<T>();
      constexpr auto verbNeg = GetReflectedNegativeVerbToken<T>();

      static_assert(verbPos != "", "Invalid verb token is not allowed");
      static_assert(verbNeg != "", "Invalid verb token is not allowed");

      constexpr auto opPos = GetReflectedPositiveVerbOperator<T>();
      constexpr auto opNeg = GetReflectedNegativeVerbOperator<T>();

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get the definition, type might have been reflected   
         // previously in another library. Unfortunately we can't keep  
         // a static pointer to the meta, because forementioned library 
         // might be reloaded, and thus produce new pointer.            
         VMeta meta = Instance.GetMetaVerb(verbPos, RTTI::Boundary);
         if (meta)
            return meta;
      #else
         // Keep a static meta pointer for each translation unit        
         static constinit ::std::unique_ptr<MetaVerb> meta;
         if (meta)
            return meta.get();
      #endif

      // If this is reached, then verb is not defined yet               
      // We immediately place it in the static here, because the        
      // reflection function might end up forever looping otherwise     
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         meta = Instance.RegisterVerb(
            CppNameOf<T>(),
            verbPos, verbNeg, opPos, opNeg,
            RTTI::Boundary
         );
         auto& generated = const_cast<MetaVerb&>(*meta);
      #else
         meta = ::std::make_unique<MetaVerb>(
            verbPos, verbNeg, opPos, opNeg
         );
         auto& generated = *const_cast<MetaVerb*>(meta.get());
      #endif

      // Verb is implicitly reflected, so let's do our best             
      LANGULUS_ASSERT(generated.mToken == verbPos,
         Meta, "Token+ not set");
      LANGULUS_ASSERT(generated.mTokenReverse == verbNeg,
         Meta, "Token- not set");
      LANGULUS_ASSERT(generated.mOperator == opPos,
         Meta, "TokenOp+ not set");
      LANGULUS_ASSERT(generated.mOperatorReverse == opNeg,
         Meta, "TokenOp- not set");
      LANGULUS_ASSERT(generated.mHash == HashOf(verbPos),
         Meta, "Hash not set");

      // Reflect info string if any                                     
      if constexpr (requires { T::CTTI_Info; })
         generated.mInfo = T::CTTI_Info;

      generated.mCppName = CppNameOf<T>();

      // Reflect precedence                                             
      if constexpr (requires { T::CTTI_Precedence; })
         generated.mPrecedence = T::CTTI_Precedence;

      // Reflect version                                                
      if constexpr (requires { T::CTTI_VersionMajor; })
         generated.mVersionMajor = T::CTTI_VersionMajor;

      if constexpr (requires { T::CTTI_VersionMinor; })
         generated.mVersionMinor = T::CTTI_VersionMinor;

      if constexpr (CT::DefaultableVerbMutable<T>)
         generated.mDefaultInvocationMutable = FDefaultVerbMutable {&T::ExecuteDefault};

      if constexpr (CT::DefaultableVerbConstant<T>)
         generated.mDefaultInvocationConstant = FDefaultVerbConstant {&T::ExecuteDefault};

      if constexpr (CT::StatelessVerb<T>)
         generated.mStatelessInvocation = FStatelessVerb {T::ExecuteStateless};

      IF_LANGULUS_MANAGED_REFLECTION(generated.mLibraryName = RTTI::Boundary);

      if (generated.mOperator.size()) {
         const auto op1 = IsolateOperator(generated.mOperator);
         VERBOSE("Operator ", Logger::PushDarkGreen, op1,
            Logger::PopGreen, " registered (", generated.mLibraryName, ")");
      }

      if (generated.mOperatorReverse.size() and generated.mOperatorReverse != generated.mOperator) {
         const auto op2 = IsolateOperator(generated.mOperatorReverse);
         VERBOSE("Operator ", Logger::PushDarkGreen, op2,
            Logger::PopGreen, " registered (", generated.mLibraryName, ")");
      }

      VERBOSE("Verb ", Logger::PushDarkGreen,
         generated.mToken, "/", generated.mTokenReverse,
         Logger::PopGreen, " registered (", generated.mLibraryName, ")");

      return &generated;
   }

   /// Check if two meta definitions match exactly                            
   ///   @param other - the verb to compare against                           
   ///   @return true if verbs match                                          
   LANGULUS(INLINED)
   bool MetaVerb::Is(VMeta other) const noexcept {
      return other and mHash == other->mHash and mToken == other->mToken;
   }	
   
   /// Check if two meta definitions match exactly                            
   ///   @tparam T - the verb to compare against                              
   ///   @return true if verbs match                                          
   template<CT::Data T> LANGULUS(INLINED)
   bool MetaVerb::Is() const {
      return Is(MetaVerb::Of<T>());
   }

} // namespace Langulus::RTTI

#undef VERBOSE
