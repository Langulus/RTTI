///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "Common.hpp"


TEMPLATE_TEST_CASE("Reflected from/to converters", "[metadata]",
   (TypePair<ConvertibleData, int       >),
   (TypePair<ConvertibleData, int const >),
   (TypePair<ConvertibleData, int const*>),
   (TypePair<Pi, ConvertibleData>),
   (TypePair<Pi, ConvertibleData const >),
   (TypePair<Pi, ConvertibleData const*>),

   (TypePair<CheckingWhatConverterGetsInherited, int       >),
   (TypePair<CheckingWhatConverterGetsInherited, int const >),
   (TypePair<CheckingWhatConverterGetsInherited, int const*>),
   (TypePair<Pi, CheckingWhatConverterGetsInherited>),
   (TypePair<Pi, CheckingWhatConverterGetsInherited const >),
   (TypePair<Pi, CheckingWhatConverterGetsInherited const*>)
) {
   using LHS = typename TestType::LHS;
   using RHS = typename TestType::RHS;

   GIVEN("Convertible data") {
      Decay<LHS> lhs;
      Decay<RHS> rhs;

      auto lhsMeta = MetaData::Of<LHS>();
      auto rhsMeta = MetaData::Of<RHS>();
      auto converter = lhsMeta->GetConverter(rhsMeta);

      REQUIRE(converter);
      converter(&lhs, &rhs);
   }
}
