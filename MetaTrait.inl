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
   
	/// Reflect or return an already reflected type meta trait definition		
	/// Reflection is done only on decayed types to avoid static variable		
	/// duplications																				
	///   @tparam T - the type to reflect (will always be decayed)					
	template<CT::Trait T>
	TMeta MetaTrait::Of() requires CT::Decayed<T> {
		// This check is not standard, but doesn't hurt afaik					
		static_assert(sizeof(T) > 0, "Can't reflect an incomplete type");

		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			static constinit TMeta meta;
		#else
			static constinit ::std::unique_ptr<MetaTrait> meta;
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
			meta = Database.GetMetaTrait(Meta::GetName<T>());
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
				meta = ::std::make_unique<MetaTrait>(T::Reflect());
				return meta.get();
			#endif
		}
		else {
			// Type is implicitly reflected, so let's do our best				
			MetaTrait generated;

			if constexpr (requires { T::CTTI_Name; })
				generated.mToken = T::CTTI_Name;
			else
				generated.mToken = Meta::GetName<T>();

			if constexpr (requires { T::CTTI_Info; })
				generated.mInfo = T::CTTI_Info;

			generated.mCppName = Meta::GetName<T>();
			generated.mHash = Meta::GetHash<T>();

			if constexpr (requires { T::CTTI_VersionMajor; })
				generated.mVersionMajor = T::CTTI_VersionMajor;

			if constexpr (requires { T::CTTI_VersionMinor; })
				generated.mVersionMinor = T::CTTI_VersionMinor;

			#if LANGULUS_FEATURE(MANAGED_REFLECTION)
				meta = Database.Register(Move(generated));
				return meta;
			#else
				meta = ::std::make_unique<MetaTrait>(Move(generated));
				return meta.get();
			#endif
		}
	}

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
