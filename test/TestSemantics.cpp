///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Main.hpp"
#include <catch2/catch.hpp>

class ImplicitlyConstructible {};

class NonDestructible {
   ~NonDestructible() = delete;
};

class Destructible {
public:
   char* someptr;
   ~Destructible() {
      if (someptr)
         delete someptr;
   }
};

class PrivatelyConstructible {
   PrivatelyConstructible() = default;
};

class NonSemanticConstructible {
public:
   NonSemanticConstructible(CT::NotSemantic auto&&) {}
};

class SemanticConstructible {
public:
   SemanticConstructible(CT::Semantic auto&&) {}
};

class DescriptorConstructible {
public:
   DescriptorConstructible(const Anyness::Neat&) {}
};


SCENARIO("Testing semantics", "[semantics]") {
   GIVEN("An implicitly constructible type") {
      auto meta = MetaData::Of<ImplicitlyConstructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(    CT::Defaultable<ImplicitlyConstructible>);
         static_assert(not CT::Destroyable<ImplicitlyConstructible>);
                           
         static_assert(    CT::CopyMakable<ImplicitlyConstructible>);
         static_assert(    CT::MoveMakable<ImplicitlyConstructible>);
         static_assert(    CT::CloneMakable<ImplicitlyConstructible>);
         static_assert(not CT::DisownMakable<ImplicitlyConstructible>);
         static_assert(    CT::AbandonMakable<ImplicitlyConstructible>);
         static_assert(not CT::DescriptorMakable<ImplicitlyConstructible>);

         static_assert(    CT::SemanticMakable<Copied, ImplicitlyConstructible>);
         static_assert(    CT::SemanticMakable<Moved, ImplicitlyConstructible>);
         static_assert(    CT::SemanticMakable<Cloned, ImplicitlyConstructible>);
         static_assert(not CT::SemanticMakable<Disowned, ImplicitlyConstructible>);
         static_assert(    CT::SemanticMakable<Abandoned, ImplicitlyConstructible>);

         static_assert(    CT::CopyAssignable<ImplicitlyConstructible>);
         static_assert(    CT::MoveAssignable<ImplicitlyConstructible>);
         static_assert(    CT::CloneAssignable<ImplicitlyConstructible>);
         static_assert(not CT::DisownAssignable<ImplicitlyConstructible>);
         static_assert(    CT::AbandonAssignable<ImplicitlyConstructible>);

         static_assert(    CT::SemanticAssignable<Copied, ImplicitlyConstructible>);
         static_assert(    CT::SemanticAssignable<Moved, ImplicitlyConstructible>);
         static_assert(    CT::SemanticAssignable<Cloned, ImplicitlyConstructible>);
         static_assert(not CT::SemanticAssignable<Disowned, ImplicitlyConstructible>);
         static_assert(    CT::SemanticAssignable<Abandoned, ImplicitlyConstructible>);

         THEN("Requirements should be met") {
            REQUIRE_FALSE(meta->mDestructor);
            REQUIRE      (meta->mDefaultConstructor);
            REQUIRE      (meta->mCopyConstructor);
            REQUIRE      (meta->mMoveConstructor);
            REQUIRE      (meta->mCloneConstructor);
            REQUIRE_FALSE(meta->mDisownConstructor);
            REQUIRE      (meta->mAbandonConstructor);
            REQUIRE_FALSE(meta->mDescriptorConstructor);

            REQUIRE      (meta->mCopier);
            REQUIRE      (meta->mMover);
            REQUIRE      (meta->mCloneCopier);
            REQUIRE_FALSE(meta->mDisownCopier);
            REQUIRE      (meta->mAbandonMover);
         }
      }
   }

   GIVEN("A non-destructible type") {
      auto meta = MetaData::Of<NonDestructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<NonDestructible>);
         static_assert(not CT::Destroyable<NonDestructible>);

         static_assert(    CT::CopyMakable<NonDestructible>);
         static_assert(    CT::MoveMakable<NonDestructible>);
         static_assert(not CT::CloneMakable<NonDestructible>);
         static_assert(not CT::DisownMakable<NonDestructible>);
         static_assert(    CT::AbandonMakable<NonDestructible>);
         static_assert(not CT::DescriptorMakable<NonDestructible>);

         static_assert(    CT::SemanticMakable<Copied, NonDestructible>);
         static_assert(    CT::SemanticMakable<Moved, NonDestructible>);
         static_assert(not CT::SemanticMakable<Cloned, NonDestructible>);
         static_assert(not CT::SemanticMakable<Disowned, NonDestructible>);
         static_assert(    CT::SemanticMakable<Abandoned, NonDestructible>);

         static_assert(    CT::CopyAssignable<NonDestructible>);
         static_assert(    CT::MoveAssignable<NonDestructible>);
         static_assert(not CT::CloneAssignable<NonDestructible>);
         static_assert(not CT::DisownAssignable<NonDestructible>);
         static_assert(    CT::AbandonAssignable<NonDestructible>);

         static_assert(    CT::SemanticAssignable<Copied, NonDestructible>);
         static_assert(    CT::SemanticAssignable<Moved, NonDestructible>);
         static_assert(not CT::SemanticAssignable<Cloned, NonDestructible>);
         static_assert(not CT::SemanticAssignable<Disowned, NonDestructible>);
         static_assert(    CT::SemanticAssignable<Abandoned, NonDestructible>);

         THEN("Requirements should be met") {
            REQUIRE_FALSE(meta->mDestructor);
            REQUIRE_FALSE(meta->mDefaultConstructor);
            REQUIRE      (meta->mCopyConstructor);
            REQUIRE      (meta->mMoveConstructor);
            REQUIRE_FALSE(meta->mCloneConstructor);
            REQUIRE_FALSE(meta->mDisownConstructor);
            REQUIRE      (meta->mAbandonConstructor);
            REQUIRE_FALSE(meta->mDescriptorConstructor);

            REQUIRE      (meta->mCopier);
            REQUIRE      (meta->mMover);
            REQUIRE_FALSE(meta->mCloneCopier);
            REQUIRE_FALSE(meta->mDisownCopier);
            REQUIRE      (meta->mAbandonMover);
         }
      }
   }

   GIVEN("An explicitly destructible type") {
      auto meta = MetaData::Of<Destructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(    CT::Defaultable<Destructible>);
         static_assert(    CT::Destroyable<Destructible>);

         static_assert(    CT::CopyMakable<Destructible>);
         static_assert(    CT::MoveMakable<Destructible>);
         static_assert(not CT::CloneMakable<Destructible>);
         static_assert(not CT::DisownMakable<Destructible>);
         static_assert(not CT::AbandonMakable<Destructible>);
         static_assert(not CT::DescriptorMakable<Destructible>);

         static_assert(    CT::SemanticMakable<Copied, Destructible>);
         static_assert(    CT::SemanticMakable<Moved, Destructible>);
         static_assert(not CT::SemanticMakable<Cloned, Destructible>);
         static_assert(not CT::SemanticMakable<Disowned, Destructible>);
         static_assert(not CT::SemanticMakable<Abandoned, Destructible>);

         static_assert(    CT::CopyAssignable<Destructible>);
         static_assert(    CT::MoveAssignable<Destructible>);
         static_assert(not CT::CloneAssignable<Destructible>);
         static_assert(not CT::DisownAssignable<Destructible>);
         static_assert(not CT::AbandonAssignable<Destructible>);

         static_assert(    CT::SemanticAssignable<Copied, Destructible>);
         static_assert(    CT::SemanticAssignable<Moved, Destructible>);
         static_assert(not CT::SemanticAssignable<Cloned, Destructible>);
         static_assert(not CT::SemanticAssignable<Disowned, Destructible>);
         static_assert(not CT::SemanticAssignable<Abandoned, Destructible>);

         THEN("Requirements should be met") {
            REQUIRE      (meta->mDestructor);
            REQUIRE      (meta->mDefaultConstructor);
            REQUIRE      (meta->mCopyConstructor);
            REQUIRE      (meta->mMoveConstructor);
            REQUIRE_FALSE(meta->mCloneConstructor);
            REQUIRE_FALSE(meta->mDisownConstructor);
            REQUIRE_FALSE(meta->mAbandonConstructor);
            REQUIRE_FALSE(meta->mDescriptorConstructor);

            REQUIRE      (meta->mCopier);
            REQUIRE      (meta->mMover);
            REQUIRE_FALSE(meta->mCloneCopier);
            REQUIRE_FALSE(meta->mDisownCopier);
            REQUIRE_FALSE(meta->mAbandonMover);
         }
      }
   }

   GIVEN("A privately-constructible type") {
      auto meta = MetaData::Of<PrivatelyConstructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<PrivatelyConstructible>);
         static_assert(    CT::Destroyable<PrivatelyConstructible>);

         static_assert(    CT::CopyMakable<PrivatelyConstructible>);
         static_assert(    CT::MoveMakable<PrivatelyConstructible>);
         static_assert(not CT::CloneMakable<PrivatelyConstructible>);
         static_assert(not CT::DisownMakable<PrivatelyConstructible>);
         static_assert(not CT::AbandonMakable<PrivatelyConstructible>);
         static_assert(not CT::DescriptorMakable<PrivatelyConstructible>);

         static_assert(    CT::SemanticMakable<Copied, PrivatelyConstructible>);
         static_assert(    CT::SemanticMakable<Moved, PrivatelyConstructible>);
         static_assert(not CT::SemanticMakable<Cloned, PrivatelyConstructible>);
         static_assert(not CT::SemanticMakable<Disowned, PrivatelyConstructible>);
         static_assert(not CT::SemanticMakable<Abandoned, PrivatelyConstructible>);

         static_assert(    CT::CopyAssignable<PrivatelyConstructible>);
         static_assert(    CT::MoveAssignable<PrivatelyConstructible>);
         static_assert(not CT::CloneAssignable<PrivatelyConstructible>);
         static_assert(not CT::DisownAssignable<PrivatelyConstructible>);
         static_assert(not CT::AbandonAssignable<PrivatelyConstructible>);

         static_assert(    CT::SemanticAssignable<Copied, PrivatelyConstructible>);
         static_assert(    CT::SemanticAssignable<Moved, PrivatelyConstructible>);
         static_assert(not CT::SemanticAssignable<Cloned, PrivatelyConstructible>);
         static_assert(not CT::SemanticAssignable<Disowned, PrivatelyConstructible>);
         static_assert(not CT::SemanticAssignable<Abandoned, PrivatelyConstructible>);

         THEN("Requirements should be met") {
            REQUIRE      (meta->mDestructor);
            REQUIRE_FALSE(meta->mDefaultConstructor);
            REQUIRE      (meta->mCopyConstructor);
            REQUIRE      (meta->mMoveConstructor);
            REQUIRE_FALSE(meta->mCloneConstructor);
            REQUIRE_FALSE(meta->mDisownConstructor);
            REQUIRE_FALSE(meta->mAbandonConstructor);
            REQUIRE_FALSE(meta->mDescriptorConstructor);

            REQUIRE      (meta->mCopier);
            REQUIRE      (meta->mMover);
            REQUIRE_FALSE(meta->mCloneCopier);
            REQUIRE_FALSE(meta->mDisownCopier);
            REQUIRE_FALSE(meta->mAbandonMover);
         }
      }
   }

   GIVEN("A non-semantically-constructible type") {
      auto meta = MetaData::Of<NonSemanticConstructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<NonSemanticConstructible>);
         static_assert(    CT::Destroyable<NonSemanticConstructible>);

         static_assert(    CT::CopyMakable<NonSemanticConstructible>);
         static_assert(    CT::MoveMakable<NonSemanticConstructible>);
         static_assert(not CT::CloneMakable<NonSemanticConstructible>);
         static_assert(not CT::DisownMakable<NonSemanticConstructible>);
         static_assert(not CT::AbandonMakable<NonSemanticConstructible>);
         static_assert(    CT::DescriptorMakable<NonSemanticConstructible>);

         static_assert(    CT::SemanticMakable<Copied, NonSemanticConstructible>);
         static_assert(    CT::SemanticMakable<Moved, NonSemanticConstructible>);
         static_assert(not CT::SemanticMakable<Cloned, NonSemanticConstructible>);
         static_assert(not CT::SemanticMakable<Disowned, NonSemanticConstructible>);
         static_assert(not CT::SemanticMakable<Abandoned, NonSemanticConstructible>);

         static_assert(    CT::CopyAssignable<NonSemanticConstructible>);
         static_assert(    CT::MoveAssignable<NonSemanticConstructible>);
         static_assert(not CT::CloneAssignable<NonSemanticConstructible>);
         static_assert(not CT::DisownAssignable<NonSemanticConstructible>);
         static_assert(not CT::AbandonAssignable<NonSemanticConstructible>);

         static_assert(    CT::SemanticAssignable<Copied, NonSemanticConstructible>);
         static_assert(    CT::SemanticAssignable<Moved, NonSemanticConstructible>);
         static_assert(not CT::SemanticAssignable<Cloned, NonSemanticConstructible>);
         static_assert(not CT::SemanticAssignable<Disowned, NonSemanticConstructible>);
         static_assert(not CT::SemanticAssignable<Abandoned, NonSemanticConstructible>);

        THEN("Requirements should be met") {
            REQUIRE      (meta->mDestructor);
            REQUIRE_FALSE(meta->mDefaultConstructor);
            REQUIRE      (meta->mCopyConstructor);
            REQUIRE      (meta->mMoveConstructor);
            REQUIRE_FALSE(meta->mCloneConstructor);
            REQUIRE_FALSE(meta->mDisownConstructor);
            REQUIRE_FALSE(meta->mAbandonConstructor);
            REQUIRE      (meta->mDescriptorConstructor);

            REQUIRE      (meta->mCopier);
            REQUIRE      (meta->mMover);
            REQUIRE_FALSE(meta->mCloneCopier);
            REQUIRE_FALSE(meta->mDisownCopier);
            REQUIRE_FALSE(meta->mAbandonMover);
         }
      }
   }

   GIVEN("A semantically-constructible type") {
      auto meta = MetaData::Of<SemanticConstructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<SemanticConstructible>);
         static_assert(    CT::Destroyable<SemanticConstructible>);

         static_assert(    CT::CopyMakable<SemanticConstructible>);
         static_assert(    CT::MoveMakable<SemanticConstructible>);
         static_assert(    CT::CloneMakable<SemanticConstructible>);
         static_assert(    CT::DisownMakable<SemanticConstructible>);
         static_assert(    CT::AbandonMakable<SemanticConstructible>);
         static_assert(not CT::DescriptorMakable<SemanticConstructible>);

         static_assert(    CT::SemanticMakable<Copied, SemanticConstructible>);
         static_assert(    CT::SemanticMakable<Moved, SemanticConstructible>);
         static_assert(    CT::SemanticMakable<Cloned, SemanticConstructible>);
         static_assert(    CT::SemanticMakable<Disowned, SemanticConstructible>);
         static_assert(    CT::SemanticMakable<Abandoned, SemanticConstructible>);

         static_assert(    CT::CopyAssignable<SemanticConstructible>);
         static_assert(    CT::MoveAssignable<SemanticConstructible>);
         static_assert(    CT::CloneAssignable<SemanticConstructible>);
         static_assert(    CT::DisownAssignable<SemanticConstructible>);
         static_assert(    CT::AbandonAssignable<SemanticConstructible>);

         static_assert(    CT::SemanticAssignable<Copied, SemanticConstructible>);
         static_assert(    CT::SemanticAssignable<Moved, SemanticConstructible>);
         static_assert(    CT::SemanticAssignable<Cloned, SemanticConstructible>);
         static_assert(    CT::SemanticAssignable<Disowned, SemanticConstructible>);
         static_assert(    CT::SemanticAssignable<Abandoned, SemanticConstructible>);

         THEN("Requirements should be met") {
            REQUIRE      (meta->mDestructor);
            REQUIRE_FALSE(meta->mDefaultConstructor);
            REQUIRE      (meta->mCopyConstructor);
            REQUIRE      (meta->mMoveConstructor);
            REQUIRE      (meta->mCloneConstructor);
            REQUIRE      (meta->mDisownConstructor);
            REQUIRE      (meta->mAbandonConstructor);
            REQUIRE_FALSE(meta->mDescriptorConstructor);

            REQUIRE      (meta->mCopier);
            REQUIRE      (meta->mMover);
            REQUIRE      (meta->mCloneCopier);
            REQUIRE      (meta->mDisownCopier);
            REQUIRE      (meta->mAbandonMover);
         }
      }
   }

   GIVEN("A descriptor-constructible type") {
      auto meta = MetaData::Of<DescriptorConstructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<DescriptorConstructible>);
         static_assert(    CT::Destroyable<DescriptorConstructible>);

         static_assert(    CT::CopyMakable<DescriptorConstructible>);
         static_assert(    CT::MoveMakable<DescriptorConstructible>);
         static_assert(not CT::CloneMakable<DescriptorConstructible>);
         static_assert(not CT::DisownMakable<DescriptorConstructible>);
         static_assert(not CT::AbandonMakable<DescriptorConstructible>);
         static_assert(    CT::DescriptorMakable<DescriptorConstructible>);

         static_assert(    CT::SemanticMakable<Copied, DescriptorConstructible>);
         static_assert(    CT::SemanticMakable<Moved, DescriptorConstructible>);
         static_assert(not CT::SemanticMakable<Cloned, DescriptorConstructible>);
         static_assert(not CT::SemanticMakable<Disowned, DescriptorConstructible>);
         static_assert(not CT::SemanticMakable<Abandoned, DescriptorConstructible>);

         static_assert(    CT::CopyAssignable<DescriptorConstructible>);
         static_assert(    CT::MoveAssignable<DescriptorConstructible>);
         static_assert(not CT::CloneAssignable<DescriptorConstructible>);
         static_assert(not CT::DisownAssignable<DescriptorConstructible>);
         static_assert(not CT::AbandonAssignable<DescriptorConstructible>);

         static_assert(    CT::SemanticAssignable<Copied, DescriptorConstructible>);
         static_assert(    CT::SemanticAssignable<Moved, DescriptorConstructible>);
         static_assert(not CT::SemanticAssignable<Cloned, DescriptorConstructible>);
         static_assert(not CT::SemanticAssignable<Disowned, DescriptorConstructible>);
         static_assert(not CT::SemanticAssignable<Abandoned, DescriptorConstructible>);

         THEN("Requirements should be met") {
            REQUIRE      (meta->mDestructor);
            REQUIRE_FALSE(meta->mDefaultConstructor);
            REQUIRE      (meta->mCopyConstructor);
            REQUIRE      (meta->mMoveConstructor);
            REQUIRE_FALSE(meta->mCloneConstructor);
            REQUIRE_FALSE(meta->mDisownConstructor);
            REQUIRE_FALSE(meta->mAbandonConstructor);
            REQUIRE      (meta->mDescriptorConstructor);

            REQUIRE      (meta->mCopier);
            REQUIRE      (meta->mMover);
            REQUIRE_FALSE(meta->mCloneCopier);
            REQUIRE_FALSE(meta->mDisownCopier);
            REQUIRE_FALSE(meta->mAbandonMover);
         }
      }
   }
   
   GIVEN("A ImplicitlyReflectedDataWithTraits2 type") {
      auto meta = MetaData::Of<ImplicitlyReflectedDataWithTraits2>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(    CT::Destroyable<ImplicitlyReflectedDataWithTraits2>);

         static_assert(    CT::CopyMakable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(    CT::MoveMakable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::CloneMakable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::DisownMakable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::AbandonMakable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::DescriptorMakable<ImplicitlyReflectedDataWithTraits2>);

         static_assert(    CT::SemanticMakable<Copied, ImplicitlyReflectedDataWithTraits2>);
         static_assert(    CT::SemanticMakable<Moved, ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::SemanticMakable<Cloned, ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::SemanticMakable<Disowned, ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::SemanticMakable<Abandoned, ImplicitlyReflectedDataWithTraits2>);

         static_assert(    CT::CopyAssignable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(    CT::MoveAssignable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::CloneAssignable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::DisownAssignable<ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::AbandonAssignable<ImplicitlyReflectedDataWithTraits2>);

         static_assert(    CT::SemanticAssignable<Copied, ImplicitlyReflectedDataWithTraits2>);
         static_assert(    CT::SemanticAssignable<Moved, ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::SemanticAssignable<Cloned, ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::SemanticAssignable<Disowned, ImplicitlyReflectedDataWithTraits2>);
         static_assert(not CT::SemanticAssignable<Abandoned, ImplicitlyReflectedDataWithTraits2>);

         THEN("Requirements should be met") {
            REQUIRE      (meta->mDestructor);
            REQUIRE_FALSE(meta->mDefaultConstructor);
            REQUIRE      (meta->mCopyConstructor);
            REQUIRE      (meta->mMoveConstructor);
            REQUIRE_FALSE(meta->mCloneConstructor);
            REQUIRE_FALSE(meta->mDisownConstructor);
            REQUIRE_FALSE(meta->mAbandonConstructor);
            REQUIRE_FALSE(meta->mDescriptorConstructor);

            REQUIRE      (meta->mCopier);
            REQUIRE      (meta->mMover);
            REQUIRE_FALSE(meta->mCloneCopier);
            REQUIRE_FALSE(meta->mDisownCopier);
            REQUIRE_FALSE(meta->mAbandonMover);
         }
      }
   }

   GIVEN("An incomplete type") {
      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<IncompleteType>);
         static_assert(not CT::Destroyable<IncompleteType>);

         static_assert(not CT::CopyMakable<IncompleteType>);
         static_assert(not CT::MoveMakable<IncompleteType>);
         static_assert(not CT::CloneMakable<IncompleteType>);
         static_assert(not CT::DisownMakable<IncompleteType>);
         static_assert(not CT::AbandonMakable<IncompleteType>);
         static_assert(not CT::DescriptorMakable<IncompleteType>);

         static_assert(not CT::SemanticMakable<Copied, IncompleteType>);
         static_assert(not CT::SemanticMakable<Moved, IncompleteType>);
         static_assert(not CT::SemanticMakable<Cloned, IncompleteType>);
         static_assert(not CT::SemanticMakable<Disowned, IncompleteType>);
         static_assert(not CT::SemanticMakable<Abandoned, IncompleteType>);

         static_assert(not CT::CopyAssignable<IncompleteType>);
         static_assert(not CT::MoveAssignable<IncompleteType>);
         static_assert(not CT::CloneAssignable<IncompleteType>);
         static_assert(not CT::DisownAssignable<IncompleteType>);
         static_assert(not CT::AbandonAssignable<IncompleteType>);

         static_assert(not CT::Referencable<IncompleteType>);
         static_assert(not CT::Swappable<IncompleteType>);
         static_assert(not CT::SwappableNoexcept<IncompleteType>);
         static_assert(not CT::Resolvable<IncompleteType>);
         static_assert(not CT::Hashable<IncompleteType>);
         static_assert(not CT::DerivedFrom<IncompleteType, ImplicitlyReflectedData>);

         static_assert(not CT::SemanticAssignable<Copied, IncompleteType>);
         static_assert(not CT::SemanticAssignable<Moved, IncompleteType>);
         static_assert(not CT::SemanticAssignable<Cloned, IncompleteType>);
         static_assert(not CT::SemanticAssignable<Disowned, IncompleteType>);
         static_assert(not CT::SemanticAssignable<Abandoned, IncompleteType>);
      }
   }
}

TEMPLATE_TEST_CASE("Testing semantics of fundamentals", "[semantics]",
   uint8_t, uint16_t, uint32_t, uint64_t,
    int8_t,  int16_t,  int32_t,  int64_t,
   double, float, char, wchar_t, char8_t, char16_t, char32_t, Langulus::Byte
) {
   GIVEN("A fundamental type") {
      auto meta = MetaData::Of<TestType>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(    CT::Defaultable<TestType>);
         static_assert(not CT::Destroyable<TestType>);

         static_assert(    CT::CopyMakable<TestType>);
         static_assert(    CT::MoveMakable<TestType>);
         static_assert(    CT::CloneMakable<TestType>);
         static_assert(not CT::DisownMakable<TestType>);
         static_assert(    CT::AbandonMakable<TestType>);
         static_assert(not CT::DescriptorMakable<TestType>);

         static_assert(    CT::SemanticMakable<Copied, TestType>);
         static_assert(    CT::SemanticMakable<Moved, TestType>);
         static_assert(    CT::SemanticMakable<Cloned, TestType>);
         static_assert(not CT::SemanticMakable<Disowned, TestType>);
         static_assert(    CT::SemanticMakable<Abandoned, TestType>);

         static_assert(    CT::CopyAssignable<TestType>);
         static_assert(    CT::MoveAssignable<TestType>);
         static_assert(    CT::CloneAssignable<TestType>);
         static_assert(not CT::DisownAssignable<TestType>);
         static_assert(    CT::AbandonAssignable<TestType>);

         static_assert(    CT::SemanticAssignable<Copied, TestType>);
         static_assert(    CT::SemanticAssignable<Moved, TestType>);
         static_assert(    CT::SemanticAssignable<Cloned, TestType>);
         static_assert(not CT::SemanticAssignable<Disowned, TestType>);
         static_assert(    CT::SemanticAssignable<Abandoned, TestType>);

         THEN("Requirements should be met") {
            REQUIRE_FALSE(meta->mDestructor);
            REQUIRE      (meta->mDefaultConstructor);
            REQUIRE      (meta->mCopyConstructor);
            REQUIRE      (meta->mMoveConstructor);
            REQUIRE      (meta->mCloneConstructor);
            REQUIRE_FALSE(meta->mDisownConstructor);
            REQUIRE      (meta->mAbandonConstructor);
            REQUIRE_FALSE(meta->mDescriptorConstructor);

            REQUIRE      (meta->mCopier);
            REQUIRE      (meta->mMover);
            REQUIRE      (meta->mCloneCopier);
            REQUIRE_FALSE(meta->mDisownCopier);
            REQUIRE      (meta->mAbandonMover);
         }
      }
   }
}
