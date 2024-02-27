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
#include <Core/Utilities.hpp>
#include <string>
#include <array>


namespace Langulus::RTTI
{
   namespace Inner
   {

      /// String length at compile-time                                       
      consteval int LengthOf(const char* str) {
         int n = 0;
         while (*str++) n++;
         return n;
      }

      /// String match at compile-time                                        
      consteval bool Match(const char* str1, const char* what) {
         while (*str1 and *what and *(str1++) == *(what++));
         return *what == 0;
      }

      template<class T>
      consteval auto WrappedTypeName() {
         return LANGULUS_FUNCTION();
      }

      template<auto T>
      consteval auto WrappedEnumName() {
         return LANGULUS_FUNCTION();
      }

      /// Skip all decorations in front and the back of a WrappedTypeName     
      ///   @return the type token                                            
      template<class T>
      consteval Token IsolateTypename() {
         enum Oddly_Specific_Type {};
         constexpr auto name = WrappedTypeName<T>();
         constexpr auto len = LengthOf(name);
         constexpr auto helper_name = WrappedTypeName<Oddly_Specific_Type>();
         constexpr auto helper_len = LengthOf(helper_name);
         static_assert(len > 0);
         static_assert(helper_len > 19);

         int left = 0;
         while (left < helper_len and left < len and helper_name[left] == name[left])
            left++;

         int right = 1;
         while (right + 19 <= helper_len and right <= len
         and helper_name[helper_len - right] == name[len - right]
         and not Match(helper_name + (helper_len - right - 19), "Oddly_Specific_Type"))
            right++;

         if (len - right <= left)
            throw "invalid token";
         return Token {name + left, name + len - right};
      }

      /// Skip all decorations in front and back of a WrappedEnumName         
      ///   @return the named value token                                     
      template<auto T>
      consteval Token IsolateConstant() {
         enum { Oddly_Specific_Enum };
         constexpr auto name = WrappedEnumName<T>();
         constexpr auto len = LengthOf(name);
         constexpr auto helper_name = WrappedEnumName<Oddly_Specific_Enum>();
         constexpr auto helper_len = LengthOf(helper_name);
         static_assert(len > 0);
         static_assert(helper_len > 19);

         int left = 0;
         while (left < helper_len and left < len and helper_name[left] == name[left])
            left++;

         int right = 1;
         while (right + 19 <= helper_len and right <= len
         and helper_name[helper_len - right] == name[len - right]
         and not Match(helper_name + (helper_len - right - 19), "Oddly_Specific_Enum"))
            right++;

         if (len - right <= left)
            throw "invalid token";
         return Token {name + left, name + len - right};
      }

      /// Replace these patterns when demangling names                        
      ///   @attention replacing will cease after the first match, so when    
      ///              having similar tokens to replace, order them correctly 
      ///              so that the most complex ones come first               
      ///   @attention replacement will not commence, if IsTransition doesn't 
      ///              pass                                                   
      constexpr ::std::pair<Token, Token> ReplacePatterns[] {
         {"*const ",  "* const"},
         {" *const",  "* const"},
         {" *",  "*"},
         {" &",  "&"},
         {" >",  ">"},

         {IsolateTypename<::std::int8_t>(),  "int8"},
         {IsolateTypename<::std::int16_t>(), "int16"},
         {IsolateTypename<::std::int32_t>(), "int32"},
         {IsolateTypename<::std::int64_t>(), "int64"},

         {IsolateTypename<::std::uint8_t>(),  "uint8"},
         {IsolateTypename<::std::uint16_t>(), "uint16"},
         {IsolateTypename<::std::uint32_t>(), "uint32"},
         {IsolateTypename<::std::uint64_t>(), "uint64"},

         {"class ", ""},
         {"struct ", ""},
         {"enum ", ""},
         {"Langulus::", ""},

         {"(__cdecl *)", ""},
         {" (*)", ""}
      };

      /// Check if a token transition happens at the given offsets            
      ///   @param source - what are we checking                              
      ///   @param remaining - part of source that is potentially transitive  
      ///   @param s - the offset in remaining to check if transitive         
      ///   @return true if a transition occurs at the given offset           
      NOD() consteval bool IsTransition(const Token& source, const Token& remaining, Count s) {
         return (
               remaining.data() == source.data()
               or     IsAlpha(*(remaining.data()))
                  !=  IsAlpha(*(remaining.data() - 1))
               or not IsAlpha(*(remaining.data()))
            ) and (
               remaining.data() + s == source.data() + source.size()
               or     IsAlpha(*(remaining.data() + s))
                  !=  IsAlpha(*(remaining.data() + s - 1))
               or not IsAlpha(*(remaining.data() + s - 1))
            );
      }

      /// Do the skip/replace scan without writing to any buffer, in order    
      /// to anticipate the required buffer size                              
      ///   @attention this function should be in sync with                   
      ///              StatefulNameOfFunc::Normalize()                        
      ///   @return the required buffer size in bytes                         
      NOD() consteval ::std::size_t AnticipateSizeOfFunc(const Token& source) {
         ::std::size_t it = 9; // size of "Function<" prefix            
         Token remaining = source;
         while (remaining.size() > 0) {
            bool recycle = false;

            // Do replacements                                          
            for (auto& replace : ReplacePatterns) {
               if (not remaining.starts_with(replace.first))
                  continue;

               if (IsTransition(source, remaining, replace.first.size())) {
                  it += replace.second.size();
                  remaining.remove_prefix(replace.first.size());
                  recycle = true;
                  break;
               }
            }

            if (recycle or remaining.size() == 0)
               continue;

            // Push anything else                                       
            ++it;
            remaining.remove_prefix(1);
         }

         return it + 3; // append ">*\0"                                
      }
      
      /// Do the skip/replace scan without writing to any buffer, in order    
      /// to anticipate the required buffer size                              
      ///   @attention this function should be in sync with                   
      ///              StatefulNameOfType::Normalize()                        
      ///   @return the required buffer size in bytes                         
      NOD() consteval ::std::size_t AnticipateSizeOfType(const Token& source) {
         ::std::size_t it = 0;
         Token remaining = source;
         while (remaining.size() > 0) {
            bool recycle = false;

            // Do replacements                                          
            for (auto& replace : ReplacePatterns) {
               if (not remaining.starts_with(replace.first))
                  continue;

               if (IsTransition(source, remaining, replace.first.size())) {
                  it += replace.second.size();
                  remaining.remove_prefix(replace.first.size());
                  recycle = true;
                  break;
               }
            }

            if (recycle or remaining.size() == 0)
               continue;

            // Push anything else                                       
            ++it;
            remaining.remove_prefix(1);
         }

         return it + 1; // append \0                                    
      }

      /// Do the skip/replace scan without writing to any buffer, in order    
      /// to anticipate the required buffer size                              
      ///   @attention this function should be in sync with                   
      ///              StatefulNameOfEnum::Normalize()                        
      ///   @return the required buffer size in bytes                         
      NOD() consteval ::std::size_t AnticipateSizeOfEnum(const Token& source, const Token& enumName) {
         ::std::size_t it = 0;

         // Copy the normalized enum name                               
         it += enumName.size();

         // And reappend the named value                                
         it += 2;

         const auto nameStart = source.find_last_of(':');
         auto name = source;
         name.remove_prefix(nameStart + 1);
         return it + name.size() + 1; // append \0                      
      }


      /// Inner structure, used to retain state computed at compile time      
      /// It's a workaround for the lack of static variables in constexpr     
      /// functions in c++20                                                  
      template<class T>
      struct StatefulNameOfFunc {
         /// Do the skip/replace scan and return the demangled name           
         ///   @attention this function should be in sync with                
         ///              AnticipateSizeOfFunc()                              
         ///   @return the normalized compile-time token for T                
         NOD() static consteval auto Normalize() {
            constexpr Token Original = IsolateTypename<T>();
            ::std::size_t it = 0;
            ::std::array<char, AnticipateSizeOfFunc(Original)> output {};

            for (auto c : "Function<")
               output[it++] = c;
            --it;

            Token remaining = Original;
            while (remaining.size() > 0) {
               bool recycle = false;
               
               // Do replacements                                       
               for (auto replace : ReplacePatterns) {
                  if (not remaining.starts_with(replace.first))
                     continue;

                  if (IsTransition(Original, remaining, replace.first.size())) {
                     for (auto c : replace.second)
                        output[it++] = c;

                     remaining.remove_prefix(replace.first.size());
                     recycle = true;
                     break;
                  }
               }

               if (recycle or remaining.size() == 0)
                  continue;

               // Push anything else                                    
               output[it++] = remaining[0];
               remaining.remove_prefix(1);
            }

            output[it++] = '>';
            output[it++] = '*';
            output[it] = '\0';
            if (it >= output.size())
               throw "Limit breached";
            return output;
         }

         static constexpr auto Name = Normalize();
      };
      

      /// Inner structure, used to retain state computed at compile time      
      /// It's a workaround for the lack of static variables in constexpr     
      /// functions in c++20                                                  
      template<class T>
      struct StatefulNameOfType {
         /// Do the skip/replace scan and return the demangled name           
         ///   @attention this function should be in sync with                
         ///              AnticipateSizeOfType()                              
         ///   @return the normalized compile-time token for T                
         NOD() static consteval auto Normalize() {
            constexpr Token Original = IsolateTypename<T>();
            ::std::size_t it = 0;
            ::std::array<char, AnticipateSizeOfType(Original)> output {};

            Token remaining = Original;
            while (remaining.size() > 0) {
               bool recycle = false;
               
               // Do replacements                                       
               for (auto replace : ReplacePatterns) {
                  if (not remaining.starts_with(replace.first))
                     continue;

                  if (IsTransition(Original, remaining, replace.first.size())) {
                     for (auto c : replace.second)
                        output[it++] = c;

                     remaining.remove_prefix(replace.first.size());
                     recycle = true;
                     break;
                  }
               }

               if (recycle or remaining.size() == 0)
                  continue;

               // Push anything else                                    
               output[it++] = remaining[0];
               remaining.remove_prefix(1);
            }

            // Last symbol might be ' ' due to the "const " replacement 
            // Make sure we remove it before returning                  
            if (it and output[it-1] == ' ')
               output[it-1] = '\0';
            else
               output[it] = '\0';
            if (it >= output.size())
               throw "Limit breached";
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
         ///   @attention this function should be in sync with                
         ///              AnticipateSizeOfEnum()                              
         ///   @return the normalized compile-time token for E                
         NOD() static consteval auto Normalize() {
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
            if (it >= output.size())
               throw "Limit breached";
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
   NOD() consteval Token CppNameOf() noexcept {
      if constexpr (::std::is_function_v<Decay<T>>)
         return Inner::StatefulNameOfFunc<T>::Name.data();
      else
         return Inner::StatefulNameOfType<T>::Name.data();
   }
   
   /// Same as NameOf, but removes all namespaces                             
   ///   @tparam T - the type to get the name of                              
   ///   @return the type name                                                
   template<class T>
   NOD() consteval Token LastCppNameOf() noexcept {
      if constexpr (::std::is_function_v<Decay<T>>)
         return Inner::StatefulNameOfFunc<T>::Name.data();
      else {
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

         throw "Invalid token";
      }
   }

   /// Get the name of a named constant                                       
   ///   @tparam E - the constant to get the name of                          
   ///   @return the name of the constant                                     
   template<auto E>
   NOD() consteval Token CppNameOf() noexcept {
      return Inner::StatefulNameOfEnum<E>::Name.data();
   }
   
   /// Same as CppNameOf, but removes all namespaces                          
   ///   @tparam T - the enum to get the name of                              
   ///   @return the name                                                     
   template<auto E>
   NOD() consteval Token LastCppNameOf() noexcept {
      // Find the last ':' symbol, that is not inside <...> scope       
      Token name = Inner::StatefulNameOfEnum<E>::Name.data();
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

      throw "Invalid token";
   }

} // namespace Langulus::RTTI

namespace Langulus
{
   
   template<class CLASS>
   struct Of {};

   ///                                                                        
   /// A constexpr custom name generator used in various places               
   /// You have to define CustomName(Of<your type>)                           
   ///                                                                        
   template<class CLASS>
   consteval auto CustomName(Of<CLASS>&&) noexcept;

   template<class CLASS>
   struct CustomNameOf {
   private:
      // This intermediate step is required, so that we don't get:      
      // warning : object backing the pointer will be destroyed at the  
      //           end of the full-expression [-Wdangling-gsl]          
      static constexpr auto GeneratedClassName = CustomName(Of<CLASS> {});

   public:
      static consteval Token Generate() noexcept {
         return Token {GeneratedClassName.data()};
      }
   };

   template<CT::Data T>
   consteval Token NameOf() noexcept;
   template<auto E>
   consteval Token NameOf() noexcept;

   /// Custom name generator at compile-time for sparse stuff                 
   template<CT::NotDecayed T>
   consteval auto CustomName(Of<T>&&) noexcept {
      if constexpr (CT::Sparse<T>) {
         // Get the depointered name, and append a pointer to it        
         constexpr auto token = NameOf<Deptr<T>>();
         ::std::array<char, token.size() + 2> tokenPtr;
         ::std::size_t idx {};

         for (auto i : token)
            tokenPtr[idx++] = i;
         tokenPtr[idx++] = '*';
         tokenPtr[idx] = '\0';
         if (idx >= tokenPtr.size())
            throw "Limit breached";
         return tokenPtr;
      }
      else {
         // Get the unqualified name, and append const to it            
         constexpr auto token = NameOf<Decvq<T>>();
         ::std::array<char, token.size() + 7> constToken;
         ::std::size_t idx {};

         if constexpr (CT::Constant<T>) {
            for (auto i : "const ")
               constToken[idx++] = i;
            --idx;
         }

         for (auto i : token)
            constToken[idx++] = i;
         constToken[idx] = '\0';
         if (idx >= constToken.size())
            throw "Limit breached";
         return constToken;
      }
   }


   ///                                                                        
   ///   NameOf that considers the following name reflections, if any,        
   ///   in the specified order:                                              
   ///      LANGULUS(POSITIVE_VERB)                                           
   ///      LANGULUS(VERB)                                                    
   ///      LANGULUS(TRAIT)                                                   
   ///      LANGULUS(NAME)                                                    
   ///      or fallbacks to the C++ name                                      
   ///                                                                        
   template<CT::Data T>
   consteval Token NameOf() noexcept {
      using DT = Decay<T>;
      if constexpr (requires { DT::CTTI_PositiveVerb; }) {
         if constexpr (CT::Decayed<Deref<T>>)
            return DT::CTTI_PositiveVerb;
         else
            return CustomNameOf<Deref<T>>::Generate();
      }
      else if constexpr (requires { DT::CTTI_Verb; }) {
         if constexpr (CT::Decayed<Deref<T>>)
            return DT::CTTI_Verb;
         else
            return CustomNameOf<Deref<T>>::Generate();
      }
      else if constexpr (requires { DT::CTTI_Trait; }) {
         if constexpr (CT::Decayed<Deref<T>>)
            return DT::CTTI_Trait;
         else
            return CustomNameOf<Deref<T>>::Generate();
      }
      else if constexpr (requires { DT::CTTI_Name; }) {
         if constexpr (CT::Decayed<Deref<T>>)
            return DT::CTTI_Name;
         else
            return CustomNameOf<Deref<T>>::Generate();
      }
      else return RTTI::CppNameOf<Deref<T>>();
   }
   
   ///                                                                        
   ///   NameOf for enum types                                                
   ///                                                                        
   template<auto E>
   consteval Token NameOf() noexcept {
      return RTTI::CppNameOf<E>();
   }

} // namespace Langulus
