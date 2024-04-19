///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
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
   ///           using LANGULUS_ASSERTION_LEVEL and LANGULUS_SAFE_MODE        
   ///   @param condition - the condition that must hold true                 
   ///   @param message - an error message if condition doesn't hold          
   ///   @param location - the location of the error, if any                  
   template<unsigned LEVEL, class EXCEPTION = Except::Assertion, class...MORE>
   LANGULUS(INLINED) IF_UNSAFE(constexpr)
   void Assume(
      bool condition, 
      const char* message = "<unknown assumption failure>", 
      const char* location = "<unknown location>",
      UNUSED() MORE&&...additional_messages
   ) noexcept (LEVEL > LANGULUS(SAFE)) {
      if constexpr (LEVEL <= LANGULUS(SAFE)) {
         if (not condition) {
            #if LANGULUS(SAFE)
               // Log location first, because message might cause       
               // additional errors                                     
               Logger::Error("At ", location);

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
                  Logger::Error("Assumption level ", LEVEL, " failure: ",
                     message, Forward<MORE>(additional_messages)...);
            #endif

            // Throw                                                    
            Throw<EXCEPTION>(message, location);
         }
      }
   }

} // namespace Langulus


#if LANGULUS(DEBUG)
   /// Convenience macros for declaring assumptions, assertions, etc.         
   #if LANGULUS_COMPILER(CLANG) or LANGULUS_COMPILER(GCC)
      /// Logs error, and throws Except::Assertion if condition isn't met     
      ///   @param level - the level at which assumption will be checked -    
      ///      if level is larger than LANGULUS_SAFE(), no check is done      
      ///   @attention zero level assumptions are always checked              
      ///   @attention assumption macro is entirely disabled when building    
      ///      without LANGULUS(SAFE)                                         
      ///   @param condition - the condition to check for failure             
      ///   @param message - the exception message, if condition doesn't hold 
      #if LANGULUS(SAFE)
         #define LANGULUS_ASSUME(level, condition, message, ...) \
            ::Langulus::Assume<level>((condition)?true:false, message, \
               LANGULUS_LOCATION() __VA_OPT__(,) __VA_ARGS__)
      #else
         #define LANGULUS_ASSUME(level, condition, message, ...) 
      #endif

      /// Logs error, and throws an exception of your choice, if condition    
      /// wasn't met                                                          
      ///   @attention assertions are always checked, even in release builds  
      ///      use assumptions instead, if that is not desired                
      ///   @param condition - the condition to check for failure             
      ///   @param exception - the exception to throw if condition isn't met  
      ///   @param message - the exception message, if condition doesn't hold 
      #define LANGULUS_ASSERT(condition, exception, message, ...) \
         ::Langulus::Assume<0, ::Langulus::Except::exception>((condition)?true:false, message, \
            LANGULUS_LOCATION() __VA_OPT__(,) __VA_ARGS__)

      /// Logs error, and throws an exception of your choice                  
      /// If you want to throw without logging, use LANGULUS_THROW instead    
      ///   @attention always throws (even in release builds)                 
      ///   @param exception - the exception to throw                         
      ///   @param message - the exception message                            
      #define LANGULUS_OOPS(exception, message, ...) \
         ::Langulus::Assume<0, ::Langulus::Except::exception>(false, message, \
            LANGULUS_LOCATION() __VA_OPT__(,) __VA_ARGS__)

      /// Just throws, without logging anything                               
      ///   @param exception - the exception to throw                         
      ///   @param message - the exception message                            
      #define LANGULUS_THROW(exception, message, ...) \
         ::Langulus::Throw<::Langulus::Except::exception>(message, \
            LANGULUS_LOCATION() __VA_OPT__(,) __VA_ARGS__)
   #else
      #if LANGULUS(SAFE)
         #define LANGULUS_ASSUME(level, condition, message, ...) \
            ::Langulus::Assume<level>((condition)?true:false, message, \
               LANGULUS_LOCATION(), __VA_ARGS__)
      #else
         #define LANGULUS_ASSUME(level, condition, message, ...) 
      #endif

      #define LANGULUS_ASSERT(condition, exception, message, ...) \
         ::Langulus::Assume<0, ::Langulus::Except::exception>((condition)?true:false, message, \
            LANGULUS_LOCATION(), __VA_ARGS__)

      #define LANGULUS_OOPS(exception, message, ...) \
         ::Langulus::Assume<0, ::Langulus::Except::exception>(false, message, \
            LANGULUS_LOCATION(), __VA_ARGS__)

      #define LANGULUS_THROW(exception, message, ...) \
         ::Langulus::Throw<::Langulus::Except::exception>(message, \
            LANGULUS_LOCATION(), __VA_ARGS__)
   #endif
#else
   #if LANGULUS(SAFE)
      #define LANGULUS_ASSUME(level, condition, message, ...) \
         ::Langulus::Assume<level>((condition)?true:false)
   #else
      #define LANGULUS_ASSUME(level, condition, message, ...) 
   #endif

   #define LANGULUS_ASSERT(condition, exception, message, ...) \
      ::Langulus::Assume<0, ::Langulus::Except::exception>((condition)?true:false)

   #define LANGULUS_OOPS(exception, message, ...) \
      ::Langulus::Assume<0, ::Langulus::Except::exception>(false)

   #define LANGULUS_THROW(exception, message, ...) \
      ::Langulus::Throw<::Langulus::Except::exception>()
#endif

/// Convenience macro for specifying temporary lazyness                       
#define TODO() LANGULUS_OOPS(ToDo, "Unfinished code")
