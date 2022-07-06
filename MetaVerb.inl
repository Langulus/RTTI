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

	/// Get the reflected positive token for a verb										
	///	@return the token																		
	template<CT::Data T>
	constexpr Token MetaVerb::GetReflectedPositiveVerbToken() noexcept {
		if constexpr (requires { T::CTTI_Name; })
			return T::CTTI_Name;
		else if constexpr (requires { T::CTTI_PositiveVerb; }) {
			if constexpr (!requires { T::CTTI_NegativeVerb; }) {
				LANGULUS_ASSERT(
					"Positive verb defined, but no negative provided - "
					"either define the negative, or use LANGULUS(NAME) "
					"if both tokens are the same");
			}

			return T::CTTI_PositiveVerb;
		}
		else return Meta::GetCppName<T>();
	}

	/// Get the reflected negative token for a verb										
	///	@return the token																		
	template<CT::Data T>
	constexpr Token MetaVerb::GetReflectedNegativeVerbToken() noexcept {
		if constexpr (requires { T::CTTI_Name; })
			return T::CTTI_Name;
		else if constexpr (requires { T::CTTI_NegativeVerb; }) {
			if constexpr (!requires { T::CTTI_PositiveVerb; }) {
				LANGULUS_ASSERT(
					"Negative verb defined, but no positive provided - "
					"either define the positive, or use LANGULUS(NAME) "
					"if both tokens are the same");
			}

			return T::CTTI_NegativeVerb;
		}
		else return Meta::GetCppName<T>();
	}

	/// Get the constexpr hash of a verb													
	///	@return the hash of the type														
	template<CT::Data T>
	constexpr Hash MetaVerb::GetVerbHash() noexcept {
		const auto name = MetaVerb::GetReflectedPositiveVerbToken<T>();
		return {::std::hash<Token>()(name)};
	}

	/// Meta of a void always returns nullptr												
	///	@return nullptr																		
	template<CT::Void T>
	constexpr VMeta MetaVerb::Of() requires CT::Decayed<T> {
		return nullptr;
	}

	/// Reflect or return an already reflected verb definition						
	/// Reflection is done only on decayed types to avoid static variable		
	/// duplications																				
	///   @tparam T - the type to reflect (will always be decayed)					
	template<CT::Data T>
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
			meta = Database.GetMetaVerb(MetaVerb::GetReflectedPositiveVerbToken<T>());
			if (meta)
				return meta;
		#endif

		// If this is reached, then trait is not defined yet					
		// We immediately place it in the static here, because the			
		// reflection function might end up forever looping otherwise		
		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			meta = Database.RegisterVerb(
				MetaVerb::GetReflectedPositiveVerbToken<T>(),
				MetaVerb::GetReflectedNegativeVerbToken<T>()
			);
		#else
			meta = ::std::make_unique<MetaVerb>();
		#endif

		// We'll try to explicitly or implicitly reflect it					
		if constexpr (CT::Reflectable<T>) {
			// The verb is explicitly reflected with a custom function		
			// Let's call it...															
			#if LANGULUS_FEATURE(MANAGED_REFLECTION)
				*const_cast<MetaVerb*>(meta) = T::Reflect();
			#else
				*const_cast<MetaVerb*>(meta.get()) = T::Reflect();
			#endif
		}
		else {
			// Type is implicitly reflected, so let's do our best				
			MetaVerb generated;
			generated.mToken = 
				MetaVerb::GetReflectedPositiveVerbToken<T>();
			generated.mTokenReverse = 
				MetaVerb::GetReflectedNegativeVerbToken<T>();

			// Reflect the operator tokens, if any									
			if constexpr (requires { T::CTTI_PositiveOperator; }) {
				if constexpr (!requires { T::CTTI_NegativeOperator; })
					LANGULUS_ASSERT("Positive operator defined, but no negative provided");

				generated.mOperator = T::CTTI_PositiveOperator;
				generated.mOperatorReverse = T::CTTI_NegativeOperator;
			}
			else if constexpr (requires { T::CTTI_NegativeOperator; }) {
				if constexpr (!requires { T::CTTI_PositiveOperator; })
					LANGULUS_ASSERT("Negative operator defined, but no positive provided");

				generated.mOperator = T::CTTI_PositiveOperator;
				generated.mOperatorReverse = T::CTTI_NegativeOperator;
			}

			// Reflect info string if any												
			if constexpr (requires { T::CTTI_Info; })
				generated.mInfo = T::CTTI_Info;

			generated.mCppName = Meta::GetCppName<T>();
			generated.mHash = GetVerbHash<T>();

			// Reflect version															
			if constexpr (requires { T::CTTI_VersionMajor; })
				generated.mVersionMajor = T::CTTI_VersionMajor;

			if constexpr (requires { T::CTTI_VersionMinor; })
				generated.mVersionMinor = T::CTTI_VersionMinor;

			if constexpr (CT::DefaultableVerb<T>)
				generated.mDefaultInvocation = T::ExecuteDefault;

			if constexpr (CT::StatelessVerb<T>)
				generated.mStatelessInvocation = T::ExecuteStateless;

			#if LANGULUS_FEATURE(MANAGED_REFLECTION)
				*const_cast<MetaVerb*>(meta) = Move(generated);
			#else
				*const_cast<MetaVerb*>(meta.get()) = Move(generated);
			#endif
		}

		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			return meta;
		#else
			return meta.get();
		#endif
	}

   /// Check if two meta definitions match exactly										
	///	@param other - the verb to compare against									
	///	@return true if verbs match														
	constexpr bool MetaVerb::Is(VMeta other) const noexcept {
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
	template<CT::Data T>
	constexpr bool MetaVerb::Is() const {
		return Is(MetaVerb::Of<T>());
	}
	
	/// Compare verb definitions																
	///	@param rhs - the verb definition to compare with							
	///	@return true if definitions match												
	constexpr bool MetaVerb::operator == (const MetaVerb& rhs) const noexcept {
		return Is(&rhs);
	}

} // namespace Langulus::RTTI
