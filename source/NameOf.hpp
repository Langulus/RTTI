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
         #error "No support for this compiler, \
            you should inspect the __FUNC__ equivalent for your compiler, \
            and fill in the Prefix and Suffix around the type substring \
            (see above)"
      #endif

      /// Pretty function used to wrap a type as a template argument          
      /// Once we wrap and stringify it, we can isolate the typename itself   
      ///   @tparam T - the type to get the name of                           
      template<class T>
      NOD() constexpr Token AsTemplateArgument() {
         return LANGULUS(FUNCTION);
      }

      /// Pretty function used to wrap a constexpr constant as a template     
      /// argument. Once we wrap and stringify it, we can isolate the name of 
      /// an enum, if it's a named constant                                   
      ///   @tparam T - the constant to get the name of                       
      template<auto T>
      NOD() constexpr Token AsTemplateArgument() {
         return LANGULUS(FUNCTION);
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
         if (str.size() >= suffix.size() && suffix.size() > 0 && str.ends_with(suffix))
            str.remove_suffix(suffix.size());
      }

      /// Skip all decorations in front of a type name                        
      ///   @return the modified constexpr string view                        
      template<class T>
      NOD() constexpr Token IsolateTypename() {
         auto original = AsTemplateArgument<T>();
         SkipPrefix(original, Prefix);
         SkipSuffix(original, Suffix);
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
         return original;
      }

      /// Skip these patterns when demangling names                           
      constexpr Token SkipPatterns[] {
         " ", "\t", "class", "struct", "enum", "Langulus::"
      };
      
      /// Replace these patterns when demangling names                        
      constexpr ::std::pair<Token, Token> ReplacePatterns[] {
         {"const",  "const "},

         {IsolateTypename<::std::int8_t>(),  "int8"},
         {IsolateTypename<::std::int16_t>(), "int16"},
         {IsolateTypename<::std::int32_t>(), "int32"},
         {IsolateTypename<::std::int64_t>(), "int64"},

         {IsolateTypename<::std::uint8_t>(),  "uint8"},
         {IsolateTypename<::std::uint16_t>(), "uint16"},
         {IsolateTypename<::std::uint32_t>(), "uint32"},
         {IsolateTypename<::std::uint64_t>(), "uint64"},
      };

      /// Inner structure, used to retain state computed at compile time      
      /// It's a workaround for the lack of static variables in constexpr     
      /// functions in c++20                                                  
      template<class T>
      struct StatefulNameOfType {
         static constexpr Token Original = IsolateTypename<T>();

         /// Do the skip/replace scan without writing to any buffer, in order 
         /// to anticipate the required buffer size                           
         ///   @attention this function should be in sync with Normalize()    
         ///   @return the required buffer size in bytes                      
         NOD() static constexpr ::std::size_t AnticipateSize() {
            ::std::size_t it = 0;
            Token remaining = Original;
            while (remaining.size() > 0) {
               bool recycle = false;

               // Do skips                                              
               for (auto skip : SkipPatterns) {
                  if (!remaining.starts_with(skip))
                     continue;

                  remaining.remove_prefix(skip.size());
                  recycle = true;
                  break;
               }

               if (recycle || remaining.size() == 0)
                  continue;
               
               // Do replacements                                       
               for (auto replace : ReplacePatterns) {
                  if (!remaining.starts_with(replace.first))
                     continue;

                  it += replace.second.size();
                  remaining.remove_prefix(replace.first.size());
                  recycle = true;
                  break;
               }

               if (recycle || remaining.size() == 0)
                  continue;

               // Push anything else                                    
               ++it;
               remaining.remove_prefix(1);
            }

            ++it;
            return it;
         }

         /// Do the skip/replace scan and return the demangled name           
         ///   @attention this function should be in sync with Anticipate()   
         ///   @return the normalized compile-time token for T                
         NOD() static constexpr auto Normalize() {
            ::std::size_t it = 0;
            ::std::array<char, AnticipateSize()> output {};

            Token remaining = Original;
            while (remaining.size() > 0) {
               bool recycle = false;

               // Do skips                                              
               for (auto skip : SkipPatterns) {
                  if (!remaining.starts_with(skip))
                     continue;

                  remaining.remove_prefix(skip.size());
                  recycle = true;
                  break;
               }

               if (recycle || remaining.size() == 0)
                  continue;
               
               // Do replacements                                       
               for (auto replace : ReplacePatterns) {
                  if (!remaining.starts_with(replace.first))
                     continue;

                  for (auto c : replace.second)
                     output[it++] = c;

                  remaining.remove_prefix(replace.first.size());
                  recycle = true;
                  break;
               }

               if (recycle || remaining.size() == 0)
                  continue;

               // Push anything else                                    
               output[it++] = remaining[0];
               remaining.remove_prefix(1);
            }

            // Last symbol might be ' ' due to the "const " replacement 
            // Make sure we remove it before returning                  
            if (it && output[it-1] == ' ')
               output[it-1] = '\0';
            else
               output[it] = '\0';
            return output;
         }

         static constexpr auto Name = Normalize();
      };

      /// Inner structure, used to retain state computed at compile time      
      /// It's a workaround for the lack of static variables in constexpr     
      /// functions in c++20                                                  
      template<auto E>
      struct StatefulNameOfEnum {
         static constexpr Token Original = IsolateConstant<E>();
         static constexpr Token EnumName = StatefulNameOfType<decltype(E)>::Name.data();

         /// Do the skip/replace scan without writing to any buffer, in order 
         /// to anticipate the required buffer size                           
         ///   @attention this function should be in sync with Normalize()    
         ///   @return the required buffer size in bytes                      
         NOD() static constexpr ::std::size_t AnticipateSize() {
            ::std::size_t it = 0;

            // Copy the normalized enum name                            
            it += EnumName.size();

            // And reappend the named value                             
            it += 2;

            constexpr auto nameStart = Original.find_last_of(':');
            if constexpr (nameStart != Original.npos) {
               auto name = Original;
               name.remove_prefix(nameStart + 1);
               it += name.size();
            }
            else LANGULUS_ERROR("Bad named value provided");

            ++it;
            return it;
         }

         /// Do the skip/replace scan and return the demangled name           
         ///   @attention this function should be in sync with Anticipate()   
         ///   @return the normalized compile-time token for E                
         NOD() static constexpr auto Normalize() {
            ::std::size_t it = 0;
            ::std::array<char, AnticipateSize()> output {};

            // Copy the normalized enum name                            
            for (auto c : EnumName)
               output[it++] = c;

            // And reappend the named value                             
            output[it++] = ':';
            output[it++] = ':';

            constexpr auto nameStart = Original.find_last_of(':');
            if constexpr (nameStart != Original.npos) {
               auto name = Original;
               name.remove_prefix(nameStart + 1);
               for (auto c : name)
                  output[it++] = c;
            }
            else LANGULUS_ERROR("Bad named value provided");

            output[it] = '\0';
            return output;
         }

         static constexpr auto Name = Normalize();
      };

   } // namespace Inner


   /// Get the name of a type, templated or not, with consistently named      
   /// template arguments, even if nested, if such are required               
   ///   @tparam T - the type to get the name of                              
   ///   @return the type name                                                
   template<class T>
   NOD() constexpr Token NameOf() noexcept {
      return Inner::StatefulNameOfType<T>::Name.data();
   }
   
   /// Same as NameOf, but removes all namespaces                             
   ///   @tparam T - the type to get the name of                              
   ///   @return the type name                                                
   template<class T>
   NOD() constexpr Token LastNameOf() noexcept {
      // Find the last ':' symbol, that is not inside <...> scope       
      Token name = Inner::StatefulNameOfType<T>::Name.data();
      size_t depth = 0;
      for (auto i = name.size(); i > 0; --i) {
         switch (name[i - 1]) {
         case ':':
            if (depth == 0) {
               name.remove_prefix(i);
               return name;
            }
            break;
         case '>':
            ++depth;
            break;
         case '<':
            --depth;
            break;
         default:
            break;
         }
      }

      // Something's not right if this was reached                      
      return "";
   }

   /// Get the name of a named constant                                       
   ///   @tparam E - the constant to get the name of                          
   ///   @return the name of the constant                                     
   template<auto E>
   NOD() constexpr Token NameOf() noexcept {
      return Inner::StatefulNameOfEnum<E>::Name.data();
   }

} // namespace Langulus::RTTI
