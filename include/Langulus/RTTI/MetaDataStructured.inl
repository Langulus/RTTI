///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once

#if not LANGULUS_FEATURE(MANAGED_REFLECTION)
   #error "This file shouldn't be included if MANAGED_REFLECTION feature is disabled"
#endif

#define TEMPLATE() template<uint ID_SIZE, uint PT_SIZE>
#define ME() MetaDataStructured_XY<ID_SIZE, PT_SIZE>


namespace Langulus::RTTI::Inner
{
   /// Empty data ID construction                                             
   TEMPLATE()
   constexpr ME()::MetaDataStructured_XY(nullptr_t) noexcept
      : Base {0} {}

   /// ID from definition                                                     
   TEMPLATE()
   constexpr ME()::MetaDataStructured_XY(DefinitionData const* d) noexcept
      : Base {d ? d->mID : 0} {
      if (d) {
         sparse     = d->mDeptr != nullptr;
         constant   = d->mConst;
         deep       = d->mDeep;
         pod        = d->mPOD;
         nullable   = d->mNullable;
         referenced = d->mCurrentBoundary.mReferencer != nullptr;
         resolvable = d->mCurrentBoundary.mResolver != nullptr;
         dispatcher = d->mCurrentBoundary.mDispatcher != nullptr;

         if constexpr (PT_SIZE > 1) {
            if (d->mSize < (1 << sizeof(Structured<PT_SIZE>::size) * 8))
               Structured<PT_SIZE>::size = d->mSize;
         }
      }
   }

   /// Reset data ID                                                          
   TEMPLATE()
   constexpr auto ME()::operator = (nullptr_t)
   noexcept -> MetaDataStructured_XY& {
      Base::operator = (0);
      return *this;
   }

   /// Reassign data ID                                                       
   TEMPLATE()
   constexpr auto ME()::operator = (DefinitionData const* d)
   noexcept -> MetaDataStructured_XY& {
      Base::operator = (d ? d->mID : 0);

      if (d) {
         sparse     = d->mConst;
         constant   = d->mConst;
         deep       = d->mDeep;
         pod        = d->mPOD;
         nullable   = d->mNullable;
         referenced = d->mCurrentBoundary.mReferencer != nullptr;
         resolvable = d->mCurrentBoundary.mResolver != nullptr;
         dispatcher = d->mCurrentBoundary.mDispatcher != nullptr;

         if constexpr (PT_SIZE > 1) {
            if (d->mSize < (1 << sizeof(Structured<PT_SIZE>::size) * 8))
               Structured<PT_SIZE>::size = d->mSize;
         }
      }
      return *this;
   }

   /// Get definition                                                         
   TEMPLATE()
   auto ME()::GetDefinition() const noexcept
   -> DefinitionData const* {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant);
      return nullptr;
   }

   /// Check if type origins match.                                           
   /// Disregards all cv-qualifiers, pointers, array extents, etc.            
   ///   @param other the type to compare against                             
   ///   @return true if types match                                          
   TEMPLATE()
   bool ME()::Is(const MetaDataStructured_XY& other) const noexcept {
      const auto lid = Base::GetID();
      const auto rid = other.Base::GetID();
      if (lid == rid)
         return true;
      if (lid and rid) {
         auto o1 = Registry::GetMetaDataByID(lid,       sparse,       constant)->mOrigin;
         auto o2 = Registry::GetMetaDataByID(rid, other.sparse, other.constant)->mOrigin;
         return o1 == o2 and o1;
         //TODO figure this out: types can't be akin if incomplete, because we can't reflect incomplete types rn.
         // this is inconsistent, because Akin actually allows incomplete types in C++.
         // so reflect incomplete types, too? just... don't cache their properties?
         // this would also simplify that pesky mDeptr == 1 all over the code, when a pointer to incomplete type...
      }
      return false;
   }

   /// Check if two meta definitions match exactly                            
   ///   @param other the type to compare against                             
   ///   @return true if types match                                          
   TEMPLATE()
   constexpr bool ME()::IsExact(const MetaDataStructured_XY& other) const noexcept {
      return all == other.all and Base::operator == (other);
   }

   TEMPLATE()
   constexpr bool ME()::operator==(const MetaDataStructured_XY& other) const noexcept {
      return IsExact(other);
   }
   
   /// Check if two meta definitions match origin and sparseness, but ignores 
   /// `const` and `volatile` qualifiers. The qualifiers aren't ignored only  
   /// on the current level of indirection, but on the entire way to origin.  
   ///   @param other the type to compare against                             
   ///   @return true if types match                                          
   TEMPLATE()
   constexpr bool ME()::IsSame(const MetaDataStructured_XY& other) const noexcept {
      const auto lid = Base::GetID();
      const auto rid = other.Base::GetID();
      if (lid and rid) {
         auto o1 = Registry::GetMetaDataByID(lid,       sparse,       constant)->mDecvqAll;
         auto o2 = Registry::GetMetaDataByID(rid, other.sparse, other.constant)->mDecvqAll;
         return o1 == o2;
      }
      return not lid and not rid;
   }

   /// Get the size of the type                                               
   TEMPLATE()
   constexpr auto ME()::GetSize() const noexcept -> size_t {
      const auto id = Base::GetID();
      if (id) {
         if constexpr (PT_SIZE > 1) {
            return Structured<PT_SIZE>::size
               ? Structured<PT_SIZE>::size
               : Registry::GetMetaDataByID(id, sparse, constant)->mSize;
         }
         else return Registry::GetMetaDataByID(id, sparse, constant)->mSize;
      }
      return 0;
   }

   /// Get the alignment of the type                                          
   TEMPLATE()
   auto ME()::GetAlignment() const noexcept -> pot_t {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mAlign;
      return pot_t(Alignment);
   }

   /// Get the name of the type, the result of NameOf                         
   TEMPLATE()
   auto ME()::GetName() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mNameOf;
      return DefinitionData::InvalidName;
   }
   
   /// Get the info of the type, the result of InfoOf                         
   TEMPLATE()
   auto ME()::GetInfo() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mInfoOf;
      return {};
   }

   /// Get the name of the type as it appears in C++                          
   TEMPLATE()
   auto ME()::GetCppName() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCppNameOf;
      return {};
   }

   /// Get the type hash                                                      
   TEMPLATE()
   auto ME()::GetHash() const noexcept -> Hash {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mHash;
      return {};
   }

   /// Get the associated file extensions, separated with commas              
   TEMPLATE()
   auto ME()::GetFiles() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mFilesOf;
      return {};
   }

   /// Get the associated suffix                                              
   TEMPLATE()
   auto ME()::GetSuffix() const noexcept -> Token {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mSuffixOf;
      return {};
   }

   /// Get the type boundaries                                                
   TEMPLATE()
   auto ME()::GetBoundaries() const noexcept -> Definition::BoundarySet const& {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mBoundaries;

      static const Definition::BoundarySet fallback;
      return fallback;
   }

   /// Get the major version                                                  
   TEMPLATE()
   auto ME()::GetVersionMajor() const noexcept -> uint {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mVersionMajor;
      return {};
   }

   /// Get the minor version                                                  
   TEMPLATE()
   auto ME()::GetVersionMinor() const noexcept -> uint {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mVersionMinor;
      return {};
   }
   
   /// Get the minimal allocation size in bytes                               
   TEMPLATE()
   auto ME()::GetMinAllocation() const noexcept -> pot_t {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mMinimalAllocation;
      return pot_t(MinimalAllocation);
   }

   /// Get the precomputed allocation table for the type                      
   TEMPLATE()
   auto ME()::GetAllocationTable() const noexcept -> size_t const* {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mAllocationTable;
      return {};
   }

   #if LANGULUS_FEATURE(MANAGED_MEMORY)
      /// Get the reflected allocation page                                   
      TEMPLATE()
      auto ME()::GetMinPoolsize() const noexcept -> pot_t {
         const auto id = Base::GetID();
         if (id)
            return Registry::GetMetaDataByID(id, sparse, constant)->mMinimalPoolSize;
         return pot_t(MinimalPoolSize);
      }
   
      /// Get the reflected pool tactic                                       
      TEMPLATE()
      auto ME()::GetPoolTactic() const noexcept -> PoolTactic {
         const auto id = Base::GetID();
         if (id)
            return Registry::GetMetaDataByID(id, sparse, constant)->mPoolTactic;
         return PoolTactic::Default;
      }
   #endif

   /// Count the number of indirections.                                      
   /// int**** will result in 4; int* will result in 1, int will result in 0. 
   TEMPLATE()
   constexpr size_t ME()::GetIndirections() const noexcept {
      const auto id = Base::GetID();
      if (not id or not sparse)
         return 0;

      auto d = Registry::GetMetaDataByID(id, sparse, constant);
      size_t result = 0;
      while (d->mDeptr) {
         if (reinterpret_cast<intptr_t>(d->mDeptr) == 1)
            return result + 1;
         ++result;
         d = d->mDeptr;
      }
      return result;
   }

   /// Count the number of indirections, and check if custom pointers are     
   /// involved.                                                              
   /// int**** will result in 4; int* will result in 1, int will result in 0. 
   TEMPLATE()
   constexpr size_t ME()::GetIndirections(bool& custom_pointer_encountered) const noexcept {
      custom_pointer_encountered = false;
      const auto id = Base::GetID();
      if (not id or not sparse)
         return 0;

      auto d = Registry::GetMetaDataByID(id, sparse, constant);
      size_t result = 0;
      while (d->mDeptr) {
         if (d->mPointerSpecification.IsPacked())
            custom_pointer_encountered = true;
         if (reinterpret_cast<intptr_t>(d->mDeptr) == 1)
            return result + 1;
         ++result;
         d = d->mDeptr;
      }
      return result;
   }

   /// Get the pointer specification for a sparse type                        
   TEMPLATE()
   constexpr auto ME()::GetPointerSpecification() const noexcept -> PointerSpecification {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mPointerSpecification;
      return {};
   }

   /// Check if type is CT::Dense                                             
   TEMPLATE()
   constexpr bool ME()::IsDense() const noexcept {
      return not sparse;
   }

   /// Check if type is CT::Sparse                                            
   TEMPLATE()
   constexpr bool ME()::IsSparse() const noexcept {
      return sparse;
   }

   /// Check if the type is CT::Constant                                      
   TEMPLATE()
   constexpr bool ME()::IsConstant() const noexcept {
      return constant;
   }

   /// Check if the type is CT::Mutable                                       
   TEMPLATE()
   constexpr bool ME()::IsMutable() const noexcept {
      return not constant;
   }

   /// Check if type is CT::Deep                                              
   TEMPLATE()
   constexpr bool ME()::IsDeep() const noexcept {
      return deep;
   }

   /// Check if type is CT::POD                                               
   TEMPLATE()
   constexpr bool ME()::IsPOD() const noexcept {
      return pod;
   }

   /// Check if type is CT::Nullable                                          
   TEMPLATE()
   constexpr bool ME()::IsNullable() const noexcept {
      return nullable;
   }

   /// Check if type is CT::Abstract                                          
   TEMPLATE()
   constexpr bool ME()::IsAbstract() const noexcept {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mAbstract;
      return false;
   }

   /// Check if type is CT::Executable                                        
   TEMPLATE()
   constexpr bool ME()::IsExecutable() const noexcept {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mExecutable;
      return false;
   }

   /// Check if type has an explicit GetHash() method                         
   TEMPLATE()
   constexpr bool ME()::HasGetHashMethod() const noexcept {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mHasGetHashMethod;
      return {};
   }
   
   /// Get the reflected destructor                                           
   TEMPLATE()
   auto ME()::GetDestructor()
   const noexcept -> DefinitionData::FUnary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mDestructor;
      return {};
   }
   
   /// Get the reflected dereffer                                             
   TEMPLATE()
   auto ME()::GetDereffer()
   const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mDereference;
      return {};
   }

   /// Get the reflected referencer                                           
   TEMPLATE()
   auto ME()::GetReferencer()
   const noexcept -> DefinitionData::FReference {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mReferencer;
      return {};
   }

   /// Get the reflected resolver                                             
   TEMPLATE()
   auto ME()::GetResolver()
   const noexcept -> DefinitionData::FResolve {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mResolver;
      return {};
   }

   /// Get the reflected default constructor                                  
   TEMPLATE()
   auto ME()::GetDefaultConstructor() const noexcept -> DefinitionData::FUnary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mDefaultConstructor;
      return {};
   }
   
   /// Get the reflected describe-constructo                                  
   TEMPLATE()
   auto ME()::GetDescribeConstructor() const noexcept -> DefinitionData::FDescribe {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mDescribeConstructor;
      return {};
   }   

   /// Get the reflected refer-constructor                                    
   TEMPLATE()
   auto ME()::GetReferConstructor() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mReferConstructor;
      return {};
   }

   /// Get the reflected refer-assigner                                       
   TEMPLATE()
   auto ME()::GetReferAssigner() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mReferAssigner;
      return {};
   }

   /// Get the reflected move-constructor                                     
   TEMPLATE()
   auto ME()::GetMoveConstructor() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mMoveConstructor;
      return {};
   }

   /// Get the reflected move-assigner                                        
   TEMPLATE()
   auto ME()::GetMoveAssigner() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mMoveAssigner;
      return {};
   }

   /// Get the reflected abandon-constructor                                  
   TEMPLATE()
   auto ME()::GetAbandonConstructor() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mAbandonConstructor;
      return {};
   }

   /// Get the reflected abandon-assigner                                     
   TEMPLATE()
   auto ME()::GetAbandonAssigner() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mAbandonAssigner;
      return {};
   }

   /// Get the reflected disown-constructor                                   
   TEMPLATE()
   auto ME()::GetDisownConstructor() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mDisownConstructor;
      return {};
   }

   /// Get the reflected disown-assigner                                      
   TEMPLATE()
   auto ME()::GetDisownAssigner() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mDisownAssigner;
      return {};
   }

   /// Get the reflected clone-constructor                                    
   TEMPLATE()
   auto ME()::GetCloneConstructor() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mCloneConstructor;
      return {};
   }

   /// Get the reflected clone-assigner                                       
   TEMPLATE()
   auto ME()::GetCloneAssigner() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mCloneAssigner;
      return {};
   }

   /// Get the reflected copy-constructor                                     
   TEMPLATE()
   auto ME()::GetCopyConstructor() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mCopyConstructor;
      return {};
   }

   /// Get the reflected copy-assigner                                        
   TEMPLATE()
   auto ME()::GetCopyAssigner() const noexcept -> DefinitionData::FBinary {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mCopyAssigner;
      return {};
   }

   /// Get the reflected comparer                                             
   TEMPLATE()
   auto ME()::GetComparer() const noexcept -> DefinitionData::FCompare {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mComparer;
      return {};
   }

   /// Get the reflected comparer                                             
   TEMPLATE()
   auto ME()::GetComparerEqual() const noexcept -> DefinitionData::FCompareEqual {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mComparerEqual;
      return {};
   }

   /// Get the reflected hasher                                               
   TEMPLATE()
   auto ME()::GetHasher() const noexcept -> DefinitionData::FHash {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mHasher;
      return {};
   }

   /// Get the reflected dispatcher                                           
   TEMPLATE()
   auto ME()::GetDispatcher() const noexcept -> DefinitionData::FDispatch {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mDispatcher;
      return {};
   }

   /// Remove a layer of indirection                                          
   ///   @attention will return invalid meta if type is incomplete            
   TEMPLATE()
   auto ME()::GetDeptr(size_t count) const -> MetaDataStructured_XY {
      if (not count)
         return *this;

      const auto id = Base::GetID();
      if (id) {
         auto d = Registry::GetMetaDataByID(id, sparse, constant);
         MetaDataStructured_XY result =
            d->mDeptr <= reinterpret_cast<DefinitionData*>(intptr_t {1})
            ? nullptr : d->mDeptr;
         return result.GetDeptr(count - 1);
      }
      return {};
   }
   
   /// Get the origin type, if complete                                       
   /// The origin type has all indirections and qualifiers removed            
   TEMPLATE()
   auto ME()::GetOrigin() const -> MetaDataStructured_XY {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mOrigin;
      return {};
   }
   
   /// Strip all qualifiers from all levels of indirection                    
   TEMPLATE()
   auto ME()::GetDecvqAll() const -> MetaDataStructured_XY {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mDecvqAll;
      return {};
   }
   
   /// Strip topmost qualifiers                                               
   TEMPLATE()
   auto ME()::GetDecvq() const -> MetaDataStructured_XY {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mDecvqOnce;
      return {};
   }
   
   /// Add a level of indirection to the type                                 
   ///   @attention this is possible only if that level of indirection has    
   ///      already been reflected at runtime prior to calling this function  
   TEMPLATE()
   auto ME()::AddPtr() const -> MetaDataStructured_XY {
      const auto id = Base::GetID();
      if (id) {
         auto ptr = Registry::GetMetaDataByID(id, sparse, constant)->mAddPtr;
         LglsAssert(ptr, "Pointer type hasn't been reflected yet");
         return ptr;
      }
      return {};
   }
   
   /// Add a constant qualifier to the type                                   
   ///   @attention this is possible only if the qualified type has           
   ///      already been reflected at runtime prior to calling this function  
   TEMPLATE()
   auto ME()::AddConst() const -> MetaDataStructured_XY {
      const auto id = Base::GetID();
      if (id) {
         auto cnst = Registry::GetMetaDataByID(id, sparse, constant)->mAddConst;
         LglsAssert(cnst, "Constant type hasn't been reflected yet");
         return cnst;
      }
      return {};
   }
   
   /// Get the default concretization for an abstract type                    
   TEMPLATE()
   auto ME()::GetConcrete() const -> MetaDataStructured_XY {
      const auto id = Base::GetID();
      if (id) {
         auto d = Registry::GetMetaDataByID(id, sparse, constant);
         return d->mCurrentBoundary.mConcrete
            ? d->mCurrentBoundary.mConcrete()
            : nullptr;
      }
      return {};
   }
   
   /// Get the runtime producer of the type, if any                           
   TEMPLATE()
   auto ME()::GetProducer() const -> MetaDataStructured_XY {
      const auto id = Base::GetID();
      if (id) {
         auto d = Registry::GetMetaDataByID(id, sparse, constant);
         return d->mCurrentBoundary.mProducer
            ? d->mCurrentBoundary.mProducer()
            : nullptr;
      }
      return {};
   }

   /// Get the reflected bases                                                
   TEMPLATE()
   auto ME()::GetBases() const noexcept -> DefinitionData::BaseList const& {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mBases;

      static const DefinitionData::BaseList fallback;
      return fallback;
   }
   
   /// Get the reflected verbs                                                
   TEMPLATE()
   auto ME()::GetVerbs() const noexcept -> DefinitionData::VerbList const& {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mVerbs;

      static const DefinitionData::VerbList fallback;
      return fallback;
   }
   
   /// Get the reflected members                                              
   TEMPLATE()
   auto ME()::GetMembers() const noexcept -> DefinitionData::MemberList const& {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mMembers;

      static const DefinitionData::MemberList fallback;
      return fallback;
   }
   
   /// Get the reflected named values                                         
   TEMPLATE()
   auto ME()::GetNamedValues()
   const noexcept -> DefinitionData::ValuesList const& {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mNamedValues;

      static const DefinitionData::ValuesList fallback;
      return fallback;
   }
   
   /// Get morphisms to other types                                           
   TEMPLATE()
   auto ME()::GetMorphismsTo()
   const noexcept -> DefinitionData::MorphismList const& {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mMorphismsTo;

      static const DefinitionData::MorphismList fallback;
      return fallback;
   }
   
   /// Get morphisms from other types                                         
   /*TEMPLATE()
   auto ME()::GetMorphismsFrom()
   const noexcept -> DefinitionData::MorphismList const& {
      const auto id = Base::GetID();
      if (id)
         return Registry::GetMetaDataByID(id, sparse, constant)->mCurrentBoundary.mMorphismsFrom;

      static const DefinitionData::MorphismList fallback;
      return fallback;
   }*/

   /// Get a specific coverter, if it exists.                                 
   /// This will check if this type has a morphism TO the desired type, or    
   /// the desired type has a morphism FROM this type.                        
   TEMPLATE()
   auto ME()::GetMorphism(MetaDataStructured_XY to)
   const noexcept -> DefinitionData::Morphism {
      const auto from_id = Base::GetID();
      const auto to_id = to.Base::GetID();
      if (not from_id or not to_id)
         return {nullptr, nullptr};

      const auto dfrom = Registry::GetMetaDataByID(from_id, sparse, constant);
      const auto dto = Registry::GetMetaDataByID(to_id, to.sparse, to.constant);
      auto found = dfrom->mCurrentBoundary.mMorphismsTo.find(dto->mDecvqAll);
      if (found != dfrom->mCurrentBoundary.mMorphismsTo.end())
         return found->second;

      /*found = dto->mCurrentBoundary.mMorphismsFrom.find(dfrom->mDecvqAll);
      if (found != dto->mCurrentBoundary.mMorphismsFrom.end())
         return found->second;*/
      return {nullptr, nullptr};
   }

#if LANGULUS(SAFE)
   TEMPLATE() ME()::operator bool() const noexcept {
      if (Base::operator bool()) {
         LglsAssert(GetDefinition(), "Valid meta with invalid definition");
         return true;
      }
      return false;
   }
#endif
}

#undef TEMPLATE
#undef ME
