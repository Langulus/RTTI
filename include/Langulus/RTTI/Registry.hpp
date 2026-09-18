///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Definition.hpp"
#include <unordered_set>

#if not LANGULUS_FEATURE(MANAGED_REFLECTION)
   #error "This file shouldn't be included if MANAGED_REFLECTION is disabled"
#endif

namespace Langulus::RTTI
{
   struct MetaException : Exception {
      using Exception::Exception;
   };

   using MetaSet = ::std::unordered_set<Inner::Definition const*>;

   ///                                                                        
   ///   The RTTI registry                                                    
   ///                                                                        
   /// Available only if managed reflection feature is enabled                
   class Registry {
      friend class DefinitionVerb;
      friend class DefinitionData;
      friend class DefinitionTag;
      friend class DefinitionConst;
      friend struct Inner::MetaTagPacked_16;
      friend struct Inner::MetaConstPacked_16;
      template<unsigned, unsigned> friend struct Inner::MetaDataStructured_XY;
      template<unsigned>           friend struct Inner::MetaVerbStructured_X8;

      LANGULUS_API(RTTI)
      static auto RegisterData(Token const& cppname, Token const& token) -> DefinitionData&;
      LANGULUS_API(RTTI)
      static auto ReserveDataID(DefinitionData const*) -> size_t;
      LANGULUS_API(RTTI)
      static auto RegisterConst(Token const& cppname, Token const& token) -> DefinitionConst&;
      LANGULUS_API(RTTI)
      static auto RegisterTag(Token const& cppname, Token const& token) -> DefinitionTag&;
      LANGULUS_API(RTTI)
      static auto RegisterVerb(
         Token const& cppname,
         Token const& token,
         Token const& tokenRev,
         Token const& op,
         Token const& opRev
      ) -> DefinitionVerb&;
      
      LANGULUS_API(RTTI)
      static void RegisterFileExtension(Token const&, DefinitionData*) assumptious;

      LANGULUS_API(RTTI)
      static auto GetMetaDataByCppName(Token const&) noexcept -> DefinitionData const*;
      LANGULUS_API(RTTI)
      static auto GetMetaTagByCppName(Token const&) noexcept -> DefinitionTag const*;
      LANGULUS_API(RTTI)
      static auto GetMetaVerbByCppName(Token const&) noexcept -> DefinitionVerb const*;
      LANGULUS_API(RTTI)
      static auto GetMetaConstByCppName(Token const&) noexcept -> DefinitionConst const*;

      LANGULUS_API(RTTI)
      static auto GetMetaDataByID(size_t, bool sparse, bool constant) assumptious-> DefinitionData const*;
      LANGULUS_API(RTTI)
      static auto GetMetaTagByID(size_t) assumptious-> DefinitionTag const*;
      LANGULUS_API(RTTI)
      static auto GetMetaVerbByID(size_t) assumptious-> DefinitionVerb const*;
      LANGULUS_API(RTTI)
      static auto GetMetaConstByID(size_t) assumptious-> DefinitionConst const*;

   public:
      LANGULUS_API(RTTI)
      static auto GetMetaDataByToken (Token const&) assumptious -> DefinitionData const*;
      LANGULUS_API(RTTI)
      static auto GetMetaTagByToken  (Token const&) assumptious -> DefinitionTag const*;
      LANGULUS_API(RTTI)
      static auto GetMetaVerbByToken (Token const&) assumptious -> DefinitionVerb const*;
      LANGULUS_API(RTTI)
      static auto GetMetaConstByToken(Token const&) assumptious -> DefinitionConst const*;

      LANGULUS_API(RTTI)
      static auto GetAmbiguousMeta(Token const&) assumptious -> const MetaSet&;
      LANGULUS_API(RTTI)
      static auto DisambiguateMeta(Token const&) -> Inner::Definition const*;
      LANGULUS_API(RTTI)
      static auto ResolveFileExtension(Token const&) assumptious -> const MetaSet&;
      LANGULUS_API(RTTI)
      static void UnloadBoundary(Token const&);
   };
}
