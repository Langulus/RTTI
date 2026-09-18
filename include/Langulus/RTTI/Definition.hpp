///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include <Langulus/HashOf.hpp>
#include <Langulus/NameOf.hpp>
#include <Langulus/CT/Info.hpp>
#include <Langulus/CT/Versioned.hpp>

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   #include <unordered_set>
   #include "Boundary.hpp"
#endif

#define LANGULUS_META_VERBOSITY_MASTER_SWITCH() 0

namespace Langulus::RTTI
{
   class DefinitionConst;
   class DefinitionData;
   class DefinitionTag;
   class DefinitionVerb;
   class Registry;
      
   namespace Inner
   {
      struct MetaDataNaked;
      template<unsigned, unsigned>
      struct MetaDataStructured_XY;

      struct MetaTagNaked;
      struct MetaTagPacked_16;

      struct MetaConstNaked;
      struct MetaConstPacked_16;

      struct MetaVerbNaked;
      template<unsigned>
      struct MetaVerbStructured_X8;
   }   

   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      using MetaConst = Inner::MetaConstPacked_16;
      using MetaData = Inner::MetaDataStructured_XY<2, 2>;
      using MetaTag = Inner::MetaTagPacked_16;
      using MetaVerb = Inner::MetaVerbStructured_X8<1>;
   #else
      using MetaConst = Inner::MetaConstNaked;
      using MetaData = Inner::MetaDataNaked;
      using MetaTag = Inner::MetaTagNaked;
      using MetaVerb = Inner::MetaVerbNaked;
   #endif

   using CMeta = MetaConst;
   using DMeta = MetaData;
   using TMeta = MetaTag;
   using VMeta = MetaVerb;
}

namespace Langulus::RTTI::Inner
{
   /// Convert a token to a lowercase string in the most portable way possible
   ///   @attention assumes token is ASCII                                    
   ///   @param token the token to lowercase                                  
   ///   @return the lowercase string                                         
   constexpr Lowercase ToLowercase(const Token& token) assumptious {
      LglsAssumeDev(IsASCII(token), "Token must be ASCII");
      Lowercase lc {token};
      for (char& c : lc)
         c = Langulus::ToLowercase(c);
      return lc;
   }

   /// Operators are often defined with proper spaces around them for prettier
   /// logging. This makes sure they are stripped before searching in registry
   ///   @attention assumes token is ASCII                                    
   ///   @param token the operator                                            
   ///   @return the isolated operator token                                  
   constexpr Token StripSpaces(const Token& token) assumptious {
      auto l = token.data();
      auto r = token.data() + token.size();
      while (l < r and     *l <= 32)   ++l;
      while (r > l and *(r-1) <= 32)   --r;
      return token.substr(l - token.data(), r - l);
   }
      
   /// Get the last, most relevant part of a token that may or may not have   
   /// namespaces in it. Essentially finds last "::" that isn't enclosed in   
   /// a template <>, and skip forward to that                                
   ///   @param token the token to scan                                       
   ///   @return the last token                                               
   constexpr Token ToLastToken(const Token& token) noexcept {
      size_t depth = 0;
      for (size_t i = token.size() - 1; i < token.size(); --i) {
         switch (token[i]) {
         case ':':
            // If no depth, then we found it                            
            if (not depth)
               return token.substr(i + 1, token.size() - i - 1);
            break;
         case '>':
            // Open template scope                                      
            ++depth;
            break;
         case '<':
            // Close template scope                                     
            if (depth)
               --depth;
            break;
         default:
            break;
         }
      }
      return token;
   }

   ///                                                                        
   ///   Abstract definition                                                  
   ///                                                                        
   class Definition {
   protected:
      friend class RTTI::Registry;
      template<class T>
      friend struct MetaNaked;
      
      // Each reflected type has a unique hash based on C++ name        
      const Hash mHash;

      // @attention we can't afford strings to be pointers to static    
      //    data to avoid data behind them getting unloaded on a shared 
      //    object unload                                               
      // Original name of the type as it appears in C++                 
      const ::std::string mCppNameOf;
      // Sanitized mToken with proper capitalization, used in scripts   
      ::std::string mNameOf;
      // Each reflection may or may not have some info                  
      ::std::string mInfoOf;

      // Major version                                                  
      unsigned mVersionMajor;
      // Minor version                                                  
      unsigned mVersionMinor;

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Precomputed lowercase nameof                                
         Lowercase mNameOfLowercased;

         // A sequential identifier provided by the registry            
         // Used for packing type ids                                   
         size_t mID IF_SAFE(= 0);

         // Populated from LANGULUS_BOUNDARY on reflection-time         
         // Types can be reflected from the point of view of different  
         // shared libraries. Each new reflection will be applied on the
         // top of the old one, but overwriting properties only if the  
         // changes come from the MainBoundary. Once mBoundary becomes  
         // the MainBoundary, the definition shall never be unregistered
         public: using BoundarySet = ::std::unordered_set<::std::string>;
         protected: BoundarySet mBoundaries;
      #endif

      /// Construct an abstract definition                                    
      ///   @param cppname the C++ name of the definition                     
      Definition(const Token& cppname)
         : mHash      {HashOf(cppname)}
         , mCppNameOf {cppname} {}

      /// Reflect some common type properties, like info and version          
      ///   @attention must always be inline, so that boundary is relative    
      ///   @attention call this first, so that version is checked before any 
      ///      other changes are made to the type                             
      ///   @tparam T the type to reflect                                     
      template<class T> LANGULUS(ALWAYS_INLINED)
      void ReflectCommon() {
         // Reflected version                                           
         auto v = VersionOf<T>();
         mVersionMajor = v.Major;
         mVersionMinor = v.Minor;
         
         // Save the boundary at time of reflection, but don't even     
         // bother if it is the main one                                
         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            mBoundaries.insert(LglsBoundary());
         #endif

         // Reflected info                                              
         mInfoOf = InfoOf<T>();
      }

      /// Check whether the definition is in the current boundary, or has     
      /// been reflected from the main one                                    
      ///   @attention must always be inline, so that boundary is relative    
      LANGULUS(ALWAYS_INLINED)
      bool IsInRelevantBoundary() const noexcept {
         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            return mBoundaries.contains(LglsBoundary());
         #else
            return true;
         #endif
      }

   public:
      using CTTI_ReflectAs = void;

      Definition() = delete;
      virtual ~Definition() = default;
   };
}
