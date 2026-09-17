///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once


namespace Langulus::RTTI::Inner
{
   /// Get the positive verb token                                            
   inline auto MetaVerbNaked::GetPositiveName() const noexcept -> Token {
      if (mDefinition)
         return mDefinition->mNameOf;
      return DefinitionVerb::InvalidName;
   }

   /// Get the negative verb token, a.k.a. the antonym                        
   inline auto MetaVerbNaked::GetNegativeName() const noexcept -> Token {
      if (mDefinition)
         return mDefinition->mNameOfReverse;
      return DefinitionVerb::InvalidName;
   }

   /// Get the positive reflected operator token                              
   inline auto MetaVerbNaked::GetPositiveOperator() const noexcept -> Token {
      if (mDefinition)
         return mDefinition->mOperator;
      return {};
   }

   /// Get the negative reflected operator token                              
   inline auto MetaVerbNaked::GetNegativeOperator() const noexcept -> Token {
      if (mDefinition)
         return mDefinition->mOperatorReverse;
      return {};
   }

   /// Get the default reflected precedence for the verb                      
   inline auto MetaVerbNaked::GetPrecedence() const noexcept -> float {
      return mDefinition ? mDefinition->mPrecedence : 0;
   }

   /// Get the contextless execution routine if such was defined              
   inline auto MetaVerbNaked::GetContextless() const noexcept -> DefinitionVerb::FContextless {
      if (mDefinition)
         return mDefinition->mCurrentBoundary.mContextless;
      return nullptr;
   }

   /// Check if the verb has a negative token defined                         
   constexpr bool MetaVerbNaked::IsReversible() const noexcept {
      if (mDefinition)
         return not mDefinition->mNameOfReverse.empty();
      return false;
   }

   /// Check if the verb provides a contextless execution routine             
   constexpr bool MetaVerbNaked::IsContextless() const noexcept {
      if (mDefinition)
         return mDefinition->mCurrentBoundary.mContextless != nullptr;
      return false;
   }
}
