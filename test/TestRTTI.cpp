///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Common.hpp"


SCENARIO("Comparing different meta types with one another", "[meta]") {
   GIVEN("Reflected types") {
      const auto dmeta = MetaDataOf<Type>();
      REQUIRE(dmeta);

      const AMeta a_dmeta = dmeta;
      REQUIRE(a_dmeta);
      REQUIRE(dmeta == a_dmeta);

      const auto vmeta = MetaVerbOf<Verbs::Create>();
      REQUIRE(vmeta);

      const AMeta a_vmeta = vmeta;
      REQUIRE(a_vmeta);
      REQUIRE(vmeta == a_vmeta);

      const auto tmeta = MetaTraitOf<Traits::Tag>();
      REQUIRE(tmeta);

      const AMeta a_tmeta = tmeta;
      REQUIRE(a_tmeta);
      REQUIRE(tmeta == a_tmeta);

      static_assert(not CT::Comparable<DMeta, TMeta>);
      static_assert(not CT::Comparable<DMeta, CMeta>);
      static_assert(not CT::Comparable<DMeta, VMeta>);
      static_assert(not CT::Comparable<TMeta, VMeta>);
      static_assert(not CT::Comparable<TMeta, CMeta>);
      static_assert(    CT::Comparable<DMeta, AMeta>);
      static_assert(    CT::Comparable<TMeta, AMeta>);
      static_assert(    CT::Comparable<CMeta, AMeta>);
      static_assert(    CT::Comparable<VMeta, AMeta>);

      REQUIRE(a_dmeta != a_vmeta);
      REQUIRE(a_dmeta != a_tmeta);
      REQUIRE(a_vmeta != a_tmeta);
   }
}

SCENARIO("Testing ambiguous symbols", "[ambiguity]") {
   GIVEN("Reflected types with similar names in different namespaces") {
      const auto n0t = MetaDataOf<Type>();
      const auto n1t = MetaDataOf<N1::Type>();
      const auto n1c = MetaDataOf<N1::Create>();
      const auto n2t = MetaDataOf<N2::Type>();
      const auto n3t = MetaDataOf<N3::type>();
      const auto vvv = MetaVerbOf<Verbs::Create>();
      const auto complexMeta = MetaDataOf<ImplicitlyReflectedDataWithTraits>();
      (void) MetaDataOf<AnotherTypeWithSimilarilyNamedValues>();

      WHEN("Meta is retrieved by exact token, that is not case-sensitive") {
         REQUIRE(n0t == RTTI::GetMetaData("Type"));
         REQUIRE(n1t == RTTI::GetMetaData("N1::Type"));
         REQUIRE(n1c == RTTI::GetMetaData("N1::Create"));
         REQUIRE(n2t == RTTI::GetMetaData("N2::Type"));
         REQUIRE(n3t == RTTI::GetMetaData("N3::Type"));

         REQUIRE(n0t == RTTI::GetMetaData("type"));
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
         auto found0 = RTTI::GetAmbiguousMeta("type");
         auto found1 = RTTI::GetAmbiguousMeta("::type");
         auto found2 = RTTI::GetAmbiguousMeta("create");
         auto found3 = RTTI::GetAmbiguousMeta("one");
         auto found4 = RTTI::GetAmbiguousMeta("two");
         auto found5 = RTTI::GetAmbiguousMeta("three");
         auto found6 = RTTI::GetAmbiguousMeta("MyType");
         REQUIRE(found0.size() == 10);
         REQUIRE(found0.find(n0t) != found0.end());
         REQUIRE(found0.find(n1t) != found0.end());
         REQUIRE(found0.find(n2t) != found0.end());
         REQUIRE(found0.find(n3t) != found0.end());
         REQUIRE(found1.size() == 10);
         REQUIRE(found1.find(n0t) != found1.end());
         REQUIRE(found1.find(n1t) != found1.end());
         REQUIRE(found1.find(n2t) != found1.end());
         REQUIRE(found1.find(n3t) != found1.end());
         REQUIRE(found2.size() == 4);
         REQUIRE(found2.find(vvv) != found2.end());
         REQUIRE(found2.find(n1c) != found2.end());
         REQUIRE(found3.size() == 2);
         REQUIRE(found4.size() == 2);
         REQUIRE(found5.size() == 2);
         REQUIRE(found6.size() == 1);
      }
      
      WHEN("Attempting to disambiguate token") {
         REQUIRE(RTTI::DisambiguateMeta("type"));
         REQUIRE_THROWS(RTTI::DisambiguateMeta("::type"));
         REQUIRE(RTTI::DisambiguateMeta("create"));
         REQUIRE_THROWS(RTTI::DisambiguateMeta("one"));
         REQUIRE(RTTI::DisambiguateMeta("YetAnotherNamedType::one"));
         REQUIRE(RTTI::DisambiguateMeta("ImplicitlyReflectedData::one"));
         REQUIRE_THROWS(RTTI::DisambiguateMeta("MyType::one"));
         REQUIRE_THROWS(RTTI::DisambiguateMeta("two"));
         REQUIRE_THROWS(RTTI::DisambiguateMeta("three"));
         REQUIRE(RTTI::DisambiguateMeta("MyType"));
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

SCENARIO("Modifying a data type at runtime", "[meta]") {
   GIVEN("TODO") {
      WHEN("TODO") {

      }
   }
}