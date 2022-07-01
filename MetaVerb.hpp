///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
/// Include this only when building standalone											
#pragma once
#include "MetaData.hpp"

namespace Langulus::RTTI
{

	///																								
	///	Meta verb																				
	///																								
	struct MetaVerb : public Meta {
		static constexpr Token DefaultToken = "NoVerb";

		// Verbs have antonyms, denoted via this 'negative' token			
		// For example, 'Destroy' is the reverse of 'Create'					
		// This is just syntax sugar - reverse token just does mass *= -1	
		Token mTokenReverse;

		// Verbs can be tokenized as operators - just syntax sugar			
		Token mOperator;
		Token mOperatorReverse;

	public:
		template<CT::Verb T>
		NOD() static VMeta Of() requires CT::Decayed<T>;
		
		NOD() bool constexpr Is(VMeta) const;
		template<CT::Verb T>
		NOD() bool constexpr Is() const;

		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			bool operator == (const MetaVerb&) const noexcept;
		#endif
	};

} // namespace Langulus::RTTI