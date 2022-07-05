///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include "MetaData.hpp"

namespace Langulus::RTTI
{

	///																								
	///	Meta trait																				
	///																								
	struct MetaTrait : public Meta {
		static constexpr Token DefaultToken = "NoTrait";

		// Data filter for the trait (optional)									
		DMeta mDataType {};

	public:
		template<CT::Void T>
		NOD() static constexpr TMeta Of() requires CT::Decayed<T>;
		template<CT::Data T>
		NOD() static TMeta Of() requires CT::Decayed<T>;
		
		NOD() bool constexpr Is(TMeta) const noexcept;
		template<CT::Data T>
		NOD() bool constexpr Is() const;

		constexpr bool operator == (const MetaTrait&) const noexcept;
	};

} // namespace Langulus::RTTI