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
   
   /// Check if two meta definitions match exactly										
	///	@param other - the trait to compare against									
	///	@return true if traits match														
	constexpr bool MetaTrait::Is(TMeta other) const {
		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			// This function is reduced to a pointer match, if the meta		
			// database is centralized, because it guarantees that			
			// definitions in separate translation units are always the		
			// same instance																
			return this == other;
		#else
			return mHash == other->mHash && mToken == other->mToken;
		#endif
	}	
	
	/// Check if two meta definitions match exactly										
	///	@tparam T - the trait to compare against										
	///	@return true if traits match														
	template<CT::Trait T>
	constexpr bool MetaTrait::Is() const {
		return Is(MetaTrait::Of<T>());
	}	
	
	#if LANGULUS_FEATURE(MANAGED_REFLECTION)
		/// Compare definitions																	
		///	@param rhs - definition to compare against								
		///	@return true if definitions match fully									
		inline bool MetaTrait::operator == (const MetaTrait& rhs) const noexcept {
			return Meta::operator == (rhs)
				&& mDataType == rhs.mDataType;
		}
	#endif
	
} // namespace Langulus::RTTI
