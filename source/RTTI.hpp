///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
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
   using Lowercase = ::std::string;
   using MetaList = ::std::unordered_set<const Meta*>;

   ///                                                                        
   ///   The RTTI interface                                                   
   ///                                                                        
   /// Available only if managed reflection feature is enabled                
   ///                                                                        
   class Interface {
   private:
      // Database for meta data definitions                             
      ::std::unordered_map<Lowercase, DMeta> mMetaData;
      // Database for named values                                      
      ::std::unordered_map<Lowercase, CMeta> mMetaConstants;
      // Database for meta trait definitions                            
      ::std::unordered_map<Lowercase, TMeta> mMetaTraits;
      // Database for meta verb definitions                             
      ::std::unordered_map<Lowercase, VMeta> mMetaVerbs;
      // Verbs, mapped to their original C++ class name                 
      ::std::unordered_map<::std::string, VMeta> mUniqueVerbs;
      // Database for verb definitions indexed by operator token        
      ::std::unordered_map<Lowercase, VMeta> mOperators;
      // Database for ambiguous tokens                                  
      ::std::unordered_map<Lowercase, MetaList> mMetaAmbiguous;

      void RegisterAmbiguous(const Token&, const Meta*) noexcept;
      void UnregisterAmbiguous(const Token&, const Meta*) noexcept;

   public:
      ~Interface();

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
      DMeta RegisterData(const Token&) SAFETY_NOEXCEPT();

      NOD() LANGULUS_API(RTTI)
      CMeta RegisterConstant(const Token&) SAFETY_NOEXCEPT();

      NOD() LANGULUS_API(RTTI)
      TMeta RegisterTrait(const Token&) SAFETY_NOEXCEPT();

      NOD() LANGULUS_API(RTTI)
      VMeta RegisterVerb(const Token&, const Token&, const Token&, const Token& = {}, const Token& = {}) SAFETY_NOEXCEPT();

      LANGULUS_API(RTTI)
      void Unregister(DMeta) SAFETY_NOEXCEPT();

      LANGULUS_API(RTTI)
      void Unregister(TMeta) SAFETY_NOEXCEPT();

      LANGULUS_API(RTTI)
      void Unregister(VMeta) SAFETY_NOEXCEPT();

      LANGULUS_API(RTTI)
      void Unregister(CMeta) SAFETY_NOEXCEPT();

      LANGULUS_API(RTTI)
      void Unregister(const Meta*) SAFETY_NOEXCEPT();

      LANGULUS_API(RTTI)
      void UnloadLibrary(const Token&);
   };

   ///                                                                        
   ///   The global RTTI database                                             
   ///                                                                        
   LANGULUS_API(RTTI) extern Interface Database;

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

} // namespace Langulus::RTTI
