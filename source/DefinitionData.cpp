///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/RTTI/DefinitionData.inl>


namespace Langulus::RTTI
{
   namespace
   {
      /// These functions are used to reduce the number of generated unique   
      /// lambdas at reflection time. All standard pointers are the same.     
      void SparseDefaultDeref(void* from, void* to) {
         auto typed_from = static_cast<void**>(from);
         auto typed_to   = static_cast<void**>(to);
         *typed_to = *typed_from;
      };

      void SparseDefaultConstructor(void* at) noexcept {
         auto atT = static_cast<void**>(at);
         new (atT) void* {};
      };

      void SparseCopyConstructor(void* from, void* to) noexcept {
         auto fromT = static_cast<void**>(from);
         auto toT = static_cast<void**>(to);
         *toT = *fromT;
      };
      
      auto SparseCompare(const void* lhs, const void* rhs) noexcept -> Compared {
         auto lhsT = static_cast<void const* const*>(lhs);
         auto rhsT = static_cast<void const* const*>(rhs);
         return *lhsT == *rhsT ? Compared::Equal : Compared::Unordered;
      };

      bool SparseCompareEqual(const void* lhs, const void* rhs) noexcept {
         auto lhsT = static_cast<void const* const*>(lhs);
         auto rhsT = static_cast<void const* const*>(rhs);
         return *lhsT == *rhsT;
      };

      auto SparseHash(void* lhs) noexcept -> Hash {
         auto lhsT = static_cast<void**>(lhs);
         return HashOf<true>(*lhsT);
      };
   }

   /// Reflect some common origin type traits                                 
   void DefinitionData::ReflectOrigin(size_t minElements, size_t sizeofT) {
      mConst      = false;
      mOrigin     = this;
      mDecvqOnce  = this;
      mDecvqAll   = this;

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Origin type encountered, time to reserve a new ID           
         mID = Registry::ReserveDataID(this);
         mDedicatedID = true;
      #endif
      
      // Calculate the allocation table                                 
      for (size_t bit = 0; bit < sizeof(size_t) * 8u; ++bit) {
         const size_t threshold = size_t {1} << bit;
         const size_t elements = threshold / sizeofT;
         mAllocationTable[bit] = minElements > elements
            ? minElements : elements;
      }
   }
   
   /// Reflect some common constant origin properties                         
   void DefinitionData::ReflectConstOrigin() {
      mConst     = true;
      mSuffixOf  = mOrigin->mSuffixOf;
      mFilesOf   = mOrigin->mFilesOf;
      mDecvqOnce = mOrigin;
      mDecvqAll  = mOrigin;
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         mID     = mDecvqOnce->mID;
      #endif
      
      auto decvq = const_cast<DefinitionData*>(mDecvqOnce);
      decvq->mAddConst = this;
      
      mCurrentBoundary.mConcrete
         = mOrigin->mCurrentBoundary.mConcrete;
      mCurrentBoundary.mProducer
         = mOrigin->mCurrentBoundary.mProducer;

      //                                                                
      // Constructor reflections                                        
      // Reuse the same lambdas as the dequalified reflection           
      mCurrentBoundary.mDefaultConstructor
         = mOrigin->mCurrentBoundary.mDefaultConstructor;
      mCurrentBoundary.mCopyConstructor
         = mOrigin->mCurrentBoundary.mCopyConstructor;
      mCurrentBoundary.mReferConstructor
         = mOrigin->mCurrentBoundary.mReferConstructor;
      mCurrentBoundary.mCloneConstructor
         = mOrigin->mCurrentBoundary.mCloneConstructor;
      mCurrentBoundary.mDisownConstructor
         = mOrigin->mCurrentBoundary.mDisownConstructor;
      mCurrentBoundary.mMoveConstructor
         = mOrigin->mCurrentBoundary.mMoveConstructor;
      mCurrentBoundary.mAbandonConstructor
         = mOrigin->mCurrentBoundary.mAbandonConstructor;
      mCurrentBoundary.mDestructor
         = mOrigin->mCurrentBoundary.mDestructor;

      //                                                                
      // Other utilities                                                
      mCurrentBoundary.mHasher
         = mOrigin->mCurrentBoundary.mHasher;
      mCurrentBoundary.mReferencer
         = mOrigin->mCurrentBoundary.mReferencer;
      mCurrentBoundary.mComparer
         = mOrigin->mCurrentBoundary.mComparer;
      mCurrentBoundary.mComparerEqual
         = mOrigin->mCurrentBoundary.mComparerEqual;
      mCurrentBoundary.mResolver
         = mOrigin->mCurrentBoundary.mResolver;
      mMinimalAllocation
         = mOrigin->mMinimalAllocation;

      #if LANGULUS_FEATURE(MANAGED_MEMORY)
         mPoolTactic       = mOrigin->mPoolTactic;
         mMinimalPoolSize  = mOrigin->mMinimalPoolSize;
      #endif

      memcpy(mAllocationTable, mOrigin->mAllocationTable, sizeof(mAllocationTable));

      mCurrentBoundary.mBases
         = mOrigin->mCurrentBoundary.mBases;
      mCurrentBoundary.mMorphismsTo
         = mOrigin->mCurrentBoundary.mMorphismsTo;
      mNamedValues
         = mOrigin->mNamedValues;
      mCurrentBoundary.mMembers
         = mOrigin->mCurrentBoundary.mMembers;
   }
   
   /// Reflect common sparse traits                                           
   void DefinitionData::ReflectStandardSparse(bool mut, bool complete, bool denserComplete, size_t minElements) {
      mPOD       = true;
      mNullable  = true;
      mAbstract  = false;

      //                                                                
      // Constructor reflections                                        
      // @note these are allowed even if T is constant                  
      mCurrentBoundary.mDefaultConstructor = SparseDefaultConstructor;
      mCurrentBoundary.mCopyConstructor    = SparseCopyConstructor;
      mCurrentBoundary.mReferConstructor   = SparseCopyConstructor;
      mCurrentBoundary.mDisownConstructor  = SparseCopyConstructor;
      mCurrentBoundary.mMoveConstructor    = SparseCopyConstructor;
      mCurrentBoundary.mAbandonConstructor = SparseCopyConstructor;

      if (complete) {
         // Always use the origin cloning routine                       
         mCurrentBoundary.mCloneConstructor
            = mOrigin->mCurrentBoundary.mCloneConstructor;
      }

      if (denserComplete)
         mCurrentBoundary.mDereference     = SparseDefaultDeref;

      //                                                                
      // Assignment reflections                                         
      // @note allowed only if T is mutable                             
      if (mut) {
         mCurrentBoundary.mCopyAssigner    = SparseCopyConstructor;
         mCurrentBoundary.mReferAssigner   = SparseCopyConstructor;
         mCurrentBoundary.mDisownAssigner  = SparseCopyConstructor;
         mCurrentBoundary.mMoveAssigner    = SparseCopyConstructor;
         mCurrentBoundary.mAbandonAssigner = SparseCopyConstructor;
      
         if (complete) {
            // Always use the origin cloning routine                    
            mCurrentBoundary.mCloneAssigner
               = mOrigin->mCurrentBoundary.mCloneAssigner;
         }
      }

      //                                                                
      // Other utilities                                                
      mCurrentBoundary.mHasher        = SparseHash;   
      mCurrentBoundary.mComparer      = SparseCompare;         
      mCurrentBoundary.mComparerEqual = SparseCompareEqual;         

      // Calculate the allocation table                                 
      for (size_t bit = 0; bit < sizeof(size_t) * 8u; ++bit) {
         const size_t threshold = size_t {1} << bit;
         const size_t elements = threshold / sizeof(void*);
         mAllocationTable[bit] = minElements > elements ? minElements : elements;
      }

      if (complete) {
         // Bases, verbs, and members come from origin                  
         // so that we don't have unnecessary indirections when checking
         // type properties at runtime                                  
         mCurrentBoundary.mBases    = mOrigin->mCurrentBoundary.mBases;
         mCurrentBoundary.mVerbs    = mOrigin->mCurrentBoundary.mVerbs;
         mCurrentBoundary.mMembers  = mOrigin->mCurrentBoundary.mMembers;
      }
   }

   /// Reflect common custom pointer traits                                   
   void DefinitionData::ReflectCustomSparse(bool mut, bool complete, size_t minElements, size_t sizeofT) {
      mPOD       = true;
      mNullable  = true;
      mAbstract  = false;

      //                                                                
      // Constructor reflections                                        
      if (complete) {
         // Always use the origin cloning routine                       
         mCurrentBoundary.mCloneConstructor
            = mOrigin->mCurrentBoundary.mCloneConstructor;
      }
      
      //                                                                
      // Assignment reflections                                         
      // @note allowed only if T is mutable                             
      if (mut) {
         if (complete) {
            // Always use the origin cloning routine                    
            mCurrentBoundary.mCloneAssigner
               = mOrigin->mCurrentBoundary.mCloneAssigner;
         }
      }
      
      // Calculate the allocation table                                 
      for (size_t bit = 0; bit < sizeof(size_t) * 8u; ++bit) {
         const size_t threshold = size_t {1} << bit;
         const size_t elements = threshold / sizeofT;
         mAllocationTable[bit] = minElements > elements ? minElements : elements;
      }

      if (complete) {
         // Bases, verbs, and members come from origin                  
         // so that we don't have unnecessary indirections when checking
         // type properties at runtime                                  
         mCurrentBoundary.mBases    = mOrigin->mCurrentBoundary.mBases;
         mCurrentBoundary.mVerbs    = mOrigin->mCurrentBoundary.mVerbs;
         mCurrentBoundary.mMembers  = mOrigin->mCurrentBoundary.mMembers;
      }
   }
}
