///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Common.hpp"

//TODO Abstract
//TODO Unallocatable
//TODO Allocatable
//TODO Uninsertable
//TODO Insertable
//TODO Resolvable


///                                                                           
/// CT::Defaultable                                                           
///                                                                           
TEMPLATE_TEST_CASE("Testing CT::Defaultable", "[semantics]",
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   ImplicitlyConstructible,
   Destructible,
   AggregateType,
   Complex*, ContainsComplex*
) {
   static_assert(CT::Defaultable<TestType>);
   static_assert(CT::Defaultable<TestType*>);
   static_assert(CT::Defaultable<const TestType>);
   static_assert(CT::Defaultable<const TestType*>);

   auto meta1 = MetaData::Of<TestType>();
   REQUIRE(meta1);
   REQUIRE(meta1->mDefaultConstructor);

   auto meta2 = MetaData::Of<TestType*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mDefaultConstructor);
}

TEMPLATE_TEST_CASE("Testing not CT::Defaultable", "[semantics]",
   NonDestructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   IncompleteType,
   ForcefullyPod
) {
   static_assert(not CT::Defaultable<TestType>);
   static_assert(    CT::Defaultable<TestType*>);

   if constexpr (CT::Complete<TestType>) {
      auto meta1 = MetaData::Of<TestType>();
      REQUIRE(meta1);
      REQUIRE_FALSE(meta1->mDefaultConstructor);
   }

   auto meta2 = MetaData::Of<TestType*>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2->mDefaultConstructor);
}


///                                                                           
/// CT::Destroyable                                                           
///                                                                           
TEMPLATE_TEST_CASE("Testing CT::Destroyable<T>", "[semantics]",
   Destructible,
   Complex,
   ContainsComplex
) {
   static_assert(    CT::Destroyable<TestType>);
   static_assert(not CT::Destroyable<TestType*>);

   auto meta1 = MetaData::Of<TestType>();
   REQUIRE(meta1);
   REQUIRE(meta1->mDestructor);

   auto meta2 = MetaData::Of<TestType*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mDestructor);
}

TEMPLATE_TEST_CASE("Testing not CT::Destroyable<T>", "[semantics]",
   ImplicitlyConstructible,
   NonDestructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   PartiallySemanticConstructible,
   DescriptorConstructible,
   IncompleteType,
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   Complex*, ContainsComplex*
) {
   static_assert(not CT::Destroyable<TestType>);
   static_assert(not CT::Destroyable<TestType*>);

   if constexpr (CT::Complete<TestType>) {
      auto meta1 = MetaData::Of<TestType>();
      REQUIRE(meta1);
      REQUIRE_FALSE(meta1->mDestructor);
   }

   auto meta2 = MetaData::Of<TestType*>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2->mDestructor);
}


///                                                                           
/// CT::POD                                                                   
///                                                                           
TEMPLATE_TEST_CASE("Testing CT::POD<T>", "[semantics]",
   ImplicitlyConstructible,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   ForcefullyPod,
   AggregateType
) {
   static_assert(CT::POD<TestType>);
   static_assert(CT::POD<TestType*>);

   auto meta1 = MetaData::Of<TestType>();
   REQUIRE(meta1);
   REQUIRE(meta1->mIsPOD);

   auto meta2 = MetaData::Of<TestType*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mIsPOD);
}

TEMPLATE_TEST_CASE("Testing not CT::POD<T>", "[semantics]",
   Complex,
   ContainsComplex,
   IncompleteType,
   AggregateTypeComplex
) {
   static_assert(not CT::POD<TestType>);
   static_assert(    CT::POD<TestType*>);

   if constexpr (CT::Complete<TestType>) {
      auto meta1 = MetaData::Of<TestType>();
      REQUIRE(meta1);
      REQUIRE_FALSE(meta1->mIsPOD);
   }

   auto meta2 = MetaData::Of<TestType*>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2->mIsPOD);
}

//TODO Nullifiable
//TODO Unreflectable
//TODO Reflectable
//TODO Concretizable
//TODO ConcreteOf
//TODO Producible
//TODO HasNamedValues
//TODO ProducerOf
//TODO DispatcherMutable
//TODO DispatcherConstant
//TODO Dispatcher
//TODO TypeOf
//TODO Typed
//TODO Untyped
//TODO SuffixOf


TEMPLATE_TEST_CASE("Testing DecayCast (POD)", "[semantics]",
   int, Copied<int>, Referred<int>, Disowned<int>, Cloned<int>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DecayCast(i)), const int&>);
   delete value;
}

TEMPLATE_TEST_CASE("Testing DecayCast (non POD)", "[semantics]",
   Complex, Copied<Complex>, Referred<Complex>, Disowned<Complex>, Cloned<Complex>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DecayCast(i)), const Complex&>);
   delete value;
}
