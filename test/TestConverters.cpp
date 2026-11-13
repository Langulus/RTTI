///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/Testing/CommonTypes.hpp>
#include <Langulus/MetaOf.hpp>


TEST_CASE_TEMPLATE("Reflected from/to converters", TestType,
   Types<ConvertibleData, int       >,
   Types<ConvertibleData, int const >,
   Types<ConvertibleData, int const*>,
   Types<Pi, ConvertibleData>,
   Types<Pi, ConvertibleData const >,
   Types<Pi, ConvertibleData const*>,

   Types<CheckingWhatConverterGetsInherited, int       >,
   Types<CheckingWhatConverterGetsInherited, int const >,
   Types<CheckingWhatConverterGetsInherited, int const*>,
   Types<Pi, CheckingWhatConverterGetsInherited>,
   Types<Pi, CheckingWhatConverterGetsInherited const >,
   Types<Pi, CheckingWhatConverterGetsInherited const*>
) {
   using LHS = typename TestType::First;
   using RHS = typename TestType::Second;

   GIVEN("Convertible data") {
      Decay<LHS> lhs;
      Decay<RHS> rhs;

      auto lhsMeta = MetaDataOf<LHS>();
      auto rhsMeta = MetaDataOf<RHS>();
      auto converter = lhsMeta.GetMorphism(rhsMeta);

      REQUIRE(converter.convert);
      converter.convert(&lhs, &rhs);
   }
}
