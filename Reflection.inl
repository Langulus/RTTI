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
	constexpr Hash Meta::GetHash() noexcept {
		const auto name = Meta::GetReflectedToken<T>();
		return {::std::hash<Token>()(name)};
	}
   
	/// Get the constexpr name of a type													
	///	@return the hash of the type														
	template<CT::Data T>
	constexpr Token Meta::GetCppName() noexcept {
		return NameOf<T>();
	}

	/// Get the reflected token for a type/trait/positive verb						
	///	@return the token																		
	template<CT::Data T>
	constexpr Token Meta::GetReflectedToken() noexcept {
		if constexpr (requires { T::CTTI_Name; })
			return T::CTTI_Name;
		else
			return Meta::GetCppName<T>();
	}

} // namespace Langulus::RTTI
