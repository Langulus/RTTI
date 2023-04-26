///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Reflection.hpp"
#include <vector>

namespace Langulus::RTTI
{

   ///                                                                        
   ///   Meta                                                                 
   ///                                                                        
   /// Base for meta definitions                                              
   ///                                                                        
   struct Meta {
   public:
      LANGULUS(UNALLOCATABLE) true;

      enum MetaType {
         Data, Trait, Verb, Constant
      };

      virtual MetaType GetMetaType() const noexcept = 0;

      // Each reflection primitive has a unique token, but that         
      // uniqueness is checked only if MANAGED_REFLECTION feature is    
      // enabled                                                        
      Token mToken;
      // Each reflection may or may not have some info                  
      Token mInfo {"<no info provided>"};
      // Original name of the type                                      
      Token mCppName;
      // Each reflected type has an unique hash                         
      Hash mHash {};
      // Major version                                                  
      Count mVersionMajor {1};
      // Minor version                                                  
      Count mVersionMinor {0};
      // References, increased each time a definition is merged,        
      // and decreases each time a definition is unregistered           
      Count mReferences {1};
      // The shared library that defined the module, used to unload     
      // definitions when module is unloaded                            
      Token mLibraryName;

      NOD() LANGULUS_API(RTTI)
      const Hash& GetHash() const noexcept;

      IF_LANGULUS_MANAGED_REFLECTION(
         NOD() LANGULUS_API(RTTI)
         Token GetShortestUnambiguousToken() const
      );

      template<CT::Data T>
      NOD() static constexpr Hash GenerateHash(const Token&) noexcept;

      virtual ~Meta() = default;
   };

} // namespace Langulus::RTTI

namespace Langulus::CT
{
   /// Concept for meta definitions                                           
   template<class... T>
   concept Meta = (DerivedFrom<T, RTTI::Meta> && ...);
}

namespace std
{
   using ::Langulus::RTTI::DMeta;
   using ::Langulus::RTTI::TMeta;
   using ::Langulus::RTTI::VMeta;

   template<>
   struct hash<DMeta> {
      size_t operator()(DMeta k) const noexcept;
   };

   template<>
   struct hash<vector<DMeta>> {
      size_t operator()(const vector<DMeta>& k) const noexcept;
   };

   template<>
   struct hash<TMeta> {
      size_t operator()(TMeta k) const noexcept;
   };

   template<>
   struct hash<VMeta> {
      size_t operator()(VMeta k) const noexcept;
   };

} // namespace std