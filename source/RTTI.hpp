///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Hashing.hpp"
#include <unordered_map>
#include <unordered_set>

namespace Langulus::RTTI
{

   template<class T>
   using BoundedMeta = ::std::unordered_map<Token, T>;
   using MetaList = ::std::unordered_set<const Meta*>;


   ///                                                                        
   ///   The RTTI registry                                                    
   ///                                                                        
   /// Available only if managed reflection feature is enabled                
   ///                                                                        
   class Registry {
   private:
      // Database for meta data definitions                             
      ::std::unordered_map<Lowercase, BoundedMeta<DMeta>> mMetaData;
      // Database for named values                                      
      ::std::unordered_map<Lowercase, BoundedMeta<CMeta>> mMetaConstants;
      // Database for meta trait definitions                            
      ::std::unordered_map<Lowercase, BoundedMeta<TMeta>> mMetaTraits;
      // Database for meta verb definitions                             
      ::std::unordered_map<Lowercase, BoundedMeta<VMeta>> mMetaVerbs;

      // Verbs, mapped to their original C++ class name                 
      ::std::unordered_map<::std::string, BoundedMeta<VMeta>> mUniqueVerbs;
      // Database for verb definitions indexed by operator token        
      ::std::unordered_map<Lowercase, BoundedMeta<VMeta>> mOperators;
      // Database for ambiguous tokens                                  
      ::std::unordered_map<Lowercase, BoundedMeta<MetaList>> mMetaAmbiguous;
      // Meta data definitions, indexed by file extensions              
      ::std::unordered_map<Lowercase, BoundedMeta<MetaList>> mFileDatabase;

      void RegisterAmbiguous(const Token&, const Meta*) noexcept;
      void UnregisterAmbiguous(const Token&, const Token&, const Meta*) noexcept;

   public:
      ~Registry();

      NOD() LANGULUS_API(RTTI)
      DMeta GetMetaData(const Token&) const noexcept;

      NOD() LANGULUS_API(RTTI)
      TMeta GetMetaTrait(const Token&) const noexcept;

      NOD() LANGULUS_API(RTTI)
      VMeta GetMetaVerb(const Token&) const noexcept;

      NOD() LANGULUS_API(RTTI)
      CMeta GetMetaConstant(const Token&) const noexcept;

      NOD() LANGULUS_API(RTTI)
      VMeta GetOperator(const Token&) const noexcept;

      NOD() LANGULUS_API(RTTI)
      const MetaList& GetAmbiguousMeta(const Token&) const noexcept;

      NOD() LANGULUS_API(RTTI)
      const Meta* DisambiguateMeta(const Token&) const;

      NOD() LANGULUS_API(RTTI)
      const MetaList& ResolveFileExtension(const Token&) const;

      NOD() LANGULUS_API(RTTI)
      DMeta RegisterData(const Token&) IF_UNSAFE(noexcept);

      NOD() LANGULUS_API(RTTI)
      CMeta RegisterConstant(const Token&) IF_UNSAFE(noexcept);

      NOD() LANGULUS_API(RTTI)
      TMeta RegisterTrait(const Token&) IF_UNSAFE(noexcept);

      NOD() LANGULUS_API(RTTI)
      VMeta RegisterVerb(const Token&, const Token&, const Token&, const Token& = {}, const Token& = {}) IF_UNSAFE(noexcept);
      
      LANGULUS_API(RTTI)
      void RegisterFileExtension(const Token&, DMeta) IF_UNSAFE(noexcept);

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
   /// equal exactly to "MAIN", pooling will be PoolTactic::Type by default,  
   /// so that allocation that happens from external libraries can be easily  
   /// tracked and not pollute other pools.                                   
   ///                                                                        
   extern Token Boundary;

   
   NOD() LANGULUS(INLINED)
   DMeta GetMetaData(const Token& token) noexcept {
      return Instance.GetMetaData(token);
   }

   NOD() LANGULUS(INLINED)
   TMeta GetMetaTrait(const Token& token) noexcept {
      return Instance.GetMetaTrait(token);
   }

   NOD() LANGULUS(INLINED)
   VMeta GetMetaVerb(const Token& token) noexcept {
      return Instance.GetMetaVerb(token);
   }

   NOD() LANGULUS(INLINED)
   CMeta GetMetaConstant(const Token& token) noexcept {
      return Instance.GetMetaConstant(token);
   }

   NOD() LANGULUS(INLINED)
   VMeta GetOperator(const Token& token) noexcept {
      return Instance.GetOperator(token);
   }

   NOD() LANGULUS(INLINED)
   const MetaList& GetAmbiguousMeta(const Token& token) noexcept {
      return Instance.GetAmbiguousMeta(token);
   }

   NOD() LANGULUS(INLINED)
   const Meta* DisambiguateMeta(const Token& token) {
      return Instance.DisambiguateMeta(token);
   }

   NOD() LANGULUS(INLINED)
   const MetaList& ResolveFileExtension(const Token& token) {
      return Instance.ResolveFileExtension(token);
   }

   NOD() LANGULUS(INLINED)
   DMeta RegisterData(const Token& token) IF_UNSAFE(noexcept) {
      return Instance.RegisterData(token);
   }

   NOD() LANGULUS(INLINED)
   CMeta RegisterConstant(const Token& token) IF_UNSAFE(noexcept) {
      return Instance.RegisterConstant(token);
   }

   NOD() LANGULUS(INLINED)
   TMeta RegisterTrait(const Token& token) IF_UNSAFE(noexcept) {
      return Instance.RegisterTrait(token);
   }

   NOD() LANGULUS(INLINED)
   VMeta RegisterVerb(const Token& a, const Token& b, const Token& c, const Token& d, const Token& e) IF_UNSAFE(noexcept) {
      return Instance.RegisterVerb(a,b,c,d,e);
   }
      
   LANGULUS(INLINED)
   void RegisterFileExtension(const Token& a, DMeta b) IF_UNSAFE(noexcept) {
      Instance.RegisterFileExtension(a, b);
   }

   LANGULUS(INLINED)
   void UnloadBoundary(const Token& boundary) {
      Instance.UnloadBoundary(boundary);
   }

} // namespace Langulus::RTTI
