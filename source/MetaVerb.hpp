///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "MetaData.hpp"
#include <unordered_set>


namespace Langulus::RTTI
{

   /// The default verb execution functor													
   using FDefaultVerbMutable = bool (*)(Anyness::Block&, Flow::Verb&);
   using FDefaultVerbConstant = bool (*)(const Anyness::Block&, Flow::Verb&);
   using FStatelessVerb = bool (*)(Flow::Verb&);
   using AbleList = ::std::unordered_set<DMeta>;


   ///                                                                        
   ///   Meta verb                                                            
   ///                                                                        
   struct MetaVerb final : public Meta {
      LANGULUS(NAME) "VMeta";
      LANGULUS_BASES(Meta);

      static constexpr Token DefaultToken = "NoVerb";

      MetaType GetMetaType() const noexcept final { return Meta::Verb; }

      // Verbs have antonyms, denoted via this 'negative' token         
      // For example, 'Destroy' is the reverse of 'Create'              
      // This is just syntax sugar - reverse token just does mass *= -1 
      Token mTokenReverse;

      // Verbs can be tokenized as operators - just syntax sugar        
      Token mOperator;
      Token mOperatorReverse;

      // Verb's reflected precedence                                    
      Real mPrecedence {};

      // Reflected default verb for mutable context, if available       
      FDefaultVerbMutable mDefaultInvocationMutable {};
      // Reflected default verb for immutable context, if available     
      FDefaultVerbConstant mDefaultInvocationConstant {};

      // Reflected stateless verb, if available                         
      FStatelessVerb mStatelessInvocation {};

      // A set of data types that are capable of doing the verb (TODO)  
      AbleList mAble;

   public:
      template<CT::Void T>
      NOD() static constexpr VMeta Of();
      template<CT::Decayed T>
      NOD() static VMeta Of();
      
      NOD() bool Is(VMeta) const noexcept;
      template<CT::Data T>
      NOD() bool Is() const;

      bool operator == (const MetaVerb&) const noexcept;

   protected:
      template<CT::Data T>
      static constexpr Token GetReflectedPositiveVerbToken() noexcept;
      template<CT::Data T>
      static constexpr Token GetReflectedNegativeVerbToken() noexcept;
      template<CT::Data T>
      static constexpr Token GetReflectedPositiveVerbOperator() noexcept;
      template<CT::Data T>
      static constexpr Token GetReflectedNegativeVerbOperator() noexcept;
      template<CT::Data T>
      static constexpr Hash GetVerbHash() noexcept;
   };

} // namespace Langulus::RTTI


namespace Langulus::CT
{

   /// Checks if a verb is defaultable in a mutable context                   
   template<class T>
   concept DefaultableVerbMutable = requires {
      RTTI::FDefaultVerbMutable {&T::ExecuteDefault};
   };

   /// Checks if a verb is defaultable in an immutable context                
   template<class T>
   concept DefaultableVerbConstant = requires {
      RTTI::FDefaultVerbConstant {&T::ExecuteDefault};
   };

   /// Checks if a verb is defaultable in any context                         
   template<class T>
   concept DefaultableVerb = DefaultableVerbMutable<T> or DefaultableVerbConstant<T>;

   /// Checks if a verb is executable as stateless                            
   template<class T>
   concept StatelessVerb = requires {{
      T::ExecuteStateless(
         Fake<::Langulus::Flow::Verb&>()
      )} -> CT::Same<bool>;
   };

} // namespace Langulus::CT
