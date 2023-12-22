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
   DescriptorConstructible(Describe&&) {}
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
         static_assert(    CT::DisownMakable<ImplicitlyConstructible>);
         static_assert(    CT::AbandonMakable<ImplicitlyConstructible>);
         static_assert(not CT::DescriptorMakable<ImplicitlyConstructible>);

         static_assert(    CT::SemanticMakable<Copied, ImplicitlyConstructible>);
         static_assert(    CT::SemanticMakable<Moved, ImplicitlyConstructible>);
         static_assert(    CT::SemanticMakable<Cloned, ImplicitlyConstructible>);
         static_assert(    CT::SemanticMakable<Disowned, ImplicitlyConstructible>);
         static_assert(    CT::SemanticMakable<Abandoned, ImplicitlyConstructible>);

         static_assert(    CT::CopyAssignable<ImplicitlyConstructible>);
         static_assert(    CT::MoveAssignable<ImplicitlyConstructible>);
         static_assert(    CT::CloneAssignable<ImplicitlyConstructible>);
         static_assert(    CT::DisownAssignable<ImplicitlyConstructible>);
         static_assert(    CT::AbandonAssignable<ImplicitlyConstructible>);

         static_assert(    CT::SemanticAssignable<Copied, ImplicitlyConstructible>);
         static_assert(    CT::SemanticAssignable<Moved, ImplicitlyConstructible>);
         static_assert(    CT::SemanticAssignable<Cloned, ImplicitlyConstructible>);
         static_assert(    CT::SemanticAssignable<Disowned, ImplicitlyConstructible>);
         static_assert(    CT::SemanticAssignable<Abandoned, ImplicitlyConstructible>);

         REQUIRE_FALSE(meta->mDestructor);
         REQUIRE      (meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE      (meta->mCloneConstructor);
         REQUIRE      (meta->mDisownConstructor);
         REQUIRE      (meta->mAbandonConstructor);
         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE      (meta->mCloneAssigner);
         REQUIRE      (meta->mDisownAssigner);
         REQUIRE      (meta->mAbandonAssigner);
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
         static_assert(    CT::CloneMakable<NonDestructible>);
         static_assert(    CT::DisownMakable<NonDestructible>);
         static_assert(    CT::AbandonMakable<NonDestructible>);
         static_assert(not CT::DescriptorMakable<NonDestructible>);

         static_assert(    CT::SemanticMakable<Copied, NonDestructible>);
         static_assert(    CT::SemanticMakable<Moved, NonDestructible>);
         static_assert(    CT::SemanticMakable<Cloned, NonDestructible>);
         static_assert(    CT::SemanticMakable<Disowned, NonDestructible>);
         static_assert(    CT::SemanticMakable<Abandoned, NonDestructible>);

         static_assert(    CT::CopyAssignable<NonDestructible>);
         static_assert(    CT::MoveAssignable<NonDestructible>);
         static_assert(    CT::CloneAssignable<NonDestructible>);
         static_assert(    CT::DisownAssignable<NonDestructible>);
         static_assert(    CT::AbandonAssignable<NonDestructible>);

         static_assert(    CT::SemanticAssignable<Copied, NonDestructible>);
         static_assert(    CT::SemanticAssignable<Moved, NonDestructible>);
         static_assert(    CT::SemanticAssignable<Cloned, NonDestructible>);
         static_assert(    CT::SemanticAssignable<Disowned, NonDestructible>);
         static_assert(    CT::SemanticAssignable<Abandoned, NonDestructible>);

         REQUIRE_FALSE(meta->mDestructor);
         REQUIRE_FALSE(meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE      (meta->mCloneConstructor);
         REQUIRE      (meta->mDisownConstructor);
         REQUIRE      (meta->mAbandonConstructor);
         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE      (meta->mCloneAssigner);
         REQUIRE      (meta->mDisownAssigner);
         REQUIRE      (meta->mAbandonAssigner);
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

         REQUIRE      (meta->mDestructor);
         REQUIRE      (meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE_FALSE(meta->mCloneConstructor);
         REQUIRE_FALSE(meta->mDisownConstructor);
         REQUIRE_FALSE(meta->mAbandonConstructor);
         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE_FALSE(meta->mCloneAssigner);
         REQUIRE_FALSE(meta->mDisownAssigner);
         REQUIRE_FALSE(meta->mAbandonAssigner);
      }
   }

   GIVEN("A privately-constructible type") {
      auto meta = MetaData::Of<PrivatelyConstructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<PrivatelyConstructible>);
         static_assert(not CT::Destroyable<PrivatelyConstructible>);

         static_assert(    CT::CopyMakable<PrivatelyConstructible>);
         static_assert(    CT::MoveMakable<PrivatelyConstructible>);
         static_assert(    CT::CloneMakable<PrivatelyConstructible>);
         static_assert(    CT::DisownMakable<PrivatelyConstructible>);
         static_assert(    CT::AbandonMakable<PrivatelyConstructible>);
         static_assert(not CT::DescriptorMakable<PrivatelyConstructible>);

         static_assert(    CT::SemanticMakable<Copied, PrivatelyConstructible>);
         static_assert(    CT::SemanticMakable<Moved, PrivatelyConstructible>);
         static_assert(    CT::SemanticMakable<Cloned, PrivatelyConstructible>);
         static_assert(    CT::SemanticMakable<Disowned, PrivatelyConstructible>);
         static_assert(    CT::SemanticMakable<Abandoned, PrivatelyConstructible>);

         static_assert(    CT::CopyAssignable<PrivatelyConstructible>);
         static_assert(    CT::MoveAssignable<PrivatelyConstructible>);
         static_assert(    CT::CloneAssignable<PrivatelyConstructible>);
         static_assert(    CT::DisownAssignable<PrivatelyConstructible>);
         static_assert(    CT::AbandonAssignable<PrivatelyConstructible>);

         static_assert(    CT::SemanticAssignable<Copied, PrivatelyConstructible>);
         static_assert(    CT::SemanticAssignable<Moved, PrivatelyConstructible>);
         static_assert(    CT::SemanticAssignable<Cloned, PrivatelyConstructible>);
         static_assert(    CT::SemanticAssignable<Disowned, PrivatelyConstructible>);
         static_assert(    CT::SemanticAssignable<Abandoned, PrivatelyConstructible>);

         REQUIRE_FALSE(meta->mDestructor);
         REQUIRE_FALSE(meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE      (meta->mCloneConstructor);
         REQUIRE      (meta->mDisownConstructor);
         REQUIRE      (meta->mAbandonConstructor);
         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE      (meta->mCloneAssigner);
         REQUIRE      (meta->mDisownAssigner);
         REQUIRE      (meta->mAbandonAssigner);
      }
   }

   GIVEN("A non-semantically-constructible type") {
      auto meta = MetaData::Of<NonSemanticConstructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<NonSemanticConstructible>);
         static_assert(not CT::Destroyable<NonSemanticConstructible>);

         static_assert(    CT::CopyMakable<NonSemanticConstructible>);
         static_assert(    CT::MoveMakable<NonSemanticConstructible>);
         static_assert(    CT::CloneMakable<NonSemanticConstructible>);
         static_assert(    CT::DisownMakable<NonSemanticConstructible>);
         static_assert(    CT::AbandonMakable<NonSemanticConstructible>);
         static_assert(not CT::DescriptorMakable<NonSemanticConstructible>);

         static_assert(    CT::SemanticMakable<Copied, NonSemanticConstructible>);
         static_assert(    CT::SemanticMakable<Moved, NonSemanticConstructible>);
         static_assert(    CT::SemanticMakable<Cloned, NonSemanticConstructible>);
         static_assert(    CT::SemanticMakable<Disowned, NonSemanticConstructible>);
         static_assert(    CT::SemanticMakable<Abandoned, NonSemanticConstructible>);

         static_assert(    CT::CopyAssignable<NonSemanticConstructible>);
         static_assert(    CT::MoveAssignable<NonSemanticConstructible>);
         static_assert(    CT::CloneAssignable<NonSemanticConstructible>);
         static_assert(    CT::DisownAssignable<NonSemanticConstructible>);
         static_assert(    CT::AbandonAssignable<NonSemanticConstructible>);

         static_assert(    CT::SemanticAssignable<Copied, NonSemanticConstructible>);
         static_assert(    CT::SemanticAssignable<Moved, NonSemanticConstructible>);
         static_assert(    CT::SemanticAssignable<Cloned, NonSemanticConstructible>);
         static_assert(    CT::SemanticAssignable<Disowned, NonSemanticConstructible>);
         static_assert(    CT::SemanticAssignable<Abandoned, NonSemanticConstructible>);

         REQUIRE_FALSE(meta->mDestructor);
         REQUIRE_FALSE(meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE      (meta->mCloneConstructor);
         REQUIRE      (meta->mDisownConstructor);
         REQUIRE      (meta->mAbandonConstructor);
         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE      (meta->mCloneAssigner);
         REQUIRE      (meta->mDisownAssigner);
         REQUIRE      (meta->mAbandonAssigner);
      }
   }

   GIVEN("A semantically-constructible type") {
      auto meta = MetaData::Of<SemanticConstructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<SemanticConstructible>);
         static_assert(not CT::Destroyable<SemanticConstructible>);

         static_assert(    CT::CopyMakable<SemanticConstructible>);
         static_assert(    CT::MoveMakable<SemanticConstructible>);
         static_assert(    CT::CloneMakable<SemanticConstructible>);
         static_assert(    CT::DisownMakable<SemanticConstructible>);
         static_assert(    CT::AbandonMakable<SemanticConstructible>);
         static_assert(    CT::DescriptorMakable<SemanticConstructible>);

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

         REQUIRE_FALSE(meta->mDestructor);
         REQUIRE_FALSE(meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE      (meta->mCloneConstructor);
         REQUIRE      (meta->mDisownConstructor);
         REQUIRE      (meta->mAbandonConstructor);
         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE      (meta->mCloneAssigner);
         REQUIRE      (meta->mDisownAssigner);
         REQUIRE      (meta->mAbandonAssigner);
      }
   }

   GIVEN("A descriptor-constructible type") {
      auto meta = MetaData::Of<DescriptorConstructible>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<DescriptorConstructible>);
         static_assert(not CT::Destroyable<DescriptorConstructible>);

         static_assert(    CT::CopyMakable<DescriptorConstructible>);
         static_assert(    CT::MoveMakable<DescriptorConstructible>);
         static_assert(    CT::CloneMakable<DescriptorConstructible>);
         static_assert(    CT::DisownMakable<DescriptorConstructible>);
         static_assert(    CT::AbandonMakable<DescriptorConstructible>);
         static_assert(    CT::DescriptorMakable<DescriptorConstructible>);

         static_assert(    CT::SemanticMakable<Copied, DescriptorConstructible>);
         static_assert(    CT::SemanticMakable<Moved, DescriptorConstructible>);
         static_assert(    CT::SemanticMakable<Cloned, DescriptorConstructible>);
         static_assert(    CT::SemanticMakable<Disowned, DescriptorConstructible>);
         static_assert(    CT::SemanticMakable<Abandoned, DescriptorConstructible>);

         static_assert(    CT::CopyAssignable<DescriptorConstructible>);
         static_assert(    CT::MoveAssignable<DescriptorConstructible>);
         static_assert(    CT::CloneAssignable<DescriptorConstructible>);
         static_assert(    CT::DisownAssignable<DescriptorConstructible>);
         static_assert(    CT::AbandonAssignable<DescriptorConstructible>);

         static_assert(    CT::SemanticAssignable<Copied, DescriptorConstructible>);
         static_assert(    CT::SemanticAssignable<Moved, DescriptorConstructible>);
         static_assert(    CT::SemanticAssignable<Cloned, DescriptorConstructible>);
         static_assert(    CT::SemanticAssignable<Disowned, DescriptorConstructible>);
         static_assert(    CT::SemanticAssignable<Abandoned, DescriptorConstructible>);

         REQUIRE_FALSE(meta->mDestructor);
         REQUIRE_FALSE(meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE      (meta->mCloneConstructor);
         REQUIRE      (meta->mDisownConstructor);
         REQUIRE      (meta->mAbandonConstructor);
         REQUIRE      (meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE      (meta->mCloneAssigner);
         REQUIRE      (meta->mDisownAssigner);
         REQUIRE      (meta->mAbandonAssigner);
      }
   }
   
   GIVEN("A Complex type") {
      auto meta = MetaData::Of<Complex>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<Complex>);
         static_assert(    CT::Destroyable<Complex>);

         static_assert(    CT::CopyMakable<Complex>);
         static_assert(    CT::MoveMakable<Complex>);
         static_assert(not CT::CloneMakable<Complex>);
         static_assert(not CT::DisownMakable<Complex>);
         static_assert(not CT::AbandonMakable<Complex>);
         static_assert(not CT::DescriptorMakable<Complex>);

         static_assert(    CT::SemanticMakable<Copied, Complex>);
         static_assert(    CT::SemanticMakable<Moved, Complex>);
         static_assert(not CT::SemanticMakable<Cloned, Complex>);
         static_assert(not CT::SemanticMakable<Disowned, Complex>);
         static_assert(not CT::SemanticMakable<Abandoned, Complex>);

         static_assert(    CT::CopyAssignable<Complex>);
         static_assert(    CT::MoveAssignable<Complex>);
         static_assert(not CT::CloneAssignable<Complex>);
         static_assert(not CT::DisownAssignable<Complex>);
         static_assert(not CT::AbandonAssignable<Complex>);

         static_assert(    CT::SemanticAssignable<Copied, Complex>);
         static_assert(    CT::SemanticAssignable<Moved, Complex>);
         static_assert(not CT::SemanticAssignable<Cloned, Complex>);
         static_assert(not CT::SemanticAssignable<Disowned, Complex>);
         static_assert(not CT::SemanticAssignable<Abandoned, Complex>);

         REQUIRE      (meta->mDestructor);
         REQUIRE_FALSE(meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE_FALSE(meta->mCloneConstructor);
         REQUIRE_FALSE(meta->mDisownConstructor);
         REQUIRE_FALSE(meta->mAbandonConstructor);
         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE_FALSE(meta->mCloneAssigner);
         REQUIRE_FALSE(meta->mDisownAssigner);
         REQUIRE_FALSE(meta->mAbandonAssigner);
      }
   }

   GIVEN("A ContainsComplex type") {
      auto meta = MetaData::Of<ContainsComplex>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(not CT::Defaultable<ContainsComplex>);
         static_assert(    CT::Destroyable<ContainsComplex>);

         static_assert(    CT::CopyMakable<ContainsComplex>);
         static_assert(    CT::MoveMakable<ContainsComplex>);
         static_assert(not CT::CloneMakable<ContainsComplex>);
         static_assert(not CT::DisownMakable<ContainsComplex>);
         static_assert(not CT::AbandonMakable<ContainsComplex>);
         static_assert(not CT::DescriptorMakable<ContainsComplex>);

         static_assert(    CT::SemanticMakable<Copied, ContainsComplex>);
         static_assert(    CT::SemanticMakable<Moved, ContainsComplex>);
         static_assert(not CT::SemanticMakable<Cloned, ContainsComplex>);
         static_assert(not CT::SemanticMakable<Disowned, ContainsComplex>);
         static_assert(not CT::SemanticMakable<Abandoned, ContainsComplex>);

         static_assert(    CT::CopyAssignable<ContainsComplex>);
         static_assert(    CT::MoveAssignable<ContainsComplex>);
         static_assert(not CT::CloneAssignable<ContainsComplex>);
         static_assert(not CT::DisownAssignable<ContainsComplex>);
         static_assert(not CT::AbandonAssignable<ContainsComplex>);

         static_assert(    CT::SemanticAssignable<Copied, ContainsComplex>);
         static_assert(    CT::SemanticAssignable<Moved, ContainsComplex>);
         static_assert(not CT::SemanticAssignable<Cloned, ContainsComplex>);
         static_assert(not CT::SemanticAssignable<Disowned, ContainsComplex>);
         static_assert(not CT::SemanticAssignable<Abandoned, ContainsComplex>);

         REQUIRE      (meta->mDestructor);
         REQUIRE_FALSE(meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE_FALSE(meta->mCloneConstructor);
         REQUIRE_FALSE(meta->mDisownConstructor);
         REQUIRE_FALSE(meta->mAbandonConstructor);
         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE_FALSE(meta->mCloneAssigner);
         REQUIRE_FALSE(meta->mDisownAssigner);
         REQUIRE_FALSE(meta->mAbandonAssigner);
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
   double, float, 
   char, wchar_t, char8_t, char16_t, char32_t, 
   Langulus::Byte
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
         static_assert(    CT::DisownMakable<TestType>);
         static_assert(    CT::AbandonMakable<TestType>);
         static_assert(not CT::DescriptorMakable<TestType>);

         static_assert(    CT::SemanticMakable<Copied, TestType>);
         static_assert(    CT::SemanticMakable<Moved, TestType>);
         static_assert(    CT::SemanticMakable<Cloned, TestType>);
         static_assert(    CT::SemanticMakable<Disowned, TestType>);
         static_assert(    CT::SemanticMakable<Abandoned, TestType>);

         static_assert(    CT::CopyAssignable<TestType>);
         static_assert(    CT::MoveAssignable<TestType>);
         static_assert(    CT::CloneAssignable<TestType>);
         static_assert(    CT::DisownAssignable<TestType>);
         static_assert(    CT::AbandonAssignable<TestType>);

         static_assert(    CT::SemanticAssignable<Copied, TestType>);
         static_assert(    CT::SemanticAssignable<Moved, TestType>);
         static_assert(    CT::SemanticAssignable<Cloned, TestType>);
         static_assert(    CT::SemanticAssignable<Disowned, TestType>);
         static_assert(    CT::SemanticAssignable<Abandoned, TestType>);

         REQUIRE_FALSE(meta->mDestructor);
         REQUIRE      (meta->mDefaultConstructor);
         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);
         REQUIRE      (meta->mCloneConstructor);
         REQUIRE      (meta->mDisownConstructor);
         REQUIRE      (meta->mAbandonConstructor);
         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);
         REQUIRE      (meta->mCloneAssigner);
         REQUIRE      (meta->mDisownAssigner);
         REQUIRE      (meta->mAbandonAssigner);
      }
   }
}

TEMPLATE_TEST_CASE("Testing semantics of pointers", "[semantics]",
   uint8_t*, uint16_t*, uint32_t*, uint64_t*,
   int8_t*,  int16_t*,  int32_t*,  int64_t*,
   double*, float*, 
   char*, wchar_t*, char8_t*, char16_t*, char32_t*, 
   Langulus::Byte*,
   const uint8_t*, const uint16_t*, const uint32_t*, const uint64_t*,
   const int8_t*,  const int16_t*,  const int32_t*,  const int64_t*,
   const double*, const float*,
   const char*, const wchar_t*, const char8_t*, const char16_t*, const char32_t*,
   const Langulus::Byte*,
   Complex*, ContainsComplex*,
   const Complex*, const ContainsComplex*
) {
   GIVEN("A fundamental type") {
      using T = TestType;
      auto meta = MetaData::Of<T>();
      REQUIRE(meta);

      WHEN("Checked for reflected properties") {
         static_assert(    CT::Inner::Defaultable<T>);
         static_assert(not CT::Inner::Destroyable<T>);

         static_assert(    CT::Inner::CopyMakable<T>);
         static_assert(    CT::Inner::MoveMakable<T>);
         static_assert(    CT::Inner::CloneMakable<T> == CT::POD<T>);
         static_assert(    CT::Inner::DisownMakable<T>);
         static_assert(    CT::Inner::AbandonMakable<T>);
         static_assert(not CT::Inner::DescriptorMakable<T>);

         static_assert(    CT::Inner::SemanticMakable<Copied, T>);
         static_assert(    CT::Inner::SemanticMakable<Moved, T>);
         static_assert(    CT::Inner::SemanticMakable<Cloned, T> == CT::POD<T>);
         static_assert(    CT::Inner::SemanticMakable<Disowned, T>);
         static_assert(    CT::Inner::SemanticMakable<Abandoned, T>);

         static_assert(    CT::Inner::CopyAssignable<T>    == CT::Mutable<T>);
         static_assert(    CT::Inner::MoveAssignable<T>    == CT::Mutable<T>);
         static_assert(    CT::Inner::CloneAssignable<T>   == (CT::Mutable<Deptr<T>> and CT::POD<T>));
         static_assert(    CT::Inner::DisownAssignable<T>  == CT::Mutable<T>);
         static_assert(    CT::Inner::AbandonAssignable<T> == CT::Mutable<T>);
                           
         static_assert(    CT::Inner::SemanticAssignable<Copied, T>    == CT::Mutable<T>);
         static_assert(    CT::Inner::SemanticAssignable<Moved, T>     == CT::Mutable<T>);
         static_assert(    CT::Inner::SemanticAssignable<Cloned, T>    == (CT::Mutable<Deptr<T>> and CT::POD<T>));
         static_assert(    CT::Inner::SemanticAssignable<Disowned, T>  == CT::Mutable<T>);
         static_assert(    CT::Inner::SemanticAssignable<Abandoned, T> == CT::Mutable<T>);

         if constexpr (CT::Destroyable<TestType>)
            REQUIRE      (meta->mDestructor);
         else
            REQUIRE_FALSE(meta->mDestructor);

         if constexpr (CT::Defaultable<TestType>)
            REQUIRE      (meta->mDefaultConstructor);
         else
            REQUIRE_FALSE(meta->mDefaultConstructor);

         REQUIRE      (meta->mCopyConstructor);
         REQUIRE      (meta->mMoveConstructor);

         if constexpr (CT::CloneMakable<TestType>)
            REQUIRE      (meta->mCloneConstructor);
         else
            REQUIRE_FALSE(meta->mCloneConstructor);

         if constexpr (CT::DisownMakable<TestType>)
            REQUIRE      (meta->mDisownConstructor);
         else
            REQUIRE_FALSE(meta->mDisownConstructor);

         if constexpr (CT::AbandonMakable<TestType>)
            REQUIRE      (meta->mAbandonConstructor);
         else
            REQUIRE_FALSE(meta->mAbandonConstructor);

         REQUIRE_FALSE(meta->mDescriptorConstructor);

         REQUIRE      (meta->mCopyAssigner);
         REQUIRE      (meta->mMoveAssigner);

         if constexpr (CT::CloneAssignable<TestType>)
            REQUIRE      (meta->mCloneAssigner);
         else
            REQUIRE_FALSE(meta->mCloneAssigner);

         if constexpr (CT::DisownAssignable<TestType>)
            REQUIRE      (meta->mDisownAssigner);
         else
            REQUIRE_FALSE(meta->mDisownAssigner);

         if constexpr (CT::AbandonAssignable<TestType>)
            REQUIRE      (meta->mAbandonAssigner);
         else
            REQUIRE_FALSE(meta->mAbandonAssigner);
      }
   }
}

TEMPLATE_TEST_CASE("Testing DecayCast (POD)", "[semantics]",
   int, Copied<int>, Disowned<int>, Cloned<int>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DecayCast(i)), const int&>);
   delete value;
}

TEMPLATE_TEST_CASE("Testing DecayCast (non POD)", "[semantics]",
   Complex, Copied<Complex>, Disowned<Complex>, Cloned<Complex>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DecayCast(i)), const Complex&>);
   delete value;
}

TEMPLATE_TEST_CASE("Testing DesemCast (POD)", "[semantics]",
   int, Copied<int>, Disowned<int>, Cloned<int>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DesemCast(i)), const int&>);
   delete value;
}

TEMPLATE_TEST_CASE("Testing DesemCast (non POD)", "[semantics]",
   Complex, Copied<Complex>, Disowned<Complex>, Cloned<Complex>
) {
   const int* value = new int {656};
   const TestType i {*value};
   static_assert(CT::Exact<decltype(DesemCast(i)), const Complex&>);
   delete value;
}