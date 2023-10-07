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

using TypeDeepAlias = One::Two::Three::TypeDeepIntoNamespaces;
using TemplatedAlias = One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>;
using VeryComplexTemplatedAlias = One::Two::Three::VeryComplexTemplate<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>>;


SCENARIO("NameOf", "[nameof]") {
   GIVEN("Type uint16_t") {
      WHEN("Taken the name of uint16_t") {
         auto name = NameOf<uint16_t>();

         THEN("Name should be correct") {
            REQUIRE(name == "uint16");
         }
      }
      
      WHEN("Taken the name of uint16_t&") {
         auto name = NameOf<uint16_t&>();

         THEN("Name should be correct") {
            REQUIRE(name == "uint16&");
         }
      }

      WHEN("Taken the name of const uint16_t") {
         auto name = NameOf<const uint16_t>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16");
         }
      }

      WHEN("Taken the name of const uint16_t*") {
         auto name = NameOf<const uint16_t*>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16*");
         }
      }

      WHEN("Taken the name of const uint16_t**") {
         auto name = NameOf<const uint16_t**>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16**");
         }
      }
      
      WHEN("Taken the name of const uint16_t* const*") {
         auto name = NameOf<const uint16_t* const *>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16*const *");
         }
      }
      
      WHEN("Taken the name of const uint16_t* const* const") {
         auto name = NameOf<const uint16_t* const * const>();

         THEN("Name should be correct") {
            REQUIRE(name == "const uint16*const *const");
         }
      }

      WHEN("Taken the name of uint16*") {
         auto name = NameOf<uint16_t*>();

         THEN("Name should be correct") {
            REQUIRE(name == "uint16*");
         }
      }
   }

   GIVEN("The enum Pi") {
      WHEN("Taken the name of") {
         auto name = NameOf<Pi::Number>();

         THEN("Name should be correct") {
            REQUIRE(name == "Pi::Number");
         }
      }
   }

   GIVEN("Type IncompleteType") {
      WHEN("Taken the name of IncompleteType") {
         auto name = NameOf<IncompleteType>();

         THEN("Name should be correct") {
            REQUIRE(name == "IncompleteType");
         }
      }

      WHEN("Taken the name of IncompleteType*") {
         auto name = NameOf<IncompleteType*>();

         THEN("Name should be correct") {
            REQUIRE(name == "IncompleteType*");
         }
      }

      WHEN("Taken the name of const IncompleteType*") {
         auto name = NameOf<const IncompleteType*>();

         THEN("Name should be correct") {
            REQUIRE(name == "const IncompleteType*");
         }
      }

      WHEN("Taken the name of const IncompleteType") {
         auto name = NameOf<const IncompleteType>();

         THEN("Name should be correct") {
            REQUIRE(name == "const IncompleteType");
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

   GIVEN("Type TypeDeepAlias") {
      WHEN("Taken the name of TypeDeepAlias") {
         auto name = NameOf<TypeDeepAlias>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces");
         }
      }

      WHEN("Taken the name of TypeDeepAlias*") {
         auto name = NameOf<TypeDeepAlias*>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces*");
         }
      }

      WHEN("Taken the name of const TypeDeepAlias*") {
         auto name = NameOf<const TypeDeepAlias*>();

         THEN("Name should be correct") {
            REQUIRE(name == "const One::Two::Three::TypeDeepIntoNamespaces*");
         }
      }

      WHEN("Taken the name of const TypeDeepAlias*") {
         auto name = NameOf<const TypeDeepAlias>();

         THEN("Name should be correct") {
            REQUIRE(name == "const One::Two::Three::TypeDeepIntoNamespaces");
         }
      }
   }

   GIVEN("Type One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>");
         }
      }
   }

   GIVEN("Enum One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>::VeryDeeplyTemplatedEnum::YesYouGotThatRight") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>::VeryDeeplyTemplatedEnum::YesYouGotThatRight>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>::VeryDeeplyTemplatedEnum::YesYouGotThatRight");
         }
      }
   }

   GIVEN("Enum One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::VeryDeeplyTemplatedEnum::YesYouGotThatRight") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::VeryDeeplyTemplatedEnum::YesYouGotThatRight>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>::VeryDeeplyTemplatedEnum::YesYouGotThatRight");
         }
      }
   }

   GIVEN("Type One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::Nested<uint16_t>") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::Nested<uint16_t>>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>::Nested<uint16>");
         }
      }
   }

   GIVEN("Type TemplatedAlias") {
      WHEN("Taken the name of") {
         auto name = NameOf<TemplatedAlias>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>");
         }
      }
   }

   GIVEN("Type One::Two::Three::VeryComplexTemplate<TemplatedAlias>") {
      WHEN("Taken the name of") {
         auto name = NameOf<One::Two::Three::VeryComplexTemplate<TemplatedAlias>>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::VeryComplexTemplate<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>>");
         }
      }
   }

   GIVEN("Type VeryComplexTemplatedAlias") {
      WHEN("Taken the name of") {
         auto name = NameOf<VeryComplexTemplatedAlias>();

         THEN("Name should be correct") {
            REQUIRE(name == "One::Two::Three::VeryComplexTemplate<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>>");
         }
      }
   }

   GIVEN("Types containing many skippable patterns") {
      WHEN("Taken the name of Langulus::Flow::Construct") {
         auto name = NameOf<Langulus::Flow::Construct>();

         THEN("Name should be correct") {
            REQUIRE(name == "Flow::Construct");
         }
      }

      WHEN("Taken the name of Langulus::Flow::Constructconst") {
         auto name = NameOf<Langulus::Flow::Constructconst>();

         THEN("Name should be correct") {
            REQUIRE(name == "Flow::Constructconst");
         }
      }

      WHEN("Taken the name of Langulus::Flow::constConstructconst") {
         auto name = NameOf<Langulus::Flow::constConstructconst>();

         THEN("Name should be correct") {
            REQUIRE(name == "Flow::constConstructconst");
         }
      }

      WHEN("Taken the name of Langulus::Flow::constconst") {
         auto name = NameOf<Langulus::Flow::constconst>();

         THEN("Name should be correct") {
            REQUIRE(name == "Flow::constconst");
         }
      }
   }
}


struct TypeWithSuffix { LANGULUS(SUFFIX) "yeah"; };
struct TypeWithoutSuffix {};

SCENARIO("SuffixOf", "[metadata]") {
   WHEN("Generating a suffix for uint8_t") {
      constexpr auto token = SuffixOf<uint8_t>();
      THEN("Requirements should be met") {
         REQUIRE(token == "u8");
      }
   }

   WHEN("Generating a suffix for uint16_t") {
      constexpr auto token = SuffixOf<uint16_t>();
      THEN("Requirements should be met") {
         REQUIRE(token == "u16");
      }
   }

   WHEN("Generating a suffix for uint32_t") {
      constexpr auto token = SuffixOf<uint32_t>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<uint32_t, unsigned int>)
            REQUIRE(token == "u");
         else
            REQUIRE(token == "u32");
      }
   }
   
   WHEN("Generating a suffix for uint64_t") {
      constexpr auto token = SuffixOf<uint64_t>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<uint64_t, unsigned int>)
            REQUIRE(token == "u");
         else
            REQUIRE(token == "u64");
      }
   }

   WHEN("Generating a suffix for int8_t") {
      constexpr auto token = SuffixOf<int8_t>();
      THEN("Requirements should be met") {
         REQUIRE(token == "i8");
      }
   }

   WHEN("Generating a suffix for int16_t") {
      constexpr auto token = SuffixOf<int16_t>();
      THEN("Requirements should be met") {
         REQUIRE(token == "i16");
      }
   }

   WHEN("Generating a suffix for int32_t") {
      constexpr auto token = SuffixOf<int32_t>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<int32_t, signed int>)
            REQUIRE(token == "i");
         else
            REQUIRE(token == "i32");
      }
   }
   
   WHEN("Generating a suffix for int64_t") {
      constexpr auto token = SuffixOf<int64_t>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<int64_t, signed int>)
            REQUIRE(token == "i");
         else
            REQUIRE(token == "i64");
      }
   }

   WHEN("Generating a suffix for float") {
      constexpr auto token = SuffixOf<float>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<float, Real>)
            REQUIRE(token == "");
         else
            REQUIRE(token == "f");
      }
   }

   WHEN("Generating a suffix for double") {
      constexpr auto token = SuffixOf<double>();
      THEN("Requirements should be met") {
         if constexpr (CT::Same<double, Real>)
            REQUIRE(token == "");
         else
            REQUIRE(token == "d");
      }
   }

   WHEN("Generating a suffix for bool") {
      constexpr auto token = SuffixOf<bool>();
      THEN("Requirements should be met") {
         REQUIRE(token == "b");
      }
   }

   WHEN("Generating a suffix for a type with LANGULUS(SUFFIX)") {
      constexpr auto token = SuffixOf<TypeWithSuffix>();
      THEN("Requirements should be met") {
         REQUIRE(token == "yeah");
      }
   }

   WHEN("Generating a suffix for a type without LANGULUS(SUFFIX)") {
      constexpr auto token = SuffixOf<TypeWithoutSuffix>();
      THEN("Requirements should be met") {
         REQUIRE(token == "");
      }
   }
}