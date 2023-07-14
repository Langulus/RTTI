///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Config.hpp"
#include <Core/Utilities.hpp>
#include <string>
#include <array>

namespace Langulus::RTTI
{
   namespace Inner
   {
      
      /// These definitions might change in future compiler versions          
      /// and will probably need constant maintenance                         
      #if defined(__clang__)
         constexpr Token Prefix = "Langulus::Token Langulus::RTTI::Inner::IsolateTypename() [T = ";
         constexpr Token Suffix = "]";
      #elif defined(__GNUC__) && !defined(__clang__)
         constexpr Token Prefix = "constexpr Langulus::Token Langulus::RTTI::Inner::IsolateTypename() [with T = ";
         constexpr Token Suffix = "; Langulus::Token = std::basic_string_view<char>]";
      #elif defined(_MSC_VER)
         constexpr Token Prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Langulus::RTTI::Inner::IsolateTypename<";
         constexpr Token Suffix = ">(void)";
      #else
         #error "No support for this compiler, \
            you should inspect the __FUNC__ equivalent for your compiler, \
            and fill in the Prefix and Suffix around the type substring \
            (see above)"
      #endif

      /// Skip these patterns when demangling names                           
      constexpr Token SkipPatterns[] {
         "class", "struct", "enum", "Langulus::"
      };

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
         Token original = LANGULUS(FUNCTION);
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
         Token original = LANGULUS(FUNCTION);
         SkipPrefix(original, Prefix);
         SkipSuffix(original, Suffix);
         return original;
      }

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

      /// Check if a token transition happens at the given offsets            
      ///   @param source - what are we checking                              
      ///   @param remaining - part of source that is potentially transitive  
      ///   @param s - the offset in remaining to check if transitive         
      ///   @return true if a transition occurs at the given offset           
      NOD() constexpr bool IsTransition(const Token& source, const Token& remaining, Count s) {
         return (
               remaining.data() == source.data()
               || IsAlpha(*(remaining.data()))
               != IsAlpha(*(remaining.data() - 1))
            ) && (
               remaining.data() + s == source.data() + source.size()
               || IsAlpha(*(remaining.data() + s))
               != IsAlpha(*(remaining.data() + s - 1))
            );
      }

      /// Perform a potential skip                                            
      ///   @param remaining - [in/out] the remaining token                   
      ///   @param recycle - [out]                                            
      constexpr void Skip(const Token& source, Token& remaining, bool& recycle) {
         while (remaining.size() > 0 && IsSpace(remaining[0])) {
            remaining.remove_prefix(1);
            recycle = true;
         }

         for (auto skip : SkipPatterns) {
            if (!remaining.starts_with(skip))
               continue;

            // Skip the symbol only if it is at the border of an        
            // alphabetical sequence, or at the edge of Original        
            if (IsTransition(source, remaining, skip.size())) {
               remaining.remove_prefix(skip.size());
               recycle = true;
               break;
            }
         }
      }

      /// Do the skip/replace scan without writing to any buffer, in order    
      /// to anticipate the required buffer size                              
      ///   @attention this function should be in sync with Normalize()       
      ///   @return the required buffer size in bytes                         
      NOD() constexpr ::std::size_t AnticipateSizeOfType(const Token& source) {
         ::std::size_t it = 0;

         Token remaining = source;
         while (remaining.size() > 0) {
            bool recycle = false;

            // Do skips                                                 
            Skip(source, remaining, recycle);
            if (recycle || remaining.size() == 0)
               continue;

            // Do replacements                                          
            for (auto replace : ReplacePatterns) {
               if (!remaining.starts_with(replace.first))
                  continue;

               if (IsTransition(source, remaining, replace.first.size())) {
                  it += replace.second.size();
                  remaining.remove_prefix(replace.first.size());
                  recycle = true;
                  break;
               }
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

      /// Do the skip/replace scan without writing to any buffer, in order    
      /// to anticipate the required buffer size                              
      ///   @attention this function should be in sync with Normalize()       
      ///   @return the required buffer size in bytes                         
      NOD() constexpr ::std::size_t AnticipateSizeOfEnum(const Token& source, const Token& enumName) {
         ::std::size_t it = 0;

         // Copy the normalized enum name                               
         it += enumName.size();

         // And reappend the named value                                
         it += 2;

         const auto nameStart = source.find_last_of(':');
         auto name = source;
         name.remove_prefix(nameStart + 1);
         it += name.size();
         return ++it;
      }

      /// Inner structure, used to retain state computed at compile time      
      /// It's a workaround for the lack of static variables in constexpr     
      /// functions in c++20                                                  
      template<class T>
      struct StatefulNameOfType {
         /// Do the skip/replace scan and return the demangled name           
         ///   @attention this function should be in sync with Anticipate()   
         ///   @return the normalized compile-time token for T                
         NOD() static constexpr auto Normalize() {
            constexpr Token Original = IsolateTypename<T>();
            ::std::size_t it = 0;
            ::std::array<char, AnticipateSizeOfType(Original)> output {};

            Token remaining = Original;
            while (remaining.size() > 0) {
               bool recycle = false;

               // Do skips                                              
               Skip(Original, remaining, recycle);
               if (recycle || remaining.size() == 0)
                  continue;
               
               // Do replacements                                       
               for (auto replace : ReplacePatterns) {
                  if (!remaining.starts_with(replace.first))
                     continue;

                  if (IsTransition(Original, remaining, replace.first.size())) {
                     for (auto c : replace.second)
                        output[it++] = c;

                     remaining.remove_prefix(replace.first.size());
                     recycle = true;
                     break;
                  }
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
         /// Do the skip/replace scan and return the demangled name           
         ///   @attention this function should be in sync with Anticipate()   
         ///   @return the normalized compile-time token for E                
         NOD() static constexpr auto Normalize() {
            constexpr Token Original = IsolateConstant<E>();
            constexpr Token EnumName = StatefulNameOfType<decltype(E)>::Name.data();

            ::std::size_t it = 0;
            ::std::array<char, AnticipateSizeOfEnum(Original, EnumName)> output {};

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
   NOD() constexpr Token CppNameOf() noexcept {
      return Inner::StatefulNameOfType<T>::Name.data();
   }
   
   /// Same as NameOf, but removes all namespaces                             
   ///   @tparam T - the type to get the name of                              
   ///   @return the type name                                                
   template<class T>
   NOD() constexpr Token LastCppNameOf() noexcept {
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
   NOD() constexpr Token CppNameOf() noexcept {
      return Inner::StatefulNameOfEnum<E>::Name.data();
   }

} // namespace Langulus::RTTI

namespace Langulus
{

   ///                                                                        
   ///   NameOf that also considers LANGULUS(NAME) reflection, if any         
   ///                                                                        
   template<CT::Data T>
   constexpr Token NameOf() noexcept {
      using DT = Decay<T>;
      if constexpr (CT::Decayed<T> && requires { DT::CTTI_Name; })
         return DT::CTTI_Name;
      else
         return RTTI::CppNameOf<T>();
   }
   
   ///                                                                        
   ///   NameOf for enum types                                                
   ///                                                                        
   template<auto E>
   constexpr Token NameOf() noexcept {
      return RTTI::CppNameOf<E>();
   }

   template<class CLASS>
   struct Of {};

   ///                                                                        
   /// A constexpr custom name generator used in various places               
   /// You have to define CustomName(Of<your type>)                           
   ///                                                                        
   template<class CLASS>
   constexpr auto CustomName(Of<CLASS>&&) noexcept;

   template<class CLASS>
   struct CustomNameOf {
   private:
      // This intermediate step is required, so that we don't get:      
      // warning : object backing the pointer will be destroyed at the  
      //           end of the full-expression [-Wdangling-gsl]          
      static constexpr auto GeneratedClassName = CustomName(Of<CLASS> {});

   public:
      static constexpr Token Generate() noexcept {
         return Token {GeneratedClassName.data()};
      }
   };

} // namespace Langulus
