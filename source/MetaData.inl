///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "NameOf.hpp"
#include "Semantics.hpp"
#include "Fundamental.hpp"
#include "Hashing.hpp"
#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   #include "RTTI.hpp"
#endif
#include <Core/Utilities.hpp>

namespace Langulus::RTTI
{
   
   /// Get the minimum allocation page size of the type (in bytes)            
   /// This guarantees two things:                                            
   ///   1. The byte size is always a power-of-two                            
   ///   2. The byte size is never smaller than LANGULUS(ALIGN)               
   template<class T>
   constexpr Size GetAllocationPageOf() noexcept {
      if constexpr (CT::Dense<T> && requires {{T::CTTI_AllocationPage} -> CT::Same<Size>;}) {
         constexpr Size candidate = T::CTTI_AllocationPage * sizeof(T);
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

   /// Generate a member definition                                           
   ///   @tparam OWNER - the owner of the member (deduced)                    
   ///   @tparam DATA - the type of the member (deduced)                      
   ///   @param member - pointer to member                                    
   ///   @param name - variable name                                          
   ///   @return the generated member descriptor                              
   template<CT::Data OWNER, CT::Data DATA>
   Member Member::From(DATA OWNER::* member, const Token& name) {
      alignas(OWNER) static const Byte storage[sizeof(OWNER)];
      const auto This = reinterpret_cast<const OWNER*>(storage);
      const auto Prop = ::std::addressof(This->*member);
      const auto offset =
           reinterpret_cast<const Byte*>(Prop)
         - reinterpret_cast<const Byte*>(This);
      LANGULUS_ASSERT(offset >= 0, Meta,
         "Member is laid (memorywise) before owner");

      //TODO of offset is outside instance limits, then mark as static, instead of throw?
      Member m;
      m.mTypeRetriever = MetaData::Of<Deext<DATA>>;
      m.mOffset = static_cast<Offset>(offset);
      m.mCount = ExtentOf<DATA>;
      m.mName = name;
      return m;
   }

   /// Generate a member definition, and tag it with a trait definition       
   ///   @tparam TRAIT - the trait to tag with                                
   ///   @tparam OWNER - the owner of the member (deduced)                    
   ///   @tparam DATA - the type of the member (deduced)                      
   ///   @param member - pointer to member                                    
   ///   @param name - variable name                                          
   ///   @return the generated member descriptor                              
   template<CT::Decayed TRAIT, CT::Data OWNER, CT::Data DATA>
   Member Member::FromTagged(DATA OWNER::* member, const Token& name) {
      Member result = Member::From(member, name);
      result.mTraitRetriever = MetaTrait::Of<TRAIT>;
      return result;
   }

   /// Get the reflected member type at runtime                               
   ///   @return return the type                                              
   LANGULUS(INLINED)
   DMeta Member::GetType() const {
      LANGULUS_ASSUME(DevAssumes, mTypeRetriever != nullptr,
         "Invalid member type retriever");
      return mTypeRetriever();
   }

   /// Get the reflected member trait at runtime                              
   ///   @return return the trait, if any, or nullptr otherwise               
   LANGULUS(INLINED)
   TMeta Member::GetTrait() const {
      if (mTraitRetriever)
         return mTraitRetriever();
      return nullptr;
   }

   /// Check if member is a specific type                                     
   ///   @return true if member exactly matches the provided type             
   template<CT::Data T>
   LANGULUS(INLINED)
   bool Member::Is() const {
      LANGULUS_ASSUME(DevAssumes, GetType(), "Invalid member type");
      return GetType()->template Is<T>();
   }
   
   /// Check if member is a specific type                                     
   ///   @param meta - the meta definition to compare against                 
   ///   @return true if member exactly matches the provided type             
   LANGULUS(INLINED)
   bool Member::Is(DMeta meta) const {
      LANGULUS_ASSUME(DevAssumes, GetType(), "Invalid member type");
      return GetType()->Is(meta);
   }
   
   /// Check if member is tagged with a specific trait                        
   ///   @return true if member exactly matches the provided trait            
   template<CT::Decayed T>
   LANGULUS(INLINED)
   bool Member::TraitIs() const {
      const auto trait = GetTrait();
      if (!trait)
         return false;
      return trait->template Is<T>();
   }
   
   /// Check if member is tagged with a specific trait                        
   ///   @param meta - the meta definition to compare against                 
   ///   @return true if member exactly matches the provided trait            
   LANGULUS(INLINED)
   bool Member::TraitIs(TMeta meta) const {
      const auto trait = GetTrait();
      if (!trait)
         return false;
      return trait->Is(meta);
   }
   
   /// Compare members                                                        
   ///   @param rhs - the member to compare against                           
   ///   @return true if members match                                        
   LANGULUS(INLINED)
   bool Member::operator == (const Member& rhs) const noexcept {
      const auto type1 = GetType();
      const auto trait1 = GetTrait();
      const auto type2 = rhs.GetType();
      const auto trait2 = rhs.GetTrait();

      return (type1 == type2 || (type1 && type1->IsExact(type2)))
         && mOffset == rhs.mOffset
         && mCount == rhs.mCount
         && (trait1 == trait2 || (trait1 && trait1->Is(trait2)))
         && mName == rhs.mName;
   }

   /// Reinterpret the member as a given type and access it (const, unsafe)   
   ///   @param instance - pointer to the beginning of the owning type        
   ///   @return a reinterpreted constant reference to member                 
   template<CT::Data T>
   LANGULUS(INLINED)
   const T& Member::As(const Byte* instance) const noexcept {
      return *reinterpret_cast<const T*>(Get(instance));
   }
   
   /// Reinterpret the member as a given type and access it (unsafe)          
   ///   @param instance - pointer to the beginning of the owning type        
   ///   @return a reinterpreted reference to member                          
   template<CT::Data T>
   LANGULUS(INLINED)
   T& Member::As(Byte* instance) const noexcept {
      return *reinterpret_cast<T*>(Get(instance));
   }
   
   /// Directly get a pointer to the type-erased member (const, unsafe)       
   ///   @param instance - pointer to the beginning of the owning type        
   ///   @return a raw constant pointer to the member inside the instance     
   LANGULUS(INLINED)
   constexpr const Byte* Member::Get(const Byte* instance) const noexcept {
      return instance + mOffset;
   }
   
   /// Directly get a pointer to the type-erased member (unsafe)              
   ///   @param instance - pointer to the beginning of the owning type        
   ///   @return a raw pointer to the member inside the instance              
   LANGULUS(INLINED)
   constexpr Byte* Member::Get(Byte* instance) const noexcept {
      return instance + mOffset;
   }
   

   ///                                                                        
   ///   Ability implementation                                               
   ///                                                                        

   /// Compare two abilities                                                  
   ///   @param rhs - the ability to compare against                          
   ///   @return true if both abilities have the same verb                    
   LANGULUS(INLINED)
   bool Ability::operator == (const Ability& rhs) const noexcept {
      return mVerb->Is(rhs.mVerb);
   }
   
   /// Create an ability reflection from a type and a verb                    
   ///   @return the ability                                                  
   template<CT::Dense T, CT::Data VERB, CT::Data... A>
   Ability Ability::From() noexcept {
      static_assert(CT::DerivedFrom<VERB, ::Langulus::Flow::Verb>,
         "VERB must inherit Flow::Verb");

      Ability result;
      result.mVerb = MetaVerb::Of<VERB>();

      // Register provided argument overload (if any)...                
      if constexpr (CT::Mutable<T>) {
         if constexpr (VERB::template AvailableFor<T, A...>()) {
            // ... for mutable context                                  
            result.mOverloadsMutable.insert({
               {MetaData::Of<A>()...},
               VERB::template Of<T>()
            });
         }

         if constexpr (VERB::template AvailableFor<const T, A...>()) {
            // ... for immutable context                                
            // (you can call immutable verbs in mutable types)          
            result.mOverloadsConstant.insert({
               {MetaData::Of<A>()...},
               VERB::template Of<const T>()
            });
         }
      }
      else if constexpr (VERB::template AvailableFor<T, A...>()) {
         // ... for immutable context                                   
         // (you can call only immutable verbs in immutable types)      
         result.mOverloadsConstant.insert({
            {MetaData::Of<A>()...},
            VERB::template Of<T>()
         });
      }

      return result;
   }


   ///                                                                        
   ///   Converter implementation                                             
   ///                                                                        

   /// Compare two converters                                                 
   ///   @param rhs - the converter to compare against                        
   ///   @return true if both converters have the same type                   
   LANGULUS(INLINED)
   constexpr bool Converter::operator == (const Converter& rhs) const noexcept {
      return mDestrinationType->Is(rhs.mDestrinationType);
   }

   /// Create a converter, utilizing available cast operators/constructors    
   ///   @return the converter                                                
   template<class T, class TO>
   Converter Converter::From() noexcept {
      static_assert(!CT::Same<T, TO>,
         "Conversion types can't be similar");
      static_assert(CT::Convertible<T, TO>,
         "Converter reflected, but conversion is not possible - "
         "implement a public cast operator in T, or a public constructor in TO,"
         " these can be either explicit or not");

      using DT = Decay<T>;
      using DTO = Decay<TO>;

      return {
         MetaData::Of<DTO>(),
         [](const void* from, void* to) {
            auto fromT = reinterpret_cast<const T*>(from);
            auto toT = reinterpret_cast<TO*>(to);

            // These should be in sync with CT::Inner::Convertible      
            if constexpr (requires (DT& from) { DTO {from}; }) {
               new (&DenseCastMutable(toT)) DTO {
                  DenseCastMutable(fromT)
               };
            }
            else if constexpr (requires (DT& from) { DTO {from.operator DTO()}; }) {
               new (&DenseCastMutable(toT)) DTO {
                  DenseCastMutable(fromT).operator DTO()
               };
            }
            else if constexpr (requires (DT& from) { static_cast<DTO>(from); }) {
               new (&DenseCastMutable(toT)) DTO {
                  static_cast<DTO>(DenseCastMutable(fromT))
               };
            }
            else LANGULUS_ERROR("Unhandled conversion route");
         }
      };
   }


   ///                                                                        
   ///   Base implementation                                                  
   ///                                                                        

   /// Compare bases for equality                                             
   LANGULUS(INLINED)
   constexpr bool Base::operator == (const Base& other) const noexcept {
      return mType == other.mType && mCount == other.mCount;
   }

   /// Create a base descriptor for the derived type T                        
   ///   @return the generated base descriptor                                
   template<CT::Dense T, CT::Dense BASE>
   Base Base::From() SAFETY_NOEXCEPT() {
      static_assert(!CT::Same<T, BASE>, 
         "Base duplication not allowed to avoid regress");
      static_assert(NameOf<T>() != NameOf<BASE>(),
         "T and BASE have the same LANGULUS(NAME) token, possibly due to "
         "inheritance. Specify a different LANGULUS(NAME) for each!");

      Base result;
      result.mType = MetaData::Of<BASE>();

      if constexpr (CT::DerivedFrom<T, BASE>) {
         // This will fail if base is private                           
         // This is detectable by is_convertible_v                      
         if constexpr (::std::is_convertible_v<T*, BASE*>) {
            // The devil's work, right here                             
            alignas(T) static const Byte storage[sizeof(T)];
            // First reinterpret the storage as T                       
            const auto derived = reinterpret_cast<const T*>(storage);
            // Then cast it down to base                                
            const auto base = static_cast<const BASE*>(derived);
            // Then reinterpret back to byte arrays and get difference  
            const auto offset = 
               reinterpret_cast<const Byte*>(base) -
               reinterpret_cast<const Byte*>(derived);

            LANGULUS_ASSERT(offset >= 0, Meta,
               "BASE is laid (memorywise) before T");
            result.mOffset = static_cast<Offset>(offset);
         }
      }
      else {
         // If not inherited in C++, then always imposed                
         // Imposed bases are excluded from serialization               
         result.mImposed = true;

         if constexpr (!CT::Abstract<BASE> && sizeof(BASE) < sizeof(T)) {
            // The imposed type has a chance of being binary compatible 
            // when having a specific count                             
            result.mBinaryCompatible = 0 == sizeof(T) % sizeof(BASE);
            result.mCount = sizeof(T) / sizeof(BASE);
         }
      }

      // If sizes match and there's no byte offset, then the base and   
      // the derived type are binary compatible                         
      if constexpr (sizeof(BASE) == sizeof(T))
         result.mBinaryCompatible = (0 == result.mOffset);
      return result;
   }

   /// Reflecting a void type always returns nullptr                          
   ///   @return nullptr                                                      
   template<CT::Void T>
   LANGULUS(INLINED)
   constexpr DMeta MetaData::Of() {
      return nullptr;
   }
   
   /// Always strip references from T                                         
   ///   @return the meta data of the stripped of references T                
   template<CT::DataReference T>
   LANGULUS(INLINED)
   DMeta MetaData::Of() {
      return MetaData::Of<Decvq<Deref<T>>>();
   }

   /// Reflect a pointer, optimized to reuse origin type properties           
   /// A generalized reflection routine increases build time significantly    
   ///   @tparam T - the type to reflect                                      
   template<CT::SparseData T>
   LANGULUS(NOINLINE)
   DMeta MetaData::Of() {
      static_assert(CT::Complete<T>, "Can't reflect incomplete type");
      static_assert(!CT::Array<T>, "Can't reflect a bounded array type");
      static_assert(!NameOf<T>().empty(), "Invalid data token is not allowed");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get the definition, type might have been reflected   
         // previously in another library. Unfortunately we can't keep  
         // a static pointer to the meta, because forementioned library 
         // might be reloaded, and thus produce new pointer.            
         DMeta meta = Database.GetMetaData(NameOf<T>());
         if (meta)
            return meta;

         // If this is reached, then type is not defined yet            
         // We immediately request its spot in the database, or the     
         // reflection function might end up forever looping otherwise  
         meta = Database.RegisterData(NameOf<T>());
         auto& generated = *const_cast<MetaData*>(meta);
      #else
         // Keep a static meta pointer for each translation unit        
         static constinit ::std::unique_ptr<MetaData> meta;
         if (meta)
            return meta.get();

         // If this is reached, then type is not defined yet            
         // We immediately place it in the static here, or the          
         // reflection function might end up forever looping otherwise  
         meta = ::std::make_unique<MetaData>();
         auto& generated = *const_cast<MetaData*>(meta.get());
      #endif

      if constexpr (CT::Complete<Deptr<T>>) {
         // Reflect the denser type and propagate its origin properties 
         auto denser = MetaData::Of<Deptr<T>>();
         generated = *denser;
         generated.mDeptr = denser;

         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            // Set library boundary                                     
            generated.mLibraryName = RTTI::Boundary;

            // Pool tactic is always default for pointers, unless these 
            // pointers have been registered outside MAIN boundary      
            generated.mPoolTactic = RTTI::Boundary != "MAIN" 
               ? PoolTactic::Type
               : PoolTactic::Default;
         #endif
      }
      else {
         // The denser type is incomplete, so nothing really is known   
         // about it                                                    
         generated.mInfo = "<incomplete type pointer>";

         // Set library boundary                                        
         IF_LANGULUS_MANAGED_MEMORY(generated.mLibraryName = RTTI::Boundary);
      }
      
      // Overwrite pointer-specific stuff                               
      generated.mToken = NameOf<T>();
      generated.mCppName = CppNameOf<T>();
      generated.mHash = Meta::GenerateHash<T>(NameOf<T>());
      generated.mSize = sizeof(T);
      generated.mAlignment = alignof(T);
      generated.mIsSparse = true;
      generated.mIsConstant = CT::Constant<T>;
      
      // Calculate the allocation page and table                        
      // It is the same for all kinds of pointers                       
      generated.mAllocationPage = GetAllocationPageOf<void*>();
      constexpr auto minElements = GetAllocationPageOf<void*>() / sizeof(void*);
      for (Size bit = 0; bit < sizeof(Size) * 8; ++bit) {
         const Size threshold = Size {1} << bit;
         const Size elements = threshold / sizeof(void*);
         generated.mAllocationTable[bit] = ::std::max(minElements, elements);
      }
      
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         return meta;
      #else
         return meta.get();
      #endif
   }

   /// Reflect a constant dense type, optimized to reuse origin type          
   /// A generalized reflection routine increases build time significantly    
   ///   @tparam T - the type to reflect                                      
   template<CT::DenseData T>
   LANGULUS(NOINLINE)
   DMeta MetaData::Of() requires CT::Constant<T> {
      static_assert(CT::Complete<T>, "Can't reflect incomplete type");
      static_assert(!CT::Array<T>, "Can't reflect a bounded array type");
      static_assert(!NameOf<T>().empty(), "Invalid data token is not allowed");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get the definition, type might have been reflected   
         // previously in another library. Unfortunately we can't keep  
         // a static pointer to the meta, because forementioned library 
         // might be reloaded, and thus produce new pointer.            
         DMeta meta = Database.GetMetaData(NameOf<T>());
         if (meta)
            return meta;

         // If this is reached, then type is not defined yet            
         // We immediately request its spot in the database, or the     
         // reflection function might end up forever looping otherwise  
         meta = Database.RegisterData(NameOf<T>());
         auto& generated = *const_cast<MetaData*>(meta);
      #else
         // Keep a static meta pointer for each translation unit        
         static constinit ::std::unique_ptr<MetaData> meta;
         if (meta)
            return meta.get();

         // If this is reached, then type is not defined yet            
         // We immediately place it in the static here, or the          
         // reflection function might end up forever looping otherwise  
         meta = ::std::make_unique<MetaData>();
         auto& generated = *const_cast<MetaData*>(meta.get());
      #endif

      // Reflect the origin type and propagate its properties           
      auto denser = MetaData::Of<Decay<T>>();
      generated = *denser;
      generated.mOrigin = denser;

      #if LANGULUS_FEATURE(MANAGED_MEMORY)
         // Set library boundary                                        
         generated.mLibraryName = RTTI::Boundary;

         // If pool tactic is default, turn it Typed if outside MAIN    
         if (generated.mPoolTactic == PoolTactic::Default) {
            generated.mPoolTactic = RTTI::Boundary != "MAIN"
               ? PoolTactic::Type
               : PoolTactic::Default;
         }
      #endif
      
      // Overwrite constant-specific stuff                              
      generated.mToken = NameOf<T>();
      generated.mCppName = CppNameOf<T>();
      generated.mHash = Meta::GenerateHash<T>(NameOf<T>());
      generated.mIsConstant = true;
      
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         return meta;
      #else
         return meta.get();
      #endif
   }

   /// Reflect a fully decayed (origin) type                                  
   ///   @tparam T - the type to reflect                                      
   template<CT::DenseData T>
   LANGULUS(NOINLINE)
   DMeta MetaData::Of() requires CT::Mutable<T> {
      static_assert(CT::Complete<T>, "Can't reflect incomplete type");
      static_assert(!CT::Array<T>, "Can't reflect a bounded array type");
      static_assert(!NameOf<T>().empty(), "Invalid data token is not allowed");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get the definition, type might have been reflected   
         // previously in another library. Unfortunately we can't keep  
         // a static pointer to the meta, because forementioned library 
         // might be reloaded, and thus produce new pointer.            
         DMeta meta = Database.GetMetaData(NameOf<T>());
         if (meta)
            return meta;

         // If this is reached, then type is not defined yet            
         // We immediately request its spot in the database, or the     
         // reflection function might end up forever looping otherwise  
         meta = Database.RegisterData(NameOf<T>());
         MetaData& generated = *const_cast<MetaData*>(meta);
      #else
         // Keep a static meta pointer for each translation unit        
         static constinit ::std::unique_ptr<MetaData> meta;
         if (meta)
            return meta.get();

         // If this is reached, then type is not defined yet            
         // We immediately place it in the static here, because the     
         // reflection function might end up forever looping otherwise  
         meta = ::std::make_unique<MetaData>();
         MetaData& generated = *const_cast<MetaData*>(meta.get());
      #endif

      // We'll try to explicitly or implicitly reflect it               
      if constexpr (CT::Reflectable<T>) {
         // The type is explicitly reflected with a custom function     
         // Let's call it...                                            
         generated = T::Reflect();
      }
      else {
         // Type is implicitly reflected, so let's do our best          
         generated.mToken = NameOf<T>();
         if constexpr (requires { T::CTTI_Info; })
            generated.mInfo = T::CTTI_Info;
         generated.mCppName = CppNameOf<T>();
         generated.mHash = Meta::GenerateHash<T>(NameOf<T>());
         generated.mIsAbstract = CT::Abstract<T>;
         generated.mIsNullifiable = CT::Nullifiable<T>;
         generated.mSize = sizeof(T);
         generated.mAlignment = alignof(T);
         generated.mIsPOD = CT::POD<T>;
         generated.mIsUninsertable = CT::Uninsertable<T>;
         generated.mIsUnallocatable = CT::Unallocatable<T>;
         generated.mSuffix = SuffixOf<T>();

         // This is the origin type, so self-refer                      
         generated.mOrigin = &generated;

         // Calculate the allocation page and table                     
         // It is the same, regardless if T is const or not             
         generated.mAllocationPage = GetAllocationPageOf<T>();
         constexpr auto minElements = GetAllocationPageOf<T>() / sizeof(T);
         for (Size bit = 0; bit < sizeof(Size) * 8; ++bit) {
            const Size threshold = Size {1} << bit;
            const Size elements = threshold / sizeof(T);
            generated.mAllocationTable[bit] = ::std::max(minElements, elements);
         }

         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            // Consider the boundary and pool tactics                   
            generated.mLibraryName = RTTI::Boundary;

            if constexpr (requires { T::CTTI_Pool; })
               generated.mPoolTactic = T::CTTI_Pool;
         
            if (RTTI::Boundary != "MAIN" && generated.mPoolTactic == PoolTactic::Default)
               generated.mPoolTactic = PoolTactic::Type;
         #endif

         if constexpr (requires { T::CTTI_Files; }) {
            generated.mFileExtensions = T::CTTI_Files;

            #if LANGULUS_FEATURE(MANAGED_REFLECTION)
               // Register all file extensions                          
               const auto ext = generated.mFileExtensions;
               Offset sequential = 0;
               for (Offset e = 0; e < ext.size(); ++e) {
                  if (IsSpace(ext[e]) || ext[e] == ',') {
                     if (sequential) {
                        const auto lc = ext.substr(e - sequential, sequential);
                        Database.RegisterFileExtension(lc, &generated);
                     }

                     sequential = 0;
                     continue;
                  }

                  ++sequential;
               }

               if (sequential) {
                  const auto lc = ext.substr(ext.size() - sequential, sequential);
                  Database.RegisterFileExtension(lc, &generated);
               }
            #endif
         }

         if constexpr (requires { T::CTTI_VersionMajor; })
            generated.mVersionMajor = T::CTTI_VersionMajor;
         if constexpr (requires { T::CTTI_VersionMinor; })
            generated.mVersionMinor = T::CTTI_VersionMinor;

         ReflectOriginType<T>(generated);
      }
      
      // Make sure we register the pointered/immutable alternatives     
      // It's paramount we do this after this meta has been generated   
      (void)MetaData::Of<T*>();
      (void)MetaData::Of<const T*>();
      (void)MetaData::Of<const T>();

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         return meta;
      #else
         return meta.get();
      #endif
   }

   /// Integrate fundamental types with the reflection system                 
   /// Like, for example, implicitly adding a A::Number bases to number types 
   template<CT::Fundamental T>
   void MetaData::ReflectFundamentalType(MetaData& generated) noexcept {
      static_assert(CT::Complete<T>, "T must be a complete type");

      if constexpr (CT::Bool<T>) {
         using Bases = TTypeList<A::Bool>;
         generated.SetBases<T>(Bases {});
      }
      else if constexpr (CT::Character<T>) {
         using Bases = TTypeList<A::Text>;
         generated.SetBases<T>(Bases {});
      }
      else if constexpr (CT::SignedInteger<T>) {
         using Bases = TTypeList<A::SignedInteger>;
         generated.SetBases<T>(Bases {});
      }
      else if constexpr (CT::UnsignedInteger<T>) {
         using Bases = TTypeList<A::UnsignedInteger>;
         generated.SetBases<T>(Bases {});
      }
      else if constexpr (CT::Real<T>) {
         using Bases = TTypeList<A::Real>;
         generated.SetBases<T>(Bases {});
      }
      else LANGULUS_ERROR("Unimplemented fundamental type reflector");
   }

   /// Reflect a dense type (members, converters, named values, bases, etc    
   ///   @tparam T - the origin type to reflect                               
   ///   @param generated - the definition to populate with the information   
   template<CT::Decayed T>
   void MetaData::ReflectOriginType(MetaData& generated) noexcept {
      static_assert(CT::Complete<T>, "T must be a complete type");

      // Reflect deepness                                               
      if constexpr (requires { T::CTTI_Deep; })
         generated.mIsDeep = T::CTTI_Deep;

      // Reflect the concrete type                                      
      if constexpr (CT::Concretizable<T>)
         generated.mConcrete = MetaData::Of<CT::ConcreteOf<T>>();

      // Reflect the producer type                                      
      if constexpr (CT::Producible<T>)
         generated.mProducer = MetaData::Of<CT::ProducerOf<T>>();

      // Wrap the default constructor of the type inside lambda         
      if constexpr (CT::Defaultable<T> && !CT::Meta<T>) {
         constexpr bool NoExcept = CT::DefaultableNoexcept<T>;
         generated.mDefaultConstructor =
            [](void* at) noexcept(NoExcept) {
               auto atT = static_cast<T*>(at);
               new (atT) T {};
            };
      }
         
      // Wrap the descriptor constructor of the type inside lambda      
      if constexpr (CT::DescriptorMakable<T> && !CT::Meta<T>) {
         constexpr bool NoExcept = CT::DescriptorMakableNoexcept<T>;
         generated.mDescriptorConstructor = 
            [](void* at, const Anyness::Descriptor& desc) noexcept(NoExcept) {
               auto atT = static_cast<T*>(at);
               new (atT) T {desc};
            };
      }

      // Wrap the copy constructor of the type inside lambda            
      if constexpr (CT::CopyMakable<T> && !CT::Meta<T>) {
         generated.mCopyConstructor = 
            [](const void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticNew<T>(toT, Copy(*fromT));
            };
      }
            
      // Wrap the clone constructor of the type inside lambda           
      if constexpr (CT::CloneMakable<T> && !CT::Meta<T>) {
         generated.mCloneConstructor = 
            [](const void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticNew<T>(toT, Clone(*fromT));
            };
      }

      // Wrap the disown constructor of the type inside lambda          
      if constexpr (CT::DisownMakable<T> && !CT::Meta<T>) {
         generated.mDisownConstructor = 
            [](const void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticNew<T>(toT, Disown(*fromT));
            };
      }

      // Wrap the move constructor of the type inside a lambda          
      if constexpr (CT::MoveMakable<T> && !CT::Meta<T>) {
         generated.mMoveConstructor = 
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticNew<T>(toT, Move(*fromT));
            };
      }

      // Wrap the abandon constructor of the type inside a lambda       
      if constexpr (CT::AbandonMakable<T> && !CT::Meta<T>) {
         generated.mAbandonConstructor = 
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticNew<T>(toT, Abandon(*fromT));
            };
      }

      // Wrap the destructor of the origin type inside a lambda         
      if constexpr (CT::Destroyable<T> && !CT::Meta<T>) {
         generated.mDestructor = 
            [](void* at) {
               auto atT = static_cast<T*>(at);
               atT->~T();
            };
      }

      // Wrap the == operator of the origin type inside a lambda        
      if constexpr (CT::Comparable<T, T>) {
         generated.mComparer = 
            [](const void* t1, const void* t2) {
               auto t1T = static_cast<const T*>(t1);
               auto t2T = static_cast<const T*>(t2);
               return *t1T == *t2T;
            };
      }

      // Wrap the copy-assignment of the type inside a lambda           
      if constexpr (CT::CopyAssignable<T> && !CT::Meta<T>) {
         generated.mCopier = 
            [](const void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticAssign(*toT, Copy(*fromT));
            };
      }

      // Wrap the disown-assignment of the type inside a lambda         
      if constexpr (CT::DisownAssignable<T> && !CT::Meta<T>) {
         generated.mDisownCopier = 
            [](const void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticAssign(*toT, Disown(*fromT));
            };
      }
            
      // Wrap the cloners of the type inside a lambda                   
      if constexpr (CT::CloneAssignable<T> && !CT::Meta<T>) {
         generated.mCloneCopier = 
            [](const void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticAssign(*toT, Clone(*fromT));
            };
      }

      // Wrap the move-assignment of the type inside a lambda           
      if constexpr (CT::MoveAssignable<T> && !CT::Meta<T>) {
         generated.mMover = 
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticAssign(*toT, Move(*fromT));
            };
      }

      // Wrap the move-assignment of the type inside a lambda           
      if constexpr (CT::AbandonAssignable<T> && !CT::Meta<T>) {
         generated.mAbandonMover = 
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               SemanticAssign(*toT, Abandon(*fromT));
            };
      }

      // Wrap the GetBlock method of the type inside a lambda           
      if constexpr (CT::Resolvable<T>) {
         generated.mResolver = 
            [](const void* at) {
               auto atT = static_cast<const T*>(at);
               return atT->GetBlock();
            };
      }

      // Wrap the GetHash() method inside a lambda                      
      if constexpr (CT::Hashable<T> || CT::Number<T> || CT::POD<T>) {
         generated.mHasher = 
            [](const void* at) {
               auto atT = static_cast<const T*>(at);
               return HashOf(*atT);
            };
      }

      // Wrap the mutable Do verb method inside a lambda                
      if constexpr (CT::DispatcherMutable<T>) {
         generated.mDispatcherMutable = 
            [](void* at, Flow::Verb& verb) {
               auto atT = static_cast<T*>(at);
               atT->Do(verb);
            };
      }

      // Wrap the constant Do verb method inside a lambda               
      if constexpr (CT::DispatcherConstant<T>) {
         generated.mDispatcherConstant = 
            [](const void* at, Flow::Verb& verb) {
               auto atT = static_cast<const T*>(at);
               atT->Do(verb);
            };
      }

      // Set reflected bases                                            
      if constexpr (requires { typename T::CTTI_Bases; })
         generated.SetBases<T>(typename T::CTTI_Bases {});
      else if constexpr (CT::Fundamental<T>)
         ReflectFundamentalType<T>(generated);

      // Set reflected abilities                                        
      if constexpr (requires { typename T::CTTI_Verbs; })
         generated.SetAbilities<T>(typename T::CTTI_Verbs {});

      // Set reflected converters                                       
      if constexpr (requires { typename T::CTTI_Conversions; })
         generated.SetConverters<T>(typename T::CTTI_Conversions {});

      // Reflected named values if this type is origin type             
      if constexpr (CT::HasNamedValues<T>) {
         static_assert(CT::Comparable<T, T>, 
            "Named values specified for type, but type instances are not comparable");

         constexpr auto c = ExtentOf<decltype(T::CTTI_NamedValues)>;
         static T staticInstances[c];
         static ::std::string staticNames[c] {};
         #if !LANGULUS_FEATURE(MANAGED_REFLECTION)
            static constinit ::std::unique_ptr<MetaConst> staticMC[c] {};
         #endif

         // Register each named values as a reflected constant          
         for (Offset i = 0; i < c; ++i) {
            staticNames[i] += generated.mToken;
            staticNames[i] += "::";
            staticNames[i] += T::CTTI_NamedValues[i].mToken;

            #if LANGULUS_FEATURE(MANAGED_REFLECTION)
               const auto cmeta = const_cast<MetaConst*>(
                  Database.RegisterConstant(staticNames[i]));
               LANGULUS_ASSERT(cmeta, Meta,
                  "Meta constant conflict on registration");
               cmeta->mLibraryName = RTTI::Boundary;
            #else
               staticMC[i] = ::std::make_unique<MetaConst>();
               const auto cmeta = staticMC[i].get();
            #endif

            cmeta->mToken = staticNames[i];
            cmeta->mInfo = T::CTTI_NamedValues[i].mInfo;
            cmeta->mCppName = cmeta->mToken;
            cmeta->mHash = HashBytes(
               cmeta->mToken.data(), 
               static_cast<int>(cmeta->mToken.size())
            );
            cmeta->mValueType = &generated;
            new (staticInstances + i) T {{T::CTTI_NamedValues[i].mValue}};
            cmeta->mPtrToValue = staticInstances + i;

            generated.mNamedValues.emplace_back(cmeta);
         }
      }

      // Set reflected members                                          
      if constexpr (requires { T::CTTI_Members; })
         generated.mMembers = T::CTTI_Members;
   }

   /// Set the list of bases for a given meta definition                      
   ///   @tparam Args... - all the bases                                      
   template<class T, CT::Dense... BASE>
   void MetaData::SetBases(TTypeList<BASE...>) noexcept {
      static const Base list[] {Base::From<Decay<T>, BASE>()...};
      mBases = {list};
   }

   /// Set the list of abilities for a given meta definition                  
   ///   @tparam Args... - all the abilities                                  
   template<CT::Dense T, CT::Dense... VERB>
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
   template<class FROM, class... TO>
   void MetaData::SetConverters(TTypeList<TO...>) noexcept {
      static const ::std::pair<DMeta, Converter> list[] {
         ::std::pair<DMeta, Converter>(
            MetaData::Of<Decay<TO>>(), Converter::From<FROM, TO>()
         )...
      };

      for (const auto& i : list)
         mConverters.insert(i);
   }

   /// Get a converter to a specific static type                              
   ///   @tparam T - the type to seek a conversion to                         
   ///   @return the conversion function                                      
   template<class T>
   LANGULUS(INLINED)
   FCopyConstruct MetaData::GetConverter() const noexcept {
      return GetConverter(MetaData::Of<T>());
   }

   /// Get a converter to a specific dynamic type                             
   ///   @param meta - the type we're converting to                           
   ///   @return the conversion function                                      
   LANGULUS(INLINED)
   FCopyConstruct MetaData::GetConverter(DMeta meta) const noexcept {
      if (!mOrigin)
         return FCopyConstruct {};

      const auto found = mOrigin->mConverters.find(meta);
      if (found != mOrigin->mConverters.end())
         return found->second.mFunction;
      return FCopyConstruct {};
   }
   
   /// Get a reflected member by trait, type, and/or offset (inner)           
   ///   @param trait - filter by trait, or nullptr if trait is irrelevant    
   ///   @param type - filter by data type, or nullptr if irrelevant          
   ///   @param offset - considers only matches after that many matches       
   ///   @return the member interface if found, or nullptr if not             
   inline const Member* MetaData::GetMemberInner(TMeta trait, DMeta type, Offset& offset) const noexcept {
      if (!mOrigin)
         return nullptr;

      // Search in all bases first                                      
      for (auto& base : mOrigin->mBases) {
         const auto found = base.mType->GetMemberInner(trait, type, offset);
         if (found)
            return found;
      }

      // Then locally                                                   
      for (auto& member : mOrigin->mMembers) {
         if (!member.TraitIs(trait) || !member.Is(type))
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
   ///   @param trait - filter by trait, or nullptr if trait is irrelevant    
   ///   @param type - filter by data type, or nullptr if irrelevant          
   ///   @param offset - considers only matches after that many matches       
   ///   @return the member interface if found, or nullptr if not             
   inline Count MetaData::GetMemberCountInner(TMeta trait, DMeta type, Offset& offset) const noexcept {
      if (!mOrigin)
         return 0;

      // Search in all bases first                                      
      Count result {};
      for (auto& base : mOrigin->mBases)
         result += base.mType->GetMemberCountInner(trait, type, offset);

      // Then locally                                                   
      for (auto& member : mOrigin->mMembers) {
         if (!member.TraitIs(trait) || !member.Is(type))
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
   ///   @param trait - filter by trait, or nullptr if trait is irrelevant    
   ///   @param type - filter by data type, or nullptr if irrelevant          
   ///   @param offset - considers only matches after that many matches       
   ///   @return the member interface if found, or nullptr if not             
   LANGULUS(INLINED)
   const Member* MetaData::GetMember(TMeta trait, DMeta type, Offset offset) const noexcept {
      return GetMemberInner(trait, type, offset);
   }

   /// Count the number of matching reflected members                         
   ///   @param trait - filter by trait, or nullptr if trait is irrelevant    
   ///   @param type - filter by data type, or nullptr if irrelevant          
   ///   @param offset - considers only matches after that many matches       
   ///   @return the number of matching members                               
   LANGULUS(INLINED)
   Count MetaData::GetMemberCount(TMeta trait, DMeta type, Offset offset) const noexcept {
      return GetMemberCountInner(trait, type, offset);
   }

   /// Count the number of reflected members, in non-imposed bases included   
   ///   @return the number of members                                        
   inline Count MetaData::GetMemberCount() const noexcept {
      if (!mOrigin)
         return 0;

      Count result = mOrigin->mMembers.size();
      for (auto& base : mOrigin->mBases)
         result += base.mType->GetMemberCount();
      return result;
   }

   /// Get the most concrete type                                             
   ///   @return the most concrete type                                       
   LANGULUS(INLINED)
   DMeta MetaData::GetMostConcrete() const noexcept {
      auto concrete = this;
      while (concrete->mConcrete)
         concrete = concrete->mConcrete;
      return concrete;
   }

   /// Get a reflected base linked to this meta data definition               
   /// Traverses the whole inheritance tree, so can return distant bases      
   ///   @param type - the type of base to search for, nullptr for any        
   ///   @param offset - use this to get bases by index                       
   ///   @param base - [in/out] base info ends up here if found               
   ///   @return true if a base is available                                  
   inline bool MetaData::GetBase(DMeta type, Offset offset, Base& base) const {
      if (!mOrigin || !type)
         return false;

      Count scanned {};
      for (auto& b : mOrigin->mBases) {
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
   ///   @tparam T - the type of base to search for, void for any             
   ///   @param offset - use this to get bases by index                       
   ///   @param base - [in/out] base info ends up here if found               
   ///   @return true if a base is available                                  
   template<CT::Data T>
   LANGULUS(INLINED)
   bool MetaData::GetBase(Offset offset, Base& base) const {
      return GetBase(MetaData::Of<T>(), offset, base);
   }

   /// A simple check if a reflected base is linked to this meta data         
   /// Traverses the whole inheritance tree, so can return distant bases      
   ///   @param type - the type of base to search for                         
   ///   @return true if a base is available                                  
   inline bool MetaData::HasBase(DMeta type) const {
      if (!mOrigin || !type)
         return false;

      for (auto& b : mOrigin->mBases) {
         if (type->Is(b.mType) || b.mType->HasBase(type))
            return true;
      }

      return false;
   }
   
   /// A simple check if a reflected base is linked to this meta data         
   /// Traverses the whole inheritance tree, so can return distant bases      
   ///   @tparam T - the type of base to search for                           
   ///   @return true if a base is available                                  
   template<CT::Data T>
   LANGULUS(INLINED)
   bool MetaData::HasBase() const {
      return HasBase(MetaData::Of<T>());
   }

   /// A simple check if this meta data has a derivation                      
   /// Traverses the whole inheritance tree, so can return distant children   
   ///   @param type - the type of derivation to search for                   
   ///   @return true if a derivation is available                            
   LANGULUS(INLINED)
   bool MetaData::HasDerivation(DMeta type) const {
      return type->HasBase(this);
   }
   
   /// A simple check if this meta data has a derivation                      
   /// Traverses the whole inheritance tree, so can return distant children   
   ///   @tparam T - the type of derivation to search for                     
   ///   @return true if a derivation is available                            
   template<CT::Data T>
   LANGULUS(INLINED)
   bool MetaData::HasDerivation() const {
      return MetaData::Of<T>()->HasBase(this);
   }

   /// Check if this data type is able to do something                        
   ///   @param verb - the verb to check if able                              
   ///   @return true if this data type is able to do verb                    
   LANGULUS(INLINED)
   bool MetaData::IsAbleTo(VMeta verb) const {
      return mAbilities.find(verb) != mAbilities.end();
   }
   
   /// Check if this data type is able to do something                        
   ///   @tparam T - the verb to check if able                                
   ///   @return true if this data type is able to do verb                    
   template<CT::Data T>
   LANGULUS(INLINED)
   bool MetaData::IsAbleTo() const {
      return IsAbleTo(MetaVerb::Of<T>());
   }

   /// Get an ability                                                         
   ///   @tparam MUTABLE - whether to get mutable/immutable overload          
   ///   @param vmeta - the type of the verb                                  
   ///   @param dmeta - the type of the verb's argument (optional)            
   ///   @return the functor if found                                         
   template<bool MUTABLE>
   auto MetaData::GetAbility(VMeta vmeta, DMeta dmeta) const {
      const auto foundv = mAbilities.find(vmeta);
      if constexpr (MUTABLE) {
         if (foundv != mAbilities.end()) {
            const auto& overloads = foundv->second.mOverloadsMutable;
            auto foundo = overloads.find({dmeta});
            if (foundo != overloads.end())
               // Specific overload is available                        
               return foundo->second;

            // Always fallback to default function, if reflected        
            foundo = overloads.find({});
            if (foundo != overloads.end())
               return foundo->second;
         }
      }
      else {
         if (foundv != mAbilities.end()) {
            const auto& overloads = foundv->second.mOverloadsConstant;
            auto foundo = overloads.find({dmeta});
            if (foundo != overloads.end())
               // Specific overloads is available                       
               return foundo->second;

            // Always fallback to default function, if reflected        
            foundo = overloads.find({});
            if (foundo != overloads.end())
               return foundo->second;
         }
      }

      // No match found, if reached                                     
      return FVerbMutable {};
   }

   /// Get an ability with static verb                                        
   ///   @tparam MUTABLE - whether to get mutable/immutable overload          
   ///   @tparam V - the type of the verb                                     
   ///   @param dmeta - the type of the verb's argument (optional)            
   ///   @return the functor if found                                         
   template<bool MUTABLE, CT::Data V>
   LANGULUS(INLINED)
   auto MetaData::GetAbility(DMeta dmeta) const {
      static_assert(CT::DerivedFrom<V, ::Langulus::Flow::Verb>,
         "V must be derived from Flow::Verb");
      return GetAbility<MUTABLE>(MetaVerb::Of<V>(), dmeta);
   }

   /// Get an ability with static verb and argument type                      
   ///   @tparam MUTABLE - whether to get mutable/immutable overload          
   ///   @tparam V - the type of the verb                                     
   ///   @tparam D - the type of the verb's argument                          
   ///   @return the functor if found                                         
   template<bool MUTABLE, CT::Data V, CT::Data... A>
   LANGULUS(INLINED)
   auto MetaData::GetAbility() const {
      static_assert(CT::DerivedFrom<V, ::Langulus::Flow::Verb>,
         "V must be derived from Flow::Verb");
      return GetAbility<MUTABLE>(MetaVerb::Of<V>(), MetaData::Of<A>()...);
   }

   /// Get the token of a reflected named value                               
   ///   @attention assumes this definition is the reflection of T            
   ///   @tparam T - type of the value to test (deducible)                    
   ///   @param value - the value to check the name of                        
   ///   @return the token for the value, or an empty token if not found      
   template<class T>
   LANGULUS(INLINED)
   Token MetaData::GetNamedValueOf(const T& value) const {
      if (!mOrigin)
         return "";

      for (auto& constant : mOrigin->mNamedValues) {
         if (value == *static_cast<const T*>(constant->mPtrToValue))
            return constant->mToken;
      }

      return "";
   }

   /// Check if this type interprets as another without conversion            
   ///   @tparam ADVANCED - whether or not to do an advanced search in the    
   ///                      opposite inheritance order                        
   ///	@tparam BINARY_COMPATIBLE - do we require for the other to be        
   ///                               binary compatible with this              
   ///	@param other - the type to try interpreting as                       
   ///	@return true if this type interprets as other                        
   template<bool ADVANCED, bool BINARY_COMPATIBLE>
   bool MetaData::CastsTo(DMeta other) const {
      if (Is(other))
         return true;

      // Different types might be compatible via inheritance            
      // All the following operations are done in the origin            
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
               return mOrigin->mResolver || found.mBinaryCompatible;
         }
      }

      // At this point we're pretty sure that types are incompatible    
      return false;
   }
   
   /// Check if this type interprets as another without conversion            
   ///   @tparam T - the type to try interpreting as                          
   ///   @tparam ADVANCED - whether or not to do an advanced search in the    
   ///                      opposite inheritance order                        
   ///   @tparam BINARY_COMPATIBLE - do we require for the other to be        
   ///                               binary compatible with this              
   ///   @return true if this type interprets as other                        
   template<CT::Data T, bool ADVANCED, bool BINARY_COMPATIBLE>
   LANGULUS(INLINED)
   bool MetaData::CastsTo() const {
      return CastsTo<ADVANCED, BINARY_COMPATIBLE>(MetaData::Of<T>());
   }

   /// Check if this type interprets as an exact number of another, without   
   /// any conversion                                                         
   ///   @tparam BINARY_COMPATIBLE - do we require for the other to be        
   ///                               binary compatible with this              
   ///   @param other - the type to try interpreting as                       
   ///   @param count - the number of items to interpret as                   
   ///   @return true if this type interprets as other                        
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
   ///   @tparam BINARY_COMPATIBLE - do we require for the other to be        
   ///                               binary compatible with this              
   ///   @tparam T - the type to try interpreting as                          
   ///   @return true if this type interprets as other                        
   template<CT::Data T, bool BINARY_COMPATIBLE>
   LANGULUS(INLINED)
   bool MetaData::CastsTo(Count count) const {
      return CastsTo<BINARY_COMPATIBLE>(MetaData::Of<T>(), count);
   }

   /// Check if this type is either same, base or a derivation of other       
   ///   @param other - the type to check                                     
   ///   @return true if this type is related to other                        
   LANGULUS(INLINED)
   bool MetaData::IsRelatedTo(DMeta other) const {
      return Is(other) || HasBase(other) || HasDerivation(other);
   }
   
   /// Check if this type is either same, base or a derivation of other       
   ///   @tparam T - the type to check                                        
   ///   @return true if this type is related to other                        
   template<CT::Data T>
   LANGULUS(INLINED)
   bool MetaData::IsRelatedTo() const {
      return IsRelatedTo(MetaData::Of<T>());
   }

   /// Get the number of conversions required to map one type to another      
   ///   @param other - the type to check distance to                         
   ///   @return the distance                                                 
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
   ///   @tparam T - the type to check distance to                            
   ///   @return the distance                                                 
   template<CT::Data T>
   LANGULUS(INLINED)
   MetaData::Distance MetaData::GetDistanceTo() const {
      return GetDistanceTo(MetaData::Of<T>());
   }

   /// Check if two meta definitions match loosely, ignoring all qualifiers   
   ///   @param other - the type to compare against                           
   ///   @return true if types match                                          
   LANGULUS(INLINED)
   constexpr bool MetaData::Is(DMeta other) const noexcept {
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // This function is reduced to a pointer match, if the meta    
         // database is centralized, because it guarantees that         
         // definitions in separate translation units are always the    
         // same instance                                               
         return mOrigin && other && mOrigin == other->mOrigin;
      #else
         return other
            && mOrigin && other->mOrigin
            && mOrigin->mHash == other->mOrigin->mHash
            && mOrigin->mToken == other->mOrigin->mToken;
      #endif
   }
   
   /// Check if two meta definitions match loosely, ignoring all qualifiers   
   ///   @tparam T - the type to compare against                              
   ///   @return true if types match                                          
   template<CT::Data T>
   LANGULUS(INLINED)
   constexpr bool MetaData::Is() const {
      return Is(MetaData::Of<T>());
   }
   
   /// Check if two meta definitions match exactly                            
   ///   @param other - the type to compare against                           
   ///   @return true if types match                                          
   LANGULUS(INLINED)
   constexpr bool MetaData::IsExact(DMeta other) const noexcept {
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
   ///   @tparam T - the type to compare against                              
   ///   @return true if types match                                          
   template<CT::Data T>
   LANGULUS(INLINED)
   constexpr bool MetaData::IsExact() const {
      return IsExact(MetaData::Of<T>());
   }

   LANGULUS(INLINED)
   constexpr bool MetaData::operator == (const MetaData& rhs) const noexcept {
      return IsExact(&rhs);
   }

   /// Get a size based on reflected allocation page and count (unsafe)       
   ///   @attention assumes byteSize is not zero                              
   ///   @param count - the number of elements to request                     
   ///   @returns both the provided byte size and reserved count              
   LANGULUS(INLINED)
   AllocationRequest MetaData::RequestSize(const Count& count) const noexcept {
      AllocationRequest result;
      result.mByteSize = Roof2(::std::max(count * mSize, mAllocationPage));
      const auto msb = CountTrailingZeroes(result.mByteSize);
      result.mElementCount = mAllocationTable[msb];
      return result;
   }
   
   /// Check if two meta definitions match exactly                            
   ///   @param other - the type to compare against                           
   ///   @return true if types match                                          
   LANGULUS(INLINED)
   constexpr bool MetaConst::Is(CMeta other) const noexcept {
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
   
   LANGULUS(INLINED)
   constexpr bool MetaConst::operator == (const MetaConst& rhs) const noexcept {
      return Is(&rhs);
   }

} // namespace Langulus::RTTI
