///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include "DataState.hpp"
#include "NameOf.hpp"
#include <vector>

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

/// You can provide a custom operator for your verb									
/// This is purely for syntax sugar															
/// This is used when both positive and negative operators are the same			
/// If you want them to be different, use POSITIVE and NEGATIVE names			
/// explicitly instead																			
#define LANGULUS_OPERATOR() \
	public: static constexpr ::Langulus::Token CTTI_Operator = 

/// You can provide a custom positive operator for your verb						
/// This is purely for syntax sugar															
/// If positive operator is specified, you need to also specify the negative	
/// If both negative and positive verbs are same, just use LANGULUS_OPERATOR()
#define LANGULUS_POSITIVE_OPERATOR() \
	public: static constexpr ::Langulus::Token CTTI_PositiveOperator = 

/// You can provide a custom negative operator for your verb						
/// This is purely for syntax sugar															
/// If negative operator is specified, you need to also specify the positive	
/// If both negative and positive verbs are same, just use LANGULUS_OPERATOR()
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
	public: using CTTI_Concrete = 

/// You can make types not insertable to Anyness containers, such as some		
/// intermediate types, like Block::KnownPointer. These types will produce		
/// a compile-time error when a push is attempted. All reflected types are		
/// insertable by default																		
#define LANGULUS_UNINSERTABLE() \
	public: static constexpr bool CTTI_Uninsertable =

/// You can give names to specific values of a given type, such as enums		
/// These names can be used as constants when parsing code, and are used for	
/// serialization to code/text/debug														
#define LANGULUS_NAMED_VALUES(T) \
	public: static constexpr ::Langulus::RTTI::CMetaTriplet<T> CTTI_NamedValues[] =

/// You can give an optional suffix to your type										
#define LANGULUS_SUFFIX() \
	public: static constexpr ::Langulus::Token CTTI_Suffix = 

/// You can define an allocation page (number of elements) by using				
/// LANGULUS(ALLOCATION_PAGE) X. When allocating memory for your type, X		
/// will be the minimum amount of elements to allocate, aligned to the			
/// nearest upper power-of-two amount of bytes. By default, allocation page	
/// size	is never lower than Alignment														
#define LANGULUS_ALLOCATION_PAGE() \
	public: static constexpr ::Langulus::Count CTTI_AllocationPage = 

/// Make a type abstract																		
#define LANGULUS_ABSTRACT() \
	public: static constexpr bool CTTI_Abstract = 

/// Reflect a property with a trait tag													
#define LANGULUS_PROPERTY_TRAIT(name, trait) \
	public: template<class SELF> static auto CTTI_Property_##name() { \
		alignas(SELF) const ::Langulus::Byte storage[sizeof(SELF)]; \
		const auto This = reinterpret_cast<const SELF*>(storage); \
		const auto Prop = ::std::addressof(This->name); \
		using MemberType = decltype(This->name); \
		::Langulus::RTTI::Member m; \
		m.mType = ::Langulus::RTTI::MetaData::Of<Decay<MemberType>>(); \
		m.mState = {}; \
		const auto offset = \
			  reinterpret_cast<const ::Langulus::Byte*>(This) \
			- reinterpret_cast<const ::Langulus::Byte*>(Prop); \
		SAFETY(if (offset < 0) \
			::Langulus::Throw<::Langulus::Except::Access>( \
				"Property is laid (memorywise) before the owner")); \
		m.mOffset = static_cast<Offset>(offset); \
		m.mCount = ::Langulus::ExtentOf<MemberType>; \
		m.mTrait = ::Langulus::RTTI::MetaTrait::Of<trait>(); \
		m.mName = #name; \
		return m; \
	}

/// Reflect a property																			
#define LANGULUS_PROPERTY(name) LANGULUS_PROPERTY_TRAIT(name, void)

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

/// Compile-time checks and concepts associated with RTTI							
namespace Langulus::CT
{

	/// A reflected type is a type that has a public Reflection field				
	/// This field is automatically added when using LANGULUS(REFLECT) macro	
	/// inside the type you want to reflect												
	template<class T>
	concept Reflectable = requires {
		{Decay<T>::Reflect()} -> Same<::Langulus::RTTI::MetaData>;
	};

	/// An uninsertable type is any type with a true static member					
	/// T::CTTI_Uninsertable. All types are insertable by default					
	/// Useful to mark some intermediate types, that are not supposed to be		
	/// inserted in containers																	
	template<class T>
	concept Uninsertable = Decay<T>::CTTI_Uninsertable;

	/// A POD (Plain Old Data) type is any type with a static member				
	/// T::CTTI_POD set to true. If no such member exists, the type is			
	/// assumed NOT POD by default, unless ::std::is_trivial.						
	/// POD types improve construction, destruction, copying, and cloning		
	/// by using some batching runtime optimizations									
	/// All POD types are also directly serializable to binary						
	/// Use LANGULUS(POD) macro as member to tag POD types							
	template<class T>
	concept POD = ::std::is_trivial_v<Decay<T>> || Decay<T>::CTTI_POD;

	/// A nullifiable type is any type with a static member							
	/// T::CTTI_Nullifiable set to true. If no such member exists, the type		
	/// is assumed NOT nullifiable by default												
	/// Nullifiable types improve construction by using some batching				
	/// runtime optimizations																	
	/// Use LANGULUS(NULLIFIABLE) macro as member to tag nullifiable types		
	template<class T>
	concept Nullifiable = Decay<T>::CTTI_Nullifiable;

	/// A concretizable type is any type with a member type CTTI_Concrete		
	/// If no such member exists, the type is assumed NOT concretizable by		
	/// default. Concretizable types provide a default concretization for		
	/// when	allocating abstract types														
	/// Use LANGULUS(CONCRETIZABLE) macro as member to tag such types				
	template<class T>
	concept Concretizable = requires {
		typename Decay<T>::CTTI_Concrete;
	};

	/// Check if T is abstract (has at least one pure virtual function)			
	template<class... T>
	concept Abstract = ((!Sparse<T> && (::std::is_abstract_v<Decay<T>> || Decay<T>::CTTI_Abstract)) && ...);

	namespace Inner
	{
		template<class T>
		concept DispatcherMutable = requires (Decay<T>& a, ::Langulus::Flow::Verb& b) {
			{a.Do(b)};
		};
		template<class T>
		concept DispatcherConstant = requires (const Decay<T>& a, ::Langulus::Flow::Verb& b) {
			{a.Do(b)};
		};
	}

	/// Check if all T have a mutable dispatcher (has Do() method for verbs)	
	template<class... T>
	concept DispatcherMutable = (Inner::DispatcherMutable<T> && ...);

	/// Check if all T have a constant dispatcher (has Do() method for verbs)	
	template<class... T>
	concept DispatcherConstant = (Inner::DispatcherConstant<T> && ...);

	/// Check if all T has a dispatcher, compatible with the cv-quality of T	
	template<class... T>
	concept Dispatcher = ((CT::DispatcherMutable<T> || (CT::Constant<T> && CT::DispatcherConstant<T>)) && ...);

} // namespace Langulus::CT


namespace Langulus::RTTI
{

	///																								
	///	Meta																						
	///																								
	/// Base for meta definitions																
	///																								
	struct Meta {
	public:
		enum MetaType {
			Data, Trait, Verb, Constant
		};

		virtual MetaType GetMetaType() const noexcept = 0;

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
		static constexpr Token GetCppName() noexcept;
		template<CT::Data T>
		static constexpr Token GetReflectedToken() noexcept;
	};

	template<class T>
	struct CMetaTriplet {
		Token mToken;
		T mValue;
		Token mInfo;
	};

} // namespace Langulus::RTTI


namespace Langulus::CT
{
	/// Concept for meta definitions															
	template<class... T>
	concept Meta = ((Same<T, ::Langulus::RTTI::Meta> || DerivedFrom<T, ::Langulus::RTTI::Meta>) && ...);
}


namespace std
{
	using ::Langulus::RTTI::DMeta;
	using ::Langulus::RTTI::TMeta;
	using ::Langulus::RTTI::VMeta;

	template<>
	struct hash<DMeta> {
		LANGULUS(ALWAYSINLINE) size_t operator()(DMeta k) const noexcept;
	};
	template<>
	struct hash<vector<DMeta>> {
		LANGULUS(ALWAYSINLINE) size_t operator()(const vector<DMeta>& k) const noexcept;
	};
	template<>
	struct hash<TMeta> {
		LANGULUS(ALWAYSINLINE) size_t operator()(TMeta k) const noexcept;
	};
	template<>
	struct hash<VMeta> {
		LANGULUS(ALWAYSINLINE) size_t operator()(VMeta k) const noexcept;
	};
}

#include "MetaData.hpp"
#include "MetaTrait.hpp"
#include "MetaVerb.hpp"
#include "Fundamental.hpp"

#include "MetaData.inl"
#include "MetaTrait.inl"
#include "MetaVerb.inl"
#include "Reflection.inl"
#include "Hashing.hpp"

namespace std
{
	size_t hash<DMeta>::operator()(DMeta k) const noexcept {
		return k->mHash.mHash;
	}
	size_t hash<vector<DMeta>>::operator()(const vector<DMeta>& k) const noexcept {
		using ::Langulus::Hash;
		vector<Hash> coalesced;
		for (auto& i : k)
			coalesced.emplace_back(i->mHash);
		return ::Langulus::HashBytes<::Langulus::DefaultHashSeed, false>(coalesced.data(), coalesced.size() * sizeof(Hash)).mHash;
	}
	size_t hash<TMeta>::operator()(TMeta k) const noexcept {
		return k->mHash.mHash;
	}
	size_t hash<VMeta>::operator()(VMeta k) const noexcept {
		return k->mHash.mHash;
	}
}

