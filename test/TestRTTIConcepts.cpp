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
//TODO make sure tested types are the same as the tests in CT
//TODO make sure access to incomplete types throws until they are complete

///                                                                           
/// CT::Abstract                                                              
TEST_CASE_TEMPLATE("Testing CT::Abstract", T,
   PureAbstract,
   ForcedAbstractExternally,
   ForcedAbstractInternally
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.IsAbstract());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.IsAbstract());
}

TEST_CASE_TEMPLATE("Testing not CT::Abstract", T,
   //IncompleteType, //shouldn't compile
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
   ForcefullyPod
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(not meta1.IsAbstract());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.IsAbstract());
}

//TODO Unallocatable
//TODO Allocatable
//TODO Uninsertable
//TODO Insertable
//TODO Resolvable


///                                                                           
/// CT::Defaultable                                                           
TEST_CASE_TEMPLATE("Testing CT::Defaultable", T
   , bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte
   , EmptyType
   , ActualAggregateType
   , Complex*, ContainsComplex*
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetDefaultConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetDefaultConstructor());
}

TEST_CASE_TEMPLATE("Testing not CT::Defaultable", T
   //IncompleteType, //shouldn't compile
   , PrivatelyConstructible
   , NonIntentConstructible
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , DescribeConstructible
   , Complex
   , ContainsComplex
   , ForcefullyPod
   , PureAbstract
   , ForcedAbstractExternally
   , ForcedAbstractInternally
   , NonDestructible 
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetDefaultConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetDefaultConstructor());
}


///                                                                           
/// CT::Destroyable                                                           
TEST_CASE_TEMPLATE("Testing CT::Destroyable<T>", T
   //, IncompleteType                     // shouldn't compile
   //, IncompleteType const               // shouldn't compile
   //, SheddableType<IncompleteType>      // shouldn't compile
   //, SheddableType<PureAbstract>        // shouldn't compile
   //, SheddableType<PureAbstract const>  // shouldn't compile
   //, SheddableType<PureAbstract const&> // shouldn't compile
   , Complex
   , ContainsComplex
   , PureAbstract
   , ImpureVirtual
   , VirtuallyDerived
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetDestructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetDestructor());
}

TEST_CASE_TEMPLATE("Testing not CT::Destroyable<T>", T
   //, IncompleteType                // shouldn't compile
   //, IncompleteType const          // shouldn't compile
   //, SheddableType<IncompleteType> // shouldn't compile
   , EmptyType
   , NonDestructible
   , PrivatelyConstructible
   , NonIntentConstructible
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , DescribeConstructible
   , bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte
   , Complex*, ContainsComplex*
   , ForcedAbstractExternally
   , ForcedAbstractInternally
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetDestructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetDestructor());
}


///                                                                           
/// CT::POD                                                                   
TEST_CASE_TEMPLATE("Testing CT::POD<T>", T
   , EmptyType
   , TMeta, CMeta, DMeta, VMeta
   , bool, uint32_t, double, char, wchar_t, char8_t, Langulus::Byte
   , ForcefullyPod
   , ActualAggregateType
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.IsPOD());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.IsPOD());
}

TEST_CASE_TEMPLATE("Testing not CT::POD<T>", T
   //IncompleteType, //shouldn't compile
   , Complex
   , ContainsComplex
   , AggregateTypeComplex
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.IsPOD());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.IsPOD());
}