///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/Testing/CommonTypes.hpp>
#include <Langulus/MetaOf.hpp>
#include <Langulus/Utils/Byte.hpp>

//TODO test all other reflected compile-time traits at runtime

///                                                                           
/// CT::Abstract                                                              
TEST_CASE_TEMPLATE("Testing CT::Abstract", T,
   PureAbstract,
   ForcedAbstractExternally,
   ForcedAbstractInternally
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mIsAbstract);

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mIsAbstract);
}

TEST_CASE_TEMPLATE("Testing not CT::Abstract", T,
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   EmptyType,
   ActualAggregateType,
   NonDestructible,
   PrivatelyConstructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
   DescribeConstructible,
   Complex,
   ContainsComplex,
   IncompleteType,
   ForcefullyPod
) {
   if constexpr (CT::Complete<T>) {
      auto meta1 = MetaDataOf<T>();
      REQUIRE(meta1);
      REQUIRE(not meta1->mIsAbstract);
   }

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(not meta2->mIsAbstract);
}

//TODO Unallocatable
//TODO Allocatable
//TODO Uninsertable
//TODO Insertable
//TODO Resolvable


///                                                                           
/// CT::Defaultable                                                           
TEST_CASE_TEMPLATE("Testing CT::Defaultable", T,
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   EmptyType,
   ActualAggregateType,
   Complex*, ContainsComplex*
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(bool(meta1->mDefaultConstructor) == CT::Defaultable<Decay<T>>);

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(bool(meta2->mDefaultConstructor) == CT::Defaultable<Decay<T>>);
}

TEST_CASE_TEMPLATE("Testing not CT::Defaultable", TestType,
   NonDestructible,
   PrivatelyConstructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   IncompleteType,
   ForcefullyPod,
   PureAbstract,
   ForcedAbstractExternally,
   ForcedAbstractInternally
) {
   if constexpr (CT::Complete<TestType>) {
      auto meta1 = MetaDataOf<TestType>();
      REQUIRE(meta1);
      REQUIRE_FALSE(meta1->mDefaultConstructor);
   }

   auto meta2 = MetaDataOf<TestType*>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2->mDefaultConstructor);
}


///                                                                           
/// CT::Destroyable                                                           
TEST_CASE_TEMPLATE("Testing CT::Destroyable<T>", TestType,
   Destructible,
   Complex,
   ContainsComplex,
   PureVirtual,
   ImpureVirtual,
   VirtuallyDerived
) {
   auto meta1 = MetaDataOf<TestType>();
   REQUIRE(meta1);
   REQUIRE(meta1->mDestructor);

   auto meta2 = MetaDataOf<TestType*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mDestructor);
}

TEST_CASE_TEMPLATE("Testing not CT::Destroyable<T>", T,
   EmptyType,
   NonDestructible,
   PrivatelyConstructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
   DescriptorConstructible,
   IncompleteType,
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   Complex*, ContainsComplex*,
   ForcedAbstract
) {
   if constexpr (CT::Complete<T>) {
      auto meta1 = MetaDataOf<T>();
      REQUIRE(meta1);
      REQUIRE(bool(meta1->mDestructor) == CT::Destroyable<Decay<T>>);
   }

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(bool(meta2->mDestructor) == CT::Destroyable<Decay<T>>);
}


///                                                                           
/// CT::POD                                                                   
TEST_CASE_TEMPLATE("Testing CT::POD<T>", TestType,
   EmptyType,
   TMeta, CMeta, DMeta, VMeta,
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   ForcefullyPod,
   AggregateType
) {
   auto meta1 = MetaDataOf<TestType>();
   REQUIRE(meta1);
   REQUIRE(meta1->mIsPOD);

   auto meta2 = MetaDataOf<TestType*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mIsPOD);
}

TEST_CASE_TEMPLATE("Testing not CT::POD<T>", TestType,
   Complex,
   ContainsComplex,
   IncompleteType,
   AggregateTypeComplex
) {
   if constexpr (CT::Complete<TestType>) {
      auto meta1 = MetaDataOf<TestType>();
      REQUIRE(meta1);
      REQUIRE_FALSE(meta1->mIsPOD);
   }

   auto meta2 = MetaDataOf<TestType*>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2->mIsPOD);
}