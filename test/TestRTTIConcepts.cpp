///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "Common.hpp"


///                                                                           
/// CT::Abstract                                                              
///                                                                           
TEMPLATE_TEST_CASE("Testing CT::Abstract", "[concepts]",
   PureVirtual,
   ForcedAbstract
) {
   using T = TestType;
   static_assert(    CT::Abstract<T>);
   static_assert(not CT::Abstract<T*>);
   static_assert(    CT::Abstract<const T>);
   static_assert(not CT::Abstract<const T*>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mIsAbstract);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mIsAbstract);
}

TEMPLATE_TEST_CASE("Testing not CT::Abstract", "[concepts]",
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   ImplicitlyConstructible,
   Destructible,
   AggregateType,
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
   ForcefullyPod
) {
   static_assert(not CT::Abstract<TestType>);
   static_assert(not CT::Abstract<TestType*>);

   if constexpr (CT::Complete<TestType>) {
      auto meta1 = MetaData::Of<TestType>();
      REQUIRE(meta1);
      REQUIRE(not meta1->mIsAbstract);
   }

   auto meta2 = MetaData::Of<TestType*>();
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
///                                                                           
TEMPLATE_TEST_CASE("Testing CT::Defaultable", "[concepts]",
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   ImplicitlyConstructible,
   Destructible,
   AggregateType,
   Complex*, ContainsComplex*
) {
   using T = TestType;
   static_assert(CT::Defaultable<T>);
   static_assert(CT::Defaultable<T*>);
   static_assert(CT::Defaultable<const T>);
   static_assert(CT::Defaultable<const T*>);

   auto meta1 = MetaData::Of<T>();
   REQUIRE(meta1);
   REQUIRE(bool(meta1->mDefaultConstructor) == CT::Defaultable<Decay<T>>);

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(bool(meta2->mDefaultConstructor) == CT::Defaultable<Decay<T>>);
}

TEMPLATE_TEST_CASE("Testing not CT::Defaultable", "[concepts]",
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
   PureVirtual,
   ForcedAbstract
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
TEMPLATE_TEST_CASE("Testing CT::Destroyable<T>", "[concepts]",
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

TEMPLATE_TEST_CASE("Testing not CT::Destroyable<T>", "[concepts]",
   ImplicitlyConstructible,
   NonDestructible,
   PrivatelyConstructible,
   NonIntentConstructible,
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable,
   PartiallyIntentConstructible,
   DescriptorConstructible,
   IncompleteType,
   bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte,
   Complex*, ContainsComplex*
) {
   using T = TestType;
   static_assert(not CT::Destroyable<T>);
   static_assert(not CT::Destroyable<T*>);

   if constexpr (CT::Complete<T>) {
      auto meta1 = MetaData::Of<T>();
      REQUIRE(meta1);
      REQUIRE(bool(meta1->mDestructor) == CT::Destroyable<Decay<T>>);
   }

   auto meta2 = MetaData::Of<T*>();
   REQUIRE(meta2);
   REQUIRE(bool(meta2->mDestructor) == CT::Destroyable<Decay<T>>);
}


///                                                                           
/// CT::POD                                                                   
///                                                                           
TEMPLATE_TEST_CASE("Testing CT::POD<T>", "[concepts]",
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

TEMPLATE_TEST_CASE("Testing not CT::POD<T>", "[concepts]",
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

///                                                                           
/// CT::Reflectable                                                           
///                                                                           
TEST_CASE("Testing CT::Reflectable<T>", "[concepts]") {
   static_assert(    CT::Reflectable<ImplicitlyReflectedData>);
   static_assert(    CT::Reflectable<ImplicitlyReflectedDataWithTraits>);
   static_assert(    CT::Reflectable<ForcedAbstract>);
   static_assert(    CT::Reflectable<PureVirtual>);
   static_assert(    CT::Reflectable<int>);
   static_assert(    CT::Reflectable<int&>);
   static_assert(    CT::Reflectable<int*>);
   static_assert(    CT::Reflectable<int**>);
   static_assert(    CT::Reflectable<TypedEnum>);
   static_assert(    CT::Reflectable<TypeErasedContainer>);
   static_assert(    CT::Reflectable<std::vector<bool>>);
   static_assert(not CT::Reflectable<IncompleteType>);
   static_assert(not CT::Reflectable<NotReflectableType>);
   static_assert(    CT::Reflectable<IncompleteType*>);
   static_assert(    CT::Reflectable<NotReflectableType*>);
}

//TODO Concretizable
//TODO ConcreteOf
//TODO Producible
//TODO HasNamedValues
//TODO ProducerOf
//TODO DispatcherMutable
//TODO DispatcherConstant
//TODO Dispatcher

///                                                                           
/// TypeOf                                                                    
///                                                                           
TEST_CASE("Testing TypeOf<T>", "[concepts]") {
   static_assert(CT::Exact<TypeOf<ImplicitlyReflectedData>, ImplicitlyReflectedData::Named>);
   static_assert(CT::Exact<TypeOf<ImplicitlyReflectedDataWithTraits>, ImplicitlyReflectedData::Named>);
   static_assert(CT::Exact<TypeOf<int>, int>);
   static_assert(CT::Exact<TypeOf<int&>, int>);
   static_assert(CT::Exact<TypeOf<TypedEnum>, int16_t>);
   static_assert(CT::Exact<TypeOf<TypeErasedContainer>, void>);
   static_assert(CT::Exact<TypeOf<std::vector<bool>>, bool>);
   static_assert(CT::Exact<TypeOf<std::string_view>, char>);
   static_assert(CT::Exact<TypeOf<std::array<double, 5>>, double>);
   static_assert(CT::Exact<TypeOf<IncompleteType>, IncompleteType>);
}

///                                                                           
/// CT::Typed                                                                 
///                                                                           
TEST_CASE("Testing CT::Typed", "[concepts]") {
   static_assert(not CT::Typed<bool>);
   static_assert(not CT::Typed<void>);
   static_assert(not CT::Typed<const void>);
   static_assert(not CT::Typed<volatile void>);
   static_assert(not CT::Typed<void*>);
   static_assert(not CT::Typed<bool, void, void*>);
   static_assert(not CT::Typed<void, void, void>);
   static_assert(not CT::Typed<TypedEnum>);
   static_assert(not CT::Typed<TypeErasedContainer>);
   static_assert(not CT::Typed<IncompleteType>);
   static_assert(    CT::Typed<std::vector<bool>>);
   static_assert(    CT::Typed<std::string_view>);
   static_assert(    CT::Typed<std::array<double, 5>>);
   static_assert(    CT::Typed<ImplicitlyReflectedData>);
   static_assert(    CT::Typed<ImplicitlyReflectedData, ImplicitlyReflectedDataWithTraits>);
   static_assert(not CT::Typed<ImplicitlyReflectedData, void>);
}

///                                                                           
/// CT::Untyped                                                               
///                                                                           
TEST_CASE("Testing CT::Untyped", "[concepts]") {
   static_assert(    CT::Untyped<bool>);
   static_assert(    CT::Untyped<void>);
   static_assert(    CT::Untyped<const void>);
   static_assert(    CT::Untyped<volatile void>);
   static_assert(    CT::Untyped<void*>);
   static_assert(    CT::Untyped<bool, void, void*>);
   static_assert(    CT::Untyped<void, void, void>);
   static_assert(    CT::Untyped<TypedEnum>);
   static_assert(    CT::Untyped<TypeErasedContainer>);
   static_assert(    CT::Untyped<IncompleteType>);
   static_assert(not CT::Untyped<std::vector<bool>>);
   static_assert(not CT::Untyped<std::string_view>);
   static_assert(not CT::Untyped<std::array<double, 5>>);
   static_assert(not CT::Untyped<ImplicitlyReflectedData>);
   static_assert(not CT::Untyped<ImplicitlyReflectedData, ImplicitlyReflectedDataWithTraits>);
   static_assert(not CT::Untyped<ImplicitlyReflectedData, void>);
}

//TODO SuffixOf


TEMPLATE_TEST_CASE("Testing DecayCast (POD)", "[concepts]",
   int, Copied<int>, Referred<int>, Disowned<int>, Cloned<int>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DecayCast(i)), const int&>);
   delete value;
}

TEMPLATE_TEST_CASE("Testing DecayCast (non POD)", "[concepts]",
   Complex, Copied<Complex>, Referred<Complex>, Disowned<Complex>, Cloned<Complex>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DecayCast(i)), const Complex&>);
   delete value;
}
