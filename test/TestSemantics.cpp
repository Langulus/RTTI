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
   static_assert(CT::Aggregate<AggregateType>);
   static_assert(CT::Aggregate<AggregateTypeComplex>);

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

   static_assert(    CT::Copied<Copied<int>>);
   static_assert(not CT::Copied<Moved<int>>);
   static_assert(not CT::Copied<Referred<int>>);
   static_assert(not CT::Copied<Abandoned<int>>);
   static_assert(not CT::Copied<Disowned<int>>);
   static_assert(not CT::Copied<Cloned<int>>);
   static_assert(not CT::Copied<Describe>);

   static_assert(CT::Exact<SemanticOf<Referred<int>>, Referred<int>>);
   static_assert(CT::Exact<SemanticOf<Referred<int>&>, Referred<int>>);
   static_assert(CT::Exact<SemanticOf<Referred<int>&&>, Referred<int>>);
   static_assert(CT::Exact<SemanticOf<const Referred<int>&>, Referred<int>>);

   static_assert(    CT::Referred<Referred<int>>);
   static_assert(not CT::Referred<Moved<int>>);
   static_assert(not CT::Referred<Copied<int>>);
   static_assert(not CT::Referred<Abandoned<int>>);
   static_assert(not CT::Referred<Disowned<int>>);
   static_assert(not CT::Referred<Cloned<int>>);
   static_assert(not CT::Referred<Describe>);

   static_assert(CT::Exact<SemanticOf<Moved<int>>, Moved<int>>);
   static_assert(CT::Exact<SemanticOf<Moved<int>&>, Moved<int>>);
   static_assert(CT::Exact<SemanticOf<Moved<int>&&>, Moved<int>>);
   static_assert(CT::Exact<SemanticOf<const Moved<int>&>, Moved<int>>);

   static_assert(    CT::Moved<Moved<int>>);
   static_assert(not CT::Moved<Referred<int>>);
   static_assert(not CT::Moved<Copied<int>>);
   static_assert(not CT::Moved<Abandoned<int>>);
   static_assert(not CT::Moved<Disowned<int>>);
   static_assert(not CT::Moved<Cloned<int>>);
   static_assert(not CT::Moved<Describe>);

   static_assert(CT::Exact<SemanticOf<Abandoned<int>>, Abandoned<int>>);
   static_assert(CT::Exact<SemanticOf<Abandoned<int>&>, Abandoned<int>>);
   static_assert(CT::Exact<SemanticOf<Abandoned<int>&&>, Abandoned<int>>);
   static_assert(CT::Exact<SemanticOf<const Abandoned<int>&>, Abandoned<int>>);

   static_assert(    CT::Abandoned<Abandoned<int>>);
   static_assert(not CT::Abandoned<Referred<int>>);
   static_assert(not CT::Abandoned<Copied<int>>);
   static_assert(not CT::Abandoned<Moved<int>>);
   static_assert(not CT::Abandoned<Disowned<int>>);
   static_assert(not CT::Abandoned<Cloned<int>>);
   static_assert(not CT::Abandoned<Describe>);

   static_assert(CT::Exact<SemanticOf<Disowned<int>>, Disowned<int>>);
   static_assert(CT::Exact<SemanticOf<Disowned<int>&>, Disowned<int>>);
   static_assert(CT::Exact<SemanticOf<Disowned<int>&&>, Disowned<int>>);
   static_assert(CT::Exact<SemanticOf<const Disowned<int>&>, Disowned<int>>);

   static_assert(    CT::Disowned<Disowned<int>>);
   static_assert(not CT::Disowned<Referred<int>>);
   static_assert(not CT::Disowned<Copied<int>>);
   static_assert(not CT::Disowned<Moved<int>>);
   static_assert(not CT::Disowned<Abandoned<int>>);
   static_assert(not CT::Disowned<Cloned<int>>);
   static_assert(not CT::Disowned<Describe>);

   static_assert(CT::Exact<SemanticOf<Cloned<int>>, Cloned<int>>);
   static_assert(CT::Exact<SemanticOf<Cloned<int>&>, Cloned<int>>);
   static_assert(CT::Exact<SemanticOf<Cloned<int>&&>, Cloned<int>>);
   static_assert(CT::Exact<SemanticOf<const Cloned<int>&>, Cloned<int>>);

   static_assert(    CT::Cloned<Cloned<int>>);
   static_assert(not CT::Cloned<Referred<int>>);
   static_assert(not CT::Cloned<Copied<int>>);
   static_assert(not CT::Cloned<Moved<int>>);
   static_assert(not CT::Cloned<Abandoned<int>>);
   static_assert(not CT::Cloned<Disowned<int>>);
   static_assert(not CT::Cloned<Describe>);

   static_assert(CT::Exact<SemanticOf<Describe>, Describe>);
   static_assert(CT::Exact<SemanticOf<Describe&>, Describe>);
   static_assert(CT::Exact<SemanticOf<Describe&&>, Describe>);
   static_assert(CT::Exact<SemanticOf<const Describe&>, Describe>);

   /*static_assert(    CT::Describe<Describe>);
   static_assert(not CT::Describe<Referred<int>>);
   static_assert(not CT::Describe<Copied<int>>);
   static_assert(not CT::Describe<Moved<int>>);
   static_assert(not CT::Describe<Abandoned<int>>);
   static_assert(not CT::Describe<Disowned<int>>);
   static_assert(not CT::Describe<Cloned<int>>);*/

   const std::string_view anArrayOfStrings[] {
      "one", "two", "three", "four"
   };
   static_assert(CT::Exact<SemanticOf<decltype(anArrayOfStrings)>,   Referred<decltype(anArrayOfStrings)>>);
   static_assert(CT::Exact<SemanticOf<decltype(anArrayOfStrings)&&>, Referred<decltype(anArrayOfStrings)>>);
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
   AggregateType,
   ImplicitlyConstructible,
   Destructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex, ContainsComplex,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(CT::ReferMakable<T>);
   static_assert(CT::ReferMakable<T*>);
   static_assert(CT::SemanticMakable<Referred, T>);
   static_assert(CT::SemanticMakable<Referred, T*>);
   static_assert(CT::SemanticMakableAlt<Referred<T>>);
   static_assert(CT::SemanticMakableAlt<Referred<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mReferConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mReferConstructor);
}

TEMPLATE_TEST_CASE("Testing non-refer-makable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::ReferMakable<T>);
   static_assert(    CT::ReferMakable<T*>);
   static_assert(not CT::SemanticMakable<Referred, T>);
   static_assert(    CT::SemanticMakable<Referred, T*>);
   static_assert(not CT::SemanticMakableAlt<Referred<T>>);
   static_assert(    CT::SemanticMakableAlt<Referred<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mReferConstructor);
}

TEMPLATE_TEST_CASE("Testing refer-assignable types", "[semantics]",
   AggregateType,
   ImplicitlyConstructible,
   NonDestructible,
   Destructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(    CT::ReferAssignable<T>);
   static_assert(not CT::ReferAssignable<const T>);
   static_assert(    CT::ReferAssignable<T*>);
   static_assert(    CT::ReferAssignable<const T*>);
   static_assert(    CT::SemanticAssignable<Referred, T>);
   static_assert(not CT::SemanticAssignable<Referred, const T>);
   static_assert(    CT::SemanticAssignable<Referred, T*>);
   static_assert(    CT::SemanticAssignable<Referred, const T*>);
   static_assert(    CT::SemanticAssignableAlt<Referred<T>>);
   static_assert(not CT::SemanticAssignableAlt<Referred<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Referred<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Referred<const T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mReferAssigner);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mReferAssigner);

   auto meta3 = MetaData::Of<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3->mReferAssigner);
}

TEMPLATE_TEST_CASE("Testing non-refer-assignable types", "[semantics]",
   IncompleteType,
   Complex,
   ContainsComplex,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::ReferAssignable<T>);
   static_assert(not CT::ReferAssignable<const T>);
   static_assert(    CT::ReferAssignable<T*>);
   static_assert(    CT::ReferAssignable<const T*>);
   static_assert(not CT::SemanticAssignable<Referred, T>);
   static_assert(not CT::SemanticAssignable<Referred, const T>);
   static_assert(    CT::SemanticAssignable<Referred, T*>);
   static_assert(    CT::SemanticAssignable<Referred, const T*>);
   static_assert(not CT::SemanticAssignableAlt<Referred<T>>);
   static_assert(not CT::SemanticAssignableAlt<Referred<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Referred<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Referred<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mReferAssigner);
}


///                                                                           
///   Move semantics                                                          
///                                                                           
TEMPLATE_TEST_CASE("Testing move-makable types", "[semantics]",
   AggregateType,
   ImplicitlyConstructible,
   Destructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(CT::MoveMakable<T>);
   static_assert(CT::MoveMakable<T*>);
   static_assert(CT::SemanticMakable<Moved, T>);
   static_assert(CT::SemanticMakable<Moved, T*>);
   static_assert(CT::SemanticMakableAlt<Moved<T>>);
   static_assert(CT::SemanticMakableAlt<Moved<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mMoveConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mMoveConstructor);
}

TEMPLATE_TEST_CASE("Testing non-move-makable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::MoveMakable<T>);
   static_assert(    CT::MoveMakable<T*>);
   static_assert(not CT::SemanticMakable<Moved, T>);
   static_assert(    CT::SemanticMakable<Moved, T*>);
   static_assert(not CT::SemanticMakableAlt<Moved<T>>);
   static_assert(    CT::SemanticMakableAlt<Moved<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mMoveConstructor);
}

TEMPLATE_TEST_CASE("Testing move-assignable types", "[semantics]",
   AggregateType,
   ImplicitlyConstructible,
   Destructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(    CT::MoveAssignable<T>);
   static_assert(not CT::MoveAssignable<const T>);
   static_assert(    CT::MoveAssignable<T*>);
   static_assert(    CT::MoveAssignable<const T*>);
   static_assert(    CT::SemanticAssignable<Moved, T>);
   static_assert(not CT::SemanticAssignable<Moved, const T>);
   static_assert(    CT::SemanticAssignable<Moved, T*>);
   static_assert(    CT::SemanticAssignable<Moved, const T*>);
   static_assert(    CT::SemanticAssignableAlt<Moved<T>>);
   static_assert(not CT::SemanticAssignableAlt<Moved<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Moved<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Moved<const T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mMoveAssigner);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mMoveAssigner);

   auto meta3 = MetaData::Of<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3->mMoveAssigner);
}

TEMPLATE_TEST_CASE("Testing non-move-assignable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   Complex,
   ContainsComplex,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::MoveAssignable<T>);
   static_assert(not CT::MoveAssignable<const T>);
   static_assert(    CT::MoveAssignable<T*>);
   static_assert(    CT::MoveAssignable<const T*>);
   static_assert(not CT::SemanticAssignable<Moved, T>);
   static_assert(not CT::SemanticAssignable<Moved, const T>);
   static_assert(    CT::SemanticAssignable<Moved, T*>);
   static_assert(    CT::SemanticAssignable<Moved, const T*>);
   static_assert(not CT::SemanticAssignableAlt<Moved<T>>);
   static_assert(not CT::SemanticAssignableAlt<Moved<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Moved<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Moved<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mMoveAssigner);
}


///                                                                           
///   Copy semantics                                                          
///                                                                           
TEMPLATE_TEST_CASE("Testing copy-makable types", "[semantics]",
   ImplicitlyConstructible,
   AggregateType,
   AllSemanticConstructible,
   AllSemanticConstructibleImplicit,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(CT::CopyMakable<T>);
   static_assert(CT::CopyMakable<T*>);
   static_assert(CT::SemanticMakable<Copied, T>);
   static_assert(CT::SemanticMakable<Copied, T*>);
   static_assert(CT::SemanticMakableAlt<Copied<T>>);
   static_assert(CT::SemanticMakableAlt<Copied<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mCopyConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mCopyConstructor);
}

TEMPLATE_TEST_CASE("Testing non-copy-makable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   Destructible,
   Complex,
   ContainsComplex,
   PrivatelyConstructible,
   NonSemanticConstructible,
   DescriptorConstructible,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::CopyMakable<T>);
   static_assert(    CT::CopyMakable<T*>);
   static_assert(not CT::SemanticMakable<Copied, T>);
   static_assert(    CT::SemanticMakable<Copied, T*>);
   static_assert(not CT::SemanticMakableAlt<Copied<T>>);
   static_assert(    CT::SemanticMakableAlt<Copied<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mCopyConstructor);
}

TEMPLATE_TEST_CASE("Testing copy-assignable types", "[semantics]",
   ImplicitlyConstructible,
   AggregateType,
   AllSemanticConstructibleImplicit,
   AllSemanticConstructibleAndAssignable,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(    CT::CopyAssignable<T>);
   static_assert(not CT::CopyAssignable<const T>);
   static_assert(    CT::CopyAssignable<T*>);
   static_assert(    CT::CopyAssignable<const T*>);
   static_assert(    CT::SemanticAssignable<Copied, T>);
   static_assert(not CT::SemanticAssignable<Copied, const T>);
   static_assert(    CT::SemanticAssignable<Copied, T*>);
   static_assert(    CT::SemanticAssignable<Copied, const T*>);
   static_assert(    CT::SemanticAssignableAlt<Copied<T>>);
   static_assert(not CT::SemanticAssignableAlt<Copied<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Copied<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Copied<const T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mCopyAssigner);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mCopyAssigner);

   auto meta3 = MetaData::Of<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3->mCopyAssigner);
}

TEMPLATE_TEST_CASE("Testing non-copy-assignable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   Destructible,
   Complex,
   ContainsComplex,
   PrivatelyConstructible,
   NonSemanticConstructible,
   DescriptorConstructible,
   AllSemanticConstructible,
   PartiallySemanticConstructible,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::CopyAssignable<T>);
   static_assert(not CT::CopyAssignable<const T>);
   static_assert(    CT::CopyAssignable<T*>);
   static_assert(    CT::CopyAssignable<const T*>);
   static_assert(not CT::SemanticAssignable<Copied, T>);
   static_assert(not CT::SemanticAssignable<Copied, const T>);
   static_assert(    CT::SemanticAssignable<Copied, T*>);
   static_assert(    CT::SemanticAssignable<Copied, const T*>);
   static_assert(not CT::SemanticAssignableAlt<Copied<T>>);
   static_assert(not CT::SemanticAssignableAlt<Copied<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Copied<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Copied<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mCopyAssigner);
}


///                                                                           
///   Clone semantics                                                         
///                                                                           
TEMPLATE_TEST_CASE("Testing clone-makable types", "[semantics]",
   ImplicitlyConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod,
   AggregateType
) {
   using T = TestType;
   static_assert(CT::CloneMakable<T>);
   static_assert(CT::CloneMakable<T*>);
   static_assert(CT::SemanticMakable<Cloned, T>);
   static_assert(CT::SemanticMakable<Cloned, T*>);
   static_assert(CT::SemanticMakableAlt<Cloned<T>>);
   static_assert(CT::SemanticMakableAlt<Cloned<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mCloneConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mCloneConstructor);
}

TEMPLATE_TEST_CASE("Testing non-clone-makable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::CloneMakable<T>);
   static_assert(not CT::CloneMakable<T*>);
   static_assert(not CT::SemanticMakable<Cloned, T>);
   static_assert(not CT::SemanticMakable<Cloned, T*>);
   static_assert(not CT::SemanticMakableAlt<Cloned<T>>);
   static_assert(not CT::SemanticMakableAlt<Cloned<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mCloneConstructor);
}

TEMPLATE_TEST_CASE("Testing clone-assignable types", "[semantics]",
   ImplicitlyConstructible,
   AllSemanticConstructibleImplicit,
   AllSemanticConstructibleAndAssignable,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod,
   AggregateType
) {
   using T = TestType;
   static_assert(    CT::CloneAssignable<T>);
   static_assert(not CT::CloneAssignable<const T>);
   static_assert(    CT::CloneAssignable<T*>);
   static_assert(not CT::CloneAssignable<const T*>);
   static_assert(    CT::SemanticAssignable<Cloned, T>);
   static_assert(not CT::SemanticAssignable<Cloned, const T>);
   static_assert(    CT::SemanticAssignable<Cloned, T*>);
   static_assert(not CT::SemanticAssignable<Cloned, const T*>);
   static_assert(    CT::SemanticAssignableAlt<Cloned<T>>);
   static_assert(not CT::SemanticAssignableAlt<Cloned<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Cloned<T*>>);
   static_assert(not CT::SemanticAssignableAlt<Cloned<const T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mCloneAssigner);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mCloneAssigner);

   auto meta3 = MetaData::Of<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3->mCloneAssigner);
}

TEMPLATE_TEST_CASE("Testing non-clone-assignable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   Complex,
   ContainsComplex,
   AllSemanticConstructible,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::CloneAssignable<T>);
   static_assert(not CT::CloneAssignable<const T>);
   static_assert(not CT::CloneAssignable<T*>);
   static_assert(not CT::CloneAssignable<const T*>);
   static_assert(not CT::SemanticAssignable<Cloned, T>);
   static_assert(not CT::SemanticAssignable<Cloned, const T>);
   static_assert(not CT::SemanticAssignable<Cloned, T*>);
   static_assert(not CT::SemanticAssignable<Cloned, const T*>);
   static_assert(not CT::SemanticAssignableAlt<Cloned<T>>);
   static_assert(not CT::SemanticAssignableAlt<Cloned<const T>>);
   static_assert(not CT::SemanticAssignableAlt<Cloned<T*>>);
   static_assert(not CT::SemanticAssignableAlt<Cloned<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mCloneAssigner);
}


///                                                                           
///   Disown semantics                                                        
///                                                                           
TEMPLATE_TEST_CASE("Testing disown-makable types", "[semantics]",
   ImplicitlyConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod,
   AggregateType
) {
   using T = TestType;
   static_assert(CT::DisownMakable<T>);
   static_assert(CT::DisownMakable<T*>);
   static_assert(CT::SemanticMakable<Disowned, T>);
   static_assert(CT::SemanticMakable<Disowned, T*>);
   static_assert(CT::SemanticMakableAlt<Disowned<T>>);
   static_assert(CT::SemanticMakableAlt<Disowned<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mDisownConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mDisownConstructor);
}

TEMPLATE_TEST_CASE("Testing non-disown-makable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::DisownMakable<T>);
   static_assert(    CT::DisownMakable<T*>);
   static_assert(not CT::SemanticMakable<Disowned, T>);
   static_assert(    CT::SemanticMakable<Disowned, T*>);
   static_assert(not CT::SemanticMakableAlt<Disowned<T>>);
   static_assert(    CT::SemanticMakableAlt<Disowned<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mDisownConstructor);
}

TEMPLATE_TEST_CASE("Testing disown-assignable types", "[semantics]",
   ImplicitlyConstructible,
   AllSemanticConstructibleImplicit,
   AllSemanticConstructibleAndAssignable,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod,
   AggregateType
) {
   using T = TestType;
   static_assert(    CT::DisownAssignable<T>);
   static_assert(not CT::DisownAssignable<const T>);
   static_assert(    CT::DisownAssignable<T*>);
   static_assert(    CT::DisownAssignable<const T*>);
   static_assert(    CT::SemanticAssignable<Disowned, T>);
   static_assert(not CT::SemanticAssignable<Disowned, const T>);
   static_assert(    CT::SemanticAssignable<Disowned, T*>);
   static_assert(    CT::SemanticAssignable<Disowned, const T*>);
   static_assert(    CT::SemanticAssignableAlt<Disowned<T>>);
   static_assert(not CT::SemanticAssignableAlt<Disowned<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Disowned<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Disowned<const T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mDisownAssigner);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mDisownAssigner);

   auto meta3 = MetaData::Of<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3->mDisownAssigner);
}

TEMPLATE_TEST_CASE("Testing non-disown-assignable types", "[semantics]",
   IncompleteType,
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
   static_assert(not CT::DisownAssignable<const T>);
   static_assert(    CT::DisownAssignable<T*>);
   static_assert(    CT::DisownAssignable<const T*>);
   static_assert(not CT::SemanticAssignable<Disowned, T>);
   static_assert(not CT::SemanticAssignable<Disowned, const T>);
   static_assert(    CT::SemanticAssignable<Disowned, T*>);
   static_assert(    CT::SemanticAssignable<Disowned, const T*>);
   static_assert(not CT::SemanticAssignableAlt<Disowned<T>>);
   static_assert(not CT::SemanticAssignableAlt<Disowned<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Disowned<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Disowned<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mDisownAssigner);
}


///                                                                           
///   Abandon semantics                                                       
///                                                                           
TEMPLATE_TEST_CASE("Testing abandon-makable types", "[semantics]",
   ImplicitlyConstructible,
   Destructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   AggregateType,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(CT::AbandonMakable<T>);
   static_assert(CT::AbandonMakable<T*>);
   static_assert(CT::SemanticMakable<Abandoned, T>);
   static_assert(CT::SemanticMakable<Abandoned, T*>);
   static_assert(CT::SemanticMakableAlt<Abandoned<T>>);
   static_assert(CT::SemanticMakableAlt<Abandoned<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mAbandonConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mAbandonConstructor);
}

TEMPLATE_TEST_CASE("Testing non-abandon-makable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::AbandonMakable<T>);
   static_assert(    CT::AbandonMakable<T*>);
   static_assert(not CT::SemanticMakable<Abandoned, T>);
   static_assert(    CT::SemanticMakable<Abandoned, T*>);
   static_assert(not CT::SemanticMakableAlt<Abandoned<T>>);
   static_assert(    CT::SemanticMakableAlt<Abandoned<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mAbandonConstructor);
}

TEMPLATE_TEST_CASE("Testing abandon-assignable types", "[semantics]",
   ImplicitlyConstructible,
   Destructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   AggregateType,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(    CT::AbandonAssignable<T>);
   static_assert(not CT::AbandonAssignable<const T>);
   static_assert(    CT::AbandonAssignable<T*>);
   static_assert(    CT::AbandonAssignable<const T*>);
   static_assert(    CT::SemanticAssignable<Abandoned, T>);
   static_assert(not CT::SemanticAssignable<Abandoned, const T>);
   static_assert(    CT::SemanticAssignable<Abandoned, T*>);
   static_assert(    CT::SemanticAssignable<Abandoned, const T*>);
   static_assert(    CT::SemanticAssignableAlt<Abandoned<T>>);
   static_assert(not CT::SemanticAssignableAlt<Abandoned<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Abandoned<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Abandoned<const T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mAbandonAssigner);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mAbandonAssigner);

   auto meta3 = MetaData::Of<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3->mAbandonAssigner);
}

TEMPLATE_TEST_CASE("Testing non-abandon-assignable types", "[semantics]",
   IncompleteType,
   NonDestructible,
   Complex,
   ContainsComplex,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::AbandonAssignable<T>);
   static_assert(not CT::AbandonAssignable<const T>);
   static_assert(    CT::AbandonAssignable<T*>);
   static_assert(    CT::AbandonAssignable<const T*>);
   static_assert(not CT::SemanticAssignable<Abandoned, T>);
   static_assert(not CT::SemanticAssignable<Abandoned, const T>);
   static_assert(    CT::SemanticAssignable<Abandoned, T*>);
   static_assert(    CT::SemanticAssignable<Abandoned, const T*>);
   static_assert(not CT::SemanticAssignableAlt<Abandoned<T>>);
   static_assert(not CT::SemanticAssignableAlt<Abandoned<const T>>);
   static_assert(    CT::SemanticAssignableAlt<Abandoned<T*>>);
   static_assert(    CT::SemanticAssignableAlt<Abandoned<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mAbandonAssigner);
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
   static_assert(    CT::DescriptorMakable<T>);
   static_assert(not CT::DescriptorMakable<T*>);
   static_assert(not CT::SemanticMakableAlt<Describe>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mDescriptorConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mDescriptorConstructor);
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
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   AggregateType,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(not CT::DescriptorMakable<T>);
   static_assert(not CT::DescriptorMakable<T*>);
   static_assert(not CT::SemanticMakableAlt<Describe>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mDescriptorConstructor);
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