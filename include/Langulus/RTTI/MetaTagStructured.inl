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
   constexpr MetaTagPacked_16::MetaTagPacked_16(nullptr_t) noexcept
      : Base {0} {}

   constexpr MetaTagPacked_16::MetaTagPacked_16(DefinitionTag const* d) noexcept
      : Base {d ? d->mID : 0} {}

   constexpr auto MetaTagPacked_16::operator = (nullptr_t)
   noexcept -> MetaTagPacked_16& {
      Base::operator = (0);
      return *this;
   }

   constexpr auto MetaTagPacked_16::operator = (DefinitionTag const* d)
   noexcept -> MetaTagPacked_16& {
      Base::operator = (d ? d->mID : 0);
      return *this;
   }

   constexpr bool MetaTagPacked_16::operator == (const MetaTagPacked_16& rhs) const noexcept {
      return Base::operator == (rhs);
   }
 
   /// Get the name of the tag, the result of NameOf                          
   inline auto MetaTagPacked_16::GetName() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaTagByID(id)->mNameOf;
      return DefinitionTag::InvalidName;
   }

   /// Get the info of the tag, the result of InfoOf                          
   inline auto MetaTagPacked_16::GetInfo() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaTagByID(id)->mInfoOf;
      return {};
   }

   /// Get the C++ name of the tag, the result of CppNameOf                   
   inline auto MetaTagPacked_16::GetCppName() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaTagByID(id)->mCppNameOf;
      return {};
   }

   /// Get the tag hash                                                       
   inline auto MetaTagPacked_16::GetHash() const noexcept -> Hash {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaTagByID(id)->mHash;
      return {};
   }

   /// Get the major version                                                  
   inline auto MetaTagPacked_16::GetVersionMajor() const noexcept -> uint {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaTagByID(id)->mVersionMajor;
      return 0;
   }

   /// Get the minor version                                                  
   inline auto MetaTagPacked_16::GetVersionMinor() const noexcept -> uint {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaTagByID(id)->mVersionMinor;
      return 0;
   }

#if LANGULUS(SAFE)
   inline MetaTagPacked_16::operator bool() const noexcept {
      if (Base::operator bool()) {
         LglsAssert(Registry::GetMetaTagByID(Base::GetID()),
            "Valid meta with invalid definition");
         return true;
      }
      return false;
   }
#endif
}
