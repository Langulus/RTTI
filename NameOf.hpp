///																									
/// Langulus::RTTI																			   
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include "Config.hpp"
#include <string>

namespace Langulus::RTTI
{
   namespace Inner
   {

      /// Pretty function used to wrap a type as a template argument          
      /// Once we wrap and stringify it, we can isolate the typename itself   
      template<class T>
      constexpr Token TypeAsTemplateArgument() {
         return LANGULUS_FUNCTION();
      }

      /// Filter a literal matching at the front                              
      constexpr void SkipPrefix(Token& str, const Token& prefix) {
         if (str.starts_with(prefix))
            str.remove_prefix(prefix.size());
      }

      /// Filter a literal matching at the back                               
      constexpr void SkipSuffix(Token& str, const Token& suffix) {
         if (str.ends_with(suffix))
            str.remove_suffix(suffix.size());
      }

      /// Skip empty space                                                    
      constexpr void SkipSpace(Token& str) {
         while (str.size() > 0 && (str[0] == ' ' || str[0] == '\t'))
            str.remove_prefix(1);
      }

      /// Skip class identifier                                               
      constexpr void SkipClass(Token& name) {
         SkipSpace(name);
         SkipPrefix(name, "class");
         SkipSpace(name);
      }

      /// Skip struct identifier                                              
      constexpr void SkipStruct(Token& name) {
         SkipSpace(name);
         SkipPrefix(name, "struct");
         SkipSpace(name);
      }

      /// Skip enum identifier                                                
      constexpr void SkipEnum(Token& name) {
         SkipSpace(name);
         SkipPrefix(name, "enum");
         SkipSpace(name);
      }

      /// Skip all decorations in front of a type name                        
      constexpr void SkipDecorations(Token& name) {
         SkipStruct(name);
         SkipClass(name);
         SkipEnum(name);
      }

      /// Skip all decorations in front of a type name                        
      constexpr void SkipTemplateArguments(Token& name) {
         auto found = name.find_first_of('<', 0);
         if (found == name.npos)
            return;
         name.remove_suffix(name.size() - found);
      }

      /// Skip all decorations in front of a type name                        
      template<class T>
      constexpr Token Isolate() {
         // These definitions might change in future compiler versions  
         // and will probably need constant maintenance                 
         #if defined(__clang__)
            constexpr Token Prefix = "Langulus::Token Langulus::RTTI::Inner::TypeAsTemplateArgument() [T = ";
            constexpr Token Suffix = "]";
         #elif defined(__GNUC__) && !defined(__clang__)
            constexpr Token Prefix = "constexpr Langulus::Token Langulus::RTTI::Inner::TypeAsTemplateArgument() [with T = ";
            constexpr Token Suffix = "; Langulus::Token = std::basic_string_view<char>]";
         #elif defined(_MSC_VER)
            constexpr Token Prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Langulus::RTTI::Inner::TypeAsTemplateArgument<";
            constexpr Token Suffix = ">(void)";
         #else
            #error "No support for this compiler"
         #endif

         auto original = TypeAsTemplateArgument<Decay<T>>();
         SkipPrefix(original, Prefix);
         SkipSuffix(original, Suffix);
         SkipDecorations(original);
         return original;
      }

      template<class HEAD, class... TAIL>
      constexpr Size NameSplitAndEstimate() noexcept;
      template<class HEAD, class... TAIL>
      constexpr Size NameSplitAndAppend(char*) noexcept;

      template<class T>
      struct Overrider {};

      template<template<class...> class T, class... TArgs>
      constexpr Size NameOfEstimate(Overrider<T<TArgs...>>&&) noexcept;

      template<class T>
      constexpr Size NameOfEstimate(Overrider<T>&&) noexcept;

      /// Correctly handles templated types, but isn't constexpr due to static   
      ///   @tparam T - the templated type                                       
      ///   @return TArgs - the template arguments                               
      template<template<class...> class T, class... TArgs>
      Token NameOf(Overrider<T<TArgs...>>&&) noexcept {
         auto original = Isolate<T<TArgs...>>();
         SkipTemplateArguments(original);
         static char buffer[NameOfEstimate(Overrider<T<TArgs...>>{})] = {};
         ::std::memcpy(buffer, original.data(), original.size());
         auto i = original.size();
         buffer[i] = '<'; ++i;
         i += NameSplitAndAppend<TArgs...>(buffer + i);
         buffer[i] = '>'; ++i;
         return Token {buffer, i};
      }

      /// Gets the name of T, without the decorations                            
      /// See here for more details: https://github.com/Manu343726/ctti          
      template<class T>
      constexpr Token NameOf(Overrider<T>&&) noexcept {
         // Some fundamental types are stringified differently on different
         // compilers, and we need to make sure their names cross-match    
         if constexpr (CT::Same<T, ::std::int8_t>)
            return "int8";
         else if constexpr (CT::Same<T, ::std::int16_t>)
            return "int16";
         else if constexpr (CT::Same<T, ::std::int32_t>)
            return "int32";
         else if constexpr (CT::Same<T, ::std::int64_t>)
            return "int64";
         else if constexpr (CT::Same<T, ::std::uint8_t>)
            return "uint8";
         else if constexpr (CT::Same<T, ::std::uint16_t>)
            return "uint16";
         else if constexpr (CT::Same<T, ::std::uint32_t>)
            return "uint32";
         else if constexpr (CT::Same<T, ::std::uint64_t>)
            return "uint64";
         else
            return Isolate<T>();
      }

   } // namespace Inner

   template<class T>
   Token NameOf() noexcept {
      return Inner::NameOf(Inner::Overrider<Decay<T>>{});
   }

   template<template<class...> class T, class... TArgs>
   constexpr Size Inner::NameOfEstimate(Overrider<T<TArgs...>>&&) noexcept {
      return Inner::Isolate<T<TArgs...>>().size() + NameSplitAndEstimate<TArgs...>() + 3;
   }

   template<class T>
   constexpr Size Inner::NameOfEstimate(Overrider<T>&& a) noexcept {
      return Inner::NameOf(Overrider<T>{}).size();
   }

   template<class HEAD, class... TAIL>
   constexpr Size Inner::NameSplitAndEstimate() noexcept {
      constexpr auto original = NameOfEstimate(Overrider<HEAD>{});
      if constexpr (sizeof...(TAIL) > 0)
         return original + 2 + NameSplitAndEstimate<TAIL...>();
      else
         return original;
   }

   template<class HEAD, class... TAIL>
   constexpr Size Inner::NameSplitAndAppend(char* buffer) noexcept {
      auto original = ::Langulus::RTTI::NameOf<HEAD>();
      if constexpr (sizeof...(TAIL) > 0) {
         int i = 0;
         for (; i < original.size(); ++i)
            buffer[i] = original[i];
         buffer[i] = ','; ++i;
         buffer[i] = ' '; ++i;
         return original.size() + 2 + NameSplitAndAppend<TAIL...>(buffer + i);
      }
      else {
         for (int i = 0; i < original.size(); ++i)
            buffer[i] = original[i];
         return original.size();
      }
   }

} // namespace Langulus::RTTI
