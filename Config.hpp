///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
/// Include this only when building standalone											
#pragma once
#include <Langulus.Logger.hpp>

/// Reflections will be registered in a centralized location, allowing for		
/// runtime type modification. Meta primitives will always be in the same		
/// place in memory regardless of translation unit, which significantly			
/// speeds up meta definition comparisons.												
/// Naming collisions will be detected upon type registration						
/// Gives a significant overhead on program launch, no dependencies				
#ifdef LANGULUS_ENABLE_MANAGED_REFLECTION
	#undef LANGULUS_FEATURE_MANAGED_REFLECTION
	#define LANGULUS_FEATURE_MANAGED_REFLECTION() 1
#else
	#define LANGULUS_FEATURE_MANAGED_REFLECTION() 0
#endif