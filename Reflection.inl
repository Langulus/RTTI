///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include "Reflection.hpp"

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
	#include "RTTI.hpp"
#endif

namespace Langulus::RTTI
{
   
	/// Get the minimum allocation page size of the type (in bytes)				
	/// This guarantees two things:															
	///	1. The byte size is always a power-of-two										
	///	2. The byte size is never smaller than LANGULUS(ALIGN)					
	template<class T>
	constexpr Size GetAllocationPageOf() noexcept {
		if constexpr (requires {{Decay<T>::CTTI_AllocationPage} -> CT::Same<Size>;}) {
			constexpr Size candidate = Decay<T>::CTTI_AllocationPage * sizeof(T);
			if constexpr (candidate < Alignment)
				return Alignment;
			else 
				return Roof2cexpr(candidate);
		}
		else if constexpr (sizeof(T) < Alignment)
			return Alignment;
		else 
			return Roof2cexpr(sizeof(T));
	}

   ///                                                                        
   ///   Member implementation                                                
   ///                                                                        
   
   /// Check if member is a specific type                                     
   ///   @return true if member exactly matches the provided type             
   template<CT::Data T>
   constexpr bool Member::Is() const noexcept {
      return mType->Is<T>();
   }
   
	/// Compare members																			
	///	@param rhs - the member to compare against									
	///	@return true if members match														
	constexpr bool Member::operator == (const Member& rhs) const noexcept {
		return mType == rhs.mType
			&& mState == rhs.mState
			&& mOffset == rhs.mOffset
			&& mCount == rhs.mCount
			&& (mTrait == rhs.mTrait || (mTrait && mTrait->Is(rhs.mTrait)))
			&& mName == rhs.mName;
	}

   /// Reinterpret the member as a given type and access it (const, unsafe)   
   ///   @param instance - pointer to the beginning of the owning type        
   ///   @return a reinterpreted constant reference to member                 
   template<CT::Data T>
   const T& Member::As(const Byte* instance) const noexcept {
      return *reinterpret_cast<const T*>(Get(instance));
   }
   
   /// Reinterpret the member as a given type and access it (unsafe)          
   ///   @param instance - pointer to the beginning of the owning type        
   ///   @return a reinterpreted reference to member                          
   template<CT::Data T>
   T& Member::As(Byte* instance) const noexcept {
      return *reinterpret_cast<T*>(Get(instance));
   }
   
   /// Directly get a pointer to the type-erased member (const, unsafe)       
   ///   @param instance - pointer to the beginning of the owning type        
   ///   @return a raw constant pointer to the member inside the instance     
   constexpr const Byte* Member::Get(const Byte* instance) const noexcept {
      return instance + mOffset;
   }
   
   /// Directly get a pointer to the type-erased member (unsafe)              
   ///   @param instance - pointer to the beginning of the owning type        
   ///   @return a raw pointer to the member inside the instance              
   constexpr Byte* Member::Get(Byte* instance) const noexcept {
      return instance + mOffset;
   }
   

	///                                                                        
	///   Ability implementation																
	///                                                                        
	constexpr bool Ability::operator == (const Ability& rhs) const noexcept {
		return mVerb->Is(rhs.mVerb);
	}


	///                                                                        
   ///   Base implementation																	
   ///                                                                        

	/// Compare bases for equality															
	constexpr bool Base::operator == (const Base& other) const noexcept {
		return mType == other.mType && mCount == other.mCount;
	}

	/// Create a base descriptor for the derived type T								
	///	@return the generated base descriptor											
	template<CT::Dense T, CT::Dense BASE>
	Base Base::From() SAFETY_NOEXCEPT() {
		static_assert(!CT::Same<T, BASE>, 
			"Base duplication not allowed to avoid regress");

		Base result;
		result.mType = MetaData::Of<BASE>();

		if constexpr (CT::DerivedFrom<T, BASE>) {
			// This will fail if base is private									
			// This is detectable by is_convertible_v								
			if constexpr (::std::is_convertible_v<T*, BASE*>) {
				// The devil's work, right here										
				const Byte storage[sizeof(T)] = {};
				// First reinterpret the storage as T								
				const auto derived = reinterpret_cast<const T*>(storage);
				// Then cast it down to base											
				const auto base = static_cast<const BASE*>(derived);
				// Then reinterpret back to byte arrays and get difference	
				const auto offset = 
					reinterpret_cast<const Byte*>(derived) 
				 - reinterpret_cast<const Byte*>(base);
				SAFETY(if (offset > 0)
					Throw<Except::Access>("Base class is laid (memorywise) before the derived"));
				result.mOffset = static_cast<Offset>(offset);
			}
		}

		// If sizes match and there's no byte offset, then the base and	
		// the derived type are binary compatible									
		if constexpr (sizeof(BASE) == sizeof(T))
			result.mBinaryCompatible = (0 == result.mOffset);
		return result;
	}

	/// Create a mapping to a type															
	/// This will check if types are binary compatible by size, but no			
	/// further checks are done. Use at your own risk									
	/// It also makes the base imposed, which excludes it from serialization	
	///	@return the generated base descriptor											
	template<class T, class BASE, Count COUNT>
	Base Base::Map() noexcept {
		static_assert(!CT::Same<T, BASE>, 
			"Base duplication not allowed to avoid regress");
		static_assert(sizeof(BASE) * COUNT == sizeof(T),
			"Size mismatch while mapping types");
		static_assert(COUNT > 0,
			"Invalid mapping of zero count");
		static_assert(CT::Abstract<BASE>,
			"Can't map to an abstract type - size is always zero");

		Base result;
		result.mBinaryCompatible = true;
		result.mType = MetaData::Of<BASE>();
		result.mCount = COUNT;
		result.mImposed = true;
		return result;
	}


	///                                                                        
   ///   Meta implementation																	
   ///                                                                        

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


   ///																								
   ///   MetaData implementation																
   ///																								
   
   /// Reflect or return an already reflected type meta definition				
   /// Reflection is done only on decayed types to avoid static variable		
	/// duplications																				
   ///   @tparam T - the type to reflect (will always be decayed)					
   template<CT::Data T>
   DMeta MetaData::Of() requires CT::Decayed<T> {
		// This check is not standard, but doesn't hurt afaik					
		static_assert(sizeof(T) > 0, "Can't reflect an incomplete type");

		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			static constinit DMeta meta;
		#else
			static constinit ::std::unique_ptr<MetaData> meta;
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
			meta = Database.GetMetaData(Meta::GetName<T>());
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
				meta = ::std::make_unique<MetaData>(T::Reflect());
				return meta.get();
			#endif
		}
		else {
			// Type is implicitly reflected, so let's do our best				
			MetaData generated;
			generated.mToken = Meta::GetName<T>();
			generated.mInfo = "<no info provided due to implicit reflection>";
			generated.mName = Meta::GetName<T>();
			generated.mHash = Meta::GetHash<T>();
			generated.mIsAbstract = CT::Abstract<T>;
			generated.mIsNullifiable = CT::Nullifiable<T>;
			generated.mSize = CT::Abstract<T> ? 0 : sizeof(T);
			generated.mAlignment = alignof(T);
			generated.mAllocationPage = GetAllocationPageOf<T>();
			constexpr auto minElements = GetAllocationPageOf<T>() / sizeof(T);
			for (Size bit = 0; bit < sizeof(Size) * 8; ++bit) {
				const Size threshold = Size {1} << bit;
				const Size elements = threshold / sizeof(T);
				generated.mAllocationTable[bit] = ::std::max(minElements, elements);
			}
			generated.mIsPOD = CT::POD<T>;
			generated.mIsDeep = CT::Deep<T>;
			
			if constexpr (CT::Concretizable<T>)
				generated.mConcrete = MetaData::Of<Decay<typename T::CTTI_Concrete>>();

			// Wrap the default constructor of the type inside a lambda		
			if constexpr (CT::Defaultable<T>) {
				generated.mDefaultConstructor = [](void* at) {
					new (at) T {};
				};
			}

			// Wrap the copy constructor of the type inside a lambda			
			if constexpr (CT::CopyMakable<T>) {
				generated.mCopyConstructor = [](void* at, const void* from) {
					auto fromInstance = static_cast<const T*>(from);
					new (at) T {*fromInstance};
				};
			}

			// Wrap the move constructor of the type inside a lambda			
			if constexpr (CT::MoveMakable<T>) {
				generated.mMoveConstructor = [](void* at, void* from) {
					auto fromInstance = static_cast<T*>(from);
					new (at) T {Forward<T>(*fromInstance)};
				};
			}

			// Wrap the destructor of the type inside a lambda					
			if constexpr (CT::Destroyable<T>) {
				generated.mDestructor = [](void* at) {
					auto instance = static_cast<T*>(at);
					instance->~T();
				};
			}

			// Wrap the cloners of the type inside a lambda						
			if constexpr (CT::CloneMakable<T>) {
				generated.mCloneInUninitilizedMemory = [](const void* from, void* to) {
					auto fromInstance = static_cast<const T*>(from);
					new (to) T {fromInstance->Clone()};
				};
			}

			if constexpr (CT::CloneCopyable<T>) {
				generated.mCloneInInitializedMemory = [](const void* from, void* to) {
					auto toInstance = static_cast<T*>(to);
					auto fromInstance = static_cast<const T*>(from);
					*toInstance = fromInstance->Clone();
				};
			}

			// Wrap the == operator of the type inside a lambda				
			if constexpr (CT::Comparable<T>) {
				generated.mComparer = [](const void* t1, const void* t2) {
					auto t1Instance = static_cast<const T*>(t1);
					auto t2Instance = static_cast<const T*>(t2);
					return *t1Instance == *t2Instance;
				};
			}

			// Wrap the copy operator of the type inside a lambda				
			if constexpr (CT::Copyable<T>) {
				generated.mCopier = [](const void* from, void* to) {
					auto toInstance = static_cast<T*>(to);
					auto fromInstance = static_cast<const T*>(from);
					*toInstance = *fromInstance;
				};
			}

			// Wrap the move operator of the type inside a lambda				
			if constexpr (CT::Movable<T>) {
				generated.mMover = [](void* from, void* to) {
					auto toInstance = static_cast<T*>(to);
					auto fromInstance = static_cast<T*>(from);
					*toInstance = Move(*fromInstance);
				};
			}

			// Wrap the GetBlock method of the type inside a lambda			
			if constexpr (CT::Resolvable<T>) {
				generated.mResolver = [](const void* at) {
					auto instance = static_cast<const T*>(at);
					return instance->GetBlock();
				};
			}

			// Wrap the GetHash() method inside a lambda							
			if constexpr (CT::Hashable<T> || CT::Number<T> || CT::POD<T>) {
				generated.mHasher = [](const void* at) {
					auto instance = static_cast<const T*>(at);
					return HashData(*instance);
				};
			}

			// Wrap the Do verb method inside a lambda							
			if constexpr (CT::Dispatcher<T>) {
				generated.mDispatcher = [](void* at, Flow::Verb& verb) {
					auto instance = static_cast<T*>(at);
					instance->Do(verb);
				};
			}

			// Set reflected bases														
			if constexpr (requires { typename T::CTTI_Bases; })
				generated.SetBases<T>(typename T::CTTI_Bases {});

			// Set reflected abilities													
			if constexpr (requires { typename T::CTTI_Verbs; })
				generated.SetAbilities<T>(typename T::CTTI_Verbs {});

			// Set some additional stuff if T is fundamental					
			if constexpr (CT::Fundamental<T>)
				generated.ReflectFundamentalType<T>();

			#if LANGULUS_FEATURE(MANAGED_REFLECTION)
				meta = Database.Register(Move(generated));
				return meta;
			#else
				meta = ::std::make_unique<MetaData>(Move(generated));
				return meta.get();
			#endif
		}
	}

	/// Integrate fundamental types with the reflection system						
	/// Like, for example, implicitly adding a ANumber bases to number types	
	template<CT::Fundamental T>
	void MetaData::ReflectFundamentalType() noexcept {
		if constexpr (CT::Bool<T>) {
			using Bases = TTypeList<A::Bool>;
			SetBases<T>(Bases {});
		}
		else if constexpr (CT::Character<T>) {
			using Bases = TTypeList<A::Text>;
			SetBases<T>(Bases {});
		}
		else if constexpr (CT::SignedInteger<T>) {
			using Bases = TTypeList<A::SignedInteger>;
			SetBases<T>(Bases {});
		}
		else if constexpr (CT::UnsignedInteger<T>) {
			using Bases = TTypeList<A::UnsignedInteger>;
			SetBases<T>(Bases {});
		}
		else if constexpr (CT::Real<T>) {
			using Bases = TTypeList<A::Real>;
			SetBases<T>(Bases {});
		}
		else LANGULUS_ASSERT("Unimplemented fundamental type reflector");
	}

   /// Set the list of bases for a given meta definition                      
   ///   @tparam Args... - all the bases                                      
	template<class T, CT::Dense... BASE>
	void MetaData::SetBases(TTypeList<BASE...>) noexcept {
		static Base list[] = {Base::From<T, BASE>()...};
		mBases = {list};
	}

   /// Set the list of abilities for a given meta definition                  
   ///   @tparam Args... - all the abilities                                  
	template<class T, CT::Dense... VERB>
	void MetaData::SetAbilities(TTypeList<VERB...>) noexcept {
		static Ability list[] = {Ability::From<T, VERB>()...};
		mAbilities = {list};
	}

   /// Set the list of members for a given meta definition                    
   ///   @tparam Args... - all the members                                    
	template<CT::Dense... Args>
	void MetaData::SetMembers(Args&&... items) noexcept requires (... && CT::Same<Args, Member>) {
		mMembers = {Forward<Member>(items)...};
	}
	
	/// Get the most concrete type															
	///	@return the most concrete type													
	inline DMeta MetaData::GetMostConcrete() const noexcept {
		auto concrete = this;
		while (concrete->mConcrete)
			concrete = concrete->mConcrete;
		return concrete;
	}

	/// Get a reflected base linked to this meta data definition					
	/// Traverses the whole inheritance tree, so can return distant bases		
	///	@param type - the type of base to search for, nullptr for any			
	///	@param offset - use this to get bases by index								
	///	@param base - [in/out] base info ends up here if found					
	///	@return true if a base is available												
	inline bool MetaData::GetBase(DMeta type, Offset offset, Base& base) const {
		Count scanned {};
		for (auto& b : mBases) {
			// Check base																	
			if (type->Is(b.mType)) {
				if (scanned == offset) {
					base = b;
					return true;
				}
				else ++scanned;
			}

			// Dig deeper																	
			Base local {};
			Offset index {};
			while (b.mType->GetBase(type, index, local)) {
				if (scanned == offset) {
					local.mOffset += b.mOffset;
					local.mCount *= b.mCount;
					local.mBinaryCompatible = 
						b.mBinaryCompatible && local.mBinaryCompatible;
					local.mImposed = b.mImposed || local.mImposed;
					base = local;
					return true;
				}
				else ++scanned;
				
				++index;
			}
		}

		return false;
	}
	
	/// Get a reflected base linked to this meta data definition					
	/// Traverses the whole inheritance tree, so can return distant bases		
	///	@tparam T - the type of base to search for, void for any					
	///	@param offset - use this to get bases by index								
	///	@param base - [in/out] base info ends up here if found					
	///	@return true if a base is available												
	template<CT::Data T>
	bool MetaData::GetBase(Offset offset, Base& base) const {
		return GetBase(MetaData::Of<Decay<T>>(), offset, base);
	}

	/// A simple check if a reflected base is linked to this meta data			
	/// Traverses the whole inheritance tree, so can return distant bases		
	///	@param type - the type of base to search for									
	///	@return true if a base is available												
	inline bool MetaData::HasBase(DMeta type) const {
		for (auto& b : mBases) {
			if (type->Is(b.mType) || b.mType->HasBase(type))
				return true;
		}

		return false;
	}
	
	/// A simple check if a reflected base is linked to this meta data			
	/// Traverses the whole inheritance tree, so can return distant bases		
	///	@tparam T - the type of base to search for									
	///	@return true if a base is available												
	template<CT::Data T>
	bool MetaData::HasBase() const {
		return HasBase(MetaData::Of<T>());
	}

	/// A simple check if this meta data has a derivation								
	/// Traverses the whole inheritance tree, so can return distant bases		
	///	@param type - the type of derivation to search for							
	///	@return true if a base is available												
	inline bool MetaData::HasDerivation(DMeta type) const {
		return type->HasBase(this);
	}
	
	/// A simple check if this meta data has a derivation								
	/// Traverses the whole inheritance tree, so can return distant bases		
	///	@tparam T - the type of derivation to search for							
	///	@return true if a base is available												
	template<CT::Data T>
	bool MetaData::HasDerivation() const {
		return MetaData::Of<T>()->HasBase(this);
	}

	/// Check if this data type is able to do something								
	///	@param verb - the verb to check if able										
	///	@return true if this data type is able to do verb							
	inline bool MetaData::IsAbleTo(VMeta verb) const {
		for (auto& v : mAbilities) {
			if (verb->Is(v.mVerb))
				return true;
		}

		return false;
	}
	
	/// Check if this data type is able to do something								
	///	@tparam T - the verb to check if able											
	///	@return true if this data type is able to do verb							
	template<CT::Verb T>
	bool MetaData::IsAbleTo() const {
		return IsAbleTo(MetaVerb::Of<T>());
	}

	/// Check if this type interprets as another without conversion				
	///	@param other - the type to try interpreting as								
	///	@return true if this type interprets as other								
	template<bool ADVANCED>
	bool MetaData::CastsTo(DMeta other) const {
		if (Is(other))
			return true;

		// Different types might be compatible via inheritance				
		if (HasBase(other))
			return true;

		if constexpr (ADVANCED) {
			// Do reverse inheritance check, in search of mappings			
			Base found {};
			if (other->GetBase(this, 0, found))
				return mResolver || found.mBinaryCompatible;
		}

		// At this point we're pretty sure that types are incompatible		
		return false;
	}
	
	/// Check if this type interprets as another without conversion				
	///	@tparam T - the type to try interpreting as									
	///	@return true if this type interprets as other								
	template<CT::Data T, bool ADVANCED>
	bool MetaData::CastsTo() const {
		return CastsTo<ADVANCED>(MetaData::Of<Decay<T>>());
	}

	/// Check if this type interprets as an exact number of another without		
	/// conversion																					
	///	@param other - the type to try interpreting as								
	///	@param count - the number of items to interpret as							
	///	@return true if this type interprets as other								
	inline bool MetaData::CastsTo(DMeta other, Count count) const {
		if (Is(other) && count == 1)
			return true;

		Base found {};
		Count scanned {};
		while (GetBase(other, scanned, found)) {
			if (found.mOffset != 0)
				// Base caused a memory gap, so early failure occurs			
				// All bases must fit neatly into the original type			
				return false;

			if ((other->mIsAbstract || found.mBinaryCompatible) && count == found.mCount)
				return true;
			
			scanned += found.mCount;
		}

		if (scanned == count && !other->mIsAbstract)
			return true;

		// At this point we're pretty sure that types are incompatible		
		return false;
	}
	
	/// Check if this type interprets as an exact number of another without		
	/// conversion																					
	///	@tparam T - the type to try interpreting as									
	///	@return true if this type interprets as other								
	template<CT::Data T>
	bool MetaData::CastsTo(Count count) const {
		return CastsTo(MetaData::Of<T>(), count);
	}

	/// Check if this type is either same, base or a derivation of other			
	///	@param other - the type to check													
	///	@return true if this type is related to other								
	inline bool MetaData::IsRelatedTo(DMeta other) const {
		return Is(other) || HasBase(other) || HasDerivation(other);
	}
	
	/// Check if this type is either same, base or a derivation of other			
	///	@tparam T - the type to check														
	///	@return true if this type is related to other								
	template<CT::Data T>
	bool MetaData::IsRelatedTo() const {
		return IsRelatedTo(MetaData::Of<T>());
	}

	/// Get the number of conversions required to map one type to another		
	///	@param other - the type to check distance to									
	///	@return the distance																	
	inline MetaData::Distance MetaData::GetDistanceTo(DMeta other) const {
		if (Is(other))
			return Distance{0};

		// Check bases																		
		Distance jumps = Distance::Infinite;
		for (auto& b : mBases) {
			if (b.mImposed)
				continue;

			auto d = b.mType->GetDistanceTo(other);
			if (d != Distance::Infinite && d + 1 < jumps)
				jumps = Distance{d + 1};
		}

		return jumps;
	}
	
	/// Get the number of conversions required to map one type to another		
	///	@tparam T - the type to check distance to										
	///	@return the distance																	
	template<CT::Data T>
	MetaData::Distance MetaData::GetDistanceTo() const {
		return GetDistanceTo(MetaData::Of<T>());
	}

	/// Check if two meta definitions match exactly										
	///	@param other - the type to compare against									
	///	@return true if types match														
	constexpr bool MetaData::Is(DMeta other) const {
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
	///	@tparam T - the type to compare against										
	///	@return true if types match														
	template<CT::Data T>
	constexpr bool MetaData::Is() const {
		return Is(MetaData::Of<Decay<T>>());
	}

	/// Get a size based on reflected allocation page and count (unsafe)			
	///	@attention assumes byteSize is not zero										
	///	@param count - the number of elements to request							
	///	@returns both the provided byte size and reserved count					
	inline AllocationRequest MetaData::RequestSize(const Size& byteSize) const noexcept {
		AllocationRequest result;
		result.mByteSize = Roof2(::std::max(byteSize, mAllocationPage));
		const auto msb = CountTrailingZeroes(result.mByteSize);
		result.mElementCount = mAllocationTable[msb];
		return result;
	}

	#if LANGULUS_FEATURE(MANAGED_REFLECTION)
		/// Compare definitions																	
		///	@param rhs - definition to compare against								
		///	@return true if definitions match fully									
		bool MetaData::operator == (const MetaData& rhs) const noexcept {
			if (mMembers.size() != rhs.mMembers.size()
				|| mAbilities.size() != rhs.mAbilities.size()
				|| mBases.size() != rhs.mBases.size())
				return false;

			for (int i = 0; i < mMembers.size(); ++i) {
				if (mMembers[i] != rhs.mMembers[i])
					return false;
			}
			for (int i = 0; i < mAbilities.size(); ++i) {
				if (mAbilities[i] != rhs.mAbilities[i])
					return false;
			}
			for (int i = 0; i < mBases.size(); ++i) {
				if (mBases[i] != rhs.mBases[i])
					return false;
			}

			return mToken == rhs.mToken
				&& mInfo == rhs.mInfo
				&& mName == rhs.mName
				&& mConcrete == rhs.mConcrete
				&& mProducer == rhs.mProducer
				&& mIsPOD == rhs.mIsPOD
				&& mIsNullifiable == rhs.mIsNullifiable
				&& mIsAbstract == rhs.mIsAbstract
				&& mIsDeep == rhs.mIsDeep
				&& mSize == rhs.mSize
				&& mAlignment == rhs.mAlignment
				&& mAllocationPage == rhs.mAllocationPage
				&& mFileExtension == rhs.mFileExtension;
		}
	#endif

	
	///                                                                        
   ///   MetaTrait implementation															
   ///                                                                        
   
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
		bool MetaTrait::operator == (const MetaTrait& rhs) const noexcept {
			return mToken == rhs.mToken
				&& mInfo == rhs.mInfo
				&& mName == rhs.mName
				&& mDataType == rhs.mDataType;
		}
	#endif

	
	///                                                                        
   ///   MetaVerb implementation																
   ///                                                                        
   
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
		bool MetaVerb::operator == (const MetaVerb& rhs) const noexcept {
			return mToken == rhs.mToken
				&& mInfo == rhs.mInfo
				&& mName == rhs.mName
				&& mTokenReverse == rhs.mTokenReverse;
		}
	#endif

	/// A freestanding type compatibility check											
	/// Purely cosmetic, to avoid typing `template` before member function		
	template<CT::Data T, bool ADVANCED>
	bool CastsTo(DMeta from) {
		return from->template CastsTo<T, ADVANCED>();
	}

	/// A freestanding type compatibility check											
	/// Purely cosmetic, to avoid typing `template` before member function		
	template<CT::Data T>
	bool CastsTo(DMeta from, Count count) {
		return from->template CastsTo<T>(count);
	}
	
} // namespace Langulus::RTTI
