///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Config.hpp"
#include <unordered_map>
#include <unordered_set>

namespace Langulus::RTTI
{

   namespace Inner
   {
      /// Static initializer for every translation unit                       
      struct InterfaceInitializer {
         InterfaceInitializer();
         ~InterfaceInitializer();
      };

      LANGULUS_API(RTTI) extern InterfaceInitializer InterfaceInitializerInstance;
   }


   ///                                                                        
   ///   The RTTI interface                                                   
   /// Available only if managed reflection feature is enabled                
   ///                                                                        
   class Interface {
   friend struct Inner::InterfaceInitializer;
   private:
      using Lowercase = ::std::string;
      using MetaList = ::std::unordered_set<const Meta*>;
      static Lowercase ToLowercase(const Token&) noexcept;
      static Token ToLastToken(const Token&) noexcept;
      static Lowercase IsolateOperator(const Token&) noexcept;

      Interface();
      ~Interface();

      // Database for meta data definitions                             
      ::std::unordered_map<Lowercase, DMeta> mMetaData;
      // Database for named values                                      
      ::std::unordered_map<Lowercase, CMeta> mMetaConstants;
      // Database for meta trait definitions                            
      ::std::unordered_map<Lowercase, TMeta> mMetaTraits;
      // Database for meta verb definitions                             
      ::std::unordered_map<Lowercase, VMeta> mMetaVerbs;
      ::std::unordered_set<VMeta> mUniqueVerbs;
      // Database for verb definitions indexed by operator token        
      ::std::unordered_map<Lowercase, VMeta> mOperators;
      // Database for ambiguous tokens                                  
      ::std::unordered_map<Lowercase, MetaList> mMetaAmbiguous;

      void RegisterAmbiguous(const Token&, const Meta*) noexcept;
      void UnregisterAmbiguous(const Token&, const Meta*) noexcept;

   public:
      NOD() DMeta GetMetaData(const Token&) const noexcept;
      NOD() TMeta GetMetaTrait(const Token&) const noexcept;
      NOD() VMeta GetMetaVerb(const Token&) const noexcept;
      NOD() CMeta GetMetaConstant(const Token&) const noexcept;
      NOD() VMeta GetOperator(const Token&) const noexcept;
      NOD() const MetaList& GetAmbiguousMeta(const Token&) const noexcept;

      NOD() DMeta RegisterData(const Token&) SAFETY_NOEXCEPT();
      NOD() CMeta RegisterConstant(const Token&) SAFETY_NOEXCEPT();
      NOD() TMeta RegisterTrait(const Token&) SAFETY_NOEXCEPT();
      NOD() VMeta RegisterVerb(const Token&, const Token&, const Token& = {}, const Token& = {}) SAFETY_NOEXCEPT();

      void Unregister(DMeta) SAFETY_NOEXCEPT();
      void Unregister(TMeta) SAFETY_NOEXCEPT();
      void Unregister(VMeta) SAFETY_NOEXCEPT();
      void Unregister(CMeta) SAFETY_NOEXCEPT();
      void Unregister(const Meta*) SAFETY_NOEXCEPT();
   };

   ///                                                                        
   ///   The global RTTI instance, initialized via a schwarz counter          
   ///                                                                        
   LANGULUS_API(RTTI) extern Interface& Database;

} // namespace Langulus::RTTI
