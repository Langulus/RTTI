///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Common.hpp"


TEMPLATE_TEST_CASE("Reflected from/to converters", "[metadata]",
   (TypePair<ImplicitlyReflectedDataWithTraits,       int>),
   (TypePair<ImplicitlyReflectedDataWithTraits, const int>),
   (TypePair<ImplicitlyReflectedDataWithTraits, const int*>),
   (TypePair<Pi,       ImplicitlyReflectedDataWithTraits>),
   (TypePair<Pi, const ImplicitlyReflectedDataWithTraits>),
   (TypePair<Pi, const ImplicitlyReflectedDataWithTraits*>),

   (TypePair<CheckingWhatGetsInherited,       int>),
   (TypePair<CheckingWhatGetsInherited, const int>),
   (TypePair<CheckingWhatGetsInherited, const int*>),
   (TypePair<Pi,       CheckingWhatGetsInherited>),
   (TypePair<Pi, const CheckingWhatGetsInherited>),
   (TypePair<Pi, const CheckingWhatGetsInherited*>)
) {
   using LHS = typename TestType::LHS;
   using RHS = typename TestType::RHS;

   GIVEN("ImplicitlyReflectedDataWithTraits") {
      Decay<LHS> lhs;
      Decay<RHS> rhs;

      auto lhsMeta = MetaData::Of<LHS>();
      auto rhsMeta = MetaData::Of<RHS>();
      auto converter = lhsMeta->GetConverter(rhsMeta);

      REQUIRE(converter);
      converter(&lhs, &rhs);
   }
}
