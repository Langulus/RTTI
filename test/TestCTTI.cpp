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

SCENARIO("NameOf", "[nameof]") {
	GIVEN("Type uint16_t") {
		WHEN("Taken the name of") {
			auto name = NameOf<uint16_t>();

			THEN("Name should be correct") {
				REQUIRE(name == "uint16");
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

	GIVEN("Type One::Two::Three::TypeDeepIntoNamespaces") {
		WHEN("Taken the name of") {
			auto name = NameOf<One::Two::Three::TypeDeepIntoNamespaces>();

			THEN("Name should be correct") {
				REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces");
			}
		}
	}

	GIVEN("Type TypeDeepAlias") {
		WHEN("Taken the name of") {
			auto name = NameOf<TypeDeepAlias>();

			THEN("Name should be correct") {
				REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces");
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

	
	/*The following cases are, unfortunately, known not to work, and warnings are emitted
	in such cases. These are the limitations of NameOf, sorry. If someone knows how to
	improve these use cases, please inform me!*/

	/*GIVEN("Enum One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::VeryDeeplyTemplatedEnum::YesYouGotThatRight") {
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
	}*/

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
}

TEMPLATE_TEST_CASE("Unsigned integer RTTI interpretation", "[metadata]", /*uint8_t,*/ uint16_t, uint32_t, uint64_t) {
	GIVEN("An unsigned integer type") {
		auto meta = MetaData::Of<TestType>();
		REQUIRE(meta != nullptr);

		WHEN("Interpreted as another type") {
			THEN("Requirements should be met") {
				REQUIRE(CastsTo<A::Number>(meta));
				REQUIRE(CastsTo<A::Integer>(meta));
				REQUIRE(CastsTo<A::Unsigned>(meta));
				REQUIRE(CastsTo<A::UnsignedInteger>(meta));

				REQUIRE_FALSE(CastsTo<A::Real>(meta));
				REQUIRE_FALSE(CastsTo<A::Signed>(meta));
				REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta));

				REQUIRE(CastsTo<A::Number>(meta, 1));
				REQUIRE(CastsTo<A::Integer>(meta, 1));
				REQUIRE(CastsTo<A::Unsigned>(meta, 1));
				REQUIRE(CastsTo<A::UnsignedInteger>(meta, 1));

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
				REQUIRE(meta->mPoolTactic == PoolTactic::Size);
				REQUIRE(meta->mConcrete->Is<ImplicitlyReflectedData>());
				REQUIRE(meta->mIsUninsertable == true);
				REQUIRE(meta->mAllocationPage == Roof2(250 * sizeof(ImplicitlyReflectedDataWithTraits)));
				REQUIRE(meta->mIsAbstract == true);
				REQUIRE(meta->mSize == 0);
				REQUIRE(meta->mAlignment == alignof(ImplicitlyReflectedDataWithTraits));

				REQUIRE(meta->mBases.size() == 1);
				REQUIRE(meta->mBases[0].mType->Is<ImplicitlyReflectedData>());
				REQUIRE(meta->mBases[0].mImposed == false);
				REQUIRE(meta->mBases[0].mBinaryCompatible == false);
				REQUIRE(meta->mBases[0].mCount == 1);
				REQUIRE(meta->mBases[0].mOffset == baseoffset);

				REQUIRE(meta->mAbilities.size() == 1);
				REQUIRE(meta->mAbilities.begin()->first->Is<Verbs::Create>());
				REQUIRE(meta->mAbilities.begin()->second.mVerb->Is<Verbs::Create>());
				REQUIRE(meta->mAbilities.begin()->second.mOverloadsConstant.size() == 1);
				REQUIRE(meta->mAbilities.begin()->second.mOverloadsConstant.contains(Ability::Signature {}));
				REQUIRE(meta->mAbilities.begin()->second.mOverloadsMutable.size() == 1);
				REQUIRE(meta->mAbilities.begin()->second.mOverloadsMutable.contains(Ability::Signature {}));
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
				REQUIRE(meta->mPoolTactic == PoolTactic::Default);
				REQUIRE(meta->mConcrete == nullptr);
				REQUIRE(meta->mIsUninsertable == false);
				REQUIRE(meta->mAllocationPage >= Alignment);
				REQUIRE(meta->mIsAbstract == false);
				REQUIRE(meta->mSize == sizeof(ImplicitlyReflectedData));
				REQUIRE(meta->mAlignment == alignof(ImplicitlyReflectedData));
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
				REQUIRE(meta->mOperator == "");
				REQUIRE(meta->mOperatorReverse == "");
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