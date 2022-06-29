///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
/// Include this only when building standalone											
#pragma once
#include "DataState.hpp"
#include "NameOf.hpp"

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
	LANGULUS_EXCEPTION(Meta);
#endif


/// You can provide a custom token to your data type, instead of using NameOf	
/// When verbs are reflected with this, their positive and negative tokens		
/// shall be the same. If you want them to be different, use POSITIVE and		
/// NEGATIVE names explicitly instead														
#define LANGULUS_NAME() \
	public: static constexpr ::Langulus::Token CTTI_Name = 

/// You can provide a custom positive token for your verb							
/// This has effect only when reflecting verbs, and if specified, then you		
/// should also specify the negative verb, too, or get a compile-time error	
/// If both negative and positive verbs are same, just use LANGULUS_NAME()		
#define LANGULUS_POSITIVE_VERB() \
	public: static constexpr ::Langulus::Token CTTI_PositiveVerb = 

/// You can provide a custom negative token for your verb							
/// This has effect only when reflecting verbs, and if specified, then you		
/// should also specify the positive verb, too, or get a compile-time error	
/// If both negative and positive verbs are same, just use LANGULUS_NAME()		
#define LANGULUS_NEGATIVE_VERB() \
	public: static constexpr ::Langulus::Token CTTI_NegativeVerb = 

/// You can provide a custom positive operator for your verb						
/// This is purely for syntax sugar															
/// If positive operator is specified, you need to also specify the negative	
#define LANGULUS_POSITIVE_OPERATOR() \
	public: static constexpr ::Langulus::Token CTTI_PositiveOperator = 

/// You can provide a custom negative operator for your verb						
/// This is purely for syntax sugar															
/// If negative operator is specified, you need to also specify the positive	
#define LANGULUS_NEGATIVE_OPERATOR() \
	public: static constexpr ::Langulus::Token CTTI_NegativeOperator = 

/// You can provide information string with your reflection, for developers	
#define LANGULUS_INFO() \
	public: static constexpr ::Langulus::Token CTTI_Info = 

/// You can provide file format associations by extensions							
/// You can specify multiple extensions by separating them with commas			
/// When serializing data of this type to a file, the first reflected			
/// extension will be used, if no extension has been explicitly stated for		
/// the file upon opening it for writing													
/// When deserializing from a file, the type to deserialize will be deduced	
/// by checking the file extension with the database									
#define LANGULUS_FILES() \
	public: static constexpr ::Langulus::Token CTTI_Files = 

/// You can version your types																
/// When deserializing, you will get an error on major version mismatch			
/// If not specified, major version is always 1											
#define LANGULUS_VERSION_MAJOR() \
	public: static constexpr ::Langulus::Count CTTI_VersionMajor = 

/// You can version your types																
/// When deserializing, you will get a warning on minor version mismatch		
/// If not specified, minor version is always 0											
#define LANGULUS_VERSION_MINOR() \
	public: static constexpr ::Langulus::Count CTTI_VersionMinor = 

/// You can mark types as deep by using LANGULUS(DEEP) true / false inside		
/// class, but to fit into CT::Deep concept, your type must also inherit Block
#define LANGULUS_DEEP() \
	public: static constexpr bool CTTI_Deep = 

/// You can mark types as POD (Plain Old Data) by using LANGULUS(POD) true or	
/// false inside class. POD types are batch-copied via memcpy, and do not		
/// call constructors or destructors when contained (unless nullifiable)		
#define LANGULUS_POD() \
	public: static constexpr bool CTTI_POD = 

/// You can mark types as nullifiable, by using LANGULUS(NULLIFIABLE) true or	
/// false inside class. Nullifiable classes are batch-constructed and			
/// batch-destroyed via memset(0)															
#define LANGULUS_NULLIFIABLE() \
	public: static constexpr bool CTTI_Nullifiable = 

/// You can choose how a given type is pooled, if managed memory is enabled	
/// See RTTI::PoolTactic for options														
#define LANGULUS_POOL_TACTIC() \
	public: static constexpr ::Langulus::RTTI::PoolTactic CTTI_Pool = 

/// You can make types concretizable, by using LANGULUS(CONCRETIZABLE) Type	
/// When dynamically creating your abstract objects, the most concrete type	
/// in the chain will be used instead														
#define LANGULUS_CONCRETIZABLE() \
	public: using CTTI_Concretizable = 

/// You can make types not insertable to Anyness containers, such as some		
/// intermediate types, like Block::KnownPointer. These types will produce		
/// a compile-time error when a push is attempted. All reflected types are		
/// insertable by default																		
#define LANGULUS_INSERTABLE() \
	public: using CTTI_Insertable = 

/// You can define an allocation page (number of elements) by using				
/// LANGULUS(ALLOCATION_PAGE) X. When allocating memory for your type, X		
/// will be the minimum amount of elements to allocate, aligned to the			
/// nearest upper power-of-two amount of bytes. By default, allocation page	
/// size	is never lower than Alignment														
#define LANGULUS_ALLOCATION_PAGE() \
	public: constexpr Count CTTI_AllocationPage = 

/// Make a type abstract																		
#define LANGULUS_ABSTRACT() \
	public: static constexpr bool CTTI_Abstract = 

/// Reflect a list of members																	
#define LANGULUS_MEMBERS(...) \
	public: using CTTI_Members = ::Langulus::TTypeList<__VA_ARGS__>

/// Reflect a list of bases																	
#define LANGULUS_BASES(...) \
	public: using CTTI_Bases = ::Langulus::TTypeList<__VA_ARGS__>

/// Reflect a list of verbs																	
#define LANGULUS_VERBS(...) \
	public: using CTTI_Verbs = ::Langulus::TTypeList<__VA_ARGS__>

/// Reflect a list of possible conversions												
/// These will be automatically used by Verbs::Interpret if available			
#define LANGULUS_CONVERSIONS(...) \
	public: using CTTI_Conversions = ::Langulus::TTypeList<__VA_ARGS__>


namespace Langulus::RTTI
{

	///																								
	///	Meta																						
	///																								
	/// Base for meta definitions																
	///																								
	struct Meta {
	protected:
		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			friend class Interface;
			Count mReferences {1};
		#endif

	public:
		// Each reflection primitive has a unique token, but that			
		// uniqueness is checked only if MANAGED_REFLECTION feature is		
		// enabled																			
		Token mToken;
		// Each reflection may or may not have some info						
		Token mInfo = "<no info provided>";
		// Original name of the type													
		Token mCppName;
		// Each reflected type has an unique hash									
		Hash mHash {};
		// Major version																	
		Count mVersionMajor = 1;
		// Minor version																	
		Count mVersionMinor = 0;

		template<CT::Data T>
		static constexpr Hash GetHash() noexcept;
		template<CT::Data T>
		static constexpr Token GetName() noexcept;

		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			bool operator == (const Meta&) const noexcept;
		#endif
	};

} // namespace Langulus::RTTI

#include "MetaData.hpp"
#include "MetaTrait.hpp"
#include "MetaVerb.hpp"
#include "Fundamental.hpp"

#include "MetaData.inl"
#include "MetaTrait.inl"
#include "MetaVerb.inl"
#include "Reflection.inl"
