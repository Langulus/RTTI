///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include "Hashing.hpp"

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
	
	/// Compare two abilities																	
	///	@param rhs - the ability to compare against									
	///	@return true if both abilities have the same verb							
	constexpr bool Ability::operator == (const Ability& rhs) const noexcept {
		return mVerb->Is(rhs.mVerb);
	}
	
	/// Create an ability reflection from a type and a verb							
	///	@return the ability																	
	template<CT::Data T, CT::Data VERB, CT::Data... A>
	Ability Ability::From() noexcept {
		static_assert(CT::DerivedFrom<VERB, ::Langulus::Flow::Verb>,
			"VERB must inherit Flow::Verb");
		static_assert(
			VERB::template AvailableFor<T, A...>() || 
			VERB::template AvailableFor<const T, A...>(),
			"VERB is not available in reflected type, "
			"neither in mutable, nor in immutable form");

		Ability result;
		result.mVerb = MetaVerb::Of<VERB>();
		if constexpr (VERB::template AvailableFor<T, A...>()) {
			result.mOverloadsMutable.insert({
				{MetaData::Of<A>()...},
				VERB::template Of<T>()
			});
		}

		if constexpr (VERB::template AvailableFor<const T, A...>()) {
			result.mOverloadsConstant.insert({
				{MetaData::Of<A>()...},
				VERB::template Of<const T>()
			});
		}

		return result;
	}


	///																								
	///   Converter implementation															
	///																								
	
	/// Compare two converters																	
	///	@param rhs - the converter to compare against								
	///	@return true if both converters have the same type							
	constexpr bool Converter::operator == (const Converter& rhs) const noexcept {
		return mDestrinationType->Is(rhs.mDestrinationType);
	}

	/// Create a converter, utilizing available cast operators/constructors		
	///	@return the converter																
	template<CT::Dense T, CT::Dense TO>
	Converter Converter::From() noexcept {
		static_assert(CT::Convertible<Decay<T>, Decay<TO>>,
			"Converter reflected, but conversion is not possible - "
			"implement a public cast operator in T, or a public constructor in TO");

		return {
			MetaData::Of<TO>(), 
			[](void* to, const void* from) {
				new (to) TO {static_cast<TO>(*static_cast<const T*>(from))};
			}
		};
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
				alignas(T) Byte storage[sizeof(T)];
				// First reinterpret the storage as T								
				const auto derived = reinterpret_cast<const T*>(storage);
				// Then cast it down to base											
				const auto base = static_cast<const BASE*>(derived);
				// Then reinterpret back to byte arrays and get difference	
				const auto offset = 
					reinterpret_cast<const Byte*>(derived) 
				 - reinterpret_cast<const Byte*>(base);
				SAFETY(if (offset < 0)
					Throw<Except::Access>("Base class is laid (memorywise) after the derived"));
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
   ///   MetaData implementation																
   ///																								
	/// Get the reflected token for a type													
	///	@return the token																		
	template<CT::Data T>
	constexpr Token MetaData::GetReflectedToken() noexcept {
		if constexpr (requires { T::CTTI_Name; })
			return T::CTTI_Name;
		else
			return Meta::GetCppName<T>();
	}

	template<CT::Void T>
	constexpr DMeta MetaData::Of() requires CT::Decayed<T> {
		return nullptr;
	}

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
			meta = Database.GetMetaData(GetReflectedToken<T>());
			if (meta)
				return meta;
		#endif

		// If this is reached, then type is not defined yet					
		// We immediately place it in the static here, because the			
		// reflection function might end up forever looping otherwise		
		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			meta = Database.RegisterData(GetReflectedToken<T>());
			if (!meta)
				Throw<Except::Meta>("Meta data conflict on registration");
		#else
			meta = ::std::make_unique<MetaData>();
		#endif

		// We'll try to explicitly or implicitly reflect it					
		if constexpr (CT::Reflectable<T>) {
			// The type is explicitly reflected with a custom function		
			// Let's call it...															
			#if LANGULUS_FEATURE(MANAGED_REFLECTION)
				*const_cast<MetaData*>(meta) = T::Reflect();
			#else
				*const_cast<MetaData*>(meta.get()) = T::Reflect();
			#endif
		}
		else {
			// Type is implicitly reflected, so let's do our best				
			#if LANGULUS_FEATURE(MANAGED_REFLECTION)
				MetaData& generated = *const_cast<MetaData*>(meta);
			#else
				MetaData& generated = *const_cast<MetaData*>(meta.get());
			#endif

			generated.mToken = GetReflectedToken<T>();
			if constexpr (requires { T::CTTI_Info; })
				generated.mInfo = T::CTTI_Info;
			generated.mCppName = Meta::GetCppName<T>();
			generated.mHash = Meta::GenerateHash<T>(GetReflectedToken<T>());
			generated.mIsAbstract = CT::Abstract<T>;
			generated.mIsNullifiable = CT::Nullifiable<T>;
			generated.mSize = CT::Abstract<T> ? 0 : sizeof(T);
			generated.mAlignment = alignof(T);

			// Calculate the allocation page and table							
			generated.mAllocationPage = GetAllocationPageOf<T>();
			constexpr auto minElements = GetAllocationPageOf<T>() / sizeof(T);
			for (Size bit = 0; bit < sizeof(Size) * 8; ++bit) {
				const Size threshold = Size {1} << bit;
				const Size elements = threshold / sizeof(T);
				generated.mAllocationTable[bit] = ::std::max(minElements, elements);
			}

			if constexpr (requires { T::CTTI_Pool; })
				generated.mPoolTactic = T::CTTI_Pool;
			if constexpr (requires { T::CTTI_Files; })
				generated.mFileExtensions = T::CTTI_Files;
			if constexpr (requires { T::CTTI_Suffix; })
				generated.mSuffix = T::CTTI_Suffix;
			if constexpr (requires { T::CTTI_VersionMajor; })
				generated.mVersionMajor = T::CTTI_VersionMajor;
			if constexpr (requires { T::CTTI_VersionMinor; })
				generated.mVersionMinor = T::CTTI_VersionMinor;

			generated.mIsPOD = CT::POD<T>;

			if constexpr (requires { T::CTTI_Deep; })
				generated.mIsDeep = T::CTTI_Deep;

			generated.mIsUninsertable = CT::Uninsertable<T>;
			
			// Wrap the default constructor of the type inside a lambda		
			if constexpr (CT::Defaultable<T> && !CT::Meta<T>) {
				generated.mDefaultConstructor = [](void* at) {
					new (at) T {};
				};
			}

			// Wrap the copy constructor of the type inside a lambda			
			if constexpr (CT::CopyMakable<T> && !CT::Meta<T>) {
				generated.mCopyConstructor = [](const void* from, void* to) {
					auto fromInstance = static_cast<const T*>(from);
					new (to) T {*fromInstance};
				};
			}

			// Wrap the disown constructor of the type inside a lambda		
			if constexpr (CT::DisownMakable<T> && !CT::Meta<T>) {
				generated.mDisownConstructor = [](const void* from, void* to) {
					auto fromInstance = static_cast<const T*>(from);
					new (to) T {Disown(*fromInstance)};
				};
			}

			// Wrap the move constructor of the type inside a lambda			
			if constexpr (CT::MoveMakable<T> && !CT::Meta<T>) {
				generated.mMoveConstructor = [](void* from, void* to) {
					auto fromInstance = static_cast<T*>(from);
					new (to) T {Forward<T>(*fromInstance)};
				};
			}

			// Wrap the abandon constructor of the type inside a lambda		
			if constexpr (CT::AbandonMakable<T> && !CT::Meta<T>) {
				generated.mAbandonConstructor = [](void* from, void* to) {
					auto fromInstance = static_cast<T*>(from);
					new (to) T {Abandon(*fromInstance)};
				};
			}

			// Wrap the destructor of the type inside a lambda					
			if constexpr (CT::Destroyable<T> && !CT::Meta<T>) {
				generated.mDestructor = [](void* at) {
					auto instance = static_cast<T*>(at);
					instance->~T();
				};
			}

			// Wrap the cloners of the type inside a lambda						
			if constexpr (CT::Clonable<T> && !CT::Meta<T>) {
				generated.mCloner = [](const void* from, void* to) {
					auto fromInstance = static_cast<const T*>(from);
					new (to) T {fromInstance->Clone()};
				};
			}

			// Wrap the == operator of the type inside a lambda				
			if constexpr (CT::Comparable<T, T>) {
				generated.mComparer = [](const void* t1, const void* t2) {
					auto t1Instance = static_cast<const T*>(t1);
					auto t2Instance = static_cast<const T*>(t2);
					return *t1Instance == *t2Instance;
				};
			}

			// Wrap the copy-assignment of the type inside a lambda			
			if constexpr (CT::Copyable<T> && !CT::Meta<T>) {
				generated.mCopier = [](const void* from, void* to) noexcept(CT::CopyableNoexcept<T>) {
					auto toInstance = static_cast<T*>(to);
					auto fromInstance = static_cast<const T*>(from);
					*toInstance = *fromInstance;
				};
			}

			// Wrap the disown-assignment of the type inside a lambda		
			if constexpr (CT::DisownCopyable<T> && !CT::Meta<T>) {
				generated.mDisownCopier = [](const void* from, void* to) noexcept(CT::DisownCopyableNoexcept<T>) {
					auto toInstance = static_cast<T*>(to);
					auto fromInstance = static_cast<const T*>(from);
					*toInstance = Disown(*fromInstance);
				};
			}

			// Wrap the move-assignment of the type inside a lambda			
			if constexpr (CT::Movable<T> && !CT::Meta<T>) {
				generated.mMover = [](void* from, void* to) noexcept(CT::MovableNoexcept<T>) {
					auto toInstance = static_cast<T*>(to);
					auto fromInstance = static_cast<T*>(from);
					*toInstance = Move(*fromInstance);
				};
			}

			// Wrap the move-assignment of the type inside a lambda			
			if constexpr (CT::AbandonCopyable<T> && !CT::Meta<T>) {
				generated.mAbandonMover = [](void* from, void* to) noexcept(CT::AbandonCopyableNoexcept<T>) {
					auto toInstance = static_cast<T*>(to);
					auto fromInstance = static_cast<T*>(from);
					*toInstance = Abandon(*fromInstance);
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

			// Wrap the mutable Do verb method inside a lambda					
			if constexpr (CT::DispatcherMutable<T>) {
				generated.mDispatcherMutable = [](void* at, Flow::Verb& verb) {
					auto instance = static_cast<T*>(at);
					instance->Do(verb);
				};
			}

			// Wrap the constant Do verb method inside a lambda				
			if constexpr (CT::DispatcherConstant<T>) {
				generated.mDispatcherConstant = [](const void* at, Flow::Verb& verb) {
					auto instance = static_cast<const T*>(at);
					instance->Do(verb);
				};
			}

			// Reflect the concrete type												
			if constexpr (CT::Concretizable<T>)
				generated.mConcrete = MetaData::Of<Decay<typename T::CTTI_Concrete>>();

			// Reflect the producer type												
			if constexpr (CT::Producible<T>)
				generated.mProducer = MetaData::Of<Decay<typename T::CTTI_Producer>>();

			// Set reflected bases														
			if constexpr (requires { typename T::CTTI_Bases; })
				generated.SetBases<T>(typename T::CTTI_Bases {});

			// Set reflected abilities													
			if constexpr (requires { typename T::CTTI_Verbs; })
				generated.SetAbilities<T>(typename T::CTTI_Verbs {});

			// Set reflected converters												
			if constexpr (requires { typename T::CTTI_Conversions; })
				generated.SetConverters<T>(typename T::CTTI_Conversions {});

			// Set reflected named values												
			if constexpr (requires { T::CTTI_NamedValues; }) {
				static_assert(CT::Comparable<T, T>, 
					"Named values specified for type, but type instances are not comparable");
				constexpr auto c = ExtentOf<decltype(T::CTTI_NamedValues)>;
				static T staticInstances[c];
				static ::std::string staticNames[c] {};
				#if !LANGULUS_FEATURE(MANAGED_REFLECTION)
					static constinit ::std::unique_ptr<MetaConst> staticMC[c] {};
				#endif

				for (int i = 0; i < c; ++i) {
					staticNames[i] += generated.mToken;
					staticNames[i] += "::";
					staticNames[i] += T::CTTI_NamedValues[i].mToken;

					#if LANGULUS_FEATURE(MANAGED_REFLECTION)
						const auto cmeta = const_cast<MetaConst*>(Database.RegisterConstant(staticNames[i]));
						if (!cmeta)
							Throw<Except::Meta>("Meta constant conflict on registration");
					#else
						staticMC[i] = ::std::make_unique<MetaConst>();
						const auto cmeta = staticMC[i].get();
					#endif

					cmeta->mToken = staticNames[i];
					cmeta->mInfo = T::CTTI_NamedValues[i].mInfo;
					cmeta->mCppName = cmeta->mToken;
					cmeta->mHash = HashBytes(cmeta->mToken.data(), cmeta->mToken.size());
					cmeta->mValueType = &generated;
					new (staticInstances + i) T {T::CTTI_NamedValues[i].mValue};
					cmeta->mPtrToValue = staticInstances + i;

					generated.mNamedValues.emplace_back(cmeta);
				}
			}

			// Set some additional stuff if T is fundamental					
			if constexpr (CT::Fundamental<T>)
				generated.ReflectFundamentalType<T>();
		}

		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			return meta;
		#else
			return meta.get();
		#endif
	}

	/// Integrate fundamental types with the reflection system						
	/// Like, for example, implicitly adding a A::Number bases to number types	
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
		else LANGULUS_ERROR("Unimplemented fundamental type reflector");
	}

   /// Set the list of bases for a given meta definition								
   ///   @tparam Args... - all the bases													
	template<class T, CT::Dense... BASE>
	void MetaData::SetBases(TTypeList<BASE...>) noexcept {
		static const Base list[] {Base::From<T, BASE>()...};
		mBases = {list};
	}

   /// Set the list of abilities for a given meta definition						
   ///   @tparam Args... - all the abilities												
	template<class T, CT::Dense... VERB>
	void MetaData::SetAbilities(TTypeList<VERB...>) noexcept {
		static const ::std::pair<VMeta, Ability> list[] {
			::std::pair<VMeta, Ability>(
				MetaVerb::Of<VERB>(), Ability::From<T, VERB>()
			)...
		};

		for (const auto& i : list)
			mAbilities.insert(i);
	}

   /// Set the list of converters for a given meta definition						
   ///   @tparam Args... - all the abilities												
	template<class T, CT::Dense... TO>
	void MetaData::SetConverters(TTypeList<TO...>) noexcept {
		static const ::std::pair<DMeta, Converter> list[] {
			::std::pair<DMeta, Converter>(
				MetaData::Of<TO>(), Converter::From<T, TO>()
			)...
		};

		for (const auto& i : list)
			mConverters.insert(i);
	}

	/// Get a converter to a specific static type										
	///	@tparam T - the type to seek a conversion to									
	///	@return the conversion function													
	template<class T>
	FCopyConstruct MetaData::GetConverter() const noexcept {
		return GetConverter(MetaData::Of<T>());
	}

	/// Get a converter to a specific dynamic type										
	///	@param meta - the type we're converting to									
	///	@return the conversion function													
	inline FCopyConstruct MetaData::GetConverter(DMeta meta) const noexcept {
		const auto found = mConverters.find(meta);
		if (found != mConverters.end())
			return found->second.mFunction;
		return FCopyConstruct {};
	}

   /// Set the list of members for a given meta definition							
   ///   @tparam Args... - all the members												
	template<CT::Dense... Args>
	void MetaData::SetMembers(Args&&... items) noexcept requires (... && CT::Same<Args, Member>) {
		mMembers = {Forward<Member>(items)...};
	}
	
	/// Get a reflected member by trait, type, and/or offset (inner)				
	///	@param trait - filter by trait, or nullptr if trait is irrelevant		
	///	@param type - filter by data type, or nullptr if irrelevant				
	///	@param offset - considers only matches after that many matches			
	///	@return the member interface if found, or nullptr if not					
	inline const Member* MetaData::GetMemberInner(TMeta trait, DMeta type, Offset& offset) const noexcept {
		// Search in all bases first													
		for (auto& base : mBases) {
			const auto found = base.mType->GetMemberInner(trait, type, offset);
			if (found)
				return found;
		}

		// Then locally																	
		for (auto& member : mMembers) {
			if (trait && !trait->Is(member.mTrait))
				continue;
			if (type && !type->Is(member.mType))
				continue;

			// Match found																	
			if (0 == offset)
				return &member;

			// Offset not reached yet													
			--offset;
		}

		// Nothing was found																
		return nullptr;
	}

	/// Get member count by matching trait, type, and/or offset (inner)			
	///	@param trait - filter by trait, or nullptr if trait is irrelevant		
	///	@param type - filter by data type, or nullptr if irrelevant				
	///	@param offset - considers only matches after that many matches			
	///	@return the member interface if found, or nullptr if not					
	inline Count MetaData::GetMemberCountInner(TMeta trait, DMeta type, Offset& offset) const noexcept {
		Count result {};

		// Search in all bases first													
		for (auto& base : mBases)
			result += base.mType->GetMemberCountInner(trait, type, offset);

		// Then locally																	
		for (auto& member : mMembers) {
			if (trait && !trait->Is(member.mTrait))
				continue;
			if (type && !type->Is(member.mType))
				continue;

			// Match found																	
			if (0 == offset) {
				++result;
				continue;
			}

			// Offset not reached yet													
			--offset;
		}

		// Nothing was found																
		return result;
	}

	/// Get a reflected member by trait, type, and/or offset							
	///	@param trait - filter by trait, or nullptr if trait is irrelevant		
	///	@param type - filter by data type, or nullptr if irrelevant				
	///	@param offset - considers only matches after that many matches			
	///	@return the member interface if found, or nullptr if not					
	inline const Member* MetaData::GetMember(TMeta trait, DMeta type, Offset offset) const noexcept {
		return GetMemberInner(trait, type, offset);
	}

	/// Count the number of matching reflected members									
	///	@param trait - filter by trait, or nullptr if trait is irrelevant		
	///	@param type - filter by data type, or nullptr if irrelevant				
	///	@param offset - considers only matches after that many matches			
	///	@return the number of matching members											
	inline Count MetaData::GetMemberCount(TMeta trait, DMeta type, Offset offset) const noexcept {
		return GetMemberCountInner(trait, type, offset);
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
		return mAbilities.find(verb) != mAbilities.end();
	}
	
	/// Check if this data type is able to do something								
	///	@tparam T - the verb to check if able											
	///	@return true if this data type is able to do verb							
	template<CT::Data T>
	bool MetaData::IsAbleTo() const {
		return IsAbleTo(MetaVerb::Of<T>());
	}

	/// Get an ability																			
	///	@param vmeta - the type of the verb												
	///	@param dmeta - the type of the verb's argument (optional)				
	///	@return the functor if found														
	template<bool MUTABLE>
	auto MetaData::GetAbility(VMeta vmeta, DMeta dmeta) const {
		const auto foundv = mAbilities.find(vmeta);
		if constexpr (MUTABLE) {
			if (foundv != mAbilities.end()) {
				const auto& overrides = foundv->second.mOverloadsMutable;
				const auto foundo = overrides.find({dmeta});
				if (foundo != overrides.end())
					return foundo->second;
			}
			return FVerbMutable {};
		}
		else {
			if (foundv != mAbilities.end()) {
				const auto& overrides = foundv->second.mOverloadsConstant;
				const auto foundo = overrides.find({dmeta});
				if (foundo != overrides.end())
					return foundo->second;
			}
			return FVerbConstant {};
		}
	}

	/// Get an ability with static verb														
	///	@tparam V - the type of the verb													
	///	@param dmeta - the type of the verb's argument (optional)				
	///	@return the functor if found														
	template<bool MUTABLE, CT::Data V>
	auto MetaData::GetAbility(DMeta dmeta) const {
		static_assert(CT::DerivedFrom<V, ::Langulus::Flow::Verb>,
			"V must be derived from Flow::Verb");
		return GetAbility<MUTABLE>(MetaVerb::Of<V>(), dmeta);
	}

	/// Get an ability with static verb and argument type								
	///	@tparam V - the type of the verb													
	///	@tparam D - the type of the verb's argument									
	///	@return the functor if found														
	template<bool MUTABLE, CT::Data V, CT::Data... A>
	auto MetaData::GetAbility() const {
		static_assert(CT::DerivedFrom<V, ::Langulus::Flow::Verb>,
			"V must be derived from Flow::Verb");
		return GetAbility<MUTABLE>(MetaVerb::Of<V>(), MetaData::Of<A>()...);
	}

	/// Get the token of a reflected named value											
	///	@attention assumes this definition is the reflection of T				
	///	@tparam T - type of the value to test (deducible)							
	///	@param value - the value to check the name of								
	///	@return the token for the value, or an empty token if not found		
	template<class T>
	Token MetaData::GetNamedValueOf(const T& value) const {
		for (auto& constant : mNamedValues) {
			if (value == *static_cast<const T*>(constant->mPtrToValue))
				return constant->mToken;
		}

		return {};
	}

	/// Check if this type interprets as another without conversion				
	///	@tparam ADVANCED - whether or not to do an advanced search in the		
	///		opposite inheritance order														
	///	@tparam BINARY_COMPATIBLE - do we require for the other to be			
	///		binary compatible with this													
	///	@param other - the type to try interpreting as								
	///	@return true if this type interprets as other								
	template<bool ADVANCED, bool BINARY_COMPATIBLE>
	bool MetaData::CastsTo(DMeta other) const {
		if (Is(other))
			return true;

		// Different types might be compatible via inheritance				
		if constexpr (!BINARY_COMPATIBLE) {
			// We don't need binary compatibility									
			if (HasBase(other))
				return true;
		}
		else {
			// Get the base and check if binary compatible						
			Base found {};
			if (GetBase(other, 0, found))
				return found.mBinaryCompatible;
		}

		if constexpr (ADVANCED) {
			// Do inheritance check from the view of the other type - it	
			// might be derived from this one. The inherited should either	
			// have a resolver and be later checked at runtime, or be		
			// binary-compatible with this											
			Base found {};
			if (other->GetBase(this, 0, found)) {
				if constexpr (BINARY_COMPATIBLE)
					return found.mBinaryCompatible;
				else
					return mResolver || found.mBinaryCompatible;
			}
		}

		// At this point we're pretty sure that types are incompatible		
		return false;
	}
	
	/// Check if this type interprets as another without conversion				
	///	@tparam T - the type to try interpreting as									
	///	@tparam ADVANCED - whether or not to do an advanced search in the		
	///		opposite inheritance order														
	///	@tparam BINARY_COMPATIBLE - do we require for the other to be			
	///		binary compatible with this													
	///	@return true if this type interprets as other								
	template<CT::Data T, bool ADVANCED, bool BINARY_COMPATIBLE>
	bool MetaData::CastsTo() const {
		return CastsTo<ADVANCED, BINARY_COMPATIBLE>(MetaData::Of<Decay<T>>());
	}

	/// Check if this type interprets as an exact number of another, without	
	/// any conversion																			
	///	@tparam BINARY_COMPATIBLE - do we require for the other to be			
	///		binary compatible with this													
	///	@param other - the type to try interpreting as								
	///	@param count - the number of items to interpret as							
	///	@return true if this type interprets as other								
	template<bool BINARY_COMPATIBLE>
	bool MetaData::CastsTo(DMeta other, Count count) const {
		if (Is(other) && count == 1)
			return true;

		Base found {};
		Count scanned {};
		while (GetBase(other, scanned, found)) {
			if (found.mOffset != 0)
				// Base caused a memory gap, so early failure occurs			
				// All bases must fit neatly into the original type			
				return false;

			if constexpr (BINARY_COMPATIBLE) {
				if (found.mBinaryCompatible && count == found.mCount)
					return true;
			}
			else {
				if ((other->mIsAbstract || found.mBinaryCompatible) && count == found.mCount)
					return true;
			}
			
			scanned += found.mCount;
		}

		if (scanned == count && !other->mIsAbstract)
			return true;

		// At this point we're pretty sure that types are incompatible		
		return false;
	}
	
	/// Check if this type interprets as an exact number of another, without	
	/// any conversion																			
	///	@tparam BINARY_COMPATIBLE - do we require for the other to be			
	///		binary compatible with this													
	///	@tparam T - the type to try interpreting as									
	///	@return true if this type interprets as other								
	template<CT::Data T, bool BINARY_COMPATIBLE>
	bool MetaData::CastsTo(Count count) const {
		return CastsTo<BINARY_COMPATIBLE>(MetaData::Of<T>(), count);
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
	constexpr bool MetaData::Is(DMeta other) const noexcept {
		#if LANGULUS_FEATURE(MANAGED_REFLECTION)
			// This function is reduced to a pointer match, if the meta		
			// database is centralized, because it guarantees that			
			// definitions in separate translation units are always the		
			// same instance																
			return this == other;
		#else
			return other && mHash == other->mHash && mToken == other->mToken;
		#endif
	}
	
	/// Check if two meta definitions match exactly										
	///	@tparam T - the type to compare against										
	///	@return true if types match														
	template<CT::Data T>
	constexpr bool MetaData::Is() const {
		return Is(MetaData::Of<Decay<T>>());
	}

	constexpr bool MetaData::operator == (const MetaData& rhs) const noexcept {
		return Is(&rhs);
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
