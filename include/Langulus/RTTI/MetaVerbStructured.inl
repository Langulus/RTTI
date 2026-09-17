///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once

#if not LANGULUS_FEATURE(MANAGED_REFLECTION)
   #error "This file shouldn't be included if MANAGED_REFLECTION feature is disabled"
#endif


namespace Langulus::RTTI::Inner
{
   template<uint ID_SIZE>
   constexpr MetaVerbStructured_X8<ID_SIZE>::MetaVerbStructured_X8(nullptr_t) noexcept
      : Base {0} {}

   template<uint ID_SIZE>
   constexpr MetaVerbStructured_X8<ID_SIZE>::MetaVerbStructured_X8(DefinitionVerb const* d) noexcept
      : Base {d ? d->mID : 0} {
      if (d) {
         reversible = (d->mNameOfReverse != "");
         contextless = d->mCurrentBoundary.mContextless != nullptr;
      }
   }

   template<uint ID_SIZE>
   constexpr auto MetaVerbStructured_X8<ID_SIZE>::operator = (nullptr_t)
   noexcept -> MetaVerbStructured_X8& {
      Base::operator = (0);
      return *this;
   }

   template<uint ID_SIZE>
   constexpr auto MetaVerbStructured_X8<ID_SIZE>::operator = (DefinitionVerb const* d)
   noexcept -> MetaVerbStructured_X8& {
      Base::operator = (d ? d->mID : 0);

      if (d) {
         reversible = (d->mNameOfReverse != "");
         contextless = d->mCurrentBoundary.mContextless != nullptr;
      }
      return *this;
   }

   template<uint ID_SIZE>
   constexpr bool MetaVerbStructured_X8<ID_SIZE>::operator == (const MetaVerbStructured_X8& rhs) const noexcept {
      return Base::operator == (rhs);
   }
   
   /// Get the C++ name of the verb                                           
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetCppName() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mCppNameOf;
      return {};
   }
   
   /// Get any reflected information about the verb                           
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetInfo() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mInfoOf;
      return {};
   }
   
   /// Get the verb's hash                                                    
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetHash() const noexcept -> Hash {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mHash;
      return {};
   }
   
   /// Get major verb version                                                 
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetVersionMajor() const noexcept -> uint {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mVersionMajor;
      return 0;
   }

   /// Get minor verb version                                                 
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetVersionMinor() const noexcept -> uint {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mVersionMinor;
      return 0;
   }
   
   /// Get the reflected boundaries                                           
   template<uint ID_SIZE> auto MetaVerbStructured_X8<ID_SIZE>::GetBoundaries()
   const noexcept -> Definition::BoundarySet const& {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mBoundaries;
      static const Definition::BoundarySet fallback;
      return fallback;
   }


   /// Get the positive verb token                                            
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetPositiveName() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mNameOf;
      return DefinitionVerb::InvalidName;
   }

   /// Get the negative verb token, a.k.a. the antonym                        
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetNegativeName() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mNameOfReverse;
      return DefinitionVerb::InvalidName;
   }

   /// Get the positive reflected operator token                              
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetPositiveOperator() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mOperator;
      return {};
   }

   /// Get the negative reflected operator token                              
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetNegativeOperator() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mOperatorReverse;
      return {};
   }

   /// Get the default reflected precedence for the verb                      
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetPrecedence() const noexcept -> float {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaVerbByID(id)->mPrecedence;
      return 0;
   }

   /// Get the contextless execution routine if such was defined              
   template<uint ID_SIZE>
   auto MetaVerbStructured_X8<ID_SIZE>::GetContextless() const noexcept -> DefinitionVerb::FContextless {
      return contextless
         ? Registry::GetMetaVerbByID(Base::GetID())->mCurrentBoundary.mContextless
         : nullptr;
   }

   /// Check if the verb has a negative token defined                         
   template<uint ID_SIZE>
   constexpr bool MetaVerbStructured_X8<ID_SIZE>::IsReversible() const noexcept {
      return reversible;
   }

   /// Check if the verb provides a contextless execution routine             
   template<uint ID_SIZE>
   constexpr bool MetaVerbStructured_X8<ID_SIZE>::IsContextless() const noexcept {
      return contextless;
   }

#if LANGULUS(SAFE)
   template<uint ID_SIZE>
   MetaVerbStructured_X8<ID_SIZE>::operator bool() const noexcept {
      if (Base::operator bool()) {
         LglsAssert(Registry::GetMetaVerbByID(Base::GetID()),
            "Valid meta with invalid definition");
         return true;
      }
      return false;
   }
#endif
}
