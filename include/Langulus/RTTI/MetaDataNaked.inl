///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "MetaData.hpp"


namespace Langulus::RTTI::Inner
{
   /// Check if type origins match.                                           
   /// Disregards all cv-qualifiers, pointers, array extents, etc.            
   ///   @param other the type to compare against                             
   ///   @return true if types match                                          
   inline bool MetaDataNaked::Is(const MetaDataNaked& other) const noexcept {
      return mDefinition == other.mDefinition or (
                mDefinition and other.mDefinition and mDefinition->mOrigin
            and mDefinition->mOrigin == other.mDefinition->mOrigin
         );
   }

   /// Check if two meta definitions match origin and sparseness, but ignores 
   /// `const` and `volatile` qualifiers. The qualifiers aren't ignored only  
   /// on the current level of indirection, but on the entire way to origin.  
   ///   @param other the type to compare against                             
   ///   @return true if types match                                          
   inline bool MetaDataNaked::IsSame(const MetaDataNaked& other) const noexcept {
      return mDefinition == other.mDefinition or (
                mDefinition and other.mDefinition
            and mDefinition->mDecvqAll == other.mDefinition->mDecvqAll
         );
   }

   /// Get the size of the type                                               
   inline auto MetaDataNaked::GetSize() const noexcept -> size_t {
      return mDefinition ? mDefinition->mSize : 0;
   }

   /// Get the alignment of the type                                          
   inline auto MetaDataNaked::GetAlignment() const noexcept -> pot_t {
      return mDefinition ? mDefinition->mAlign : pot_t(Alignment);
   }

   /// Get the reflected file extensions, separated with commas               
   inline auto MetaDataNaked::GetFiles() const noexcept -> Token {
      return mDefinition ? mDefinition->mFilesOf : Token{};
   }

   /// Get the reflected suffix                                               
   inline auto MetaDataNaked::GetSuffix() const noexcept -> Token {
      return mDefinition ? mDefinition->mSuffixOf : Token{};
   }

   /// Get the minimal allocation of the type in bytes                        
   inline auto MetaDataNaked::GetMinAllocation() const noexcept -> pot_t {
      return mDefinition ? mDefinition->mMinimalAllocation : pot_t(MinimalAllocation);
   }

   /// Get the precomputed allocation table for the type                      
   inline auto MetaDataNaked::GetAllocationTable() const noexcept -> size_t const* {
      return mDefinition ? mDefinition->mAllocationTable : nullptr;
   }

#if LANGULUS_FEATURE(MANAGED_MEMORY)
   /// Get the minimal allocation page                                        
   inline auto MetaDataNaked::GetMinPoolsize() const noexcept -> pot_t {
      return mDefinition ? mDefinition->mMinimalPoolSize : pot_t(MinimalPoolSize);
   }
   
   /// Get the reflected pool tactic                                          
   inline auto MetaDataNaked::GetPoolTactic() const noexcept -> PoolTactic {
      return mDefinition ? mDefinition->mPoolTactic : PoolTactic::Default;
   }
#endif

   /// Count the number of indirections.                                      
   /// int**** will result in 4; int* will result in 1, int will result in 0. 
   inline size_t MetaDataNaked::GetIndirections() const noexcept {
      if (not mDefinition)
         return 0;

      size_t result = 0;
      auto d = mDefinition;
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
   inline size_t MetaDataNaked::GetIndirections(bool& custom_pointer_encountered) const noexcept {
      custom_pointer_encountered = false;
      if (not mDefinition)
         return 0;

      size_t result = 0;
      auto d = mDefinition;
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

   /// Get the pointer specification of a sparse type                         
   inline auto MetaDataNaked::GetPointerSpecification() const noexcept -> PointerSpecification {
      return mDefinition ? mDefinition->mPointerSpecification : PointerSpecification {};
   }

   /// Check if type is CT::Dense                                             
   inline bool MetaDataNaked::IsDense() const noexcept {
      return mDefinition ? mDefinition->mDeptr == nullptr : true;
   }

   /// Check if type is CT::Sparse                                            
   inline bool MetaDataNaked::IsSparse() const noexcept {
      return mDefinition ? mDefinition->mDeptr != nullptr : false;
   }

   /// Check if the type is CT::Constant                                      
   inline bool MetaDataNaked::IsConstant() const noexcept {
      return mDefinition ? mDefinition->mConst : false;
   }

   /// Check if the type is CT::Mutable                                       
   inline bool MetaDataNaked::IsMutable() const noexcept {
      return mDefinition ? not mDefinition->mConst : true;
   }

   /// Check if type is CT::Deep                                              
   inline bool MetaDataNaked::IsDeep() const noexcept {
      return mDefinition ? mDefinition->mDeep : false;
   }

   /// Check if type is CT::POD                                               
   inline bool MetaDataNaked::IsPOD() const noexcept {
      return mDefinition ? mDefinition->mPOD : false;
   }

   /// Check if type is CT::Nullable                                          
   inline bool MetaDataNaked::IsNullable() const noexcept {
      return mDefinition ? mDefinition->mNullable : false;
   }

   /// Check if type is CT::Abstract                                          
   inline bool MetaDataNaked::IsAbstract() const noexcept {
      return mDefinition ? mDefinition->mAbstract : false;
   }

   /// Check if type is CT::Executable                                        
   inline bool MetaDataNaked::IsExecutable() const noexcept {
      return mDefinition ? mDefinition->mExecutable : false;
   }

   /// Check if type has an explicit GetHash() method                         
   inline bool MetaDataNaked::HasGetHashMethod() const noexcept {
      return mDefinition ? mDefinition->mHasGetHashMethod : false;
   }
   
   /// Get the reflected dereffer                                             
   inline auto MetaDataNaked::GetDereffer()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mDereference : nullptr;
   }
   
   /// Get the reflected destructor                                           
   inline auto MetaDataNaked::GetDestructor()
   const noexcept -> DefinitionData::FUnary {
      return mDefinition ? mDefinition->mCurrentBoundary.mDestructor : nullptr;
   }

   /// Get the reflected referencer                                           
   inline auto MetaDataNaked::GetReferencer()
   const noexcept -> DefinitionData::FReference {
      return mDefinition ? mDefinition->mCurrentBoundary.mReferencer : nullptr;
   }

   /// Get the reflected resolver                                             
   inline auto MetaDataNaked::GetResolver()
   const noexcept -> DefinitionData::FResolve {
      return mDefinition ? mDefinition->mCurrentBoundary.mResolver : nullptr;
   }

   /// Get the reflected default-constructor                                  
   inline auto MetaDataNaked::GetDefaultConstructor()
   const noexcept -> DefinitionData::FUnary {
      return mDefinition ? mDefinition->mCurrentBoundary.mDefaultConstructor : nullptr;
   }
   
   /// Get the reflected describe-constructor                                 
   inline auto MetaDataNaked::GetDescribeConstructor()
   const noexcept -> DefinitionData::FDescribe {
      return mDefinition ? mDefinition->mCurrentBoundary.mDescribeConstructor : nullptr;
   }
   
   /// Get the reflected refer-constructor                                    
   inline auto MetaDataNaked::GetReferConstructor()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mReferConstructor : nullptr;
   }

   /// Get the reflected refer-assigner                                       
   inline auto MetaDataNaked::GetReferAssigner()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mReferAssigner : nullptr;
   }

   /// Get the reflected move-constructor                                     
   inline auto MetaDataNaked::GetMoveConstructor()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mMoveConstructor : nullptr;
   }

   /// Get the reflected move-assigner                                        
   inline auto MetaDataNaked::GetMoveAssigner()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mMoveAssigner : nullptr;
   }

   /// Get the reflected abandon-constructor                                  
   inline auto MetaDataNaked::GetAbandonConstructor()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mAbandonConstructor : nullptr;
   }

   /// Get the reflected abandon-assigner                                     
   inline auto MetaDataNaked::GetAbandonAssigner()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mAbandonAssigner : nullptr;
   }

   /// Get the reflected disown-constructor                                   
   inline auto MetaDataNaked::GetDisownConstructor()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mDisownConstructor : nullptr;
   }

   /// Get the reflected disown-assigner                                      
   inline auto MetaDataNaked::GetDisownAssigner()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mDisownAssigner : nullptr;
   }

   /// Get the reflected clone-constructor                                    
   inline auto MetaDataNaked::GetCloneConstructor()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mCloneConstructor : nullptr;
   }

   /// Get the reflected clone-assigner                                       
   inline auto MetaDataNaked::GetCloneAssigner()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mCloneAssigner : nullptr;
   }

   /// Get the reflected copy-constructor                                     
   inline auto MetaDataNaked::GetCopyConstructor()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mCopyConstructor : nullptr;
   }

   /// Get the reflected copy-assigner                                        
   inline auto MetaDataNaked::GetCopyAssigner()
   const noexcept -> DefinitionData::FBinary {
      return mDefinition ? mDefinition->mCurrentBoundary.mCopyAssigner : nullptr;
   }

   /// Get the reflected comparer                                             
   inline auto MetaDataNaked::GetComparer()
   const noexcept -> DefinitionData::FCompare {
      return mDefinition ? mDefinition->mCurrentBoundary.mComparer : nullptr;
   }

   /// Get the reflected comparer                                             
   inline auto MetaDataNaked::GetComparerEqual()
   const noexcept -> DefinitionData::FCompareEqual {
      return mDefinition ? mDefinition->mCurrentBoundary.mComparerEqual : nullptr;
   }

   /// Get the reflected hasher                                               
   inline auto MetaDataNaked::GetHasher()
   const noexcept -> DefinitionData::FHash {
      return mDefinition ? mDefinition->mCurrentBoundary.mHasher : nullptr;
   }

   /// Get the reflected dispatcher                                           
   inline auto MetaDataNaked::GetDispatcher()
   const noexcept -> DefinitionData::FDispatch {
      return mDefinition ? mDefinition->mCurrentBoundary.mDispatcher : nullptr;
   }

   /// Remove a layer of indirection                                          
   ///   @attention will return invalid meta if type is incomplete            
   inline auto MetaDataNaked::GetDeptr(size_t count) const -> MetaDataNaked {
      if (not count)
         return *this;

      if (mDefinition) {
         MetaDataNaked result =
            mDefinition->mDeptr <= reinterpret_cast<DefinitionData*>(intptr_t{1})
            ? nullptr : mDefinition->mDeptr;
         return result.GetDeptr(count - 1);
      }
      return {};
   }

   /// Get the origin type, if complete.                                      
   /// The origin type has all indirections and qualifiers removed.           
   inline auto MetaDataNaked::GetOrigin() const -> MetaDataNaked {
      return mDefinition ? mDefinition->mOrigin : nullptr;
   }

   /// Strip all qualifiers from all levels of indirection                    
   inline auto MetaDataNaked::GetDecvqAll() const -> MetaDataNaked {
      return mDefinition ? mDefinition->mDecvqAll : nullptr;
   }

   /// Strip topmost qualifiers                                               
   inline auto MetaDataNaked::GetDecvq() const -> MetaDataNaked {
      return mDefinition ? mDefinition->mDecvqOnce : nullptr;
   }

   /// Add a level of indirection to the type                                 
   ///   @attention this is possible only if that level of indirection has    
   ///      already been reflected at runtime prior to calling this function  
   inline auto MetaDataNaked::AddPtr() const -> MetaDataNaked {
      if (not mDefinition)
         return {};

      auto ptr = mDefinition->mAddPtr;
      LglsAssert(ptr, "Pointer type hasn't been reflected yet");
      return ptr;
   }

   /// Add a constant qualifier to the type                                   
   ///   @attention this is possible only if the qualified type has           
   ///      already been reflected at runtime prior to calling this function  
   inline auto MetaDataNaked::AddConst() const -> MetaDataNaked {
      if (not mDefinition)
         return {};

      auto cnst = mDefinition->mAddConst;
      LglsAssert(cnst, "Constant type hasn't been reflected yet");
      return cnst;
   }

   /// Get the default concretization for an abstract type                    
   inline auto MetaDataNaked::GetConcrete() const -> MetaDataNaked {
      if (not mDefinition)
         return {};

      return mDefinition->mCurrentBoundary.mConcrete
         ? mDefinition->mCurrentBoundary.mConcrete()
         : nullptr;
   }

   /// Get the runtime producer of the type, if any                           
   inline auto MetaDataNaked::GetProducer() const -> MetaDataNaked {
      if (not mDefinition)
         return {};

      return mDefinition->mCurrentBoundary.mProducer
         ? mDefinition->mCurrentBoundary.mProducer()
         : nullptr;
   }

   /// Get the reflected bases                                                
   inline auto MetaDataNaked::GetBases()
   const noexcept -> DefinitionData::BaseList const& {
      if (not mDefinition) {
         static const DefinitionData::BaseList fallback;
         return fallback;
      }

      return mDefinition->mCurrentBoundary.mBases;
   }

   /// Get the reflected verbs                                                
   inline auto MetaDataNaked::GetVerbs()
   const noexcept -> DefinitionData::VerbList const& {
      if (not mDefinition) {
         static const DefinitionData::VerbList fallback;
         return fallback;
      }

      return mDefinition->mCurrentBoundary.mVerbs;
   }

   /// Get the reflected members                                              
   inline auto MetaDataNaked::GetMembers()
   const noexcept -> DefinitionData::MemberList const& {
      if (not mDefinition) {
         static const DefinitionData::MemberList fallback;
         return fallback;
      }

      return mDefinition->mCurrentBoundary.mMembers;
   }

   /// Get the reflected named values                                         
   inline auto MetaDataNaked::GetNamedValues()
   const noexcept -> DefinitionData::ValuesList const& {
      if (not mDefinition) {
         static const DefinitionData::ValuesList fallback;
         return fallback;
      }

      return mDefinition->mNamedValues;
   }

   /// Get morphisms to other types                                           
   inline auto MetaDataNaked::GetMorphismsTo()
   const noexcept -> DefinitionData::MorphismList const& {
      if (not mDefinition) {
         static const DefinitionData::MorphismList fallback;
         return fallback;
      }

      return mDefinition->mCurrentBoundary.mMorphismsTo;
   }
   
   /// Get morphisms from other types                                         
   /*inline auto MetaDataNaked::GetMorphismsFrom()
   const noexcept -> DefinitionData::MorphismList const& {
      if (not mDefinition) {
         static const DefinitionData::MorphismList fallback;
         return fallback;
      }

      return mDefinition->mCurrentBoundary.mMorphismsFrom;
   }*/
   
   /// Get a specific coverter, if it exists.                                 
   /// This will check if this type has a morphism TO the desired type, or    
   /// the desired type has a morphism FROM this type.                        
   inline auto MetaDataNaked::GetMorphism(MetaDataNaked to)
   const noexcept -> DefinitionData::Morphism {
      if (not mDefinition or not to.mDefinition)
         return {nullptr, nullptr};

      auto other = to.mDefinition;
      auto found = mDefinition->mCurrentBoundary.mMorphismsTo.find(other);
      if (found != mDefinition->mCurrentBoundary.mMorphismsTo.end())
         return found->second;

      /*found = other->mCurrentBoundary.mMorphismsFrom.find(mDefinition);
      if (found != other->mCurrentBoundary.mMorphismsFrom.end())
         return found->second;*/
      return {nullptr, nullptr};
   }
}
