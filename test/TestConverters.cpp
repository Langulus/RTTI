///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/Testing/CommonTypes.hpp>
#include <Langulus/MetaOf.hpp>


TEST_CASE_TEMPLATE("Reflected morphisms", T,
   Types<Pi, ConvertibleData>,
   Types<Pi, ConvertibleData const >,
   Types<ConvertibleData, int       >,
   Types<ConvertibleData, int const >
) {
   using LHS = typename T::First;
   using RHS = typename T::Second;

   GIVEN("Convertible data") {
      LHS lhs;
      DecvqAll<RHS> rhs;

      auto lhsMeta = MetaDataOf<LHS>();
      auto rhsMeta = MetaDataOf<RHS>();
      auto converter = lhsMeta.GetMorphism(rhsMeta);

      REQUIRE(converter.convert);
      converter.convert(&lhs, &rhs);
   }
}

TEST_CASE_TEMPLATE("Non-existent morphisms", T,
   Types<ConvertibleData, int const*>,
   Types<Pi, ConvertibleData const*>,
   Types<CheckingWhatConverterGetsInherited, int const*>,
   Types<Pi, CheckingWhatConverterGetsInherited const*>,

   Types<CheckingWhatConverterGetsInherited, int       >,
   Types<CheckingWhatConverterGetsInherited, int const >,
   Types<Pi, CheckingWhatConverterGetsInherited>,
   Types<Pi, CheckingWhatConverterGetsInherited const >
) {
   using LHS = typename T::First;
   using RHS = typename T::Second;

   GIVEN("Non-convertible data") {
      auto lhsMeta = MetaDataOf<LHS>();
      auto rhsMeta = MetaDataOf<RHS>();
      auto converter = lhsMeta.GetMorphism(rhsMeta);

      REQUIRE_FALSE(converter.convert);
   }
}
