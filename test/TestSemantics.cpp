///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Common.hpp"


///                                                                           
TEST_CASE("Testing SemanticOf", "[semantics]") {
   static_assert(CT::Exact<SemanticOf<int&&>, Moved<int>>);

   static_assert(CT::Exact<SemanticOf<const int&&>, Referred<const int>>);
   static_assert(CT::Exact<SemanticOf<int&>, Referred<int>>);
   static_assert(CT::Exact<SemanticOf<const int&>, Referred<const int>>);

   static_assert(CT::Exact<Desem<int&&>, int&&>);
   static_assert(CT::Exact<Desem<const int&&>, const int&&>);
   static_assert(CT::Exact<Desem<int&>, int&>);
   static_assert(CT::Exact<Desem<const int&>, const int&>);

   static_assert(CT::Exact<SemanticOf<Copied<int>>, Copied<int>>);
   static_assert(CT::Exact<SemanticOf<Copied<int>&>, Copied<int>>);
   static_assert(CT::Exact<SemanticOf<Copied<int>&&>, Copied<int>>);
   static_assert(CT::Exact<SemanticOf<const Copied<int>&>, Copied<int>>);

   static_assert(CT::Exact<SemanticOf<Referred<int>>, Referred<int>>);
   static_assert(CT::Exact<SemanticOf<Referred<int>&>, Referred<int>>);
   static_assert(CT::Exact<SemanticOf<Referred<int>&&>, Referred<int>>);
   static_assert(CT::Exact<SemanticOf<const Referred<int>&>, Referred<int>>);

   static_assert(CT::Exact<SemanticOf<Moved<int>>, Moved<int>>);
   static_assert(CT::Exact<SemanticOf<Moved<int>&>, Moved<int>>);
   static_assert(CT::Exact<SemanticOf<Moved<int>&&>, Moved<int>>);
   static_assert(CT::Exact<SemanticOf<const Moved<int>&>, Moved<int>>);

   static_assert(CT::Exact<SemanticOf<Abandoned<int>>, Abandoned<int>>);
   static_assert(CT::Exact<SemanticOf<Abandoned<int>&>, Abandoned<int>>);
   static_assert(CT::Exact<SemanticOf<Abandoned<int>&&>, Abandoned<int>>);
   static_assert(CT::Exact<SemanticOf<const Abandoned<int>&>, Abandoned<int>>);

   static_assert(CT::Exact<SemanticOf<Disowned<int>>, Disowned<int>>);
   static_assert(CT::Exact<SemanticOf<Disowned<int>&>, Disowned<int>>);
   static_assert(CT::Exact<SemanticOf<Disowned<int>&&>, Disowned<int>>);
   static_assert(CT::Exact<SemanticOf<const Disowned<int>&>, Disowned<int>>);

   static_assert(CT::Exact<SemanticOf<Cloned<int>>, Cloned<int>>);
   static_assert(CT::Exact<SemanticOf<Cloned<int>&>, Cloned<int>>);
   static_assert(CT::Exact<SemanticOf<Cloned<int>&&>, Cloned<int>>);
   static_assert(CT::Exact<SemanticOf<const Cloned<int>&>, Cloned<int>>);

   static_assert(CT::Exact<SemanticOf<Describe>, Describe>);
   static_assert(CT::Exact<SemanticOf<Describe&>, Describe>);
   static_assert(CT::Exact<SemanticOf<Describe&&>, Describe>);
   static_assert(CT::Exact<SemanticOf<const Describe&>, Describe>);
}

///                                                                           
TEST_CASE("Testing Desem", "[semantics]") {
   static_assert(CT::Exact<Desem<Copied<int>>, int>);
   static_assert(CT::Exact<Desem<Referred<int>>, int>);
   static_assert(CT::Exact<Desem<Moved<int>>, int>);
   static_assert(CT::Exact<Desem<Abandoned<int>>, int>);
   static_assert(CT::Exact<Desem<Disowned<int>>, int>);
   static_assert(CT::Exact<Desem<Cloned<int>>, int>);
   static_assert(CT::Exact<Desem<Describe>, Describe>);
}


///                                                                           
///   Refer semantics                                                         
///                                                                           
TEMPLATE_TEST_CASE("Testing refer-makable types", "[semantics]",
   ImplicitlyConstructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::ReferMakable<T>);
   static_assert(CT::SemanticMakable<Referred, T>);
   static_assert(CT::SemanticMakableAlt<Referred<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mReferConstructor);
}

TEMPLATE_TEST_CASE("Testing non-refer-makable types", "[semantics]",
   IncompleteType,
   NonDestructible
) {
   using T = TestType;
   static_assert(not CT::ReferMakable<T>);
   static_assert(not CT::SemanticMakable<Referred, T>);
   static_assert(not CT::SemanticMakableAlt<Referred<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mReferConstructor);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mReferConstructor);
   }
}

TEMPLATE_TEST_CASE("Testing refer-assignable types", "[semantics]",
   ImplicitlyConstructible,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::ReferAssignable<T>);
   static_assert(CT::SemanticAssignable<Referred, T>);
   static_assert(CT::SemanticAssignableAlt<Referred<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mReferAssigner);
}

TEMPLATE_TEST_CASE("Testing non-refer-assignable types", "[semantics]",
   IncompleteType
) {
   using T = TestType;
   static_assert(not CT::ReferAssignable<T>);
   static_assert(not CT::SemanticAssignable<Referred, T>);
   static_assert(not CT::SemanticAssignableAlt<Referred<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mReferAssigner);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mReferAssigner);
   }
}


///                                                                           
///   Move semantics                                                          
///                                                                           
TEMPLATE_TEST_CASE("Testing move-makable types", "[semantics]",
   ImplicitlyConstructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::MoveMakable<T>);
   static_assert(CT::SemanticMakable<Moved, T>);
   static_assert(CT::SemanticMakableAlt<Moved<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mMoveConstructor);
}

TEMPLATE_TEST_CASE("Testing non-move-makable types", "[semantics]",
   IncompleteType,
   NonDestructible
) {
   using T = TestType;
   static_assert(not CT::MoveMakable<T>);
   static_assert(not CT::SemanticMakable<Moved, T>);
   static_assert(not CT::SemanticMakableAlt<Moved<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mMoveConstructor);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mMoveConstructor);
   }
}

TEMPLATE_TEST_CASE("Testing move-assignable types", "[semantics]",
   ImplicitlyConstructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::MoveAssignable<T>);
   static_assert(CT::SemanticAssignable<Moved, T>);
   static_assert(CT::SemanticAssignableAlt<Moved<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mMoveAssigner);
}

TEMPLATE_TEST_CASE("Testing non-move-assignable types", "[semantics]",
   IncompleteType,
   NonDestructible
) {
   using T = TestType;
   static_assert(not CT::MoveAssignable<T>);
   static_assert(not CT::SemanticAssignable<Moved, T>);
   static_assert(not CT::SemanticAssignableAlt<Moved<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mMoveAssigner);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mMoveConstructor);
   }
}


///                                                                           
///   Copy semantics                                                          
///                                                                           
TEMPLATE_TEST_CASE("Testing copy-makable types", "[semantics]",
   AllSemanticConstructible,
   AllSemanticConstructibleImplicit,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::CopyMakable<T>);
   static_assert(CT::SemanticMakable<Copied, T>);
   static_assert(CT::SemanticMakableAlt<Copied<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mCopyConstructor);
}

TEMPLATE_TEST_CASE("Testing non-copy-makable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   Destructible,
   Complex,
   ContainsComplex,
   ImplicitlyConstructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   DescriptorConstructible
) {
   using T = TestType;
   static_assert(not CT::CopyMakable<T>);
   static_assert(not CT::SemanticMakable<Copied, T>);
   static_assert(not CT::SemanticMakableAlt<Copied<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mCopyConstructor);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mCopyConstructor);
   }
}

TEMPLATE_TEST_CASE("Testing copy-assignable types", "[semantics]",
   AllSemanticConstructibleImplicit,
   AllSemanticConstructibleAndAssignable,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::CopyAssignable<T>);
   static_assert(CT::SemanticAssignable<Copied, T>);
   static_assert(CT::SemanticAssignableAlt<Copied<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mCopyAssigner);
}

TEMPLATE_TEST_CASE("Testing non-copy-assignable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   Destructible,
   Complex,
   ContainsComplex,
   ImplicitlyConstructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   DescriptorConstructible,
   AllSemanticConstructible,
   PartiallySemanticConstructible
) {
   using T = TestType;
   static_assert(not CT::CopyAssignable<T>);
   static_assert(not CT::SemanticAssignable<Copied, T>);
   static_assert(not CT::SemanticAssignableAlt<Copied<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mCopyAssigner);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mCopyAssigner);
   }
}


///                                                                           
///   Clone semantics                                                         
///                                                                           
TEMPLATE_TEST_CASE("Testing clone-makable types", "[semantics]",
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::CloneMakable<T>);
   static_assert(CT::SemanticMakable<Cloned, T>);
   static_assert(CT::SemanticMakableAlt<Cloned<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mCloneConstructor);
}

TEMPLATE_TEST_CASE("Testing non-clone-makable types", "[semantics]",
   IncompleteType,
   ImplicitlyConstructible,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex
) {
   using T = TestType;
   static_assert(not CT::CloneMakable<T>);
   static_assert(not CT::SemanticMakable<Cloned, T>);
   static_assert(not CT::SemanticMakableAlt<Cloned<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mCloneConstructor);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mCloneConstructor);
   }
}

TEMPLATE_TEST_CASE("Testing clone-assignable types", "[semantics]",
   AllSemanticConstructibleImplicit,
   AllSemanticConstructibleAndAssignable,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::CloneAssignable<T>);
   static_assert(CT::SemanticAssignable<Cloned, T>);
   static_assert(CT::SemanticAssignableAlt<Cloned<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mCloneAssigner);
}

TEMPLATE_TEST_CASE("Testing non-clone-assignable types", "[semantics]",
   IncompleteType,
   ImplicitlyConstructible,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   Complex,
   ContainsComplex,
   AllSemanticConstructible,
   PartiallySemanticConstructible,
   DescriptorConstructible
) {
   using T = TestType;
   static_assert(not CT::CloneAssignable<T>);
   static_assert(not CT::SemanticAssignable<Cloned, T>);
   static_assert(not CT::SemanticAssignableAlt<Cloned<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mCloneAssigner);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mCloneAssigner);
   }
}


///                                                                           
///   Disown semantics                                                        
///                                                                           
TEMPLATE_TEST_CASE("Testing disown-makable types", "[semantics]",
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::DisownMakable<T>);
   static_assert(CT::SemanticMakable<Disowned, T>);
   static_assert(CT::SemanticMakableAlt<Disowned<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mDisownConstructor);
}

TEMPLATE_TEST_CASE("Testing non-disown-makable types", "[semantics]",
   IncompleteType,
   ImplicitlyConstructible,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex
) {
   using T = TestType;
   static_assert(not CT::DisownMakable<T>);
   static_assert(not CT::SemanticMakable<Disowned, T>);
   static_assert(not CT::SemanticMakableAlt<Disowned<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mDisownConstructor);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mDisownConstructor);
   }
}

TEMPLATE_TEST_CASE("Testing disown-assignable types", "[semantics]",
   AllSemanticConstructibleImplicit,
   AllSemanticConstructibleAndAssignable,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::DisownAssignable<T>);
   static_assert(CT::SemanticAssignable<Disowned, T>);
   static_assert(CT::SemanticAssignableAlt<Disowned<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mDisownAssigner);
}

TEMPLATE_TEST_CASE("Testing non-disown-assignable types", "[semantics]",
   IncompleteType,
   ImplicitlyConstructible,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   AllSemanticConstructible,
   PartiallySemanticConstructible
) {
   using T = TestType;
   static_assert(not CT::DisownAssignable<T>);
   static_assert(not CT::SemanticAssignable<Disowned, T>);
   static_assert(not CT::SemanticAssignableAlt<Disowned<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mDisownAssigner);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mDisownAssigner);
   }
}


///                                                                           
///   Abandon semantics                                                       
///                                                                           
TEMPLATE_TEST_CASE("Testing abandon-makable types", "[semantics]",
   ImplicitlyConstructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::AbandonMakable<T>);
   static_assert(CT::SemanticMakable<Abandoned, T>);
   static_assert(CT::SemanticMakableAlt<Abandoned<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mAbandonConstructor);
}

TEMPLATE_TEST_CASE("Testing non-abandon-makable types", "[semantics]",
   IncompleteType,
   NonDestructible
) {
   using T = TestType;
   static_assert(not CT::AbandonMakable<T>);
   static_assert(not CT::SemanticMakable<Abandoned, T>);
   static_assert(not CT::SemanticMakableAlt<Abandoned<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mAbandonConstructor);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mAbandonConstructor);
   }
}

TEMPLATE_TEST_CASE("Testing abandon-assignable types", "[semantics]",
   ImplicitlyConstructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(CT::AbandonAssignable<T>);
   static_assert(CT::SemanticAssignable<Abandoned, T>);
   static_assert(CT::SemanticAssignableAlt<Abandoned<T>>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mAbandonAssigner);
}

TEMPLATE_TEST_CASE("Testing non-abandon-assignable types", "[semantics]",
   IncompleteType,
   NonDestructible
) {
   using T = TestType;
   static_assert(not CT::AbandonAssignable<T>);
   static_assert(not CT::SemanticAssignable<Abandoned, T>);
   static_assert(not CT::SemanticAssignableAlt<Abandoned<T>>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mAbandonAssigner);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mAbandonAssigner);
   }
}


///                                                                           
///   Descriptor semantics                                                    
///                                                                           
TEMPLATE_TEST_CASE("Testing descriptor-makable types", "[semantics]",
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   DescriptorConstructible
) {
   using T = TestType;
   static_assert(CT::DescriptorMakable<T>);

   auto meta = MetaData::Of<T>();
   REQUIRE(meta);
   REQUIRE(meta->mDescriptorConstructor);
}

TEMPLATE_TEST_CASE("Testing non-descriptor-makable types", "[semantics]",
   IncompleteType,
   ImplicitlyConstructible,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   PartiallySemanticConstructible,
   Complex,
   ContainsComplex,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta
) {
   using T = TestType;
   static_assert(not CT::DescriptorMakable<T>);

   if constexpr (CT::Complete<T>) {
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mDescriptorConstructor);
   }
   else {
      auto meta = MetaData::Of<T*>();
      REQUIRE(meta);
      REQUIRE_FALSE(meta->mDescriptorConstructor);
   }
}


TEMPLATE_TEST_CASE("Testing semantics of pointers", "[semantics]",
   uint8_t*, uint16_t*, uint32_t*, uint64_t*,
   int8_t*,  int16_t*,  int32_t*,  int64_t*,
   double*, float*, 
   char*, wchar_t*, char8_t*, char16_t*, char32_t*, 
   Langulus::Byte*,
   const uint8_t*, const uint16_t*, const uint32_t*, const uint64_t*,
   const int8_t*,  const int16_t*,  const int32_t*,  const int64_t*,
   const double*, const float*,
   const char*, const wchar_t*, const char8_t*, const char16_t*, const char32_t*,
   const Langulus::Byte*,
   Complex*, ContainsComplex*,
   const Complex*, const ContainsComplex*
) {
   GIVEN("A fundamental type") {
      using T = TestType;
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);

      static_assert(    CT::CopyMakable<T>);
      static_assert(    CT::ReferMakable<T>);
      static_assert(    CT::MoveMakable<T>);
      static_assert(    CT::CloneMakable<T> == CT::POD<Decay<T>>);
      static_assert(    CT::DisownMakable<T>);
      static_assert(    CT::AbandonMakable<T>);
      static_assert(not CT::DescriptorMakable<T>);

      static_assert(    CT::SemanticMakable<Copied, T>);
      static_assert(    CT::SemanticMakable<Referred, T>);
      static_assert(    CT::SemanticMakable<Moved, T>);
      static_assert(    CT::SemanticMakable<Cloned, T> == CT::POD<Decay<T>>);
      static_assert(    CT::SemanticMakable<Disowned, T>);
      static_assert(    CT::SemanticMakable<Abandoned, T>);

      static_assert(    CT::SemanticMakableAlt<Copied<T>>);
      static_assert(    CT::SemanticMakableAlt<Referred<T>>);
      static_assert(    CT::SemanticMakableAlt<Moved<T>>);
      static_assert(    CT::SemanticMakableAlt<Cloned<T>> == CT::POD<Decay<T>>);
      static_assert(    CT::SemanticMakableAlt<Disowned<T>>);
      static_assert(    CT::SemanticMakableAlt<Abandoned<T>>);

      static_assert(    CT::CopyAssignable<T>    == CT::Mutable<T>);
      static_assert(    CT::ReferAssignable<T>   == CT::Mutable<T>);
      static_assert(    CT::MoveAssignable<T>    == CT::Mutable<T>);
      static_assert(    CT::CloneAssignable<T>   == (CT::Mutable<Deptr<T>> and CT::POD<Decay<T>>));
      static_assert(    CT::DisownAssignable<T>  == CT::Mutable<T>);
      static_assert(    CT::AbandonAssignable<T> == CT::Mutable<T>);
                           
      static_assert(    CT::SemanticAssignable<Copied, T>    == CT::Mutable<T>);
      static_assert(    CT::SemanticAssignable<Referred, T>  == CT::Mutable<T>);
      static_assert(    CT::SemanticAssignable<Moved, T>     == CT::Mutable<T>);
      static_assert(    CT::SemanticAssignable<Cloned, T>    == (CT::Mutable<Deptr<T>> and CT::POD<Decay<T>>));
      static_assert(    CT::SemanticAssignable<Disowned, T>  == CT::Mutable<T>);
      static_assert(    CT::SemanticAssignable<Abandoned, T> == CT::Mutable<T>);

      static_assert(    CT::SemanticAssignableAlt<Copied<T>>    == CT::Mutable<T>);
      static_assert(    CT::SemanticAssignableAlt<Referred<T>>  == CT::Mutable<T>);
      static_assert(    CT::SemanticAssignableAlt<Moved<T>>     == CT::Mutable<T>);
      static_assert(    CT::SemanticAssignableAlt<Cloned<T>>    == (CT::Mutable<Deptr<T>> and CT::POD<Decay<T>>));
      static_assert(    CT::SemanticAssignableAlt<Disowned<T>>  == CT::Mutable<T>);
      static_assert(    CT::SemanticAssignableAlt<Abandoned<T>> == CT::Mutable<T>);

      if constexpr (CT::Destroyable<T>)
         REQUIRE(meta->mDestructor);
      else
         REQUIRE_FALSE(meta->mDestructor);

      if constexpr (CT::Defaultable<T>)
         REQUIRE(meta->mDefaultConstructor);
      else
         REQUIRE_FALSE(meta->mDefaultConstructor);

      if constexpr (CT::CopyMakable<T>)
         REQUIRE(meta->mCopyConstructor);
      else
         REQUIRE_FALSE(meta->mCopyConstructor);

      REQUIRE(meta->mReferConstructor);
      REQUIRE(meta->mMoveConstructor);

      if constexpr (CT::CloneMakable<T>)
         REQUIRE(meta->mCloneConstructor);
      else
         REQUIRE_FALSE(meta->mCloneConstructor);

      if constexpr (CT::DisownMakable<T>)
         REQUIRE(meta->mDisownConstructor);
      else
         REQUIRE_FALSE(meta->mDisownConstructor);

      if constexpr (CT::AbandonMakable<T>)
         REQUIRE(meta->mAbandonConstructor);
      else
         REQUIRE_FALSE(meta->mAbandonConstructor);

      REQUIRE_FALSE(meta->mDescriptorConstructor);

      if constexpr (CT::CopyAssignable<T>)
         REQUIRE(meta->mCopyAssigner);
      else
         REQUIRE_FALSE(meta->mCopyAssigner);

      REQUIRE(meta->mReferAssigner);
      REQUIRE(meta->mMoveAssigner);

      if constexpr (CT::CloneAssignable<T>)
         REQUIRE(meta->mCloneAssigner);
      else
         REQUIRE_FALSE(meta->mCloneAssigner);

      if constexpr (CT::DisownAssignable<T>)
         REQUIRE(meta->mDisownAssigner);
      else
         REQUIRE_FALSE(meta->mDisownAssigner);

      if constexpr (CT::AbandonAssignable<T>)
         REQUIRE(meta->mAbandonAssigner);
      else
         REQUIRE_FALSE(meta->mAbandonAssigner);
   }
}

TEMPLATE_TEST_CASE("Testing DesemCast (POD)", "[semantics]",
   int, Copied<int>, Referred<int>, Disowned<int>, Cloned<int>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DesemCast(i)), const int&>);
   delete value;
}

TEMPLATE_TEST_CASE("Testing DesemCast (non POD)", "[semantics]",
   Complex, Copied<Complex>, Referred<Complex>, Disowned<Complex>, Cloned<Complex>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DesemCast(i)), const Complex&>);
   delete value;
}