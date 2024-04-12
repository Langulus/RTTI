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

/// An empty trivial type                                                     
class ImplicitlyConstructible {};

/// A simple aggregate type                                                   
struct AggregateType {
   int m1, m2, m3, m4;
   bool m5;
};

/// Explicitly deleted destructor                                             
class NonDestructible {
   ~NonDestructible() = delete;
};

/// Has an explicit destructor                                                
class Destructible {
public:
   char* someptr {};

   ~Destructible() {
      if (someptr)
         delete someptr;
   }
};

/// Default-constructible, but only privately                                 
class PrivatelyConstructible {
   PrivatelyConstructible() = default;
};

/// Had no explicit semantic constructors and assigners                       
/// Has only implicit refer & move constructors and assigners                 
class NonSemanticConstructible {
public:
   NonSemanticConstructible(CT::NotSemantic auto&&) {}
};

/// Has explicit copy, move, refer, clone, abandon, disown constructors       
/// Has implicit refer & move constructors, too                               
/// Has no explicit semantic assigners, only implicit refer & move            
class PartiallySemanticConstructible {
public:
   template<template<class> class S, class T>
   PartiallySemanticConstructible(S<T>&&) requires CT::Semantic<S<T>> {}
};

/// Has all semantic constructors + implicit refer & move ones                
/// Has no explicit semantic assigners, only implicit refer & move ones       
/// Making constructor explicit makes sure, that no implicit semantic assign  
/// happens                                                                   
class AllSemanticConstructible {
public:
   LANGULUS(POD) false;
   explicit AllSemanticConstructible(CT::Semantic auto&&) {}
};

/// Has all semantic constructors + implicit refer & move ones                
/// Has no explicit semantic assigners, only implicit refer & move ones       
/// Making constructor implicit also allows for semantic assignments          
class AllSemanticConstructibleImplicit {
public:
   LANGULUS(POD) false;
   AllSemanticConstructibleImplicit(CT::Semantic auto&&) {}
};

/// Has all semantic constructors and assigners + implicit refer & move ones  
class AllSemanticConstructibleAndAssignable {
public:
   LANGULUS(POD) false;
   AllSemanticConstructibleAndAssignable(CT::Semantic auto&&) {}
   AllSemanticConstructibleAndAssignable& operator = (CT::Semantic auto&&) { return *this; }
};

/// Has explicit descriptor constructor, and implicit refer & move ones       
/// Has no explicit semantic assigners, only implicit refer & move            
class DescriptorConstructible {
public:
   DescriptorConstructible(Describe&&) {}
};