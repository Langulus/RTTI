///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#include "Main.hpp"
#include <catch2/catch.hpp>

struct IncompleteType;

SCENARIO("Testing semantics", "[semantics]") {
   static_assert(not CT::DisownMakable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::DisownAssignable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::CloneMakable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::CloneAssignable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::AbandonMakable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::AbandonAssignable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::CopyMakable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::CopyAssignable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::MoveMakable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::MoveAssignable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::SemanticMakable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::SemanticAssignable<IncompleteType>,
      "Can't make an incomplete type");
}
