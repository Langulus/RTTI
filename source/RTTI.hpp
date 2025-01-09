///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Hashing.hpp"
#include <unordered_map>
#include <unordered_set>

namespace Langulus::RTTI
{

   using MetaList = ::std::unordered_set<AMeta>;


   ///                                                                        
   ///   The RTTI registry                                                    
   ///                                                                        
   /// Available only if managed reflection feature is enabled                
   ///                                                                        
   class Registry {
   private:
      template<class T>
      using BoundedMeta = ::std::unordered_map<Token, T>;
      template<class T>
      using MetaMap = ::std::unordered_map<Lowercase, BoundedMeta<T>>;

      // Database for meta data definitions                             
      MetaMap<DMeta> mMetaData;
      // Database for named values                                      
      MetaMap<CMeta> mMetaConstants;
      // Database for meta trait definitions                            
      MetaMap<TMeta> mMetaTraits;
      // Database for meta verb definitions                             
      MetaMap<VMeta> mMetaVerbs;

      // Verbs, mapped to their original C++ class name                 
      MetaMap<VMeta> mUniqueVerbs;
      // Database for verb definitions indexed by operator token        
      MetaMap<VMeta> mOperators;
      // Database for ambiguous tokens                                  
      MetaMap<MetaList> mMetaAmbiguous;
      // Meta data definitions, indexed by file extensions              
      MetaMap<MetaList> mFileDatabase;

      void RegisterAmbiguous(const Token&, const Lowercase&, AMeta) noexcept;
      void UnregisterAmbiguous(const Token&, const Lowercase&, AMeta) noexcept;
      auto GetMeta(const auto&, const Token&, const Token&) const noexcept;
      auto GetMetaList(const auto&, const Token&, const Token&) const noexcept -> const MetaList&;

      template<bool REGISTER_AMBIGUOUS = true>
      auto Register(auto, auto&, const Lowercase&, const Token&) IF_UNSAFE(noexcept);

   public:
      LANGULUS_API(RTTI)
      DMeta RegisterData(const Token&, const Token&);

      LANGULUS_API(RTTI)
      CMeta RegisterConstant(const Token&, const Token&);

      LANGULUS_API(RTTI)
      TMeta RegisterTrait(const Token&, const Token&);

      LANGULUS_API(RTTI)
      VMeta RegisterVerb(const Token&, const Token&, const Token&, const Token&, const Token&, const Token&);
      
      LANGULUS_API(RTTI)
      void RegisterFileExtension(const Token&, DMeta, const Token&) IF_UNSAFE(noexcept);

   public:
      ~Registry();

      LANGULUS_API(RTTI)
      DMeta GetMetaData(const Token&, const Token& = "") const noexcept;

      LANGULUS_API(RTTI)
      TMeta GetMetaTrait(const Token&, const Token& = "") const noexcept;

      LANGULUS_API(RTTI)
      VMeta GetMetaVerb(const Token&, const Token& = "") const noexcept;

      LANGULUS_API(RTTI)
      CMeta GetMetaConstant(const Token&, const Token& = "") const noexcept;

      LANGULUS_API(RTTI)
      VMeta GetOperator(const Token&, const Token& = "") const noexcept;

      LANGULUS_API(RTTI)
      auto GetAmbiguousMeta(const Token&, const Token& = "") const noexcept -> const MetaList&;

      LANGULUS_API(RTTI)
      AMeta DisambiguateMeta(const Token&, const Token& = "") const;

      LANGULUS_API(RTTI)
      auto ResolveFileExtension(const Token&, const Token& = "") const -> const MetaList&;

      LANGULUS_API(RTTI)
      void UnloadBoundary(const Token&);
   };

   ///                                                                        
   ///   The global RTTI registry                                             
   ///                                                                        
   LANGULUS_API(RTTI) extern Registry Instance;

   ///                                                                        
   ///   Boundary identifier, local to every shared library/executable        
   ///   It's a simple compile-time string, that is attached upon data        
   /// reflection, so that RTTI can track from which library a type was       
   /// reflected, and thus unregister it when shared object is unloaded.      
   /// The boundary also affects pooling tactics, because if boundary is not  
   /// equal exactly to RTTI::MainBoundary, pooling will be PoolTactic::Type  
   /// by default, so that allocation that happens from external libraries    
   /// can be easily tracked and not pollute other pools                      
   ///                                                                        
   extern Token Boundary;

   
   LANGULUS(INLINED)
   DMeta GetMetaData(const Token& token, const Token& boundary = "") noexcept {
      return Instance.GetMetaData(token, boundary);
   }

   LANGULUS(INLINED)
   TMeta GetMetaTrait(const Token& token, const Token& boundary = "") noexcept {
      return Instance.GetMetaTrait(token, boundary);
   }

   LANGULUS(INLINED)
   VMeta GetMetaVerb(const Token& token, const Token& boundary = "") noexcept {
      return Instance.GetMetaVerb(token, boundary);
   }

   LANGULUS(INLINED)
   CMeta GetMetaConstant(const Token& token, const Token& boundary = "") noexcept {
      return Instance.GetMetaConstant(token, boundary);
   }

   LANGULUS(INLINED)
   VMeta GetOperator(const Token& token, const Token& boundary = "") noexcept {
      return Instance.GetOperator(token, boundary);
   }

   LANGULUS(INLINED)
   auto& GetAmbiguousMeta(const Token& token, const Token& boundary = "") noexcept {
      return Instance.GetAmbiguousMeta(token, boundary);
   }

   LANGULUS(INLINED)
   AMeta DisambiguateMeta(const Token& token, const Token& boundary = "") {
      return Instance.DisambiguateMeta(token, boundary);
   }

   LANGULUS(INLINED)
   auto& ResolveFileExtension(const Token& token, const Token& boundary = "") {
      return Instance.ResolveFileExtension(token, boundary);
   }

   LANGULUS(INLINED)
   DMeta RegisterData(const Token& token, const Token& boundary) {
      return Instance.RegisterData(token, boundary);
   }

   LANGULUS(INLINED)
   CMeta RegisterConstant(const Token& token, const Token& boundary) {
      return Instance.RegisterConstant(token, boundary);
   }

   LANGULUS(INLINED)
   TMeta RegisterTrait(const Token& token, const Token& boundary) {
      return Instance.RegisterTrait(token, boundary);
   }

   LANGULUS(INLINED)
   VMeta RegisterVerb(
      const Token& cppname,
      const Token& token,
      const Token& tokenReverse,
      const Token& op,
      const Token& opReverse,
      const Token& boundary
   ) {
      return Instance.RegisterVerb(cppname, token, tokenReverse, op, opReverse, boundary);
   }
      
   LANGULUS(INLINED)
   void RegisterFileExtension(const Token& token, DMeta type, const Token& boundary) IF_UNSAFE(noexcept) {
      Instance.RegisterFileExtension(token, type, boundary);
   }

   LANGULUS(INLINED)
   void UnloadBoundary(const Token& boundary) {
      Instance.UnloadBoundary(boundary);
   }

} // namespace Langulus::RTTI
