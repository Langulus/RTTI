///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "Common.hpp"


///                                                                           
TEST_CASE("Testing IntentOf", "[intents]") {
   static_assert(CT::Aggregate<AggregateType>);
   static_assert(CT::Aggregate<AggregateTypeComplex>);

   static_assert(CT::Exact<IntentOf<int&&>, Moved<int>>);
   static_assert(CT::Exact<IntentOf<const int&&>, Referred<const int>>);
   static_assert(CT::Exact<IntentOf<int&>, Referred<int>>);
   static_assert(CT::Exact<IntentOf<const int&>, Referred<const int>>);

   static_assert(CT::Exact<Deint<int&&>, int&&>);
   static_assert(CT::Exact<Deint<const int&&>, const int&&>);
   static_assert(CT::Exact<Deint<int&>, int&>);
   static_assert(CT::Exact<Deint<const int&>, const int&>);

   static_assert(CT::Exact<IntentOf<Copied<int>>, Copied<int>>);
   static_assert(CT::Exact<IntentOf<Copied<int>&>, Copied<int>>);
   static_assert(CT::Exact<IntentOf<Copied<int>&&>, Copied<int>>);
   static_assert(CT::Exact<IntentOf<const Copied<int>&>, Copied<int>>);

   static_assert(    CT::Copied<Copied<int>>);
   static_assert(not CT::Copied<Moved<int>>);
   static_assert(not CT::Copied<Referred<int>>);
   static_assert(not CT::Copied<Abandoned<int>>);
   static_assert(not CT::Copied<Disowned<int>>);
   static_assert(not CT::Copied<Cloned<int>>);
   static_assert(not CT::Copied<Describe>);

   static_assert(CT::Exact<IntentOf<Referred<int>>, Referred<int>>);
   static_assert(CT::Exact<IntentOf<Referred<int>&>, Referred<int>>);
   static_assert(CT::Exact<IntentOf<Referred<int>&&>, Referred<int>>);
   static_assert(CT::Exact<IntentOf<const Referred<int>&>, Referred<int>>);

   static_assert(    CT::Referred<Referred<int>>);
   static_assert(not CT::Referred<Moved<int>>);
   static_assert(not CT::Referred<Copied<int>>);
   static_assert(not CT::Referred<Abandoned<int>>);
   static_assert(not CT::Referred<Disowned<int>>);
   static_assert(not CT::Referred<Cloned<int>>);
   static_assert(not CT::Referred<Describe>);

   static_assert(CT::Exact<IntentOf<Moved<int>>, Moved<int>>);
   static_assert(CT::Exact<IntentOf<Moved<int>&>, Moved<int>>);
   static_assert(CT::Exact<IntentOf<Moved<int>&&>, Moved<int>>);
   static_assert(CT::Exact<IntentOf<const Moved<int>&>, Moved<int>>);

   static_assert(    CT::Moved<Moved<int>>);
   static_assert(not CT::Moved<Referred<int>>);
   static_assert(not CT::Moved<Copied<int>>);
   static_assert(not CT::Moved<Abandoned<int>>);
   static_assert(not CT::Moved<Disowned<int>>);
   static_assert(not CT::Moved<Cloned<int>>);
   static_assert(not CT::Moved<Describe>);

   static_assert(CT::Exact<IntentOf<Abandoned<int>>, Abandoned<int>>);
   static_assert(CT::Exact<IntentOf<Abandoned<int>&>, Abandoned<int>>);
   static_assert(CT::Exact<IntentOf<Abandoned<int>&&>, Abandoned<int>>);
   static_assert(CT::Exact<IntentOf<const Abandoned<int>&>, Abandoned<int>>);

   static_assert(    CT::Abandoned<Abandoned<int>>);
   static_assert(not CT::Abandoned<Referred<int>>);
   static_assert(not CT::Abandoned<Copied<int>>);
   static_assert(not CT::Abandoned<Moved<int>>);
   static_assert(not CT::Abandoned<Disowned<int>>);
   static_assert(not CT::Abandoned<Cloned<int>>);
   static_assert(not CT::Abandoned<Describe>);

   static_assert(CT::Exact<IntentOf<Disowned<int>>, Disowned<int>>);
   static_assert(CT::Exact<IntentOf<Disowned<int>&>, Disowned<int>>);
   static_assert(CT::Exact<IntentOf<Disowned<int>&&>, Disowned<int>>);
   static_assert(CT::Exact<IntentOf<const Disowned<int>&>, Disowned<int>>);

   static_assert(    CT::Disowned<Disowned<int>>);
   static_assert(not CT::Disowned<Referred<int>>);
   static_assert(not CT::Disowned<Copied<int>>);
   static_assert(not CT::Disowned<Moved<int>>);
   static_assert(not CT::Disowned<Abandoned<int>>);
   static_assert(not CT::Disowned<Cloned<int>>);
   static_assert(not CT::Disowned<Describe>);

   static_assert(CT::Exact<IntentOf<Cloned<int>>, Cloned<int>>);
   static_assert(CT::Exact<IntentOf<Cloned<int>&>, Cloned<int>>);
   static_assert(CT::Exact<IntentOf<Cloned<int>&&>, Cloned<int>>);
   static_assert(CT::Exact<IntentOf<const Cloned<int>&>, Cloned<int>>);

   static_assert(    CT::Cloned<Cloned<int>>);
   static_assert(not CT::Cloned<Referred<int>>);
   static_assert(not CT::Cloned<Copied<int>>);
   static_assert(not CT::Cloned<Moved<int>>);
   static_assert(not CT::Cloned<Abandoned<int>>);
   static_assert(not CT::Cloned<Disowned<int>>);
   static_assert(not CT::Cloned<Describe>);

   static_assert(CT::Exact<IntentOf<Describe>, Describe>);
   static_assert(CT::Exact<IntentOf<Describe&>, Describe>);
   static_assert(CT::Exact<IntentOf<Describe&&>, Describe>);
   static_assert(CT::Exact<IntentOf<const Describe&>, Describe>);

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
   static_assert(CT::Exact<IntentOf<decltype(anArrayOfStrings)>,   Referred<decltype(anArrayOfStrings)>>);
   static_assert(CT::Exact<IntentOf<decltype(anArrayOfStrings)&&>, Referred<decltype(anArrayOfStrings)>>);
}

///                                                                           
TEST_CASE("Testing Deint", "[intents]") {
   static_assert(CT::Exact<Deint<Copied<int>>, int>);
   static_assert(CT::Exact<Deint<Referred<int>>, int>);
   static_assert(CT::Exact<Deint<Moved<int>>, int>);
   static_assert(CT::Exact<Deint<Abandoned<int>>, int>);
   static_assert(CT::Exact<Deint<Disowned<int>>, int>);
   static_assert(CT::Exact<Deint<Cloned<int>>, int>);
   static_assert(CT::Exact<Deint<Describe>, Describe>);
}


///                                                                           
///   Refer intents                                                           
///                                                                           
TEMPLATE_TEST_CASE("Testing refer-makable types", "[intents]",
   AggregateType,
   ImplicitlyConstructible,
   Destructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
   DescriptorConstructible,
   Complex, ContainsComplex,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(CT::ReferMakable<T>);
   static_assert(CT::ReferMakable<T*>);
   static_assert(CT::IntentMakable<Referred, T>);
   static_assert(CT::IntentMakable<Referred, T*>);
   static_assert(CT::IntentMakableAlt<Referred<T>>);
   static_assert(CT::IntentMakableAlt<Referred<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mReferConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mReferConstructor);
}

TEMPLATE_TEST_CASE("Testing non-refer-makable types", "[intents]",
   IncompleteType,
   NonDestructible,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::ReferMakable<T>);
   static_assert(    CT::ReferMakable<T*>);
   static_assert(not CT::IntentMakable<Referred, T>);
   static_assert(    CT::IntentMakable<Referred, T*>);
   static_assert(not CT::IntentMakableAlt<Referred<T>>);
   static_assert(    CT::IntentMakableAlt<Referred<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mReferConstructor);
}

TEMPLATE_TEST_CASE("Testing refer-assignable types", "[intents]",
   AggregateType,
   ImplicitlyConstructible,
   NonDestructible,
   Destructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
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
   static_assert(    CT::IntentAssignable<Referred, T>);
   static_assert(not CT::IntentAssignable<Referred, const T>);
   static_assert(    CT::IntentAssignable<Referred, T*>);
   static_assert(    CT::IntentAssignable<Referred, const T*>);
   static_assert(    CT::IntentAssignableAlt<Referred<T>>);
   static_assert(not CT::IntentAssignableAlt<Referred<const T>>);
   static_assert(    CT::IntentAssignableAlt<Referred<T*>>);
   static_assert(    CT::IntentAssignableAlt<Referred<const T*>>);

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

TEMPLATE_TEST_CASE("Testing non-refer-assignable types", "[intents]",
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
   static_assert(not CT::IntentAssignable<Referred, T>);
   static_assert(not CT::IntentAssignable<Referred, const T>);
   static_assert(    CT::IntentAssignable<Referred, T*>);
   static_assert(    CT::IntentAssignable<Referred, const T*>);
   static_assert(not CT::IntentAssignableAlt<Referred<T>>);
   static_assert(not CT::IntentAssignableAlt<Referred<const T>>);
   static_assert(    CT::IntentAssignableAlt<Referred<T*>>);
   static_assert(    CT::IntentAssignableAlt<Referred<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mReferAssigner);
}


///                                                                           
///   Move intents                                                            
///                                                                           
TEMPLATE_TEST_CASE("Testing move-makable types", "[intents]",
   AggregateType,
   ImplicitlyConstructible,
   Destructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
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
   static_assert(CT::IntentMakable<Moved, T>);
   static_assert(CT::IntentMakable<Moved, T*>);
   static_assert(CT::IntentMakableAlt<Moved<T>>);
   static_assert(CT::IntentMakableAlt<Moved<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mMoveConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mMoveConstructor);
}

TEMPLATE_TEST_CASE("Testing non-move-makable types", "[intents]",
   IncompleteType,
   NonDestructible,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::MoveMakable<T>);
   static_assert(    CT::MoveMakable<T*>);
   static_assert(not CT::IntentMakable<Moved, T>);
   static_assert(    CT::IntentMakable<Moved, T*>);
   static_assert(not CT::IntentMakableAlt<Moved<T>>);
   static_assert(    CT::IntentMakableAlt<Moved<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mMoveConstructor);
}

TEMPLATE_TEST_CASE("Testing move-assignable types", "[intents]",
   NonDestructible,
   AggregateType,
   ImplicitlyConstructible,
   Destructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
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
   static_assert(    CT::IntentAssignable<Moved, T>);
   static_assert(not CT::IntentAssignable<Moved, const T>);
   static_assert(    CT::IntentAssignable<Moved, T*>);
   static_assert(    CT::IntentAssignable<Moved, const T*>);
   static_assert(    CT::IntentAssignableAlt<Moved<T>>);
   static_assert(not CT::IntentAssignableAlt<Moved<const T>>);
   static_assert(    CT::IntentAssignableAlt<Moved<T*>>);
   static_assert(    CT::IntentAssignableAlt<Moved<const T*>>);

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

TEMPLATE_TEST_CASE("Testing non-move-assignable types", "[intents]",
   IncompleteType,
   Complex,
   ContainsComplex,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::MoveAssignable<T>);
   static_assert(not CT::MoveAssignable<const T>);
   static_assert(    CT::MoveAssignable<T*>);
   static_assert(    CT::MoveAssignable<const T*>);
   static_assert(not CT::IntentAssignable<Moved, T>);
   static_assert(not CT::IntentAssignable<Moved, const T>);
   static_assert(    CT::IntentAssignable<Moved, T*>);
   static_assert(    CT::IntentAssignable<Moved, const T*>);
   static_assert(not CT::IntentAssignableAlt<Moved<T>>);
   static_assert(not CT::IntentAssignableAlt<Moved<const T>>);
   static_assert(    CT::IntentAssignableAlt<Moved<T*>>);
   static_assert(    CT::IntentAssignableAlt<Moved<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mMoveAssigner);
}


///                                                                           
///   Copy intents                                                            
///                                                                           
TEMPLATE_TEST_CASE("Testing copy-makable types", "[intents]",
   ImplicitlyConstructible,
   AggregateType,
   AllIntentConstructible,
   AllIntentConstructibleImplicit,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(CT::CopyMakable<T>);
   static_assert(CT::CopyMakable<T*>);
   static_assert(CT::IntentMakable<Copied, T>);
   static_assert(CT::IntentMakable<Copied, T*>);
   static_assert(CT::IntentMakableAlt<Copied<T>>);
   static_assert(CT::IntentMakableAlt<Copied<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mCopyConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mCopyConstructor);
}

TEMPLATE_TEST_CASE("Testing non-copy-makable types", "[intents]",
   IncompleteType,
   NonDestructible,
   Destructible,
   Complex,
   ContainsComplex,
   PrivatelyConstructible,
   NonIntentConstructible,
   DescriptorConstructible,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::CopyMakable<T>);
   static_assert(    CT::CopyMakable<T*>);
   static_assert(not CT::IntentMakable<Copied, T>);
   static_assert(    CT::IntentMakable<Copied, T*>);
   static_assert(not CT::IntentMakableAlt<Copied<T>>);
   static_assert(    CT::IntentMakableAlt<Copied<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mCopyConstructor);
}

TEMPLATE_TEST_CASE("Testing copy-assignable types", "[intents]",
   ImplicitlyConstructible,
   AggregateType,
   AllIntentConstructibleImplicit,
   AllIntentConstructibleAndAssignable,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod
) {
   using T = TestType;
   static_assert(    CT::CopyAssignable<T>);
   static_assert(not CT::CopyAssignable<const T>);
   static_assert(    CT::CopyAssignable<T*>);
   static_assert(    CT::CopyAssignable<const T*>);
   static_assert(    CT::IntentAssignable<Copied, T>);
   static_assert(not CT::IntentAssignable<Copied, const T>);
   static_assert(    CT::IntentAssignable<Copied, T*>);
   static_assert(    CT::IntentAssignable<Copied, const T*>);
   static_assert(    CT::IntentAssignableAlt<Copied<T>>);
   static_assert(not CT::IntentAssignableAlt<Copied<const T>>);
   static_assert(    CT::IntentAssignableAlt<Copied<T*>>);
   static_assert(    CT::IntentAssignableAlt<Copied<const T*>>);

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

TEMPLATE_TEST_CASE("Testing non-copy-assignable types", "[intents]",
   IncompleteType,
   NonDestructible,
   Destructible,
   Complex,
   ContainsComplex,
   PrivatelyConstructible,
   NonIntentConstructible,
   DescriptorConstructible,
   AllIntentConstructible,
   PartiallyIntentConstructible,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::CopyAssignable<T>);
   static_assert(not CT::CopyAssignable<const T>);
   static_assert(    CT::CopyAssignable<T*>);
   static_assert(    CT::CopyAssignable<const T*>);
   static_assert(not CT::IntentAssignable<Copied, T>);
   static_assert(not CT::IntentAssignable<Copied, const T>);
   static_assert(    CT::IntentAssignable<Copied, T*>);
   static_assert(    CT::IntentAssignable<Copied, const T*>);
   static_assert(not CT::IntentAssignableAlt<Copied<T>>);
   static_assert(not CT::IntentAssignableAlt<Copied<const T>>);
   static_assert(    CT::IntentAssignableAlt<Copied<T*>>);
   static_assert(    CT::IntentAssignableAlt<Copied<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mCopyAssigner);
}


///                                                                           
///   Clone intents                                                           
///                                                                           
TEMPLATE_TEST_CASE("Testing clone-makable types", "[intents]",
   ImplicitlyConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod,
   AggregateType
) {
   using T = TestType;
   static_assert(CT::CloneMakable<T>);
   static_assert(CT::CloneMakable<T*>);
   static_assert(CT::IntentMakable<Cloned, T>);
   static_assert(CT::IntentMakable<Cloned, T*>);
   static_assert(CT::IntentMakableAlt<Cloned<T>>);
   static_assert(CT::IntentMakableAlt<Cloned<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mCloneConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mCloneConstructor);
}

TEMPLATE_TEST_CASE("Testing non-clone-makable types", "[intents]",
   IncompleteType,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonIntentConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::CloneMakable<T>);
   static_assert(not CT::CloneMakable<T*>);
   static_assert(not CT::IntentMakable<Cloned, T>);
   static_assert(not CT::IntentMakable<Cloned, T*>);
   static_assert(not CT::IntentMakableAlt<Cloned<T>>);
   static_assert(not CT::IntentMakableAlt<Cloned<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mCloneConstructor);
}

TEMPLATE_TEST_CASE("Testing clone-assignable types", "[intents]",
   ImplicitlyConstructible,
   AllIntentConstructibleImplicit,
   AllIntentConstructibleAndAssignable,
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
   static_assert(    CT::IntentAssignable<Cloned, T>);
   static_assert(not CT::IntentAssignable<Cloned, const T>);
   static_assert(    CT::IntentAssignable<Cloned, T*>);
   static_assert(not CT::IntentAssignable<Cloned, const T*>);
   static_assert(    CT::IntentAssignableAlt<Cloned<T>>);
   static_assert(not CT::IntentAssignableAlt<Cloned<const T>>);
   static_assert(    CT::IntentAssignableAlt<Cloned<T*>>);
   static_assert(not CT::IntentAssignableAlt<Cloned<const T*>>);

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

TEMPLATE_TEST_CASE("Testing non-clone-assignable types", "[intents]",
   IncompleteType,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonIntentConstructible,
   Complex,
   ContainsComplex,
   AllIntentConstructible,
   PartiallyIntentConstructible,
   DescriptorConstructible,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::CloneAssignable<T>);
   static_assert(not CT::CloneAssignable<const T>);
   static_assert(not CT::CloneAssignable<T*>);
   static_assert(not CT::CloneAssignable<const T*>);
   static_assert(not CT::IntentAssignable<Cloned, T>);
   static_assert(not CT::IntentAssignable<Cloned, const T>);
   static_assert(not CT::IntentAssignable<Cloned, T*>);
   static_assert(not CT::IntentAssignable<Cloned, const T*>);
   static_assert(not CT::IntentAssignableAlt<Cloned<T>>);
   static_assert(not CT::IntentAssignableAlt<Cloned<const T>>);
   static_assert(not CT::IntentAssignableAlt<Cloned<T*>>);
   static_assert(not CT::IntentAssignableAlt<Cloned<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mCloneAssigner);
}


///                                                                           
///   Disown intents                                                          
///                                                                           
TEMPLATE_TEST_CASE("Testing disown-makable types", "[intents]",
   ImplicitlyConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
   bool, uint32_t, float, char, wchar_t, char8_t, Langulus::Byte,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   ForcefullyPod,
   AggregateType
) {
   using T = TestType;
   static_assert(CT::DisownMakable<T>);
   static_assert(CT::DisownMakable<T*>);
   static_assert(CT::IntentMakable<Disowned, T>);
   static_assert(CT::IntentMakable<Disowned, T*>);
   static_assert(CT::IntentMakableAlt<Disowned<T>>);
   static_assert(CT::IntentMakableAlt<Disowned<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mDisownConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mDisownConstructor);
}

TEMPLATE_TEST_CASE("Testing non-disown-makable types", "[intents]",
   IncompleteType,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonIntentConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   AggregateTypeComplex
) {
   using T = TestType;
   static_assert(not CT::DisownMakable<T>);
   static_assert(    CT::DisownMakable<T*>);
   static_assert(not CT::IntentMakable<Disowned, T>);
   static_assert(    CT::IntentMakable<Disowned, T*>);
   static_assert(not CT::IntentMakableAlt<Disowned<T>>);
   static_assert(    CT::IntentMakableAlt<Disowned<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mDisownConstructor);
}

TEMPLATE_TEST_CASE("Testing disown-assignable types", "[intents]",
   ImplicitlyConstructible,
   AllIntentConstructibleImplicit,
   AllIntentConstructibleAndAssignable,
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
   static_assert(    CT::IntentAssignable<Disowned, T>);
   static_assert(not CT::IntentAssignable<Disowned, const T>);
   static_assert(    CT::IntentAssignable<Disowned, T*>);
   static_assert(    CT::IntentAssignable<Disowned, const T*>);
   static_assert(    CT::IntentAssignableAlt<Disowned<T>>);
   static_assert(not CT::IntentAssignableAlt<Disowned<const T>>);
   static_assert(    CT::IntentAssignableAlt<Disowned<T*>>);
   static_assert(    CT::IntentAssignableAlt<Disowned<const T*>>);

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

TEMPLATE_TEST_CASE("Testing non-disown-assignable types", "[intents]",
   IncompleteType,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonIntentConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   AllIntentConstructible,
   PartiallyIntentConstructible
) {
   using T = TestType;
   static_assert(not CT::DisownAssignable<T>);
   static_assert(not CT::DisownAssignable<const T>);
   static_assert(    CT::DisownAssignable<T*>);
   static_assert(    CT::DisownAssignable<const T*>);
   static_assert(not CT::IntentAssignable<Disowned, T>);
   static_assert(not CT::IntentAssignable<Disowned, const T>);
   static_assert(    CT::IntentAssignable<Disowned, T*>);
   static_assert(    CT::IntentAssignable<Disowned, const T*>);
   static_assert(not CT::IntentAssignableAlt<Disowned<T>>);
   static_assert(not CT::IntentAssignableAlt<Disowned<const T>>);
   static_assert(    CT::IntentAssignableAlt<Disowned<T*>>);
   static_assert(    CT::IntentAssignableAlt<Disowned<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mDisownAssigner);
}


///                                                                           
///   Abandon semantics                                                       
///                                                                           
TEMPLATE_TEST_CASE("Testing abandon-makable types", "[intents]",
   ImplicitlyConstructible,
   Destructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
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
   static_assert(CT::IntentMakable<Abandoned, T>);
   static_assert(CT::IntentMakable<Abandoned, T*>);
   static_assert(CT::IntentMakableAlt<Abandoned<T>>);
   static_assert(CT::IntentMakableAlt<Abandoned<T*>>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mAbandonConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mAbandonConstructor);
}

TEMPLATE_TEST_CASE("Testing non-abandon-makable types", "[intents]",
   IncompleteType,
   NonDestructible,
   PrivatelyConstructible
) {
   using T = TestType;
   static_assert(not CT::AbandonMakable<T>);
   static_assert(    CT::AbandonMakable<T*>);
   static_assert(not CT::IntentMakable<Abandoned, T>);
   static_assert(    CT::IntentMakable<Abandoned, T*>);
   static_assert(not CT::IntentMakableAlt<Abandoned<T>>);
   static_assert(    CT::IntentMakableAlt<Abandoned<T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mAbandonConstructor);
}

TEMPLATE_TEST_CASE("Testing abandon-assignable types", "[intents]",
   ImplicitlyConstructible,
   Destructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
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
   static_assert(    CT::IntentAssignable<Abandoned, T>);
   static_assert(not CT::IntentAssignable<Abandoned, const T>);
   static_assert(    CT::IntentAssignable<Abandoned, T*>);
   static_assert(    CT::IntentAssignable<Abandoned, const T*>);
   static_assert(    CT::IntentAssignableAlt<Abandoned<T>>);
   static_assert(not CT::IntentAssignableAlt<Abandoned<const T>>);
   static_assert(    CT::IntentAssignableAlt<Abandoned<T*>>);
   static_assert(    CT::IntentAssignableAlt<Abandoned<const T*>>);

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

TEMPLATE_TEST_CASE("Testing non-abandon-assignable types", "[intents]",
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
   static_assert(not CT::IntentAssignable<Abandoned, T>);
   static_assert(not CT::IntentAssignable<Abandoned, const T>);
   static_assert(    CT::IntentAssignable<Abandoned, T*>);
   static_assert(    CT::IntentAssignable<Abandoned, const T*>);
   static_assert(not CT::IntentAssignableAlt<Abandoned<T>>);
   static_assert(not CT::IntentAssignableAlt<Abandoned<const T>>);
   static_assert(    CT::IntentAssignableAlt<Abandoned<T*>>);
   static_assert(    CT::IntentAssignableAlt<Abandoned<const T*>>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mAbandonAssigner);
}


///                                                                           
///   Descriptor intents                                                      
///                                                                           
TEMPLATE_TEST_CASE("Testing descriptor-makable types", "[intents]",
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   DescriptorConstructible
) {
   using T = TestType;
   static_assert(    CT::DescriptorMakable<T>);
   static_assert(not CT::DescriptorMakable<T*>);
   static_assert(not CT::IntentMakableAlt<Describe>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mDescriptorConstructor);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mDescriptorConstructor);
}

TEMPLATE_TEST_CASE("Testing non-descriptor-makable types", "[intents]",
   IncompleteType,
   ImplicitlyConstructible,
   NonDestructible,
   Destructible,
   PrivatelyConstructible,
   NonIntentConstructible,
   PartiallyIntentConstructible,
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
   static_assert(not CT::IntentMakableAlt<Describe>);

   auto meta = MetaData::Of<Conditional<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mDescriptorConstructor);
}

TEMPLATE_TEST_CASE("Testing DeintCast (POD)", "[intents]",
   int, Copied<int>, Referred<int>, Disowned<int>, Cloned<int>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DeintCast(i)), const int&>);
   delete value;
}

TEMPLATE_TEST_CASE("Testing DeintCast (non POD)", "[intents]",
   Complex, Copied<Complex>, Referred<Complex>, Disowned<Complex>, Cloned<Complex>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DeintCast(i)), const Complex&>);
   delete value;
}