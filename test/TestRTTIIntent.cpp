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
/*#include <Langulus/Anyness/SerializeText.hpp>
#include <Langulus/Anyness/SerializeBytes.hpp>*/


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
) {
   static_assert(    CT::Referred<  Refer<T>>);
   static_assert(not CT::Referred<   Move<T>>);
   static_assert(not CT::Referred<   Copy<T>>);
   static_assert(not CT::Referred<Abandon<T>>);
   static_assert(not CT::Referred< Disown<T>>);
   static_assert(not CT::Referred<  Clone<T>>);

   static_assert(    CT::ReferConstructible<T>);
   static_assert(    CT::ReferConstructible<T*>);
   static_assert(    CT::IntentConstructible<Refer, T>);
   static_assert(    CT::IntentConstructible<Refer, T*>);
   /*static_assert(    CT::IntentConstructibleAlt<Refer<T>>);
   static_assert(    CT::IntentConstructibleAlt<Refer<T*>>);*/

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
   new (test1) T {*test2};

   if constexpr (CT::Aggregate<T>) {
      static_assert(::std::same_as<decltype(Refer<int>::Nest(*test2)), T&>);
      static_assert(::std::same_as<decltype(Refer<int>::Nest(Clone(*test2))), T const&>);
      static_assert(::std::same_as<decltype(Refer<int>::Nest(Move(*test2))), T&&>);
   }
   else {
      static_assert(::std::same_as<decltype(Refer<int>::Nest(*test2)), Refer<T>>);
      static_assert(::std::same_as<decltype(Refer<int>::Nest(Clone(*test2))), Refer<T>>);
      static_assert(::std::same_as<decltype(Refer<int>::Nest(Move(*test2))), Refer<T>>);
   }
}

TEST_CASE_TEMPLATE("Testing non-refer-constructible types", T
   //IncompleteType, // should not compile at all
   , NonDestructible
   , PrivatelyConstructible
   , CopyConstructibleButNotAssignable
   , MoveConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   static_assert(not CT::ReferConstructible<T>);
   static_assert(    CT::ReferConstructible<T*>);
   static_assert(not CT::IntentConstructible<Refer, T>);
   static_assert(    CT::IntentConstructible<Refer, T*>);
   /*static_assert(not CT::IntentConstructibleAlt<Refer<T>>);
   static_assert(    CT::IntentConstructibleAlt<Refer<T*>>);*/

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
   static_assert(    CT::ReferAssignable<T>);
   static_assert(not CT::ReferAssignable<T const>);
   static_assert(    CT::ReferAssignable<T*>);
   static_assert(    CT::ReferAssignable<T const*>);
   static_assert(    CT::IntentAssignable<Refer, T>);
   static_assert(not CT::IntentAssignable<Refer, T const>);
   static_assert(    CT::IntentAssignable<Refer, T*>);
   static_assert(    CT::IntentAssignable<Refer, T const*>);
   /*static_assert(    CT::IntentAssignableAlt<Refer<T>>);
   static_assert(not CT::IntentAssignableAlt<Refer<T const>>);
   static_assert(    CT::IntentAssignableAlt<Refer<T*>>);
   static_assert(    CT::IntentAssignableAlt<Refer<T const*>>);*/

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
   *test1 = *test2;
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
   static_assert(not CT::ReferAssignable<T>);
   static_assert(not CT::ReferAssignable<T const>);
   static_assert(    CT::ReferAssignable<T*>);
   static_assert(    CT::ReferAssignable<T const*>);
   static_assert(not CT::IntentAssignable<Refer, T>);
   static_assert(not CT::IntentAssignable<Refer, T const>);
   static_assert(    CT::IntentAssignable<Refer, T*>);
   static_assert(    CT::IntentAssignable<Refer, T const*>);
   /*static_assert(not CT::IntentAssignableAlt<Refer<T>>);
   static_assert(not CT::IntentAssignableAlt<Refer<T const>>);
   static_assert(    CT::IntentAssignableAlt<Refer<T*>>);
   static_assert(    CT::IntentAssignableAlt<Refer<T const*>>);*/

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
) {
   static_assert(    CT::Moved<   Move<int>>);
   static_assert(not CT::Moved<  Refer<int>>);
   static_assert(not CT::Moved<   Copy<int>>);
   static_assert(not CT::Moved<Abandon<int>>);
   static_assert(not CT::Moved< Disown<int>>);
   static_assert(not CT::Moved<  Clone<int>>);

   static_assert(    CT::MoveConstructible<T>);
   static_assert(    CT::MoveConstructible<T*>);
   static_assert(    CT::IntentConstructible<Move, T>);
   static_assert(    CT::IntentConstructible<Move, T*>);
   /*static_assert(    CT::IntentConstructibleAlt<Move<T>>);
   static_assert(    CT::IntentConstructibleAlt<Move<T*>>);*/

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
   new (test1) T {LglsMov(*test2)};
}

TEST_CASE_TEMPLATE("Testing non-move-constructible types", T
   //IncompleteType, // should not compile at all
   , NonDestructible
   , PrivatelyConstructible
   , ReferConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , AbandonConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   static_assert(not CT::MoveConstructible<T>);
   static_assert(    CT::MoveConstructible<T*>);
   static_assert(not CT::IntentConstructible<Move, T>);
   static_assert(    CT::IntentConstructible<Move, T*>);
   /*static_assert(not CT::IntentConstructibleAlt<Move<T>>);
   static_assert(    CT::IntentConstructibleAlt<Move<T*>>);*/

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
   static_assert(    CT::MoveAssignable<T>);
   static_assert(not CT::MoveAssignable<T const>);
   static_assert(    CT::MoveAssignable<T*>);
   static_assert(    CT::MoveAssignable<T const*>);
   static_assert(    CT::IntentAssignable<Move, T>);
   static_assert(not CT::IntentAssignable<Move, T const>);
   static_assert(    CT::IntentAssignable<Move, T*>);
   static_assert(    CT::IntentAssignable<Move, T const*>);
   /*static_assert(    CT::IntentAssignableAlt<Move<T>>);
   //static_assert(not CT::IntentAssignableAlt<Move<T const>>); // should not compile
   static_assert(    CT::IntentAssignableAlt<Move<T*>>);
   static_assert(    CT::IntentAssignableAlt<Move<T const*>>);*/

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
   *test1 = LglsMov(*test2);
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
   static_assert(not CT::MoveAssignable<T>);
   static_assert(not CT::MoveAssignable<T const>);
   static_assert(    CT::MoveAssignable<T*>);
   static_assert(    CT::MoveAssignable<T const*>);
   static_assert(not CT::IntentAssignable<Move, T>);
   static_assert(not CT::IntentAssignable<Move, T const>);
   static_assert(    CT::IntentAssignable<Move, T*>);
   static_assert(    CT::IntentAssignable<Move, T const*>);
   /*static_assert(not CT::IntentAssignableAlt<Move<T>>);
   //static_assert(not CT::IntentAssignableAlt<Move<T const>>); // should not compile
   static_assert(    CT::IntentAssignableAlt<Move<T*>>);
   static_assert(    CT::IntentAssignableAlt<Move<T const*>>);*/

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
   static_assert(    CT::Copied<   Copy<int>>);
   static_assert(not CT::Copied<   Move<int>>);
   static_assert(not CT::Copied<  Refer<int>>);
   static_assert(not CT::Copied<Abandon<int>>);
   static_assert(not CT::Copied< Disown<int>>);
   static_assert(not CT::Copied<  Clone<int>>);

   static_assert(CT::CopyConstructible<T>);
   static_assert(CT::CopyConstructible<T*>);
   static_assert(CT::IntentConstructible<Copy, T>);
   static_assert(CT::IntentConstructible<Copy, T*>);
   /*static_assert(CT::IntentConstructibleAlt<Copy<T>>);
   static_assert(CT::IntentConstructibleAlt<Copy<T*>>);*/

   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetCopyConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetCopyConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetCopyConstructor());
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
   static_assert(not CT::CopyConstructible<T>);
   static_assert(    CT::CopyConstructible<T*>);
   static_assert(not CT::IntentConstructible<Copy, T>);
   static_assert(    CT::IntentConstructible<Copy, T*>);
   /*static_assert(not CT::IntentConstructibleAlt<Copy<T>>);
   static_assert(    CT::IntentConstructibleAlt<Copy<T*>>);*/

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
   static_assert(    CT::CopyAssignable<T>);
   static_assert(not CT::CopyAssignable<T const>);
   static_assert(    CT::CopyAssignable<T*>);
   static_assert(    CT::CopyAssignable<T const*>);
   static_assert(    CT::IntentAssignable<Copy, T>);
   static_assert(not CT::IntentAssignable<Copy, T const>);
   static_assert(    CT::IntentAssignable<Copy, T*>);
   static_assert(    CT::IntentAssignable<Copy, T const*>);
   /*static_assert(    CT::IntentAssignableAlt<Copy<T>>);
   //static_assert(not CT::IntentAssignableAlt<Copy<T const>>); // should not compile
   static_assert(    CT::IntentAssignableAlt<Copy<T*>>);
   static_assert(    CT::IntentAssignableAlt<Copy<T const*>>);*/

   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetCopyAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetCopyAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetCopyAssigner());
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
   static_assert(not CT::CopyAssignable<T>);
   static_assert(not CT::CopyAssignable<T const>);
   static_assert(    CT::CopyAssignable<T*>);
   static_assert(    CT::CopyAssignable<T const*>);
   static_assert(not CT::IntentAssignable<Copy, T>);
   static_assert(not CT::IntentAssignable<Copy, T const>);
   static_assert(    CT::IntentAssignable<Copy, T*>);
   static_assert(    CT::IntentAssignable<Copy, T const*>);
   /*static_assert(not CT::IntentAssignableAlt<Copy<T>>);
   static_assert(not CT::IntentAssignableAlt<Copy<T const>>);
   static_assert(    CT::IntentAssignableAlt<Copy<T*>>);
   static_assert(    CT::IntentAssignableAlt<Copy<T const*>>);*/

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
   static_assert(    CT::Cloned<  Clone<int>>);
   static_assert(not CT::Cloned<  Refer<int>>);
   static_assert(not CT::Cloned<   Copy<int>>);
   static_assert(not CT::Cloned<   Move<int>>);
   static_assert(not CT::Cloned<Abandon<int>>);
   static_assert(not CT::Cloned< Disown<int>>);

   static_assert(CT::CloneConstructible<T>);
   static_assert(CT::CloneConstructible<T*>);
   static_assert(CT::IntentConstructible<Clone, T>);
   static_assert(CT::IntentConstructible<Clone, T*>);
   /*static_assert(CT::IntentConstructibleAlt<Clone<T>>);
   static_assert(CT::IntentConstructibleAlt<Clone<T*>>);*/

   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetCloneConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetCloneConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetCloneConstructor());
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
   static_assert(not CT::CloneConstructible<T>);
   static_assert(not CT::CloneConstructible<T*>);
   static_assert(not CT::IntentConstructible<Clone, T>);
   static_assert(not CT::IntentConstructible<Clone, T*>);
   /*static_assert(not CT::IntentConstructibleAlt<Clone<T>>);
   static_assert(not CT::IntentConstructibleAlt<Clone<T*>>);*/

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
   static_assert(    CT::CloneAssignable<T>);
   static_assert(not CT::CloneAssignable<T const>);
   static_assert(    CT::CloneAssignable<T*>);
   static_assert(not CT::CloneAssignable<T const*>);
   static_assert(    CT::IntentAssignable<Clone, T>);
   static_assert(not CT::IntentAssignable<Clone, T const>);
   static_assert(    CT::IntentAssignable<Clone, T*>);
   static_assert(not CT::IntentAssignable<Clone, T const*>);
   /*static_assert(    CT::IntentAssignableAlt<Clone<T>>);
   static_assert(not CT::IntentAssignableAlt<Clone<T const>>);
   static_assert(    CT::IntentAssignableAlt<Clone<T*>>);
   static_assert(not CT::IntentAssignableAlt<Clone<T const*>>);*/

   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetCloneAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetCloneAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetCloneAssigner());
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
   static_assert(not CT::CloneAssignable<T>);
   static_assert(not CT::CloneAssignable<T const>);
   static_assert(not CT::CloneAssignable<T*>);
   static_assert(not CT::CloneAssignable<T const*>);
   static_assert(not CT::IntentAssignable<Clone, T>);
   static_assert(not CT::IntentAssignable<Clone, T const>);
   static_assert(not CT::IntentAssignable<Clone, T*>);
   static_assert(not CT::IntentAssignable<Clone, T const*>);
   /*static_assert(not CT::IntentAssignableAlt<Clone<T>>);
   static_assert(not CT::IntentAssignableAlt<Clone<T const>>);
   static_assert(not CT::IntentAssignableAlt<Clone<T*>>);
   static_assert(not CT::IntentAssignableAlt<Clone<T const*>>);*/

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
   static_assert(    CT::Disowned< Disown<int>>);
   static_assert(not CT::Disowned<  Refer<int>>);
   static_assert(not CT::Disowned<   Copy<int>>);
   static_assert(not CT::Disowned<   Move<int>>);
   static_assert(not CT::Disowned<Abandon<int>>);
   static_assert(not CT::Disowned<  Clone<int>>);

   static_assert(CT::DisownConstructible<T>);
   static_assert(CT::DisownConstructible<T*>);
   static_assert(CT::IntentConstructible<Disown, T>);
   static_assert(CT::IntentConstructible<Disown, T*>);
   /*static_assert(CT::IntentConstructibleAlt<Disown<T>>);
   static_assert(CT::IntentConstructibleAlt<Disown<T*>>);*/

   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetDisownConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetDisownConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetDisownConstructor());
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
   static_assert(not CT::DisownConstructible<T>);
   static_assert(    CT::DisownConstructible<T*>);
   static_assert(not CT::IntentConstructible<Disown, T>);
   static_assert(    CT::IntentConstructible<Disown, T*>);
   /*static_assert(not CT::IntentConstructibleAlt<Disown<T>>);
   static_assert(    CT::IntentConstructibleAlt<Disown<T*>>);*/

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
   static_assert(    CT::DisownAssignable<T>);
   static_assert(not CT::DisownAssignable<T const>);
   static_assert(    CT::DisownAssignable<T*>);
   static_assert(    CT::DisownAssignable<T const*>);
   static_assert(    CT::IntentAssignable<Disown, T>);
   static_assert(not CT::IntentAssignable<Disown, T const>);
   static_assert(    CT::IntentAssignable<Disown, T*>);
   static_assert(    CT::IntentAssignable<Disown, T const*>);
   /*static_assert(    CT::IntentAssignableAlt<Disown<T>>);
   static_assert(not CT::IntentAssignableAlt<Disown<T const>>);
   static_assert(    CT::IntentAssignableAlt<Disown<T*>>);
   static_assert(    CT::IntentAssignableAlt<Disown<T const*>>);*/

   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetDisownAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetDisownAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetDisownAssigner());
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
   static_assert(not CT::DisownAssignable<T>);
   static_assert(not CT::DisownAssignable<T const>);
   static_assert(    CT::DisownAssignable<T*>);
   static_assert(    CT::DisownAssignable<T const*>);
   static_assert(not CT::IntentAssignable<Disown, T>);
   static_assert(not CT::IntentAssignable<Disown, T const>);
   static_assert(    CT::IntentAssignable<Disown, T*>);
   static_assert(    CT::IntentAssignable<Disown, T const*>);
   /*static_assert(not CT::IntentAssignableAlt<Disown<T>>);
   static_assert(not CT::IntentAssignableAlt<Disown<T const>>);
   static_assert(    CT::IntentAssignableAlt<Disown<T*>>);
   static_assert(    CT::IntentAssignableAlt<Disown<T const*>>);*/

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
) {
   static_assert(    CT::Abandoned<Abandon<int>>);
   static_assert(not CT::Abandoned<  Refer<int>>);
   static_assert(not CT::Abandoned<   Copy<int>>);
   static_assert(not CT::Abandoned<   Move<int>>);
   static_assert(not CT::Abandoned< Disown<int>>);
   static_assert(not CT::Abandoned<  Clone<int>>);

   static_assert(CT::AbandonConstructible<T>);
   static_assert(CT::AbandonConstructible<T*>);
   static_assert(CT::IntentConstructible<Abandon, T>);
   static_assert(CT::IntentConstructible<Abandon, T*>);
   /*static_assert(CT::IntentConstructibleAlt<Abandon<T>>);
   static_assert(CT::IntentConstructibleAlt<Abandon<T*>>);*/

   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetAbandonConstructor());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetAbandonConstructor());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE(meta3.GetAbandonConstructor());
}

TEST_CASE_TEMPLATE("Testing non-abandon-constructible types", T
   //, IncompleteType // should not compile at all
   , NonDestructible
   , PrivatelyConstructible
   , ReferConstructibleButNotAssignable
   , CopyConstructibleButNotAssignable
   , DisownConstructibleButNotAssignable
   , CloneConstructibleButNotAssignable
) {
   static_assert(not CT::AbandonConstructible<T>);
   static_assert(    CT::AbandonConstructible<T*>);
   static_assert(not CT::IntentConstructible<Abandon, T>);
   static_assert(    CT::IntentConstructible<Abandon, T*>);
   /*static_assert(not CT::IntentConstructibleAlt<Abandon<T>>);
   static_assert(    CT::IntentConstructibleAlt<Abandon<T*>>);*/

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
   static_assert(    CT::AbandonAssignable<T>);
   static_assert(not CT::AbandonAssignable<T const>);
   static_assert(    CT::AbandonAssignable<T*>);
   static_assert(    CT::AbandonAssignable<T const*>);
   static_assert(    CT::IntentAssignable<Abandon, T>);
   static_assert(not CT::IntentAssignable<Abandon, T const>);
   static_assert(    CT::IntentAssignable<Abandon, T*>);
   static_assert(    CT::IntentAssignable<Abandon, T const*>);
   /*static_assert(    CT::IntentAssignableAlt<Abandon<T>>);
   //static_assert(not CT::IntentAssignableAlt<Abandon<T const>>); // should not compile
   static_assert(    CT::IntentAssignableAlt<Abandon<T*>>);
   static_assert(    CT::IntentAssignableAlt<Abandon<T const*>>);*/

   auto meta1 = MetaDataOf<T>();
   REQUIRE(meta1);
   REQUIRE(meta1.GetAbandonAssigner());

   auto meta2 = MetaDataOf<T*>();
   REQUIRE(meta2);
   REQUIRE(meta2.GetAbandonAssigner());

   auto meta3 = MetaDataOf<const T>();
   REQUIRE(meta3);
   REQUIRE_FALSE(meta3.GetAbandonAssigner());
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
   static_assert(not CT::AbandonAssignable<T>);
   static_assert(not CT::AbandonAssignable<T const>);
   static_assert(    CT::AbandonAssignable<T*>);
   static_assert(    CT::AbandonAssignable<T const*>);
   static_assert(not CT::IntentAssignable<Abandon, T>);
   static_assert(not CT::IntentAssignable<Abandon, T const>);
   static_assert(    CT::IntentAssignable<Abandon, T*>);
   static_assert(    CT::IntentAssignable<Abandon, T const*>);
   /*static_assert(not CT::IntentAssignableAlt<Abandon<T>>);
   //static_assert(not CT::IntentAssignableAlt<Abandon<T const>>); // should not compile
   static_assert(    CT::IntentAssignableAlt<Abandon<T*>>);
   static_assert(    CT::IntentAssignableAlt<Abandon<T const*>>);*/

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