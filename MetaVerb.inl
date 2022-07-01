///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include "Hashing.hpp"

namespace Langulus::RTTI
{

	/// Reflect or return an already reflected type meta trait definition		
	/// Reflection is done only on decayed types to avoid static variable		
	/// duplications																				
	///   @tparam T - the type to reflect (will always be decayed)					
	template<CT::Verb T>
	VMeta MetaVerb::Of() requires CT::Decayed<T> {
		// This check is not standard, but doesn't hurt afaik					
		static_assert(sizeof(T) > 0, "Can't reflect an incomplete type");

		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			static constinit VMeta meta;
		#else
			static constinit ::std::unique_ptr<MetaVerb> meta;
		#endif

		// Never proceed with reflection, if already reflected				
		if (meta) {
			#if LANGULUS_FEATURE(MANAGED_REFLECTION)
				return meta;
			#else
				return meta.get();
			#endif
		}

		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			// Try to get the definition, type might have been reflected	
			// previously in another translation unit. This is available	
			// only if MANAGED_REFLECTION feature is enabled					
			meta = Database.GetMetaVerb(Meta::GetName<T>());
			if (meta)
				return meta;
		#endif

		// If this is reached, then type is not defined yet					
		// We'll try to explicitly or implicitly reflect it					

		if constexpr (CT::Reflectable<T>) {
			// The type is explicitly reflected with a custom function		
			// Let's call it...															
			#if LANGULUS_FEATURE(MANAGED_REFLECTION)
				meta = Database.Register(T::Reflect());
				return meta;
			#else
				meta = ::std::make_unique<MetaVerb>(T::Reflect());
				return meta.get();
			#endif
		}
		else {
			// Type is implicitly reflected, so let's do our best				
			MetaVerb generated;

			// Reflect the name															
			if constexpr (requires { T::CTTI_Name; }) {
				generated.mToken = T::CTTI_Name;
				generated.mTokenReverse = T::CTTI_Name;
			}
			else if constexpr (requires { T::CTTI_PositiveVerb; }) {
				generated.mToken = T::CTTI_PositiveVerb;
				if constexpr (!requires { T::CTTI_NegativeVerb; }) {
					LANGULUS_ASSERT(
						"Positive verb defined, but no negative provided - "
						"either define the negative, or use LANGULUS(NAME) "
						"if both tokens are the same");
				}
				else generated.mTokenReverse = T::CTTI_NegativeVerb;
			}
			else if constexpr (requires { T::CTTI_NegativeVerb; }) {
				generated.mTokenReverse = T::CTTI_NegativeVerb;
				if constexpr (!requires { T::CTTI_PositiveVerb; }) {
					LANGULUS_ASSERT(
						"Negative verb defined, but no positive provided - "
						"either define the positive, or use LANGULUS(NAME) "
						"if both tokens are the same");
				}
				else generated.mToken = T::CTTI_PositiveVerb;
			}
			else {
				generated.mToken = Meta::GetName<T>();
				generated.mTokenReverse = Meta::GetName<T>();
			}

			// Reflect the operator tokens, if any									
			if constexpr (requires { T::CTTI_PositiveOperator; }) {
				generated.mOperator = T::CTTI_PositiveOperator;
				if constexpr (!requires { T::CTTI_NegativeOperator; }) {
					LANGULUS_ASSERT("Positive operator defined, but no negative provided");
				}
				else generated.mOperatorReverse = T::CTTI_NegativeOperator;
			}
			else if constexpr (requires { T::CTTI_NegativeOperator; }) {
				generated.mOperatorReverse = T::CTTI_NegativeOperator;
				if constexpr (!requires { T::CTTI_PositiveOperator; }) {
					LANGULUS_ASSERT("Negative operator defined, but no positive provided");
				}
				else generated.mOperator = T::CTTI_PositiveOperator;
			}

			// Reflect info string if any												
			if constexpr (requires { T::CTTI_Info; })
				generated.mInfo = T::CTTI_Info;

			generated.mCppName = Meta::GetName<T>();
			generated.mHash = Meta::GetHash<T>();

			// Reflect version															
			if constexpr (requires { T::CTTI_VersionMajor; })
				generated.mVersionMajor = T::CTTI_VersionMajor;

			if constexpr (requires { T::CTTI_VersionMinor; })
				generated.mVersionMinor = T::CTTI_VersionMinor;

			#if LANGULUS_FEATURE(MANAGED_REFLECTION)
				meta = Database.Register(Move(generated));
				return meta;
			#else
				meta = ::std::make_unique<MetaVerb>(Move(generated));
				return meta.get();
			#endif
		}
	}

   /// Check if two meta definitions match exactly										
	///	@param other - the verb to compare against									
	///	@return true if verbs match														
	constexpr bool MetaVerb::Is(VMeta other) const {
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
	///	@tparam T - the verb to compare against										
	///	@return true if verbs match														
	template<CT::Verb T>
	constexpr bool MetaVerb::Is() const {
		return Is(MetaVerb::Of<T>());
	}

	#if LANGULUS_FEATURE(MANAGED_REFLECTION)
		/// Compare definitions																	
		///	@param rhs - definition to compare against								
		///	@return true if definitions match fully									
		inline bool MetaVerb::operator == (const MetaVerb& rhs) const noexcept {
			return Meta::operator == (rhs)
				&& mTokenReverse == rhs.mTokenReverse;
		}
	#endif
	
} // namespace Langulus::RTTI
