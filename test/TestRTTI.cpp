///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/Testing/CommonTypes.hpp>
#include <Langulus/MetaOf.hpp>


SCENARIO("Comparing different meta types with one another") {
   GIVEN("Reflected types") {
      const auto dmeta = MetaDataOf<Complex>();
      REQUIRE(dmeta);
      const auto not_abstract_dmeta = dmeta.GetDefinition();
      const RTTI::Inner::Definition* a_dmeta = not_abstract_dmeta;
      REQUIRE(not_abstract_dmeta);
      REQUIRE(dmeta == not_abstract_dmeta);

      const auto vmeta = MetaVerbOf<Verbs::Create>();
      REQUIRE(vmeta);
      const auto not_abstract_vmeta = vmeta.GetDefinition();
      const RTTI::Inner::Definition* a_vmeta = not_abstract_vmeta;
      REQUIRE(not_abstract_vmeta);
      REQUIRE(vmeta == not_abstract_vmeta);

      const auto tmeta = MetaTagOf<Tags::Name>();
      REQUIRE(tmeta);
      const auto not_abstract_tmeta = tmeta.GetDefinition();
      const RTTI::Inner::Definition* a_tmeta = not_abstract_tmeta;
      REQUIRE(not_abstract_tmeta);
      REQUIRE(tmeta == not_abstract_tmeta);

      const auto cmeta = MetaConstOf<::Number>();
      REQUIRE(cmeta);
      const auto not_abstract_cmeta = cmeta.GetDefinition();
      const RTTI::Inner::Definition* a_cmeta = not_abstract_cmeta;
      REQUIRE(not_abstract_cmeta);
      REQUIRE(cmeta == not_abstract_cmeta);

      static_assert(not Exact<decltype(not_abstract_dmeta), decltype(not_abstract_vmeta), decltype(not_abstract_tmeta), decltype(not_abstract_cmeta)>);

      static_assert(not CT::ComparableEqual<DMeta, TMeta>);
      static_assert(not CT::ComparableEqual<DMeta, CMeta>);
      static_assert(not CT::ComparableEqual<DMeta, VMeta>);
      static_assert(not CT::ComparableEqual<TMeta, VMeta>);
      static_assert(not CT::ComparableEqual<TMeta, CMeta>);

      static_assert(    CT::ComparableEqual<DMeta, decltype(a_dmeta)>);
      static_assert(    CT::ComparableEqual<DMeta, decltype(a_vmeta)>);
      static_assert(    CT::ComparableEqual<DMeta, decltype(a_tmeta)>);
      static_assert(    CT::ComparableEqual<DMeta, decltype(a_cmeta)>);

      static_assert(    CT::ComparableEqual<TMeta, decltype(a_dmeta)>);
      static_assert(    CT::ComparableEqual<TMeta, decltype(a_vmeta)>);
      static_assert(    CT::ComparableEqual<TMeta, decltype(a_tmeta)>);
      static_assert(    CT::ComparableEqual<TMeta, decltype(a_cmeta)>);

      static_assert(    CT::ComparableEqual<CMeta, decltype(a_dmeta)>);
      static_assert(    CT::ComparableEqual<CMeta, decltype(a_vmeta)>);
      static_assert(    CT::ComparableEqual<CMeta, decltype(a_tmeta)>);
      static_assert(    CT::ComparableEqual<CMeta, decltype(a_cmeta)>);

      static_assert(    CT::ComparableEqual<VMeta, decltype(a_dmeta)>);
      static_assert(    CT::ComparableEqual<VMeta, decltype(a_vmeta)>);
      static_assert(    CT::ComparableEqual<VMeta, decltype(a_tmeta)>);
      static_assert(    CT::ComparableEqual<VMeta, decltype(a_cmeta)>);

      REQUIRE(a_dmeta != a_vmeta);
      REQUIRE(a_dmeta != a_tmeta);
      REQUIRE(a_vmeta != a_tmeta);
   }
}

SCENARIO("Testing ambiguous symbols") {
   GIVEN("Reflected types with similar names in different namespaces") {
      const auto n0t = MetaDataOf<EmptyType>();
      const auto n1t = MetaDataOf<N1::EmptyType>();
      const auto n1c = MetaDataOf<N1::Create>();
      const auto n2t = MetaDataOf<N2::EmptyType>();
      const auto n3t = MetaDataOf<N3::emptytype>();
      const auto vvv = MetaVerbOf<Verbs::Create>();
      const auto complexMeta = MetaDataOf<ImplicitlyReflectedDataWithTraits>();
      (void) MetaDataOf<AnotherTypeWithSimilarilyNamedValues>();

      WHEN("Meta is retrieved by exact token, that is not case-sensitive") {
         REQUIRE(n0t == RTTI::Registry::GetMetaDataByToken("Type"));
         REQUIRE(n1t == RTTI::Registry::GetMetaDataByToken("N1::Type"));
         REQUIRE(n1c == RTTI::Registry::GetMetaDataByToken("N1::Create"));
         REQUIRE(n2t == RTTI::Registry::GetMetaDataByToken("N2::Type"));
         REQUIRE(n3t == RTTI::Registry::GetMetaDataByToken("N3::Type"));

         REQUIRE(n0t == RTTI::Registry::GetMetaDataByToken("type"));
         REQUIRE(n1t == RTTI::Registry::GetMetaDataByToken("N1::type"));
         REQUIRE(n2t == RTTI::Registry::GetMetaDataByToken("N2::type"));
         REQUIRE(n3t == RTTI::Registry::GetMetaDataByToken("N3::type"));
         REQUIRE(n1t == RTTI::Registry::GetMetaDataByToken("n1::type"));
         REQUIRE(n2t == RTTI::Registry::GetMetaDataByToken("n2::type"));
         REQUIRE(n3t == RTTI::Registry::GetMetaDataByToken("n3::type"));

         REQUIRE(complexMeta == RTTI::Registry::GetMetaDataByToken("MyType"));
         REQUIRE(nullptr == RTTI::Registry::GetMetaDataByToken("Create"));
         REQUIRE(vvv == RTTI::Registry::GetMetaVerbByToken("Create"));
         REQUIRE(nullptr == RTTI::Registry::GetMetaVerbByToken("N1::Type"));
      }

      WHEN("Meta is retrieved by ambiguous token, that is not case-sensitive") {
         auto found0 = RTTI::Registry::GetAmbiguousMeta("type");
         auto found1 = RTTI::Registry::GetAmbiguousMeta("::type");
         auto found2 = RTTI::Registry::GetAmbiguousMeta("create");
         auto found3 = RTTI::Registry::GetAmbiguousMeta("one");
         auto found4 = RTTI::Registry::GetAmbiguousMeta("two");
         auto found5 = RTTI::Registry::GetAmbiguousMeta("three");
         auto found6 = RTTI::Registry::GetAmbiguousMeta("MyType");

         REQUIRE(found0.size() == 10);
         REQUIRE(found0.find(n0t.GetDefinition()) != found0.end());
         REQUIRE(found0.find(n1t.GetDefinition()) != found0.end());
         REQUIRE(found0.find(n2t.GetDefinition()) != found0.end());
         REQUIRE(found0.find(n3t.GetDefinition()) != found0.end());
         REQUIRE(found1.size() == 10);
         REQUIRE(found1.find(n0t.GetDefinition()) != found1.end());
         REQUIRE(found1.find(n1t.GetDefinition()) != found1.end());
         REQUIRE(found1.find(n2t.GetDefinition()) != found1.end());
         REQUIRE(found1.find(n3t.GetDefinition()) != found1.end());
         REQUIRE(found2.size() == 4);
         REQUIRE(found2.find(vvv.GetDefinition()) != found2.end());
         REQUIRE(found2.find(n1c.GetDefinition()) != found2.end());
         REQUIRE(found3.size() == 2);
         REQUIRE(found4.size() == 2);
         REQUIRE(found5.size() == 2);
         REQUIRE(found6.size() == 1);
      }
      
      WHEN("Attempting to disambiguate token") {
         REQUIRE(RTTI::Registry::DisambiguateMeta("type"));
         REQUIRE_THROWS(RTTI::Registry::DisambiguateMeta("::type"));
         REQUIRE(RTTI::Registry::DisambiguateMeta("create"));
         REQUIRE_THROWS(RTTI::Registry::DisambiguateMeta("one"));
         REQUIRE(RTTI::Registry::DisambiguateMeta("YetAnotherNamedType::one"));
         REQUIRE(RTTI::Registry::DisambiguateMeta("ImplicitlyReflectedData::one"));
         REQUIRE_THROWS(RTTI::Registry::DisambiguateMeta("MyType::one"));
         REQUIRE_THROWS(RTTI::Registry::DisambiguateMeta("two"));
         REQUIRE_THROWS(RTTI::Registry::DisambiguateMeta("three"));
         REQUIRE(RTTI::Registry::DisambiguateMeta("MyType"));
      }

      WHEN("Meta is retrieved by reflected file extension") {
         auto ImplicitlyReflectedDataWithTraits_meta
            = MetaDataOf<ImplicitlyReflectedDataWithTraits>();
         auto CheckingWhatGetsInherited_meta
            = MetaDataOf<CheckingWhatGetsInherited>();
         auto ImplicitlyReflectedData_meta
            = MetaDataOf<ImplicitlyReflectedData>();
         auto ConvertibleData_meta
            = MetaDataOf<ConvertibleData>();
         auto CheckingWhatConverterGetsInherited_meta
            = MetaDataOf<CheckingWhatConverterGetsInherited>();

         auto foundnonexistent = RTTI::Registry::ResolveFileExtension("whatever");
         REQUIRE(foundnonexistent.empty());

         auto foundtxt = RTTI::Registry::ResolveFileExtension("txt");
         REQUIRE(foundtxt.size() == 2);
         REQUIRE(foundtxt.contains(ImplicitlyReflectedDataWithTraits_meta.GetDefinition()));
         REQUIRE(foundtxt.contains(CheckingWhatGetsInherited_meta.GetDefinition()));

         auto foundpdf = RTTI::Registry::ResolveFileExtension("PDF");
         REQUIRE(foundpdf.size() == 2);
         REQUIRE(foundpdf.contains(ImplicitlyReflectedDataWithTraits_meta.GetDefinition()));
         REQUIRE(foundpdf.contains(CheckingWhatGetsInherited_meta.GetDefinition()));

         auto foundase = RTTI::Registry::ResolveFileExtension("ase");
         REQUIRE(foundase.size() == 3);
         REQUIRE(foundase.contains(ImplicitlyReflectedData_meta.GetDefinition()));
         REQUIRE(foundase.contains(ConvertibleData_meta.GetDefinition()));
         REQUIRE(foundase.contains(CheckingWhatConverterGetsInherited_meta.GetDefinition()));
      }
   }
}

SCENARIO("Testing operators") {
   GIVEN("Reflected verb with positive and negative operator") {
      const auto vvv = MetaVerbOf<Verbs::Create>();

      WHEN("Meta is retrieved by operator token, that is not case-sensitive and ignores spaces/tabs/escapes") {
         REQUIRE(vvv == RTTI::Registry::GetMetaVerbByToken("+"));
         REQUIRE(vvv == RTTI::Registry::GetMetaVerbByToken("-"));
         REQUIRE(vvv == RTTI::Registry::GetMetaVerbByToken("      +"));
         REQUIRE(vvv == RTTI::Registry::GetMetaVerbByToken("  -"));
         REQUIRE(vvv == RTTI::Registry::GetMetaVerbByToken("      +     "));
         REQUIRE(vvv == RTTI::Registry::GetMetaVerbByToken("  - "));
         REQUIRE(vvv == RTTI::Registry::GetMetaVerbByToken("\t\t + \n   \t\t "));
         REQUIRE(vvv == RTTI::Registry::GetMetaVerbByToken("\t\t  - \n\t\t"));
         REQUIRE(nullptr == RTTI::Registry::GetMetaVerbByToken("="));
      }
   }
}

SCENARIO("Testing constants reflected from another translation unit") {
   GIVEN("ImplicitlyReflectedData") {
      const auto vvv = MetaDataOf<ImplicitlyReflectedData>();

      WHEN("Meta is retrieved by constant token") {
         REQUIRE(RTTI::Registry::GetMetaConstByToken("ImplicitlyReflectedData::One"));
         REQUIRE(RTTI::Registry::GetMetaConstByToken("ImplicitlyReflectedData::Two"));
         REQUIRE(RTTI::Registry::GetMetaConstByToken("ImplicitlyReflectedData::Three"));
         REQUIRE(CMeta(RTTI::Registry::GetMetaConstByToken("ImplicitlyReflectedData::One")).GetType() == vvv);
         REQUIRE(CMeta(RTTI::Registry::GetMetaConstByToken("ImplicitlyReflectedData::Two")).GetType() == vvv);
         REQUIRE(CMeta(RTTI::Registry::GetMetaConstByToken("ImplicitlyReflectedData::Three")).GetType() == vvv);
      }
   }
}

SCENARIO("Modifying a data type at runtime") {
   //TODO eventually. must be constrained for the boundary-specific reflection - you can never modify master records, and any modification
   //TODO must ensure binary compatibility. in other words - all you can do is extend, never override.
   //TODO if definitions differ too much in abilities, they will be executed in an disjunctive manner and user can pick
   //TODO desired result on per-context basis if results differ
}
