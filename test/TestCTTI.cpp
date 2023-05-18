///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Main.hpp"
#include <catch2/catch.hpp>

using TypeDeepAlias = One::Two::Three::TypeDeepIntoNamespaces;
using TemplatedAlias = One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>;
using VeryComplexTemplatedAlias = One::Two::Three::VeryComplexTemplate<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>>;

/// A freestanding type compatibility check                                   
/// Purely cosmetic, to avoid typing `template` before member function        
template<CT::Data T, bool ADVANCED = false>
LANGULUS(INLINED)
bool CastsTo(DMeta from) {
   return from->template CastsTo<T, ADVANCED>();
}

/// A freestanding type compatibility check                                   
/// Purely cosmetic, to avoid typing `template` before member function        
template<CT::Data T>
LANGULUS(INLINED)
bool CastsTo(DMeta from, Count count) {
   return from->template CastsTo<T>(count);
}

SCENARIO("NameOf", "[nameof]") {
   GIVEN("Type uint16_t") {
      WHEN("Taken the name of uint16_t") {
         auto name = NameOf<uint16_t>();

         THEN("Name should be correct") {
            REQUIRE(name == "uint16");
         }
      }
      
      WHEN("Taken the name of uint16_t&") {
         auto name = NameOf<uint16_t&>();

         THEN("Name should be correct") {
            REQUIRE(name == "uint16&");
         }
      }

      WHEN("Taken the name of const uint16_t") {
         auto name = NameOf<const uint16_t>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16");
         }
      }

      WHEN("Taken the name of const uint16_t*") {
         auto name = NameOf<const uint16_t*>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16*");
         }
      }

      WHEN("Taken the name of const uint16_t**") {
         auto name = NameOf<const uint16_t**>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16**");
         }
      }
      
      WHEN("Taken the name of const uint16_t* const*") {
         auto name = NameOf<const uint16_t* const *>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16*const *");
         }
      }
      
      WHEN("Taken the name of const uint16_t* const* const") {
         auto name = NameOf<const uint16_t* const * const>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16*const *const");
         }
      }

      WHEN("Taken the name of uint16*") {
         auto name = NameOf<uint16_t*>();

         THEN("Name should be correct") {
            REQUIRE(name == "uint16*");
         }
      }
   }

   GIVEN("The enum Pi") {
      WHEN("Taken the name of") {
         auto name = NameOf<Pi::Number>();

         THEN("Name should be correct") {
            REQUIRE(name == "Pi::Number");
         }
      }
   }

   GIVEN("Type IncompleteType") {
      WHEN("Taken the name of IncompleteType") {
         auto name = NameOf<IncompleteType>();

         THEN("Name should be correct") {
            REQUIRE(name == "IncompleteType");
         }
      }

      WHEN("Taken the name of IncompleteType*") {
         auto name = NameOf<IncompleteType*>();

         THEN("Name should be correct") {
            REQUIRE(name == "IncompleteType*");
         }
      }

      WHEN("Taken the name of const IncompleteType*") {
         auto name = NameOf<const IncompleteType*>();

         THEN("Name should be correct") {
            REQUIRE(name == "const IncompleteType*");
         }
      }

      WHEN("Taken the name of const IncompleteType") {
         auto name = NameOf<const IncompleteType>();

         THEN("Name should be correct") {
            REQUIRE(name == "const IncompleteType");
         }
      }
   }
   
   GIVEN("Type One::Two::Three::TypeDeepIntoNamespaces") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::TypeDeepIntoNamespaces>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces");
         }
      }
   }

   GIVEN("Type TypeDeepAlias") {
      WHEN("Taken the name of TypeDeepAlias") {
         auto name = NameOf<TypeDeepAlias>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces");
         }
      }

      WHEN("Taken the name of TypeDeepAlias*") {
         auto name = NameOf<TypeDeepAlias*>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces*");
         }
      }

      WHEN("Taken the name of const TypeDeepAlias*") {
         auto name = NameOf<const TypeDeepAlias*>();

         THEN("Name should be correct") {
            REQUIRE(name == "const One::Two::Three::TypeDeepIntoNamespaces*");
         }
      }

      WHEN("Taken the name of const TypeDeepAlias*") {
         auto name = NameOf<const TypeDeepAlias>();

         THEN("Name should be correct") {
            REQUIRE(name == "const One::Two::Three::TypeDeepIntoNamespaces");
         }
      }
   }

   GIVEN("Type One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>");
         }
      }
   }

   GIVEN("Enum One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>::VeryDeeplyTemplatedEnum::YesYouGotThatRight") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>::VeryDeeplyTemplatedEnum::YesYouGotThatRight>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>::VeryDeeplyTemplatedEnum::YesYouGotThatRight");
         }
      }
   }

   GIVEN("Enum One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::VeryDeeplyTemplatedEnum::YesYouGotThatRight") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::VeryDeeplyTemplatedEnum::YesYouGotThatRight>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>::VeryDeeplyTemplatedEnum::YesYouGotThatRight");
         }
      }
   }

   GIVEN("Type One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::Nested<uint16_t>") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::Nested<uint16_t>>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>::Nested<uint16>");
         }
      }
   }

   GIVEN("Type TemplatedAlias") {
      WHEN("Taken the name of") {
         auto name = NameOf<TemplatedAlias>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>");
         }
      }
   }

   GIVEN("Type One::Two::Three::VeryComplexTemplate<TemplatedAlias>") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::VeryComplexTemplate<TemplatedAlias>>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::VeryComplexTemplate<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>>");
         }
      }
   }

   GIVEN("Type VeryComplexTemplatedAlias") {
      WHEN("Taken the name of") {
         auto name = NameOf<VeryComplexTemplatedAlias>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::VeryComplexTemplate<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>>");
         }
      }
   }

   GIVEN("Types containing many skippable patterns") {
      WHEN("Taken the name of Langulus::Flow::Construct") {
         auto name = NameOf<Langulus::Flow::Construct>();

         THEN("Name should be correct") {
            REQUIRE(name == "Flow::Construct");
         }
      }

      WHEN("Taken the name of Langulus::Flow::Constructconst") {
         auto name = NameOf<Langulus::Flow::Constructconst>();

         THEN("Name should be correct") {
            REQUIRE(name == "Flow::Constructconst");
         }
      }

      WHEN("Taken the name of Langulus::Flow::constConstructconst") {
         auto name = NameOf<Langulus::Flow::constConstructconst>();

         THEN("Name should be correct") {
            REQUIRE(name == "Flow::constConstructconst");
         }
      }

      WHEN("Taken the name of Langulus::Flow::constconst") {
         auto name = NameOf<Langulus::Flow::constconst>();

         THEN("Name should be correct") {
            REQUIRE(name == "Flow::constconst");
         }
      }
   }
}

TEMPLATE_TEST_CASE("Unsigned integer RTTI interpretation", "[metadata]", uint8_t, uint16_t, uint32_t, uint64_t) {
   GIVEN("An unsigned integer type") {
      auto meta = MetaData::Of<TestType>();
      REQUIRE(meta != nullptr);

      WHEN("Interpreted as another type") {
         THEN("Requirements should be met") {
            REQUIRE(CastsTo<A::Number>(meta));
            REQUIRE(CastsTo<A::Integer>(meta));
            REQUIRE(CastsTo<A::Unsigned>(meta));
            REQUIRE(CastsTo<A::UnsignedInteger>(meta));

            REQUIRE(CastsTo<A::Number>(meta, 1));
            REQUIRE(CastsTo<A::Integer>(meta, 1));
            REQUIRE(CastsTo<A::Unsigned>(meta, 1));
            REQUIRE(CastsTo<A::UnsignedInteger>(meta, 1));

            REQUIRE_FALSE(CastsTo<A::Real>(meta));
            REQUIRE_FALSE(CastsTo<A::Signed>(meta));
            REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta));

            REQUIRE_FALSE(CastsTo<A::Real>(meta, 1));
            REQUIRE_FALSE(CastsTo<A::Signed>(meta, 1));
            REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 1));

            REQUIRE_FALSE(CastsTo<A::Number>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Integer>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Real>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Signed>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 2));
         }
      }
   }
}

TEMPLATE_TEST_CASE("Signed integer RTTI interpretation", "[metadata]", int8_t, int16_t, int32_t, int64_t) {
   GIVEN("A signed integer type") {
      auto meta = MetaData::Of<TestType>();
      REQUIRE(meta != nullptr);

      WHEN("Interpreted as another type") {
         THEN("Requirements should be met") {
            REQUIRE(CastsTo<A::Number>(meta));
            REQUIRE(CastsTo<A::Integer>(meta));
            REQUIRE(CastsTo<A::Signed>(meta));
            REQUIRE(CastsTo<A::SignedInteger>(meta));

            REQUIRE_FALSE(CastsTo<A::Real>(meta));
            REQUIRE_FALSE(CastsTo<A::Unsigned>(meta));
            REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta));

            REQUIRE(CastsTo<A::Number>(meta, 1));
            REQUIRE(CastsTo<A::Integer>(meta, 1));
            REQUIRE(CastsTo<A::Signed>(meta, 1));
            REQUIRE(CastsTo<A::SignedInteger>(meta, 1));

            REQUIRE_FALSE(CastsTo<A::Real>(meta, 1));
            REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 1));
            REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 1));

            REQUIRE_FALSE(CastsTo<A::Number>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Integer>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Real>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Signed>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 2));
         }
      }
   }
}

TEMPLATE_TEST_CASE("Real number RTTI interpretation", "[metadata]", float, double) {
   GIVEN("A real number type") {
      auto meta = MetaData::Of<TestType>();
      REQUIRE(meta != nullptr);

      WHEN("Interpreted as another type") {
         THEN("Requirements should be met") {
            REQUIRE(CastsTo<A::Number>(meta));
            REQUIRE(CastsTo<A::Real>(meta));
            REQUIRE(CastsTo<A::Signed>(meta));

            REQUIRE_FALSE(CastsTo<A::Integer>(meta));
            REQUIRE_FALSE(CastsTo<A::Unsigned>(meta));
            REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta));
            REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta));

            REQUIRE(CastsTo<A::Number>(meta, 1));
            REQUIRE(CastsTo<A::Real>(meta, 1));
            REQUIRE(CastsTo<A::Signed>(meta, 1));

            REQUIRE_FALSE(CastsTo<A::Integer>(meta, 1));
            REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 1));
            REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 1));
            REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 1));

            REQUIRE_FALSE(CastsTo<A::Number>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Integer>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Real>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::Signed>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 2));
            REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 2));
         }
      }
   }
}

SCENARIO("An incomplete type reflected (as long as its a pointer)", "[metadata]") {
   GIVEN("IncompleteType*") {
      WHEN("Reflected") {
         //auto metafail = MetaData::Of<IncompleteType>(); // should cause compile error
         auto meta = MetaData::Of<IncompleteType*>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "IncompleteType*");
            REQUIRE(meta->mVersionMajor == 1);
            REQUIRE(meta->mVersionMinor == 0);
            REQUIRE(meta->mIsDeep == false);
            REQUIRE(meta->mIsPOD == false);
            REQUIRE(meta->mIsNullifiable == false);
            IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Default));
            REQUIRE(meta->mConcrete == nullptr);
            REQUIRE(meta->mIsUninsertable == false);
            REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
            REQUIRE(meta->mIsAbstract == false);
            REQUIRE(meta->mSize == sizeof(void*));
            REQUIRE(meta->mAlignment == alignof(void*));
            REQUIRE(meta->mDeptr == nullptr);
            REQUIRE(meta->mOrigin == nullptr);

            REQUIRE(meta->mBases.size() == 0);
            REQUIRE(meta->mAbilities.size() == 0);
            REQUIRE(meta->mMembers.size() == 0);
         }
      }
   }

   GIVEN("IncompleteType**") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<IncompleteType**>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "IncompleteType**");
            REQUIRE(meta->mVersionMajor == 1);
            REQUIRE(meta->mVersionMinor == 0);
            REQUIRE(meta->mIsDeep == false);
            REQUIRE(meta->mIsPOD == false);
            REQUIRE(meta->mIsNullifiable == false);
            IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Default));
            REQUIRE(meta->mConcrete == nullptr);
            REQUIRE(meta->mIsUninsertable == false);
            REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
            REQUIRE(meta->mIsAbstract == false);
            REQUIRE(meta->mSize == sizeof(void*));
            REQUIRE(meta->mAlignment == alignof(void*));
            REQUIRE(meta->mDeptr == MetaData::Of<Deptr<IncompleteType**>>());
            REQUIRE(meta->mOrigin == nullptr);

            REQUIRE(meta->mBases.size() == 0);
            REQUIRE(meta->mAbilities.size() == 0);
            REQUIRE(meta->mMembers.size() == 0);
         }
      }
   }

   GIVEN("const IncompleteType**") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<const IncompleteType**>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "const IncompleteType**");
            REQUIRE(meta->mVersionMajor == 1);
            REQUIRE(meta->mVersionMinor == 0);
            REQUIRE(meta->mIsDeep == false);
            REQUIRE(meta->mIsPOD == false);
            REQUIRE(meta->mIsNullifiable == false);
            IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Default));
            REQUIRE(meta->mConcrete == nullptr);
            REQUIRE(meta->mIsUninsertable == false);
            REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
            REQUIRE(meta->mIsAbstract == false);
            REQUIRE(meta->mSize == sizeof(void*));
            REQUIRE(meta->mAlignment == alignof(void*));
            REQUIRE(meta->mDeptr == MetaData::Of<Deptr<const IncompleteType**>>());
            REQUIRE(meta->mOrigin == nullptr);
            REQUIRE(meta->mIsConstant == false);
            REQUIRE(meta->mDeptr->mIsConstant == true);

            REQUIRE(meta->mBases.size() == 0);
            REQUIRE(meta->mAbilities.size() == 0);
            REQUIRE(meta->mMembers.size() == 0);
         }
      }
   }

   GIVEN("const IncompleteType* const*&") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<const IncompleteType* const*&>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "const IncompleteType*const *");
            REQUIRE(meta->mVersionMajor == 1);
            REQUIRE(meta->mVersionMinor == 0);
            REQUIRE(meta->mIsDeep == false);
            REQUIRE(meta->mIsPOD == false);
            REQUIRE(meta->mIsNullifiable == false);
            IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Default));
            REQUIRE(meta->mConcrete == nullptr);
            REQUIRE(meta->mIsUninsertable == false);
            REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
            REQUIRE(meta->mIsAbstract == false);
            REQUIRE(meta->mSize == sizeof(void*));
            REQUIRE(meta->mAlignment == alignof(void*));
            REQUIRE(meta->mDeptr == MetaData::Of<Deptr<const IncompleteType* const*>>());
            REQUIRE(meta->mOrigin == nullptr);
            REQUIRE(meta->mIsConstant == true);
            REQUIRE(meta->mDeptr->mIsConstant == true);

            REQUIRE(meta->mBases.size() == 0);
            REQUIRE(meta->mAbilities.size() == 0);
            REQUIRE(meta->mMembers.size() == 0);
         }
      }
   }

   GIVEN("const IncompleteType* const* const&") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<const IncompleteType* const* const&>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "const IncompleteType*const *");
            REQUIRE(meta->mVersionMajor == 1);
            REQUIRE(meta->mVersionMinor == 0);
            REQUIRE(meta->mIsDeep == false);
            REQUIRE(meta->mIsPOD == false);
            REQUIRE(meta->mIsNullifiable == false);
            IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Default));
            REQUIRE(meta->mConcrete == nullptr);
            REQUIRE(meta->mIsUninsertable == false);
            REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
            REQUIRE(meta->mIsAbstract == false);
            REQUIRE(meta->mSize == sizeof(void*));
            REQUIRE(meta->mAlignment == alignof(void*));
            REQUIRE(meta->mDeptr == MetaData::Of<Deptr<const IncompleteType* const*>>());
            REQUIRE(meta->mOrigin == nullptr);
            REQUIRE(meta->mIsConstant == true);
            REQUIRE(meta->mDeptr->mIsConstant == true);

            REQUIRE(meta->mBases.size() == 0);
            REQUIRE(meta->mAbilities.size() == 0);
            REQUIRE(meta->mMembers.size() == 0);
         }
      }
   }

   GIVEN("const IncompleteType* const* const&") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<const IncompleteType* const* const&&>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "const IncompleteType*const *");
            REQUIRE(meta->mVersionMajor == 1);
            REQUIRE(meta->mVersionMinor == 0);
            REQUIRE(meta->mIsDeep == false);
            REQUIRE(meta->mIsPOD == false);
            REQUIRE(meta->mIsNullifiable == false);
            IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Default));
            REQUIRE(meta->mConcrete == nullptr);
            REQUIRE(meta->mIsUninsertable == false);
            REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
            REQUIRE(meta->mIsAbstract == false);
            REQUIRE(meta->mSize == sizeof(void*));
            REQUIRE(meta->mAlignment == alignof(void*));
            REQUIRE(meta->mDeptr == MetaData::Of<Deptr<const IncompleteType* const*>>());
            REQUIRE(meta->mOrigin == nullptr);
            REQUIRE(meta->mIsConstant == true);
            REQUIRE(meta->mDeptr->mIsConstant == true);

            REQUIRE(meta->mBases.size() == 0);
            REQUIRE(meta->mAbilities.size() == 0);
            REQUIRE(meta->mMembers.size() == 0);
         }
      }
   }
}

SCENARIO("A complex type reflected with CTTI traits", "[metadata]") {
   GIVEN("ImplicitlyReflectedDataWithTraits") {
      WHEN("Reflected") {
         ImplicitlyReflectedDataWithTraits instance;
         ImplicitlyReflectedData* ptrtobase = &static_cast<ImplicitlyReflectedData&>(instance);
         const auto baseoffset = reinterpret_cast<char*>(ptrtobase) - reinterpret_cast<char*>(&instance);

         auto meta = MetaData::Of<ImplicitlyReflectedDataWithTraits>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "MyType");
            REQUIRE(meta->mInfo == "Info about MyType");
            REQUIRE(meta->mFileExtensions == "txt, pdf");
            REQUIRE(meta->mVersionMajor == 2);
            REQUIRE(meta->mVersionMinor == 1);
            REQUIRE(meta->mIsDeep == true);
            REQUIRE(meta->mIsPOD == true);
            REQUIRE(meta->mIsNullifiable == true);
            IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Size));
            REQUIRE(meta->mConcrete->Is<ImplicitlyReflectedData>());
            REQUIRE(meta->mIsUninsertable == true);
            REQUIRE(meta->mAllocationPage == Roof2(250 * sizeof(ImplicitlyReflectedDataWithTraits)));
            REQUIRE(meta->mIsAbstract == true);
            REQUIRE(meta->mSize == sizeof(ImplicitlyReflectedDataWithTraits));
            REQUIRE(meta->mAlignment == alignof(ImplicitlyReflectedDataWithTraits));

            REQUIRE(meta->mBases.size() == 1);
            REQUIRE(meta->mBases[0].mType->Is<ImplicitlyReflectedData>());
            REQUIRE(meta->mBases[0].mImposed == false);
            REQUIRE(meta->mBases[0].mBinaryCompatible == false);
            REQUIRE(meta->mBases[0].mCount == 1);
            REQUIRE(baseoffset >= 0);
            REQUIRE(meta->mBases[0].mOffset == static_cast<Offset>(baseoffset));

            REQUIRE(meta->mAbilities.size() == 1);
            REQUIRE(meta->mAbilities.begin()->first->Is<Verbs::Create>());
            REQUIRE(meta->mAbilities.begin()->second.mVerb->Is<Verbs::Create>());
            REQUIRE(meta->mAbilities.begin()->second.mOverloadsConstant.size() == 1);
            REQUIRE(meta->mAbilities.begin()->second.mOverloadsConstant.contains(Ability::Signature {}));
            REQUIRE(meta->mAbilities.begin()->second.mOverloadsMutable.size() == 1);
            REQUIRE(meta->mAbilities.begin()->second.mOverloadsMutable.contains(Ability::Signature {}));

            REQUIRE(meta->mMembers.size() == 4);
            REQUIRE(meta->mMembers[0].mCount == 1);
            REQUIRE(meta->mMembers[0].mName == "member");
            REQUIRE(meta->mMembers[0].mOffset >= sizeof(ImplicitlyReflectedData));
            REQUIRE(meta->mMembers[0].GetTrait() == nullptr);
            REQUIRE(meta->mMembers[0].Is<int>());

            REQUIRE(meta->mMembers[1].mCount == 1);
            REQUIRE(meta->mMembers[1].mName == "anotherMember");
            REQUIRE(meta->mMembers[1].mOffset > meta->mMembers[0].mOffset);
            REQUIRE(meta->mMembers[1].TraitIs<Traits::Tag>());
            REQUIRE(meta->mMembers[1].Is<bool>());

            REQUIRE(meta->mMembers[2].mCount == 12);
            REQUIRE(meta->mMembers[2].mName == "anotherMemberArray");
            REQUIRE(meta->mMembers[2].mOffset > meta->mMembers[1].mOffset);
            REQUIRE(meta->mMembers[2].GetTrait() == nullptr);
            REQUIRE(meta->mMembers[2].Is<int>());

            REQUIRE(meta->mMembers[3].mCount == 1);
            REQUIRE(meta->mMembers[3].mName == "sparseMember");
            REQUIRE(meta->mMembers[3].mOffset > meta->mMembers[2].mOffset);
            REQUIRE(meta->mMembers[3].GetTrait() == nullptr);
            REQUIRE(meta->mMembers[3].Is<int>());
         }
      }
   }
}

SCENARIO("A simple type reflected with CTTI traits", "[metadata]") {
   GIVEN("ImplicitlyReflectedData") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<ImplicitlyReflectedData>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "ImplicitlyReflectedData");
            REQUIRE(meta->mCppName == "ImplicitlyReflectedData");
            REQUIRE(meta->mInfo == "<no info provided>");
            REQUIRE(meta->mFileExtensions == "");
            REQUIRE(meta->mVersionMajor == 1);
            REQUIRE(meta->mVersionMinor == 0);
            REQUIRE(meta->mIsDeep == false);
            REQUIRE(meta->mIsPOD == true);
            REQUIRE(meta->mIsNullifiable == false);
            IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Default));
            REQUIRE(meta->mConcrete == nullptr);
            REQUIRE(meta->mIsUninsertable == false);
            REQUIRE(meta->mAllocationPage >= Alignment);
            REQUIRE(meta->mIsAbstract == false);
            REQUIRE(meta->mSize == sizeof(ImplicitlyReflectedData));
            REQUIRE(meta->mAlignment == alignof(ImplicitlyReflectedData));
            REQUIRE(meta->mNamedValues.size() == 3);
         }
      }
   }
}

SCENARIO("A reflected verb with CTTI traits", "[metaverb]") {
   GIVEN("Create verb with positive/negative tokens, with stateless and contextual default functions") {
      WHEN("Reflected") {
         Anyness::Block someBlock;
         Flow::Verb someVerb;
         auto meta = MetaVerb::Of<Verbs::Create>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "Create");
            REQUIRE(meta->mTokenReverse == "Destroy");
            REQUIRE(meta->mInfo.starts_with("Used for allocating new elements. "));
            REQUIRE(meta->mVersionMajor == 1);
            REQUIRE(meta->mVersionMinor == 0);
            REQUIRE(meta->mOperator == " + ");
            REQUIRE(meta->mPrecedence == 5);
            REQUIRE(meta->mOperatorReverse == " - ");
            REQUIRE(meta->mDefaultInvocationMutable);
            REQUIRE(meta->mDefaultInvocationMutable(someBlock, someVerb) == false);
            REQUIRE(meta->mDefaultInvocationConstant);
            REQUIRE(meta->mDefaultInvocationConstant(someBlock, someVerb) == true);
            REQUIRE(meta->mStatelessInvocation);
            REQUIRE(meta->mStatelessInvocation(someVerb) == false);
         }
      }
   }
}

struct TypeWithSuffix { LANGULUS(SUFFIX) "yeah"; };
struct TypeWithoutSuffix {};

SCENARIO("TypeSuffix", "[metadata]") {
   WHEN("Generating a suffix for uint8_t") {
      constexpr auto token = SuffixOf<uint8_t>();
      THEN("Requirements should be met") {
         REQUIRE(token == "u8");
      }
   }

   WHEN("Generating a suffix for uint16_t") {
      constexpr auto token = SuffixOf<uint16_t>();
      THEN("Requirements should be met") {
         REQUIRE(token == "u16");
      }
   }

   WHEN("Generating a suffix for uint32_t") {
      constexpr auto token = SuffixOf<uint32_t>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<uint32_t, unsigned int>)
            REQUIRE(token == "u");
         else
            REQUIRE(token == "u32");
      }
   }
   
   WHEN("Generating a suffix for uint64_t") {
      constexpr auto token = SuffixOf<uint64_t>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<uint64_t, unsigned int>)
            REQUIRE(token == "u");
         else
            REQUIRE(token == "u64");
      }
   }

   WHEN("Generating a suffix for int8_t") {
      constexpr auto token = SuffixOf<int8_t>();
      THEN("Requirements should be met") {
         REQUIRE(token == "i8");
      }
   }

   WHEN("Generating a suffix for int16_t") {
      constexpr auto token = SuffixOf<int16_t>();
      THEN("Requirements should be met") {
         REQUIRE(token == "i16");
      }
   }

   WHEN("Generating a suffix for int32_t") {
      constexpr auto token = SuffixOf<int32_t>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<int32_t, signed int>)
            REQUIRE(token == "i");
         else
            REQUIRE(token == "i32");
      }
   }
   
   WHEN("Generating a suffix for int64_t") {
      constexpr auto token = SuffixOf<int64_t>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<int64_t, signed int>)
            REQUIRE(token == "i");
         else
            REQUIRE(token == "i64");
      }
   }

   WHEN("Generating a suffix for float") {
      constexpr auto token = SuffixOf<float>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<float, Real>)
            REQUIRE(token == "");
         else
            REQUIRE(token == "f");
      }
   }

   WHEN("Generating a suffix for double") {
      constexpr auto token = SuffixOf<double>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<double, Real>)
            REQUIRE(token == "");
         else
            REQUIRE(token == "d");
      }
   }

   WHEN("Generating a suffix for bool") {
      constexpr auto token = SuffixOf<bool>();
      THEN("Requirements should be met") {
         REQUIRE(token == "b");
      }
   }

   WHEN("Generating a suffix for a type with LANGULUS(SUFFIX)") {
      constexpr auto token = SuffixOf<TypeWithSuffix>();
      THEN("Requirements should be met") {
         REQUIRE(token == "yeah");
      }
   }

   WHEN("Generating a suffix for a type without LANGULUS(SUFFIX)") {
      constexpr auto token = SuffixOf<TypeWithoutSuffix>();
      THEN("Requirements should be met") {
         REQUIRE(token == "");
      }
   }
}