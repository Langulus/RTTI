///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#include "Main.hpp"

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

LANGULUS_RTTI_BOUNDARY("MAIN")

int main(int argc, char* argv[]) {
   static_assert(not CT::Defaultable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::DefaultableNoexcept<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::DescriptorMakable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::DescriptorMakableNoexcept<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::Destroyable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::Referencable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::Swappable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::SwappableNoexcept<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::Resolvable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::Hashable<IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::DerivedFrom<IncompleteType, ImplicitlyReflectedData>,
      "Can't make an incomplete type");

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

   static_assert(not CT::SemanticMakable<Copied<IncompleteType>, IncompleteType>,
      "Can't make an incomplete type");
   static_assert(not CT::SemanticAssignable<Copied<IncompleteType>, IncompleteType>,
      "Can't make an incomplete type");

	Catch::Session session;
	return session.run(argc, argv);
}
