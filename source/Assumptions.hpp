///                                                                           
/// Langulus::Core                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Config.hpp"
#include <Logger/Logger.hpp>

namespace Langulus
{

   /// A developer assumption level is higher - it is less likely to fail     
   /// Useful inside inner functions, that are implementation details         
   constexpr unsigned DevAssumes {2};

   /// A user assumption level is lower - more likely to fail - no one RTFM!  
   /// Useful for public interface functions, that are exposed to user input  
   constexpr unsigned UserAssumes {1};

   /// Common Langulus assertion                                              
   ///   @tparam LEVEL - level of the assumption, configurable from CMake,    
   /// using LANGULUS_ENABLE_ASSERTION_LEVEL and LANGULUS_ENABLE_SAFE_MODE    
   ///   @param condition - the condition that must hold true                 
   ///   @param message - an error message if condition doesn't hold          
   ///   @param location - the location of the error, if any                  
   template<unsigned LEVEL, class EXCEPTION = Except::Assertion, class... MORE>
   LANGULUS(INLINED) SAFETY_CONSTEXPR()
   void Assume(
      bool condition, 
      const char* message = "<unknown assumption failure>", 
      const char* location = "<unknown location>",
      MORE&&... additional_messages
   ) noexcept (LEVEL > LANGULUS_SAFE()) {
      if constexpr (LEVEL <= LANGULUS_SAFE()) {
         if (!condition) {
            // Log error message                                        
            if constexpr (LEVEL == 0)
               Logger::Error("Assertion failure: ",
                  message, Forward<MORE>(additional_messages)...);
            else if constexpr (LEVEL == UserAssumes)
               Logger::Error("User assumption failure: ",
                  message, Forward<MORE>(additional_messages)...);
            else if constexpr (LEVEL == DevAssumes)
               Logger::Error("Dev assumption failure: ",
                  message, Forward<MORE>(additional_messages)...);
            else 
               Logger::Error("Assumption level ", LEVEL," failure: ",
                  message, Forward<MORE>(additional_messages)...);

            // Log location                                             
            Logger::Error("At ", location);

            // Throw                                                    
            Throw<EXCEPTION>(message, location);
         }
      }
   }

} // namespace Langulus


/// Convenience macro for declaring an assumption                             
/// Throws Except::Assertion if condition isn't met                           
///   @param level - the level at which assumption will be checked -          
/// if level is larger than LANGULUS_SAFE(), no check will be performed;      
/// Zero level assumptions are always checked                                 
///   @param condition - the condition to check for failure                   
///   @param message - the exception message, if condition doesn't hold       
#if LANGULUS_COMPILER(CLANG) || LANGULUS_COMPILER(GCC)
   #define LANGULUS_ASSUME(level, condition, message, ...) \
      ::Langulus::Assume<level>(condition, message, LANGULUS_LOCATION() __VA_OPT__(,) __VA_ARGS__)

   #define LANGULUS_ASSERT(condition, exception, message, ...) \
      ::Langulus::Assume<0, ::Langulus::Except::exception>(condition, message, LANGULUS_LOCATION() __VA_OPT__(,) __VA_ARGS__)

   #define LANGULUS_THROW(exception, message, ...) \
      ::Langulus::Throw<::Langulus::Except::exception>(message, LANGULUS_LOCATION() __VA_OPT__(,) __VA_ARGS__)
#else
   #define LANGULUS_ASSUME(level, condition, message, ...) \
      ::Langulus::Assume<level>(condition, message, LANGULUS_LOCATION(), __VA_ARGS__)

   #define LANGULUS_ASSERT(condition, exception, message, ...) \
      ::Langulus::Assume<0, ::Langulus::Except::exception>(condition, message, LANGULUS_LOCATION(), __VA_ARGS__)

   #define LANGULUS_THROW(exception, message, ...) \
      ::Langulus::Throw<::Langulus::Except::exception>(message, LANGULUS_LOCATION(), __VA_ARGS__)
#endif

/// Convenience macro for specifying temporary lazyness                       
#define TODO() LANGULUS_THROW(ToDo, "Unfinished code")
