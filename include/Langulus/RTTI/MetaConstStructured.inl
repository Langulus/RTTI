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
   constexpr MetaConstPacked_16::MetaConstPacked_16(nullptr_t) noexcept
      : Base {0} {}

   constexpr MetaConstPacked_16::MetaConstPacked_16(DefinitionConst const* d) noexcept
      : Base {d ? d->mID : 0} {}

   constexpr auto MetaConstPacked_16::operator = (nullptr_t)
   noexcept -> MetaConstPacked_16& {
      Base::operator = (0);
      return *this;
   }

   constexpr auto MetaConstPacked_16::operator = (DefinitionConst const* d)
   noexcept -> MetaConstPacked_16& {
      Base::operator = (d ? d->mID : 0);
      return *this;
   }

   /// Get the name of the constant, the result of NameOf                     
   inline auto MetaConstPacked_16::GetName() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaConstByID(id)->mNameOf;
      return DefinitionConst::InvalidName;
   }

   /// Get the info of the constant, the result of NameOf                     
   inline auto MetaConstPacked_16::GetInfo() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaConstByID(id)->mInfoOf;
      return {};
   }

   /// Get the C++ name of the constant, the result of CppNameOf              
   inline auto MetaConstPacked_16::GetCppName() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaConstByID(id)->mCppNameOf;
      return {};
   }

   /// Get the constant's hash                                                
   inline auto MetaConstPacked_16::GetHash() const noexcept -> Hash {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaConstByID(id)->mHash;
      return {};
   }

   /// Get the major version                                                  
   inline auto MetaConstPacked_16::GetVersionMajor() const noexcept -> uint {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaConstByID(id)->mVersionMajor;
      return 0;
   }

   /// Get the minor version                                                  
   inline auto MetaConstPacked_16::GetVersionMinor() const noexcept -> uint {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaConstByID(id)->mVersionMinor;
      return 0;
   }

#if LANGULUS(SAFE)
   inline MetaConstPacked_16::operator bool() const noexcept {
      if (Base::operator bool()) {
         LglsAssert(Registry::GetMetaConstByID(Base::GetID()),
            "Valid meta with invalid definition");
         return true;
      }
      return false;
   }
#endif
}
