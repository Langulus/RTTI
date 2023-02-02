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
#include <array>

namespace Langulus::RTTI
{
   namespace Inner
   {

      /// Pretty function used to wrap a type as a template argument          
      /// Once we wrap and stringify it, we can isolate the typename itself   
      ///   @tparam T - the type to get the name of                           
      template<class T>
      NOD() constexpr Token AsTemplateArgument() {
         return LANGULUS_FUNCTION();
      }

      /// Pretty function used to wrap a constexpr constant as a template     
      /// argument. Once we wrap and stringify it, we can isolate the name of 
      /// an enum, if it's a named constant                                   
      ///   @tparam T - the constant to get the name of                       
      template<auto T>
      NOD() constexpr Token AsTemplateArgument() {
         return LANGULUS_FUNCTION();
      }

      /// Filter a literal matching at the front                              
      ///   @param str - the string view to modify as constexpr               
      ///   @param prefix - the prefix to skip                                
      constexpr void SkipPrefix(Token& str, const Token& prefix) {
         if (str.starts_with(prefix))
            str.remove_prefix(prefix.size());
      }

      /// Filter a literal matching at the back                               
      ///   @param str - the string view to modify as constexpr               
      ///   @param suffix - the suffix to skip                                
      constexpr void SkipSuffix(Token& str, const Token& suffix) {
         // Additional checks are required due to this strange error    
         // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91397          
         if (str.size() >= suffix.size() && suffix.size() > 0 && str.ends_with(suffix))
            str.remove_suffix(suffix.size());
      }

      /// Skip empty space                                                    
      ///   @param str - the string view to modify as constexpr               
      constexpr void SkipSpace(Token& str) {
         while (str.size() > 0 && (str[0] == ' ' || str[0] == '\t'))
            str.remove_prefix(1);
      }

      /// Skip class identifier                                               
      ///   @param name - the string view to modify as constexpr              
      constexpr void SkipClass(Token& name) {
         SkipSpace(name);
         SkipPrefix(name, "class");
         SkipSpace(name);
      }

      /// Skip struct identifier                                              
      ///   @param name - the string view to modify as constexpr              
      constexpr void SkipStruct(Token& name) {
         SkipSpace(name);
         SkipPrefix(name, "struct");
         SkipSpace(name);
      }

      /// Skip enum identifier                                                
      ///   @param name - the string view to modify as constexpr              
      constexpr void SkipEnum(Token& name) {
         SkipSpace(name);
         SkipPrefix(name, "enum");
         SkipSpace(name);
      }

      /// Skip constantly occuring namespaces                                 
      ///   @param name - the string view to modify as constexpr              
      constexpr void SkipNamespace(Token& name) {
         SkipSpace(name);
         SkipPrefix(name, "Langulus::");
         SkipSpace(name);
      }

      /// Skip all namespaces                                                 
      ///   @param name - the string view to modify as constexpr              
      constexpr void SkipAllNamespaces(Token& name) {
         auto found = name.find_last_of(':', name.size());
         if (found == name.npos)
            return;

         name.remove_prefix(found + 1);
      }

      /// Skip all decorations in front of a type name                        
      ///   @param name - the string view to modify as constexpr              
      constexpr void SkipDecorations(Token& name) {
         SkipStruct(name);
         SkipClass(name);
         SkipEnum(name);
         SkipNamespace(name);
      }

      /// Skip all characters after the first encounter of a template bracket 
      /// This will result in a warning if dependent template types are found 
      ///   @param name - the string view to modify as constexpr              
      constexpr void SkipTemplateArguments(Token& name) {
         const auto found = name.find_first_of('<', 0);
         if (found == name.npos)
            return;

         name.remove_suffix(name.size() - found);
      }

      /// These definitions might change in future compiler versions          
      /// and will probably need constant maintenance                         
      #if defined(__clang__)
         constexpr Token Prefix = "Langulus::Token Langulus::RTTI::Inner::AsTemplateArgument() [T = ";
         constexpr Token Suffix = "]";
      #elif defined(__GNUC__) && !defined(__clang__)
         constexpr Token Prefix = "constexpr Langulus::Token Langulus::RTTI::Inner::AsTemplateArgument() [with T = ";
         constexpr Token Suffix = "; Langulus::Token = std::basic_string_view<char>]";
      #elif defined(_MSC_VER)
         constexpr Token Prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Langulus::RTTI::Inner::AsTemplateArgument<";
         constexpr Token Suffix = ">(void)";
      #else
         #error "No support for this compiler"
      #endif

      /// Skip all decorations in front of a type name                        
      ///   @return the modified constexpr string view                        
      template<class T>
      NOD() constexpr Token IsolateTypename() {
         auto original = AsTemplateArgument<T>();
         SkipPrefix(original, Prefix);
         SkipSuffix(original, Suffix);
         SkipDecorations(original);
         return original;
      }

      /// Skip all decorations in front of a constant, named or not           
      /// Also, skips all namespaces for enums, since those need to pass      
      /// through the name splitter anyways                                   
      ///   @return the modified constexpr string view                        
      template<auto T>
      NOD() constexpr Token IsolateConstant() {
         auto original = AsTemplateArgument<T>();
         SkipPrefix(original, Prefix);
         SkipSuffix(original, Suffix);
         SkipDecorations(original);
         SkipAllNamespaces(original);
         return original;
      }

      template<class HEAD, class... TAIL>
      NOD() constexpr Size NameSplitAndEstimate() noexcept;
      template<class HEAD, class... TAIL>
      NOD() constexpr Size NameSplitAndAppend(char*) noexcept;

      template<class T>
      struct Overrider {};

      template<template<class...> class T, class... TArgs>
      NOD() constexpr Size NameOfEstimate(Overrider<T<TArgs...>>&&) noexcept;
      template<class T>
      NOD() constexpr Size NameOfEstimate(Overrider<T>&&) noexcept;

      /// Correctly handles templated types, but isn't constexpr due to       
      /// static string buffer, that is required for substitution of          
      /// fundamental types. This will hopefully be improved in future C++    
      /// standards, and made completely constexpr, as it should be           
      ///   @tparam T - the templated type                                    
      ///   @return TArgs - the template arguments                            
      template<template<class...> class T, class... TArgs>
      NOD() constexpr Token NameOf(Overrider<T<TArgs...>>&&) noexcept {
         auto original = IsolateTypename<T<TArgs...>>();

         // Corner case, where the splitter doesn't work                
         /*#if LANGULUS(SAFE)
            const auto found = original.find_first_of('<', 0);
            int nested = 1;
            for (auto i = found + 1; i < original.size() && found != original.npos; ++i) {
               if (original[i] == '<') {
                  if (0 == nested) {
                     // Templates started again, which means a dependent
                     // template type was encountered. This behavior is 
                     // not covered by NameOf yet                       
                     Logger::Warning() <<
                        "NameOf can't guarantee, that parent template "
                        "arguments are consistently named fundamentals "
                        "- if that's probable, you should either move "
                        "your type outside the templated namespace, or "
                        "avoid implicit naming by manually defining the "
                        "reflected token via LANGULUS(NAME). "
                        "The type that triggered this warning is: "
                        << Logger::Red << original;
                     break;
                  }

                  ++nested;
               }
               else if (original[i] == '>')
                  --nested;
            }
         #endif*/

         SkipTemplateArguments(original);
         constexpr auto size = NameOfEstimate(Overrider<T<TArgs...>>{});
         ::std::array<char, size> buffer {};

         size_t i = 0;
         for (auto c : original)
            buffer[i++] = c;

         //::std::memcpy(buffer.data(), original.data(), original.size());
         //auto i = original.size();
         buffer[i++] = '<';
         i += NameSplitAndAppend<TArgs...>(buffer.data() + i);
         buffer[i++] = '>';
         return Token {buffer.data(), i};
      }

      template<class T>
      struct StatefulNameOf
      {
      private:
         using ClassName = ::std::array<char, 255/*IsolateTypename<T>().size()*/>;

         /// Gets the name of T, without the decorations                         
         /// This is an important step for consistent fundamental names, since   
         /// different compilers and architectures name types differently in     
         /// their function name macros                                          
         NOD() static constexpr ClassName ConstexprNameOf() noexcept {
            ClassName test {};
            constexpr Token constness = "const ";
            constexpr Token sparsenes = "*";
            size_t at = 0;

            // Write const                                                 
            if constexpr (CT::Constant<T>) {
               for (auto c : constness)
                  test[at++] = c;
            }

            // Write token                                                 
            if constexpr (CT::Same<T, ::std::int8_t>) {
               constexpr Token token = "int8";
               for (auto c : token)
                  test[at++] = c;
            }
            else if constexpr (CT::Same<T, ::std::int16_t>) {
               constexpr Token token = "int16";
               for (auto c : token)
                  test[at++] = c;
            }
            else if constexpr (CT::Same<T, ::std::int32_t>) {
               constexpr Token token = "int32";
               for (auto c : token)
                  test[at++] = c;
            }
            else if constexpr (CT::Same<T, ::std::int64_t>) {
               constexpr Token token = "int64";
               for (auto c : token)
                  test[at++] = c;
            }
            else if constexpr (CT::Same<T, ::std::uint8_t>) {
               constexpr Token token = "uint8";
               for (auto c : token)
                  test[at++] = c;
            }
            else if constexpr (CT::Same<T, ::std::uint16_t>) {
               constexpr Token token = "uint16";
               for (auto c : token)
                  test[at++] = c;
            }
            else if constexpr (CT::Same<T, ::std::uint32_t>) {
               constexpr Token token = "uint32";
               for (auto c : token)
                  test[at++] = c;
            }
            else if constexpr (CT::Same<T, ::std::uint64_t>) {
               constexpr Token token = "uint64";
               for (auto c : token)
                  test[at++] = c;
            }

            // Write pointer                                               
            if constexpr (CT::Sparse<T>) {
               for (auto c : sparsenes)
                  test[at++] = c;
            }

            test[at] = '\0';
            return test;
         }

         static constexpr ClassName NameBuffer = ConstexprNameOf();

      public:
         static constexpr Token Name = NameBuffer.data();
      };

      /// Gets the name of T, without the decorations                         
      /// This is an important step for consistent fundamental names, since   
      /// different compilers and architectures name types differently in     
      /// their function name macros                                          
      template<class T>
      NOD() constexpr Token NameOf(Overrider<T>&&) noexcept {
         return StatefulNameOf<T>::Name;
      }

      template<auto E>
      struct StatefulNameOfEnum
      {
      private:
         using ClassName = ::std::array<char, 255/*NameOfEstimate(Overrider<decltype(E)>{}) + IsolateConstant<E>().size() + 2*/>;
         static constexpr ClassName NameOfEnum() noexcept;
         static constexpr ClassName NameBuffer = NameOfEnum();

      public:
         static constexpr Token Name = NameBuffer.data();
      };

   } // namespace Inner


   /// Get the name of a type, templated or not, with consistently named      
   /// template arguments, even if nested, if such are required               
   /// Unfortunately, this can't be constexpr, because it may require name    
   /// substitution. But is as statically optimized as much as possible!      
   ///   @tparam T - the type to get the name of                              
   ///   @return the type name                                                
   template<class T>
   NOD() constexpr Token NameOf() noexcept {
      return Inner::NameOf(Inner::Overrider<T>{});
   }

   /// Get the last name of a type, templated or not, ignoring all namespaces 
   ///   @tparam T - the type to get the last name of                         
   ///   @return the type name                                                
   template<class T>
   NOD() constexpr Token LastNameOf() noexcept {
      auto result = Inner::StatefulNameOf<T>::Name;
      Inner::SkipAllNamespaces(result);
      return result;
   }

   /// Get the name of a named constant, or any constexpr value in fact       
   /// The type that owns the enum will be passed through the above NameOf    
   /// so that it will too have consistent template arguments if fundamental  
   ///   @tparam T - the constant to get the name of                          
   ///   @return the name of the constant                                     
   template<auto E>
   NOD() constexpr Token NameOf() noexcept {
      return Inner::StatefulNameOfEnum<E>::Name;
   }




   template<template<class...> class T, class... TArgs>
   NOD() constexpr Size Inner::NameOfEstimate(Overrider<T<TArgs...>>&&) noexcept {
      return Inner::IsolateTypename<T<TArgs...>>().size() + NameSplitAndEstimate<TArgs...>() + 3;
   }

   template<class T>
   NOD() constexpr Size Inner::NameOfEstimate(Overrider<T>&&) noexcept {
      return Inner::NameOf(Overrider<T>{}).size();
   }

   template<class HEAD, class... TAIL>
   NOD() constexpr Size Inner::NameSplitAndEstimate() noexcept {
      constexpr auto original = NameOfEstimate(Overrider<HEAD>{});
      if constexpr (sizeof...(TAIL) > 0)
         return original + 2 + NameSplitAndEstimate<TAIL...>();
      else
         return original;
   }

   template<class HEAD, class... TAIL>
   NOD() constexpr Size Inner::NameSplitAndAppend(char* buffer) noexcept {
      auto original = ::Langulus::RTTI::NameOf<HEAD>();
      if constexpr (sizeof...(TAIL) > 0) {
         size_t i = 0;
         for (; i < original.size(); ++i)
            buffer[i] = original[i];
         buffer[i] = ','; ++i;
         buffer[i] = ' '; ++i;
         return original.size() + 2 + NameSplitAndAppend<TAIL...>(buffer + i);
      }
      else {
         for (Offset i = 0; i < original.size(); ++i)
            buffer[i] = original[i];
         return original.size();
      }
   }

   /// Correctly handles templated types, but isn't constexpr due to static   
   /// string buffer, that is required for substitution of fundamental types  
   /// The owning class will be appended to the final enum name               
   ///   @tparam T - the type of the constant                                 
   ///   @tparam E - the constant                                             
   template<auto E>
   NOD() constexpr typename Inner::StatefulNameOfEnum<E>::ClassName Inner::StatefulNameOfEnum<E>::NameOfEnum() noexcept {
      using EnumType = decltype(E);
      constexpr auto base = NameOf(Inner::Overrider<EnumType>{});
      constexpr auto name = IsolateConstant<E>();

      // Corner case, where the splitter doesn't work                   
      /*#if LANGULUS(SAFE)
         const auto template_start = base.find_first_of('<', 0);
         if (template_start != base.npos)
            Logger::Warning() <<
            "NameOfEnum can't guarantee, that parent template arguments "
            "are consistently named fundamentals - if that's probable, "
            "you should move your enum outside the templated namespace: "
            "The constant that triggered this warning is: "
            << Logger::Red << base << "::" << name;
      #endif*/

      ClassName buffer {};

      size_t i = 0;
      for (auto c : base)
         buffer[i++] = c;

      //::std::memcpy(buffer.data(), base.data(), base.size());
      buffer[i++] = ':';
      buffer[i++] = ':';
      //buffer[base.size()] = buffer[base.size() + 1] = ':';
      //::std::memcpy(buffer.data() + base.size() + 2, name.data(), name.size());
      
      for (auto c : name)
         buffer[i++] = c;

      buffer[i++] = '\0';
      return buffer;
   }



} // namespace Langulus::RTTI
