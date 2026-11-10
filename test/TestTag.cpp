///                                                                           
/// Langulus::Core                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "Main.hpp"
#include <Langulus/Tag.hpp>
#include <Langulus/MetaOf.hpp>
#include "TestTypes/CommonTypes.hpp"

using namespace Langulus;

namespace Langulus::Tags
{
   /// This tag overlaps with another one that was defined in an unnamed      
   /// namespace, in another test. Getting its meta definition should result  
   /// in a runtime exception                                                 
   /*template<class T = void>
   struct Name;
   template<CT::NotVoid T>
   struct Name<T> : Tag<T, Name<void>> {};
   template<>
   struct Name<void> {
      using CTTI_Versioned = Version<7, 10>;
      using CTTI_DefineTag = Yes<"Name">;
      using CTTI_Info      = Yes<"Used for tagging names">;
   };*/

   /// A tag definition that also acts as a container                         
   template<class T = void>
   struct TCount;
   using Count = TCount<>;
   template<CT::NotVoid T>
   struct TCount<T> : Tag<T, Count> {};

   /// This is the real tag definition                                        
   template<>
   struct TCount<void> {
      using CTTI_Versioned = Version<7, 10>;
      using CTTI_DefineTag = Yes<"Count">;
      using CTTI_Info      = Yes<"Used for tagging counters">;
   };
}

TEST_CASE_TEMPLATE("Tags", T,
   int, ::std::string
) {
   // Will cause a runtime meta conflict with another test              
   // if that test has been executed prior to this one (!!!)            
   // Sad thing is, that this can't be guaranteed, so I've disabled it. 
   // Update: it can't be guaranteed to work, but can be guaranteed not 
   // to work. The solution is to disable reflection of items inside    
   // anonymous namespaces!                                             
   //REQUIRE_THROWS(MetaTagOf<Tags::Name<>>());

   {
      // Testing the tag aspect of the tag                              
      //[[maybe_unused]] auto meta1 = MetaTagOf<Tags::Count<T>>(); // shouldn't compile    
      auto meta = MetaTagOf<Tags::Count>();
      REQUIRE(meta);
      REQUIRE(meta.GetName() == RTTI::Inner::ToLowercase(RTTI::NameOfTag<Tags::Count>()));
      REQUIRE(meta.GetInfo() == InfoOf<Tags::Count>());
      REQUIRE(meta.GetVersionMajor() == 7);
      REQUIRE(meta.GetVersionMinor() == 10);

      [[maybe_unused]] Tags::TCount<T> instance;
      static_assert(sizeof(T) == sizeof(Tags::TCount<T>));
      static_assert(::std::same_as<TagsOf<Tags::TCount<T>>, Types<Tags::Count>>);
   }

   {
      // Testing the data aspect of the tag                             
      //[[maybe_unused]] auto meta1 = MetaDataOf<Tags::Count<>>(); // shouldn't compile    
      auto meta = MetaDataOf<Tags::TCount<T>>();
      REQUIRE(meta);
      if constexpr (::std::same_as<T, int>)
         REQUIRE(meta.GetName() == "Int32");
      else
         REQUIRE(meta.GetName() == "String");
      REQUIRE(meta.GetInfo() == InfoOf<T>());
      REQUIRE(meta.GetVersionMajor() == 1);
      REQUIRE(meta.GetVersionMinor() == 0);

      [[maybe_unused]] Tags::TCount<T> instance;
      static_assert(sizeof(T) == sizeof(Tags::TCount<T>));
      static_assert(::std::same_as<TagsOf<Tags::TCount<T>>, Types<Tags::Count>>);
   }
}
