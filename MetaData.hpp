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
#include <unordered_map>

namespace Langulus::CT
{

	/// Check if T is disown-constructible													
	template<class... T>
	concept DisownMakable = (::std::constructible_from<Decay<T>, ::Langulus::Disowned<Decay<T>>&&> && ...);
	template<class... T>
	concept DisownMakableNoexcept = DisownMakable<T...> && (noexcept(T {Uneval<::Langulus::Disowned<Decay<T>>&&>()}) && ...);

	/// Check if T is abandon-constructible												
	template<class... T>
	concept AbandonMakable = (::std::constructible_from<Decay<T>, ::Langulus::Abandoned<Decay<T>>&&> && ...);
	template<class... T>
	concept AbandonMakableNoexcept = AbandonMakable<T...> && (noexcept(T {Uneval<::Langulus::Abandoned<Decay<T>>&&>()}) && ...);

}

namespace Langulus::RTTI
{

	///																								
	/// Different pool tactics you can assign to your data types					
	/// Used primarily for advanced tweaking of a final product						
	/// Pooling works only if managed memory feature is enabled						
	///																								
	enum class PoolTactic {
		// Data instances will be pooled in the default pool chain			
		// If that pool chain becomes too long, it becomes costly to find	
		// entries. Works both with managed and non-managed reflection		
		Default = 0,

		// Data instances will be pooled based on their allocation page	
		// There will be pools dedicated for each allocation page size		
		// This effectively narrows the search for entries a bit				
		// This works both with managed and non-managed reflection			
		Size,

		// Data instances will be pooled based on their type					
		// Each meta definition will have its own pool chain					
		// This works only with managed reflection, otherwise acts as		
		// the PoolTactic::Size option												
		Type
	};

	template<class T>
	constexpr Size GetAllocationPageOf() noexcept;

	
	///																								
	///	These methods are sought in each reflected type								
	///																								
	/// The default constructor, wrapped in a lambda expression if available	
	/// Takes a pointer for a placement-new expression									
	using FDefaultConstruct = TFunctor<void(void*)>;

	/// The copy constructor, wrapped in a lambda expression if available		
	/// Takes a pointer for a placement-new expression, and a source				
	using FCopyConstruct = TFunctor<void(void*, const void*)>;

	/// The move constructor, wrapped in a lambda expression if available		
	/// Takes a pointer for a placement-new expression, and a source				
	using FMoveConstruct = TFunctor<void(void*, void*)>;

	/// The destructor, wrapped in a lambda expression									
	/// Takes the pointer to the instance for destruction								
	using FDestroy = TFunctor<void(void*)>;

	/// The cloner, wrapped in a lambda expression if available						
	/// Clone one instance to another														
	using FClone = TFunctor<void(const void*, void*)>;

	/// The == operator, wrapped in a lambda expression if available				
	/// Compares two instances for equality												
	using FCompare = TFunctor<bool(const void*, const void*)>;

	/// The = operator, wrapped in a lambda expression if available				
	/// Does a shallow copy from one instance to another								
	using FCopy = TFunctor<void(const void*, void*)>;

	/// The move-copy operator, wrapped in a lambda expression if available		
	/// Does a move-copy from one instance to another									
	using FMove = TFunctor<void(void*, void*)>;

	/// The class type function, wrapped in a lambda expression						
	/// Returns the typed memory block of the class instance							
	using FResolve = TFunctor<::Langulus::Anyness::Block(const void*)>;

	/// The hash getter, wrapped in a lambda expression								
	/// Takes the pointer to the instance for hashing									
	/// Returns the hash																			
	using FHash = TFunctor<Hash(const void*)>;

	/// A custom verb dispatcher, wrapped in a lambda expression					
	/// Takes the pointer to the instance that will dispatch, and a verb			
	/// There is a mutable and immutable version of this								
	using FDispatchMutable = TFunctor<void(void*, Flow::Verb&)>;
	using FDispatchConstant = TFunctor<void(const void*, Flow::Verb&)>;
	using FVerbMutable = FDispatchMutable;
	using FVerbConstant = FDispatchConstant;


	///																								
	///	Used to reflect a member variable												
	///	You can reflect arrays of elements, tag members as traits, etc.		
	///																								
	struct Member {
		// Type of data																	
		DMeta mType {};
		// State of the data																
		DataState mState {};
		// Member offset. This is relative to the type it is offsetted		
		// in! If accessed through a derived type, that offset might		
		// be wrong! Type must be resolved first!									
		Offset mOffset {};
		// Number of elements in mData (in case of an array)					
		Count mCount {1};
		// Trait tag																		
		TMeta mTrait {};
		// Member token																	
		Token mName {};

	public:
		template<CT::Data OWNER, CT::Data DATA>
		NOD() static Member From(Offset, const Token& = {}, TMeta = {});

		NOD() constexpr bool operator == (const Member&) const noexcept;
		
		template<CT::Data T>
		NOD() constexpr bool Is() const noexcept;
		
		template<CT::Data T>
		NOD() const T& As(const Byte*) const noexcept;
		template<CT::Data T>
		NOD() T& As(Byte*) const noexcept;
		
		NOD() constexpr const Byte* Get(const Byte*) const noexcept;
		NOD() constexpr Byte* Get(Byte*) const noexcept;
	};

	using MemberList = ::std::span<Member>;

	
	///																								
	///	Used to reflect abilities 															
	///																								
	struct Ability {
		// The verb ID																		
		VMeta mVerb {};

		using Signature = ::std::vector<DMeta>;

		// Functions to call, paired with their argument types				
		// For functions that can mutate the context								
		using MutableOverloadList = ::std::unordered_map<Signature, FVerbMutable>;
		MutableOverloadList mOverloadsMutable;
		
		// Functions to call, paired with their argument types				
		// For functions that can't mutate the context							
		using ConstantOverloadList = ::std::unordered_map<Signature, FVerbConstant>;
		ConstantOverloadList mOverloadsConstant {};
		
	public:
		NOD() constexpr bool operator == (const Ability&) const noexcept;

		template<CT::Data T, CT::Data VERB, CT::Data... A>
		NOD() static Ability From() noexcept;
	};


	///																								
	///	Meta constant, used to reflect named values for enums						
	///																								
	struct MetaConst : public Meta {
		LANGULUS(NAME) "CMeta";
		LANGULUS_BASES(Meta);

		MetaType GetMetaType() const noexcept final { return Meta::Constant; }

		DMeta mValueType;
		const void* mPtrToValue;
	};

	using NamedValueList = ::std::vector<CMeta>;
	using AbilityList = ::std::unordered_map<VMeta, Ability>;
	using MutableOverloadList = typename Ability::MutableOverloadList;
	using ConstantOverloadList = typename Ability::ConstantOverloadList;


	///																								
	///	Used to reflect data coversions													
	///																								
	struct Converter {
		// The data ID we're converting to											
		DMeta mDestrinationType {};
		// Address of function to call												
		FCopyConstruct mFunction {};
		
	public:
		NOD() constexpr bool operator == (const Converter&) const noexcept;

		template<CT::Dense T, CT::Dense TO>
		NOD() static Converter From() noexcept;
	};

	using ConverterList = ::std::unordered_map<DMeta, Converter>;


	///																								
	///	Used to reflect a base for a type												
	///																								
	struct Base {
		// Type of the base																
		DMeta mType {};
		// CT::Number of bases that fit in the type								
		Count mCount {1};
		// Offset of the base, relative to the derived type					
		Offset mOffset {};
		// Used to map one type onto another										
		// Usually true when base completely fills the derived type			
		bool mBinaryCompatible {false};
		// Whether or not this base is considered an imposed base or not	
		// Basically, imposed bases are not serialized and don't act in	
		// distance computation or dispatching										
		// An imposed base can be added only manually							
		bool mImposed {false};

	public:
		NOD() constexpr bool operator == (const Base&) const noexcept;

		template<CT::Dense T, CT::Dense BASE>
		NOD() static Base From() SAFETY_NOEXCEPT();

		template<class T, class BASE, Count COUNT>
		NOD() static Base Map() noexcept;
	};

	using BaseList = ::std::span<const Base>;


	///																								
	///	Meta data																				
	///																								
	struct MetaData : public Meta {
		LANGULUS(NAME) "DMeta";
		LANGULUS_BASES(Meta);

		MetaType GetMetaType() const noexcept final { return Meta::Data; }

		enum Distance : int {
			Infinite = ::std::numeric_limits<int>::max()
		};
		
		static constexpr Token DefaultToken = "NoData";
		
		// List of reflected members													
		MemberList mMembers {};
		// List of reflected abilities												
		AbilityList mAbilities {};
		// List of reflected bases														
		BaseList mBases {};
		// List of reflected converters												
		ConverterList mConverters {};
		// List of named values															
		NamedValueList mNamedValues {};
		// Default concretization														
		DMeta mConcrete {};
		// Dynamic producer of the type												
		// Types with producers can be created only via a verb				
		DMeta mProducer {};
		// True if reflected data is POD (optimization)							
		// POD data can be directly memcpy-ed, or binary-serialized			
		bool mIsPOD = false;
		// True if reflected data is nullifiable (optimization)				
		// Nullifiable data can be constructed AND destructed via			
		// memset(0) without hitting undefined behavior							
		bool mIsNullifiable = false;
		// If reflected type is abstract												
		bool mIsAbstract = false;
		// Type will be interpreted as a memory block and iterated			
		bool mIsDeep = false;
		// Type is insertable into containers										
		bool mIsUninsertable = false;
		// Size of the reflected type (in bytes)									
		Size mSize {};
		// Alignof (in bytes)															
		Size mAlignment {};
		// Minimal allocation, in bytes												
		Size mAllocationPage {};
		// Precomputed counts indexed by MSB (avoids division by stride)	
		Size mAllocationTable[sizeof(Size) * 8] {};
		// File extensions used, separated by commas								
		Token mFileExtensions {};
		// Suffix																			
		Token mSuffix {};
		// The pooling tactic used for the type									
		PoolTactic mPoolTactic = PoolTactic::Default;

		// Default constructor wrapped in a lambda upon reflection			
		FDefaultConstruct mDefaultConstructor;
		// Copy constructor wrapped in a lambda upon reflection				
		FCopyConstruct mCopyConstructor;
		// Disowned constructor wrapped in a lambda upon reflection			
		FCopyConstruct mDisownConstructor;
		// Move constructor wrapped in a lambda upon reflection				
		FMoveConstruct mMoveConstructor;
		// Abandon constructor wrapped in a lambda upon reflection			
		FMoveConstruct mAbandonConstructor;
		// Destructor wrapped in a lambda upon reflection						
		FDestroy mDestructor;
		// Cloner wrapped in a lambda upon reflection (placement new)		
		FClone mCloneInUninitilizedMemory;
		// Cloner wrapped in a lambda upon reflection							
		FClone mCloneInInitializedMemory;
		// The == operator, wrapped in a lambda upon reflection				
		FCompare mComparer;
		// The = operator, wrapped in a lambda upon reflection				
		FCopy mCopier;
		// The move operator, wrapped in a lambda upon reflection			
		FMove mMover;
		// The ClassBlock method, wrapped in a lambda upon reflection		
		FResolve mResolver;
		// The GetHash() method, wrapped in a lambda								
		FHash mHasher;
		// The Do verb, wrapped in a lambda (mutable context)					
		FDispatchMutable mDispatcherMutable;
		// The Do verb, wrapped in a lambda	(immutable context)				
		FDispatchConstant mDispatcherConstant;

	protected:
		template<CT::Fundamental T>
		void ReflectFundamentalType() noexcept;

		NOD() const Member* GetMemberInner(TMeta, DMeta, Offset&) const noexcept;
		NOD() Count GetMemberCountInner(TMeta, DMeta, Offset&) const noexcept;

	public:
		template<CT::Void T>
		NOD() static constexpr DMeta Of() requires CT::Decayed<T>;
		template<CT::Data T>
		NOD() static DMeta Of() requires CT::Decayed<T>;

		NOD() DMeta GetMostConcrete() const noexcept;
		NOD() AllocationRequest RequestSize(const Size&) const noexcept;

		//																						
		//	Base management																
		//																						
		template<class T, CT::Dense... Args>
		void SetBases(TTypeList<Args...>) noexcept;

		NOD() bool GetBase(DMeta, Offset, Base&) const;
		template<CT::Data T>
		NOD() bool GetBase(Offset, Base&) const;

		NOD() bool HasBase(DMeta) const;
		template<CT::Data T>
		NOD() bool HasBase() const;

		NOD() bool HasDerivation(DMeta) const;
		template<CT::Data T>
		NOD() bool HasDerivation() const;

		//																						
		//	Member management																
		//																						
		template<CT::Dense... Args>
		void SetMembers(Args&&...) noexcept requires (... && CT::Same<Args, Member>);

		NOD() const Member* GetMember(TMeta, DMeta = nullptr, Offset = 0) const noexcept;
		NOD() Count GetMemberCount(TMeta, DMeta = nullptr, Offset = 0) const noexcept;

		//																						
		//	Ability management															
		//																						
		template<class T, CT::Dense... Args>
		void SetAbilities(TTypeList<Args...>) noexcept;

		NOD() bool IsAbleTo(VMeta) const;
		template<CT::Data T>
		NOD() bool IsAbleTo() const;

		template<bool MUTABLE>
		NOD() auto GetAbility(VMeta, DMeta) const;
		template<bool MUTABLE, CT::Data V>
		NOD() auto GetAbility(DMeta) const;
		template<bool MUTABLE, CT::Data V, CT::Data... A>
		NOD() auto GetAbility() const;

		//																						
		//	Named value management														
		//																						
		template<class T>
		NOD() Token GetNamedValueOf(const T&) const;

		//																						
		//	Morphisms and comparison													
		//																						
		template<class T, CT::Dense... Args>
		void SetConverters(TTypeList<Args...>) noexcept;

		template<class T>
		FCopyConstruct GetConverter() const noexcept;
		FCopyConstruct GetConverter(DMeta) const noexcept;

		template<bool ADVANCED = false>
		NOD() bool CastsTo(DMeta) const;
		NOD() bool CastsTo(DMeta, Count) const;

		template<CT::Data T, bool ADVANCED = false>
		NOD() bool CastsTo() const;
		template<CT::Data T>
		NOD() bool CastsTo(Count) const;

		NOD() bool IsRelatedTo(DMeta) const;
		template<CT::Data T>
		NOD() bool IsRelatedTo() const;

		NOD() Distance GetDistanceTo(DMeta) const;
		template<CT::Data T>
		NOD() Distance GetDistanceTo() const;

		NOD() constexpr bool Is(DMeta) const noexcept;
		template<CT::Data T>
		NOD() constexpr bool Is() const;

		constexpr bool operator == (const MetaData&) const noexcept;
	};


	template<CT::Data T, bool ADVANCED = false>
	NOD() bool CastsTo(DMeta);

	template<CT::Data T>
	NOD() bool CastsTo(DMeta, Count);

} // namespace Langulus::RTTI
