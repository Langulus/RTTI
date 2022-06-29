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
		const auto name = NameOf<T>();
		return ::std::hash<Token>()(name);
	}
   
	/// Get the constexpr name of a type													
	///	@return the hash of the type														
	template<CT::Data T>
	constexpr Token Meta::GetName() noexcept {
		return NameOf<T>();
	}
	
	#if LANGULUS_FEATURE(MANAGED_REFLECTION)
		/// Compare definitions																	
		///	@param rhs - definition to compare against								
		///	@return true if definitions match fully									
		inline bool Meta::operator == (const Meta& rhs) const noexcept {
			return mToken == rhs.mToken
				&& mInfo == rhs.mInfo
				&& mCppName == rhs.mCppName
				&& mHash == rhs.mHash
				&& mVersionMajor == rhs.mVersionMajor
				&& mVersionMinor == rhs.mVersionMinor;
		}
	#endif

} // namespace Langulus::RTTI
