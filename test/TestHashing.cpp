///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "Common.hpp"


SCENARIO("Test hashing different kinds of types", "[hash]") {
   WHEN("Hashing a Token or std::string") {
      Token same1 = "Same1";
      Token same2 = "Same1";
      std::string same1str = "Same1";
      std::string same2str = "Same1";

      REQUIRE(HashOf(same1) == HashOf(same2));
      REQUIRE(HashOf(same1str) == HashOf(same2str));
      REQUIRE(HashOf(same1) == HashOf(same1str));
      REQUIRE(HashOf(same2) == HashOf(same2str));
   }
}