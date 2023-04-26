///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#include "Main.hpp"
#include <catch2/catch.hpp>

namespace N1 {
	struct Type {};
	struct Create {};
}

namespace N2 {
	struct Type {};
}

namespace N3 {
	struct type {};
}

SCENARIO("Testing ambiguous symbols", "[ambiguity]") {
	GIVEN("Three reflected types with similar names in different namespaces") {
		const auto n1t = MetaData::Of<N1::Type>();
		const auto n1c = MetaData::Of<N1::Create>();
		const auto n2t = MetaData::Of<N2::Type>();
		const auto n3t = MetaData::Of<N3::type>();
		const auto vvv = MetaVerb::Of<Verbs::Create>();
		//const auto nnn = MetaData::Of<ImplicitlyReflectedData>();

		WHEN("Meta is retrieved by exact token, that is not case-sensitive") {
			REQUIRE(n1t == RTTI::Database.GetMetaData("N1::Type"));
			REQUIRE(n1c == RTTI::Database.GetMetaData("N1::Create"));
			REQUIRE(n2t == RTTI::Database.GetMetaData("N2::Type"));
			REQUIRE(n3t == RTTI::Database.GetMetaData("N3::Type"));
			REQUIRE(n1t == RTTI::Database.GetMetaData("N1::type"));
			REQUIRE(n2t == RTTI::Database.GetMetaData("N2::type"));
			REQUIRE(n3t == RTTI::Database.GetMetaData("N3::type"));
			REQUIRE(n1t == RTTI::Database.GetMetaData("n1::type"));
			REQUIRE(n2t == RTTI::Database.GetMetaData("n2::type"));
			REQUIRE(n3t == RTTI::Database.GetMetaData("n3::type"));
			REQUIRE(nullptr == RTTI::Database.GetMetaData("Create"));
			REQUIRE(vvv == RTTI::Database.GetMetaVerb("Create"));
			REQUIRE(nullptr == RTTI::Database.GetMetaVerb("N1::Type"));
		}

		WHEN("Meta is retrieved by ambiguous token, that is not case-sensitive") {
			auto found = RTTI::Database.GetAmbiguousMeta("type");
			auto found2 = RTTI::Database.GetAmbiguousMeta("create");
			auto found3 = RTTI::Database.GetAmbiguousMeta("one");
			auto found4 = RTTI::Database.GetAmbiguousMeta("two");
			auto found5 = RTTI::Database.GetAmbiguousMeta("three");
			REQUIRE(found.size() == 6);
			REQUIRE(found.find(n1t) != found.end());
			REQUIRE(found.find(n2t) != found.end());
			REQUIRE(found.find(n3t) != found.end());
			REQUIRE(found2.size() == 3);
			REQUIRE(found2.find(vvv) != found2.end());
			REQUIRE(found2.find(n1c) != found2.end());
			REQUIRE(found3.size() == 9);
			REQUIRE(found4.size() == 9);
			REQUIRE(found5.size() == 9);
		}
	}
}

SCENARIO("Testing operators", "[operators]") {
	GIVEN("Reflected verb with positive and negative operator") {
		//const auto n1t = MetaData::Of<N1::Type>();
		//const auto n1c = MetaData::Of<N1::Create>();
		//const auto n2t = MetaData::Of<N2::Type>();
		//const auto n3t = MetaData::Of<N3::type>();
		const auto vvv = MetaVerb::Of<Verbs::Create>();

		WHEN("Meta is retrieved by operator token, that is not case-sensitive and ignores spaces/tabs/escapes") {
			REQUIRE(vvv == RTTI::Database.GetOperator("+"));
			REQUIRE(vvv == RTTI::Database.GetOperator("-"));
			REQUIRE(vvv == RTTI::Database.GetOperator("      +"));
			REQUIRE(vvv == RTTI::Database.GetOperator("  -"));
			REQUIRE(vvv == RTTI::Database.GetOperator("      +     "));
			REQUIRE(vvv == RTTI::Database.GetOperator("  - "));
			REQUIRE(vvv == RTTI::Database.GetOperator("\t\t + \n   \t\t "));
			REQUIRE(vvv == RTTI::Database.GetOperator("\t\t  - \n\t\t"));
			REQUIRE(nullptr == RTTI::Database.GetOperator("="));
		}
	}
}

SCENARIO("Testing constants reflected from another translation unit", "[constants]") {
	GIVEN("ImplicitlyReflectedData") {
		const auto vvv = MetaData::Of<ImplicitlyReflectedData>();

		WHEN("Meta is retrieved by constant token") {
			REQUIRE(RTTI::Database.GetMetaConstant("ImplicitlyReflectedData::One"));
			REQUIRE(RTTI::Database.GetMetaConstant("ImplicitlyReflectedData::Two"));
			REQUIRE(RTTI::Database.GetMetaConstant("ImplicitlyReflectedData::Three"));
			REQUIRE(RTTI::Database.GetMetaConstant("ImplicitlyReflectedData::One")->mValueType == vvv);
			REQUIRE(RTTI::Database.GetMetaConstant("ImplicitlyReflectedData::Two")->mValueType == vvv);
			REQUIRE(RTTI::Database.GetMetaConstant("ImplicitlyReflectedData::Three")->mValueType == vvv);
		}
	}
}

SCENARIO("Modifying a data type at runtime", "[metadata]") {
	GIVEN("TODO") {
		WHEN("TODO") {
			THEN("TODO") {
			}
		}
	}
}