///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "MetaVerb.hpp"

#define VERBOSE(...) //Logger::Verbose("RTTI: ", __VA_ARGS__)


namespace Langulus::RTTI
{

   constexpr Token VMeta::GetToken() const noexcept {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      return mMeta ? mMeta->GetShortestUnambiguousToken() : MetaVerb::DefaultToken;
   #else
      return mMeta ? mMeta->mToken : MetaVerb::DefaultToken;
   #endif
   }

   constexpr Hash VMeta::GetHash() const noexcept {
      return mMeta ? mMeta->mHash : Hash {};
   }

   constexpr VMeta::operator AMeta() const noexcept {
      return mMeta;
   }

   constexpr bool VMeta::operator == (const VMeta& rhs) const noexcept {
      return mMeta == rhs.mMeta or (mMeta and mMeta->Is(rhs));
   }

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
   constexpr Token MetaVerb::GetReflectedPositiveVerbToken() noexcept {
      if constexpr (requires { T::CTTI_Verb; })
         return T::CTTI_Verb;
      else if constexpr (requires { T::CTTI_PositiveVerb; }) {
         if constexpr (!requires { T::CTTI_NegativeVerb; }) {
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
   constexpr Token MetaVerb::GetReflectedNegativeVerbToken() noexcept {
      if constexpr (requires { T::CTTI_Verb; })
         return T::CTTI_Verb;
      else if constexpr (requires { T::CTTI_NegativeVerb; }) {
         if constexpr (!requires { T::CTTI_PositiveVerb; }) {
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
   constexpr Token MetaVerb::GetReflectedPositiveVerbOperator() noexcept {
      if constexpr (requires { T::CTTI_Operator; })
         return T::CTTI_Operator;
      else if constexpr (requires { T::CTTI_PositiveOperator; }) {
         if constexpr (!requires { T::CTTI_NegativeOperator; }) {
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
   constexpr Token MetaVerb::GetReflectedNegativeVerbOperator() noexcept {
      if constexpr (requires { T::CTTI_Operator; })
         return T::CTTI_Operator;
      else if constexpr (requires { T::CTTI_NegativeOperator; }) {
         if constexpr (!requires { T::CTTI_PositiveOperator; }) {
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
   constexpr Hash MetaVerb::GetVerbHash() noexcept {
      const auto name = MetaVerb::GetReflectedPositiveVerbToken<T>();
      return HashBytes(name.data(), static_cast<int>(name.size()));
   }

   /// Meta of a void always returns nullptr                                  
   ///   @return nullptr                                                      
   template<CT::Void T>
   constexpr VMeta MetaVerb::Of() {
      return nullptr;
   }

   /// Reflect or return an already reflected verb definition                 
   /// Reflection is done only on decayed types to avoid static variable      
   /// duplications                                                           
   ///   @tparam T - the type to reflect (will always be decayed)             
   template<CT::Decayed T>
   LANGULUS(NOINLINE)
   VMeta MetaVerb::Of() {
      // This check is not standard, but doesn't hurt afaik             
      static_assert(sizeof(T) > 0,
         "Can't reflect an incomplete type");
      static_assert(not GetReflectedPositiveVerbToken<T>().empty(),
         "Invalid verb token is not allowed");
      static_assert(not GetReflectedNegativeVerbToken<T>().empty(),
         "Invalid verb token is not allowed");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get the definition, type might have been reflected   
         // previously in another library. Unfortunately we can't keep  
         // a static pointer to the meta, because forementioned library 
         // might be reloaded, and thus produce new pointer.            
         VMeta meta = Instance.GetMetaVerb(
            MetaVerb::GetReflectedPositiveVerbToken<T>(),
            RTTI::Boundary
         );
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
            MetaVerb::GetReflectedPositiveVerbToken<T>(),
            MetaVerb::GetReflectedNegativeVerbToken<T>(),
            MetaVerb::GetReflectedPositiveVerbOperator<T>(),
            MetaVerb::GetReflectedNegativeVerbOperator<T>(),
            RTTI::Boundary
         );
         MetaVerb& generated = *const_cast<MetaVerb*>(meta.mMeta);
      #else
         meta = ::std::make_unique<MetaVerb>();
         MetaVerb& generated = *const_cast<MetaVerb*>(meta.get());
      #endif

      // We'll try to explicitly or implicitly reflect it               
      if constexpr (CT::Reflectable<T>) {
         // The verb is explicitly reflected with a custom function     
         // Let's call it...                                            
         generated = T::Reflect();
      }
      else {
         // Verb is implicitly reflected, so let's do our best          
         generated.mToken = 
            MetaVerb::GetReflectedPositiveVerbToken<T>();
         generated.mTokenReverse = 
            MetaVerb::GetReflectedNegativeVerbToken<T>();

         generated.mOperator =
            MetaVerb::GetReflectedPositiveVerbOperator<T>();
         generated.mOperatorReverse =
            MetaVerb::GetReflectedNegativeVerbOperator<T>();

         // Reflect info string if any                                  
         if constexpr (requires { T::CTTI_Info; })
            generated.mInfo = T::CTTI_Info;

         generated.mCppName = CppNameOf<T>();
         generated.mHash = GetVerbHash<T>();

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
      }

      IF_LANGULUS_MANAGED_REFLECTION(generated.mLibraryName = RTTI::Boundary);

      if (generated.mOperator.size()) {
         const auto op1 = IsolateOperator(generated.mOperator);
         VERBOSE("Operator ", Logger::Push, Logger::DarkGreen, op1,
            Logger::Pop, Logger::Green, " registered (", generated.mLibraryName, ")");
      }

      if (generated.mOperatorReverse.size() and generated.mOperatorReverse != generated.mOperator) {
         const auto op2 = IsolateOperator(generated.mOperatorReverse);
         VERBOSE("Operator ", Logger::Push, Logger::DarkGreen, op2,
            Logger::Pop, Logger::Green, " registered (", generated.mLibraryName, ")");
      }

      VERBOSE("Verb ", Logger::Push, Logger::DarkGreen,
         generated.mToken, "/", generated.mTokenReverse,
         Logger::Pop, Logger::Green, " registered (", generated.mLibraryName, ")");

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
   template<CT::Data T>
   LANGULUS(INLINED)
   bool MetaVerb::Is() const {
      return Is(MetaVerb::Of<T>());
   }

} // namespace Langulus::RTTI

#ifdef VERBOSE
   #undef VERBOSE
#endif