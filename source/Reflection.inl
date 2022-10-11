///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Reflection.hpp"

namespace Langulus::RTTI
{
   
   /// Get the constexpr hash of a type                                       
   ///	@return the hash of the type                                         
   template<CT::Data T>
   constexpr Hash Meta::GenerateHash(const Token& name) noexcept {
      return {::std::hash<Token>()(name)};
   }
   
   /// Get the generated hash, making any meta derivation CT::Hashable        
   ///	@return the hash of the type                                         
   inline const Hash& Meta::GetHash() const noexcept {
      return mHash;
   }
   
   /// Get the constexpr name of a type                                       
   ///	@return the hash of the type                                         
   template<CT::Data T>
   constexpr Token Meta::GetCppName() noexcept {
      return NameOf<T>();
   }

} // namespace Langulus::RTTI
