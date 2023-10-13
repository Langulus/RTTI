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


SCENARIO("Testing ambiguous symbols", "[ambiguity]") {
   GIVEN("Three reflected types with similar names in different namespaces") {
      const auto n1t = MetaData::Of<N1::Type>();
      const auto n1c = MetaData::Of<N1::Create>();
      const auto n2t = MetaData::Of<N2::Type>();
      const auto n3t = MetaData::Of<N3::type>();
      const auto vvv = MetaVerb::Of<Verbs::Create>();
      const auto complexMeta = MetaData::Of<ImplicitlyReflectedDataWithTraits>();

      //const auto nnn = MetaData::Of<ImplicitlyReflectedData>();

      WHEN("Meta is retrieved by exact token, that is not case-sensitive") {
         REQUIRE(n1t == RTTI::GetMetaData("N1::Type"));
         REQUIRE(n1c == RTTI::GetMetaData("N1::Create"));
         REQUIRE(n2t == RTTI::GetMetaData("N2::Type"));
         REQUIRE(n3t == RTTI::GetMetaData("N3::Type"));
         REQUIRE(n1t == RTTI::GetMetaData("N1::type"));
         REQUIRE(n2t == RTTI::GetMetaData("N2::type"));
         REQUIRE(n3t == RTTI::GetMetaData("N3::type"));
         REQUIRE(n1t == RTTI::GetMetaData("n1::type"));
         REQUIRE(n2t == RTTI::GetMetaData("n2::type"));
         REQUIRE(n3t == RTTI::GetMetaData("n3::type"));
         REQUIRE(complexMeta == RTTI::GetMetaData("MyType"));
         REQUIRE(nullptr == RTTI::GetMetaData("Create"));
         REQUIRE(vvv == RTTI::GetMetaVerb("Create"));
         REQUIRE(nullptr == RTTI::GetMetaVerb("N1::Type"));
      }

      WHEN("Meta is retrieved by ambiguous token, that is not case-sensitive") {
         auto found = RTTI::GetAmbiguousMeta("type");
         auto found2 = RTTI::GetAmbiguousMeta("create");
         auto found3 = RTTI::GetAmbiguousMeta("one");
         auto found4 = RTTI::GetAmbiguousMeta("two");
         auto found5 = RTTI::GetAmbiguousMeta("three");
         auto found6 = RTTI::GetAmbiguousMeta("MyType");
         REQUIRE(found.size() == 9);
         REQUIRE(found.find(n1t) != found.end());
         REQUIRE(found.find(n2t) != found.end());
         REQUIRE(found.find(n3t) != found.end());
         REQUIRE(found2.size() == 4);
         REQUIRE(found2.find(vvv) != found2.end());
         REQUIRE(found2.find(n1c) != found2.end());
         REQUIRE(found3.size() == 2);
         REQUIRE(found4.size() == 2);
         REQUIRE(found5.size() == 2);
         REQUIRE(found6.size() == 1);
      }

      WHEN("Meta is retrieved by reflected file extension") {
         auto foundtxt = RTTI::ResolveFileExtension("txt");
         REQUIRE(foundtxt.size() == 1);
         REQUIRE(*foundtxt.begin() == MetaData::Of<ImplicitlyReflectedDataWithTraits>());

         auto foundpdf = RTTI::ResolveFileExtension("PDF");
         REQUIRE(foundpdf.size() == 1);
         REQUIRE(*foundpdf.begin() == MetaData::Of<ImplicitlyReflectedDataWithTraits>());

         auto foundase = RTTI::ResolveFileExtension("ase");
         REQUIRE(foundase.size() == 1);
         REQUIRE(*foundase.begin() == MetaData::Of<ImplicitlyReflectedData>());

         auto foundasenotfound = RTTI::ResolveFileExtension("asenotfound");
         REQUIRE(foundasenotfound.size() == 0);
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
         REQUIRE(vvv == RTTI::GetOperator("+"));
         REQUIRE(vvv == RTTI::GetOperator("-"));
         REQUIRE(vvv == RTTI::GetOperator("      +"));
         REQUIRE(vvv == RTTI::GetOperator("  -"));
         REQUIRE(vvv == RTTI::GetOperator("      +     "));
         REQUIRE(vvv == RTTI::GetOperator("  - "));
         REQUIRE(vvv == RTTI::GetOperator("\t\t + \n   \t\t "));
         REQUIRE(vvv == RTTI::GetOperator("\t\t  - \n\t\t"));
         REQUIRE(nullptr == RTTI::GetOperator("="));
      }
   }
}

SCENARIO("Testing constants reflected from another translation unit", "[constants]") {
   GIVEN("ImplicitlyReflectedData") {
      const auto vvv = MetaData::Of<ImplicitlyReflectedData>();

      WHEN("Meta is retrieved by constant token") {
         REQUIRE(RTTI::GetMetaConstant("ImplicitlyReflectedData::One"));
         REQUIRE(RTTI::GetMetaConstant("ImplicitlyReflectedData::Two"));
         REQUIRE(RTTI::GetMetaConstant("ImplicitlyReflectedData::Three"));
         REQUIRE(RTTI::GetMetaConstant("ImplicitlyReflectedData::One")->mValueType == vvv);
         REQUIRE(RTTI::GetMetaConstant("ImplicitlyReflectedData::Two")->mValueType == vvv);
         REQUIRE(RTTI::GetMetaConstant("ImplicitlyReflectedData::Three")->mValueType == vvv);
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