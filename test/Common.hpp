///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           

/// INTENTIONALLY NOT GUARDED                                                 
/// Include this file once in each cpp file, after all other headers          
#ifdef TWOBLUECUBES_SINGLE_INCLUDE_CATCH_HPP_INCLUDED
   #error Catch has been included prior to this header
#endif

//#define LANGULUS_STD_BENCHMARK

#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "Main.hpp"
#include <catch2/catch.hpp>


/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   return fmt::format("{}", ex);
}

using timer = Catch::Benchmark::Chronometer;

template<class T>
using uninitialized = Catch::Benchmark::storage_for<T>;

template<class L, class R, bool RESULT = true>
struct TypePair {
   using LHS = L;
   using RHS = R;
   static constexpr bool Result = RESULT;
};
