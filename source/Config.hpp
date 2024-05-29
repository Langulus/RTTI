///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include <Core/Exceptions.hpp>
#include <Core/Utilities.hpp>
#include <Logger/Logger.hpp>


#if defined(LANGULUS_EXPORT_ALL) or defined(LANGULUS_EXPORT_RTTI)
   #define LANGULUS_API_RTTI() LANGULUS_EXPORT()
#else
   #define LANGULUS_API_RTTI() LANGULUS_IMPORT()
#endif

LANGULUS_EXCEPTION(Meta);

/// Make the rest of the code aware, that Langulus::RTTI has been included    
#define LANGULUS_LIBRARY_RTTI() 1
