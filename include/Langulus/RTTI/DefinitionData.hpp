///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include <Langulus/Typenav.hpp>
#include <Langulus/CT/Comparable.hpp>
#include <Langulus/CT/DefineTag.hpp>
#include <Langulus/Utils/Pot.hpp>
#include "Definition.hpp"
#include <unordered_set>
#include <unordered_map>

#if LANGULUS_FEATURE(MANAGED_MEMORY)
   #include <Langulus/CT/Pooled.hpp>
#endif


namespace Langulus::Annies
{
   struct Many;
   struct Any;
   struct HandleDisowned;
   template<class> 
   struct TVerb;
   using Verb = TVerb<void>;
}

namespace Langulus::RTTI
{
   ///                                                                        
   /// A data definition                                                      
   ///                                                                        
   class DefinitionData final : public Inner::Definition {
   protected:
      friend class Registry;
      friend class Definition;
      friend struct Inner::MetaDataNaked;
      template<unsigned, unsigned> friend struct Inner::MetaDataStructured_XY;

      // The origin type, with all qualifiers and sparseness removed.   
      // Will be null for incomplete types.                             
      DefinitionData const* mOrigin = nullptr;
      
      // The type, when a single level of indirection is removed.       
      // Will be null if data is dense. Will be exactly 1 if sparse,    
      // but pointing to an incomplete type.                            
      DefinitionData const* mDeptr = nullptr;
      // The pointer specification of a sparse type.                    
      PointerSpecification mPointerSpecification;
      
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // This is required in some corner cases involving multiple    
         // layers of indirection, or incomplete types.                 
         bool mPtrIncludedInID = false;
         bool mDedicatedID = false;
      #endif

      // The type, when all qualifiers are removed down to the origin.  
      DefinitionData const* mDecvqAll IF_SAFE(= nullptr);
      // The type, when topmost qualifiers are removed.                 
      DefinitionData const* mDecvqOnce IF_SAFE(= nullptr);
      // The type, but with an additional level of indirection.         
      // @attention This is not null only after the pointer type has    
      //    been reflected elsewhere at runtime.                        
      DefinitionData const* mAddPtr = nullptr;
      // The type, but constant.                                        
      // @attention This is not null only after the constant type has   
      //    been reflected elsewhere at runtime.                        
      DefinitionData const* mAddConst = nullptr;

      // True if data is constant, set by CT::Constant.                 
      bool mConst IF_SAFE(= false);
      // True if data is deep, set by CT::Deep.                         
      bool mDeep IF_SAFE(= false);
      // True if data is pod, set by CT::POD.                           
      bool mPOD IF_SAFE(= false);
      // True if data is nullable, set by CT::Nullable.                 
      bool mNullable IF_SAFE(= false);
      // True if data is abstract, set by CT::Abstract.                 
      bool mAbstract IF_SAFE(= false);
      // Decides whether POD data is batch-hashable or not.             
      // A custom GetHash() method disables POD batch-hashing.          
      bool mHasGetHashMethod = false;
      // Does data represent an executable verb.                        
      bool mExecutable IF_SAFE(= false);

      // Data instance size in bytes, set by sizeof().                  
      size_t mSize IF_SAFE(= 0);
      // Data instance alignment in bytes, set by alignof().            
      pot_t mAlign IF_SAFE(= pot_t(Alignment));
      // Minimal element allocation, in bytes.                          
      pot_t mMinimalAllocation IF_SAFE(= pot_t(MinimalAllocation));
      // Precomputed counts indexed by MSB (avoids division by stride   
      // for that extra oompf).                                         
      size_t mAllocationTable[sizeof(size_t) * 8 + 1] IF_SAFE(= {});
      // Reflected suffix.                                              
      ::std::string mSuffixOf;
      // Reflected file extensions, separated by commas.                
      ::std::string mFilesOf;
      
      #if LANGULUS_FEATURE(MANAGED_MEMORY)
         // Minimal pool allocation in bytes.                           
         pot_t mMinimalPoolSize IF_SAFE(= pot_t(MinimalPoolSize));
         // The reflected pool tactic.                                  
         PoolTactic mPoolTactic = PoolTactic::Default;
      #endif

   public:
      //                                                                
      //    These methods are sought for each reflected type            
      //                                                                
      //    These function pointers will be different for different     
      // libraries. We just collect them all. If a shared object is     
      // unloaded, we simply pick a pointer from another. Once the data 
      // is reflected from the MainBoundary, the maps are cleared and   
      // only the main code is used, because it is most persistent.     
      using FUnary         = void (*)(void* self);
      using FBinary        = void (*)(void* from, void* to);
      using FSerialize     = size_t (*)(void const* from, void* to, void* context);
      using FDescribe      = void (*)(void* self, const Annies::Many& describe);
      using FCompare       = Compared (*)(const void* lhs, const void* rhs);
      using FCompareEqual  = bool (*)(const void* lhs, const void* rhs);
      using FResolve       = Annies::HandleDisowned (*)(void* self);
      using FHash          = Hash (*)(void* self);
      using FReference     = int (*)(void* self, int modifier);
      using FDispatch      = bool (*)(void* self, Annies::Verb& verb);
      using FAccessMember  = void* (*)(void* owner);
      using FUnpack        = FAccessMember;
      using FTypeRetriever = DefinitionData const* (*)();
      
      /// Type-erased member variable reflection                              
      struct Member {
         using CTTI_ReflectAs = void;

         // Type of data.                                               
         FTypeRetriever type IF_SAFE(= nullptr);
         // Get pointer to the member.                                  
         FAccessMember member IF_SAFE(= nullptr);
         // Number of elements in mData (in case of an array).          
         size_t extent IF_SAFE(= 1);
         // Tags.                                                       
         ::std::unordered_set<DefinitionTag const*> tags;
         // Name of the member.                                         
         ::std::string name;

         template<class HANDLE>
         static auto From() -> Member;
      };
      
      /// Used to reflect a base                                              
      struct Base {
         using CTTI_ReflectAs = void;

         // Type of the base.                                           
         DefinitionData const* type IF_SAFE(= nullptr);
         // Usually true when base completely fills the derived type.   
         bool binaryCompatible = false;
         // Get a pointer to the base inside an instance.               
         // If nullptr, then base is imposed. Imposed bases are not     
         // serialized and don't participate in type-distance           
         // computation or dispatching. They are just used to associate 
         // data types.                                                 
         FAccessMember getBase = nullptr;

         template<CT::Dense T, CT::Dense BASE> static auto
         From() assumptious -> Base;
      };
      
      /// Used to reflect a morphism                                          
      struct Morphism {
         using CTTI_ReflectAs = void;

         // Simple converter, encapsulating a static_cast.              
         FBinary convert = nullptr;//TODO force these to be const on the left hand side
         // A serializer if supported, also takes in a context.         
         FSerialize serialize = nullptr;
      };

   protected:
      template<class T>
      void FillMorphisms();

      using MemberList   = ::std::vector<Member>;
      using VerbList     = ::std::unordered_map<DefinitionVerb const*, FDispatch>;
      using BaseList     = ::std::vector<Base>;
      using MorphismList = ::std::unordered_map<DefinitionData const*, Morphism>;
      using ValuesList   = ::std::vector<DefinitionConst const*>;
      
      /// Pointers in this structure depend on the library in which the type  
      /// was reflected from. These pointers become invalid when a DLL is     
      /// unloaded, and should be managed separately.                         
      struct BoundaryDependent {
         using CTTI_ReflectAs = void;

         // Dereference a pointer once. The resulting type will be      
         // an instance of mDeptr. Supports custom pointer types.       
         FBinary mDereference = nullptr;

         // The default constructor, wrapped in a lambda expression if  
         // available. Takes a pointer for a placement-new expression.  
         FUnary mDefaultConstructor = nullptr;

         // Constructor by descriptor.                                  
         // Takes a pointer for a placement-new expression, and a Many. 
         FDescribe mDescribeConstructor = nullptr;

         // The refer/copy/disown/clone/move/abandon constructors,      
         // wrapped in lambdas. They take a pointer for a placement-new 
         // expression and a source.                                    
         FBinary mReferConstructor = nullptr;
         FBinary mCopyConstructor = nullptr;
         FBinary mDisownConstructor = nullptr;
         FBinary mCloneConstructor = nullptr;
         FBinary mMoveConstructor = nullptr;
         FBinary mAbandonConstructor = nullptr;

         // The destructor, wrapped in a lambda expression.             
         // Takes the pointer to the instance for destruction.          
         FUnary mDestructor = nullptr;

         // The <=> operator, wrapped in lambda expression if available.
         FCompare mComparer = nullptr;
         // The == operator, wrapped in lambda expression if available. 
         FCompareEqual mComparerEqual = nullptr;

         // The refer/copy/disown/clone/move/abandon assignments, all   
         // wrapped in lambdas.                                         
         FBinary mReferAssigner = nullptr;
         FBinary mCopyAssigner = nullptr;
         FBinary mDisownAssigner = nullptr;
         FBinary mCloneAssigner = nullptr;
         FBinary mMoveAssigner = nullptr;
         FBinary mAbandonAssigner = nullptr;

         // The class type function, wrapped in a lambda expression.    
         // Returns typed container with most concrete class instance.  
         FResolve mResolver = nullptr;

         // The hash getter, wrapped in a lambda expression.            
         // Takes pointer to the instance for hashing, returns the hash.
         FHash mHasher = nullptr;

         // The reference function wrapped in a lambda.                 
         // Takes the pointer to the instance for referencing.          
         // Returns the number of references after being referenced.    
         // (use 0 modifier to just get references)                     
         FReference mReferencer = nullptr;

         // A custom verb dispatcher, wrapped in a lambda expression.   
         // Takes pointer to the instance that will dispatch, and a verb
         // There is a mutable and immutable version of this.           
         //FDispatch mDispatcher = nullptr;

         // Default concretization.                                     
         // Used as redirection when requesting the creation of abstract
         FTypeRetriever mConcrete = nullptr;
         
         // Types with producers can be instantiated only by the        
         // invocation of Verbs::Create in the context of the producer. 
         FTypeRetriever mProducer = nullptr;

         // List of reflected members                                   
         MemberList mMembers;
         
         // List of reflected abilities                                 
         VerbList mVerbs;

         // List of reflected bases                                     
         BaseList mBases;
         
         // List of reflected converters                                
         MorphismList mMorphismsTo;
         //MorphismList mMorphismsFrom;
      };
      
      // List of named values                                           
      ValuesList mNamedValues;

      // The currently used boundary                                    
      BoundaryDependent mCurrentBoundary;
      
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // All functions, reflected from all points of view            
         // If this map is empty, then data has been reflected from the 
         // main boundary                                               
         ::std::unordered_map<::std::string, BoundaryDependent> mOtherBoundaries;
      #endif

      LANGULUS_API(RTTI)
      void ReflectOrigin(size_t minElements, size_t sizeofT);
      LANGULUS_API(RTTI)
      void ReflectConstOrigin();
      LANGULUS_API(RTTI)
      void ReflectStandardSparse(bool mut, bool complete, bool denserComplete, size_t minElements);
      LANGULUS_API(RTTI)
      void ReflectCustomSparse(bool mut, bool complete, size_t minElements, size_t sizeofT);

   public:
      using CTTI_ReflectAs = void;

      static constexpr Token InvalidName = "nodata";

      template<class T> requires (CT::Dense<T> and not ::std::is_const_v<T>)
      static auto Reflect() -> DefinitionData const*;
      template<class T> requires (CT::Dense<T> and ::std::is_const_v<T>)
      static auto Reflect() -> DefinitionData const*;
      template<class T> requires ::std::is_pointer_v<T>
      static auto Reflect() -> DefinitionData const*;
      template<class T> requires CT::CustomPointer<T>
      static auto Reflect() -> DefinitionData const*;
      
      DefinitionData(const Token& cppname) noexcept
         : Definition {cppname} {}
      
      ~DefinitionData();
   };
}

#include "DefinitionData.inl"
