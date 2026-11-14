///                                                                           
/// Langulus::Core                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/Testing/CommonTypes.hpp>
#include <Langulus/CT/DefineTag.hpp>
#include <Langulus/MetaOf.hpp>


namespace Langulus::Tags
{
   struct Count {
      using CTTI_Versioned = Version<7, 10>;
      using CTTI_DefineTag = NamedTag<"Count">;
      using CTTI_Info      = Yes<"Used for tagging counters">;

      ::std::string data;
   };
}

TEST_CASE("Tags") {
   {
      // Testing the tag aspect of the tag                              
      auto meta = MetaTagOf<Tags::Count>();
      REQUIRE(meta);
      REQUIRE(meta.GetName() == "count");
      REQUIRE(meta.GetInfo() == InfoOf<Tags::Count>());
      REQUIRE(meta.GetVersionMajor() == 7);
      REQUIRE(meta.GetVersionMinor() == 10);
   }

   {
      // Testing the data aspect of the tag                             
      auto meta = MetaDataOf<Tags::Count>();
      REQUIRE(meta);
      REQUIRE(meta.GetName() == "Langulus::Tags::Count");
      REQUIRE(meta.GetInfo() == "Used for tagging counters");
      REQUIRE(meta.GetVersionMajor() == 7);
      REQUIRE(meta.GetVersionMinor() == 10);

      [[maybe_unused]] Tags::Count instance;
      static_assert(sizeof(::std::string) == sizeof(Tags::Count));
      static_assert(::std::same_as<TagsOf<Tags::Count>, Types<Tags::Count>>);
   }
}
