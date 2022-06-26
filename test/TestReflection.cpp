///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#include "Main.hpp"
#include <catch2/catch.hpp>

namespace One::Two::Three
{
	struct TypeDeepIntoNamespaces;
}

SCENARIO("NameOf", "[nameof]") {
	GIVEN("Type uint16_t") {
		WHEN("Taken the name of") {
			auto name = NameOf<uint16_t>();

			THEN("Name should be correct") {
				REQUIRE(name == "unsigned short"); //TODO make names consistent somehow
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