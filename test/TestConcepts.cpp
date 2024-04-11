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



TEMPLATE_TEST_CASE("Testing CT::Defaultable", "[semantics]",
   // Dense                                                             
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   ImplicitlyConstructible,
   Destructible,
   // Sparse                                                            
   uint8_t*, uint16_t*, uint32_t*, uint64_t*,
   int8_t*, int16_t*, int32_t*, int64_t*,
   double*, float*,
   char*, wchar_t*, char8_t*, char16_t*, char32_t*,
   Langulus::Byte*,
   const uint8_t*, const uint16_t*, const uint32_t*, const uint64_t*,
   const int8_t*, const int16_t*, const int32_t*, const int64_t*,
   const double*, const float*,
   const char*, const wchar_t*, const char8_t*, const char16_t*, const char32_t*,
   const Langulus::Byte*,
   Complex*, ContainsComplex*,
   const Complex*, const ContainsComplex*
) {
   static_assert(CT::Defaultable<TestType>);
   auto meta = MetaData::Of<TestType>();
   REQUIRE(meta);
   REQUIRE(meta->mDefaultConstructor);
}

TEMPLATE_TEST_CASE("Testing not CT::Defaultable", "[semantics]",
   NonDestructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   ParticallySemanticConstructible,
   DescriptorConstructible,
   Complex,
   ContainsComplex,
   IncompleteType
) {
   static_assert(not CT::Defaultable<TestType>);
   auto meta = MetaData::Of<TestType>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mDefaultConstructor);
}

TEMPLATE_TEST_CASE("Testing CT::Destroyable<T>", "[semantics]",
   Destructible,
   Complex,
   ContainsComplex
) {
   static_assert(CT::Destroyable<TestType>);
   auto meta = MetaData::Of<TestType>();
   REQUIRE(meta);
   REQUIRE(meta->mDestructor);
}

TEMPLATE_TEST_CASE("Testing not CT::Destroyable<T>", "[semantics]",
   // Dense                                                             
   ImplicitlyConstructible,
   NonDestructible,
   PrivatelyConstructible,
   NonSemanticConstructible,
   AllSemanticConstructible,
   AllSemanticConstructibleAndAssignable,
   ParticallySemanticConstructible,
   DescriptorConstructible,
   IncompleteType,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte,
   // Sparse                                                            
   uint8_t*, uint16_t*, uint32_t*, uint64_t*,
   int8_t*, int16_t*, int32_t*, int64_t*,
   double*, float*,
   char*, wchar_t*, char8_t*, char16_t*, char32_t*,
   Langulus::Byte*,
   const uint8_t*, const uint16_t*, const uint32_t*, const uint64_t*,
   const int8_t*, const int16_t*, const int32_t*, const int64_t*,
   const double*, const float*,
   const char*, const wchar_t*, const char8_t*, const char16_t*, const char32_t*,
   const Langulus::Byte*,
   Complex*, ContainsComplex*,
   const Complex*, const ContainsComplex*
) {
   static_assert(not CT::Destroyable<TestType>);
   auto meta = MetaData::Of<TestType>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mDestructor);
}

TEMPLATE_TEST_CASE("Testing CT::POD<T>", "[semantics]",
   ImplicitlyConstructible,
   AMeta, TMeta, CMeta, DMeta, VMeta,
   uint8_t, uint16_t, uint32_t, uint64_t,
   int8_t, int16_t, int32_t, int64_t,
   double, float,
   char, wchar_t, char8_t, char16_t, char32_t,
   Langulus::Byte
) {
   static_assert(CT::POD<TestType>);
   auto meta = MetaData::Of<TestType>();
   REQUIRE(meta);
   REQUIRE(meta->mIsPOD);
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
