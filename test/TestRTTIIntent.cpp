///                                                                           
/// Langulus::Core                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/Testing/CommonTypes.hpp>
#include <Langulus/IntentOf.hpp>
#include <Langulus/MetaOf.hpp>


///                                                                           
/// MARK: Refer intent                                                        
TEST_CASE_TEMPLATE("Testing refer-constructible types", T
   , ActualAggregateType
   , EmptyType
   , DestructibleType
   , NonIntentConstructible
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , PartiallyIntentConstructibleButImplicitly
   , Complex, ContainsComplex
   , ReferConstructibleButNotAssignable
   , ForcefullyPod
   , int
   , CustomTypedType
   , CustomUntypedType
   , NonDestructible //TODO moved due to changes in CT::ReferConstructible, pending verification
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetReferConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetReferConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetReferConstructor());

   alignas(T) char storage1[sizeof(T)] {};
   alignas(T) char storage2[sizeof(T)] {};
   auto test1 = reinterpret_cast<T*>(storage1);
   auto test2 = reinterpret_cast<T*>(storage2);
   new (test1) T {*test2};//TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-refer-constructible types", T
   //IncompleteType, // should not compile at all
   , PrivatelyConstructible
   , CopyConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetReferConstructor());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetReferConstructor());
}

TEST_CASE_TEMPLATE("Testing refer-assignable types", T
   , ActualAggregateType
   , EmptyType
   , NonDestructible
   , DestructibleType
   , NonIntentConstructible
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , PartiallyIntentConstructibleButImplicitly
   , int
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetReferAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetReferAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetReferAssigner());

   alignas(T) char storage1[sizeof(T)] {};
   alignas(T) char storage2[sizeof(T)] {};
   auto test1 = reinterpret_cast<T*>(storage1);
   auto test2 = reinterpret_cast<T*>(storage2);
   *test1 = *test2;//TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-refer-assignable types", T
   //, IncompleteType, // should not compile at all
   , Complex
   , ContainsComplex
   , PrivatelyConstructible
   , ReferConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
   , ForcefullyPod
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetReferAssigner());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetReferAssigner());
}


///                                                                           
/// MARK: Move intents                                                        
TEST_CASE_TEMPLATE("Testing move-constructible types", T
   , ActualAggregateType
   , EmptyType
   , DestructibleType
   , NonIntentConstructible
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , PartiallyIntentConstructibleButImplicitly
   , Complex
   , ContainsComplex
   , MoveConstructibleButNotAssignable
   , ForcefullyPod
   , int
      , NonDestructible //TODO this shifted with latest IntentNew
      , ReferConstructibleButNotAssignable //TODO this shifted with latest IntentNew
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetMoveConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetMoveConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetMoveConstructor());

   alignas(T) char storage1[sizeof(T)] {};
   alignas(T) char storage2[sizeof(T)] {};
   auto test1 = reinterpret_cast<T*>(storage1);
   auto test2 = reinterpret_cast<T*>(storage2);
   new (test1) T {LglsMov(*test2)}; //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-move-constructible types", T
   //IncompleteType, // should not compile at all
   , PrivatelyConstructible
   , CopyConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetMoveConstructor());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetMoveConstructor());
}

TEST_CASE_TEMPLATE("Testing move-assignable types", T
   , NonDestructible
   , ActualAggregateType
   , EmptyType
   , DestructibleType
   , NonIntentConstructible
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , PartiallyIntentConstructibleButImplicitly
   , int
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetMoveAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetMoveAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetMoveAssigner());

   alignas(T) char storage1[sizeof(T)] {};
   alignas(T) char storage2[sizeof(T)] {};
   auto test1 = reinterpret_cast<T*>(storage1);
   auto test2 = reinterpret_cast<T*>(storage2);
   *test1 = LglsMov(*test2); //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-move-assignable types", T
   //, IncompleteType // should not compile at all
   , Complex
   , ContainsComplex
   , PrivatelyConstructible
   , ReferConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
   , ForcefullyPod
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetMoveAssigner());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetMoveAssigner());
}


///                                                                           
/// MARK: Copy intents                                                        
TEST_CASE_TEMPLATE("Testing copy-constructible types", T
   , EmptyType
   , ActualAggregateType
   , AllIntentConstructible
   , AllIntentConstructibleImplicit
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , PartiallyIntentConstructibleButImplicitly
   , CopyConstructibleButNotAssignable
   , ForcefullyPod
   , int
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetCopyConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetCopyConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetCopyConstructor());

   //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-copy-constructible types", T
   //, IncompleteType // should not compile at all
   , NonDestructible
   , DestructibleType
   , Complex
   , ContainsComplex
   , PrivatelyConstructible
   , NonIntentConstructible
   , AggregateTypeComplex
   , ReferConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetCopyConstructor());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetCopyConstructor());
}

TEST_CASE_TEMPLATE("Testing copy-assignable types", T
   , EmptyType
   , ActualAggregateType
   , AllIntentConstructibleImplicit
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructibleButImplicitly
   , int
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetCopyAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetCopyAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetCopyAssigner());

   //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-copy-assignable types", T
   //, IncompleteType // should not compile at all
   , NonDestructible
   , DestructibleType
   , Complex
   , ContainsComplex
   , ForcefullyPod
   , PrivatelyConstructible
   , NonIntentConstructible
   , AllIntentConstructible
   , PartiallyIntentConstructible
   , AggregateTypeComplex
   , ReferConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetCopyAssigner());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetCopyAssigner());
}


///                                                                           
/// MARK: Clone intents                                                       
TEST_CASE_TEMPLATE("Testing clone-constructible types", T
   , EmptyType
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , PartiallyIntentConstructibleButImplicitly
   , CloneConstructibleButNotAssignable
   , ForcefullyPod
   , ActualAggregateType
   , int
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetCloneConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetCloneConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetCloneConstructor());

   //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-clone-constructible types", T
   //, IncompleteType // should not compile at all
   , NonDestructible
   , DestructibleType
   , PrivatelyConstructible
   , Complex
   , ContainsComplex
   , AggregateTypeComplex
   , NonIntentConstructible
   , ReferConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetCloneConstructor());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetCloneConstructor());
}

TEST_CASE_TEMPLATE("Testing clone-assignable types", T
   , EmptyType
   , AllIntentConstructibleImplicit
   , AllIntentConstructibleAndAssignable
   , ActualAggregateType
   , PartiallyIntentConstructibleButImplicitly
   , int
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetCloneAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetCloneAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetCloneAssigner());

   //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-clone-assignable types", T
   //, IncompleteType // should not compile at all
   , NonDestructible
   , DestructibleType
   , PrivatelyConstructible
   , NonIntentConstructible
   , Complex
   , ContainsComplex
   , AllIntentConstructible
   , PartiallyIntentConstructible
   , AggregateTypeComplex
   , ForcefullyPod
   , ReferConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetCloneAssigner());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetCloneAssigner());
}


///                                                                           
/// MARK: Disown intents                                                      
TEST_CASE_TEMPLATE("Testing disown-constructible types", T
   , EmptyType
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , PartiallyIntentConstructibleButImplicitly
   , DisownConstructibleButNotAssignable
   , ForcefullyPod
   , ActualAggregateType
   , int
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetDisownConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetDisownConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetDisownConstructor());

   //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-disown-constructible types", T
   //, IncompleteType // should not compile at all
   , NonDestructible
   , DestructibleType
   , PrivatelyConstructible
   , Complex
   , ContainsComplex
   , AggregateTypeComplex
   , NonIntentConstructible
   , ReferConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetDisownConstructor());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetDisownConstructor());
}

TEST_CASE_TEMPLATE("Testing disown-assignable types", T
   , EmptyType
   , AllIntentConstructibleImplicit
   , AllIntentConstructibleAndAssignable
   , ActualAggregateType
   , PartiallyIntentConstructibleButImplicitly
   , int
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetDisownAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetDisownAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetDisownAssigner());

   //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-disown-assignable types", T
   //, IncompleteType // should not compile at all
   , NonDestructible
   , DestructibleType
   , PrivatelyConstructible
   , NonIntentConstructible
   , Complex
   , ContainsComplex
   , ForcefullyPod
   , AllIntentConstructible
   , PartiallyIntentConstructible
   , ReferConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetDisownAssigner());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetDisownAssigner());
}


///                                                                           
/// MARK: Abandon semantics                                                   
TEST_CASE_TEMPLATE("Testing abandon-constructible types", T
   , EmptyType
   , DestructibleType
   , NonIntentConstructible
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , PartiallyIntentConstructibleButImplicitly
   , Complex
   , ContainsComplex
   , AbandonConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , ForcefullyPod
   , ActualAggregateType
   , int
      , ReferConstructibleButNotAssignable //TODO this shifted with latest IntentNew changes
      , NonDestructible //TODO this shifted with latest IntentNew changes
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetAbandonConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetAbandonConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetAbandonConstructor());

   //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-abandon-constructible types", T
   //, IncompleteType // should not compile at all
   , PrivatelyConstructible
   , CopyConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetAbandonConstructor());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetAbandonConstructor());
}

TEST_CASE_TEMPLATE("Testing abandon-assignable types", T
   , EmptyType
   , NonDestructible
   , DestructibleType
   , NonIntentConstructible
   , AllIntentConstructible
   , AllIntentConstructibleAndAssignable
   , PartiallyIntentConstructible
   , PartiallyIntentConstructibleButImplicitly
   , ActualAggregateType
   , int
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetAbandonAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetAbandonAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetAbandonAssigner());

   //TODO test reflected constructor instead
}

TEST_CASE_TEMPLATE("Testing non-abandon-assignable types", T
   //IncompleteType, // should not compile at all
   , Complex
   , ContainsComplex
   , ForcefullyPod
   , PrivatelyConstructible
   , ReferConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE_FALSE(meta1.GetAbandonAssigner());

   auto meta2 = MetaDataOf<const T>();
   REQUIRE(meta2);
   REQUIRE_FALSE(meta2.GetAbandonAssigner());
}


///                                                                           
/// MARK: Descriptor intents                                                  
/*TEMPLATE_TEST_CASE("Testing descriptor-makable types", "[ct]",//TODO
   AllIntentConstructible,
   AllIntentConstructibleAndAssignable
) {
   using T = TestType;
   static_assert(    CT::DescriptorConstructible<T>);
   static_assert(not CT::DescriptorConstructible<T*>);
   static_assert(not CT::IntentConstructibleAlt<Describe>);

   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1->mDescriptorConstructor);

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2->mDescriptorConstructor);
}

TEMPLATE_TEST_CASE("Testing non-descriptor-makable types", "[ct]",
   IncompleteType,
   EmptyType,
   NonDestructible,
   DestructibleType,
   PrivatelyConstructible,
   NonIntentConstructible,
   PartiallyIntentConstructible,
   Complex,
   ContainsComplex,
   ForcefullyPod,
   AggregateType,
   int
) {
   using T = TestType;
   static_assert(not CT::DescriptorConstructible<T>);
   static_assert(not CT::DescriptorConstructible<T*>);
   static_assert(not CT::IntentConstructibleAlt<Describe>);

   auto meta = MetaDataOf<Tif<CT::Complete<T>, T, T*>>();
   REQUIRE(meta);
   REQUIRE_FALSE(meta->mDescriptorConstructor);
}*/