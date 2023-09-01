///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Core/Exceptions.hpp>
#include <Logger/Logger.hpp>


/// Reflections will be registered in a centralized location, allowing for    
/// runtime type modification. Meta primitives will always be in the same     
/// place in memory regardless of translation unit, which significantly       
/// speeds up meta definition comparisons.                                    
/// Naming collisions will be detected upon type registration                 
/// Gives a significant overhead on program launch, no dependencies           
#ifdef LANGULUS_ENABLE_FEATURE_MANAGED_REFLECTION
   #define LANGULUS_FEATURE_MANAGED_REFLECTION() 1
   #define IF_LANGULUS_MANAGED_REFLECTION(a) a
   #define IF_NOT_LANGULUS_MANAGED_REFLECTION(a)
#else
   #define LANGULUS_FEATURE_MANAGED_REFLECTION() 0
   #define IF_LANGULUS_MANAGED_REFLECTION(a)
   #define IF_NOT_LANGULUS_MANAGED_REFLECTION(a) a
#endif

/// Memory allocations will be pooled, authority will be tracked,             
/// memory will be reused whenever possible, and you can also tweak           
/// runtime allocation strategies on per-type basis                           
/// Significantly improves performance, no dependencies                       
#ifdef LANGULUS_ENABLE_FEATURE_MANAGED_MEMORY
   #define LANGULUS_FEATURE_MANAGED_MEMORY() 1
   #define IF_LANGULUS_MANAGED_MEMORY(a) a
   #define IF_NOT_LANGULUS_MANAGED_MEMORY(a)
#else
   #define LANGULUS_FEATURE_MANAGED_MEMORY() 0
   #define IF_LANGULUS_MANAGED_MEMORY(a)
   #define IF_NOT_LANGULUS_MANAGED_MEMORY(a) a
#endif

#if defined(LANGULUS_EXPORT_ALL) or defined(LANGULUS_EXPORT_RTTI)
   #define LANGULUS_API_RTTI() LANGULUS_EXPORT()
#else
   #define LANGULUS_API_RTTI() LANGULUS_IMPORT()
#endif

LANGULUS_EXCEPTION(Meta);

/// Make the rest of the code aware, that Langulus::RTTI has been included    
#define LANGULUS_LIBRARY_RTTI() 1
