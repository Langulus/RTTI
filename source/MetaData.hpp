///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Hashing.hpp"
#include "MetaVerb.hpp"
#include "MetaTrait.hpp"
#include <unordered_map>


namespace Langulus::RTTI
{
   
   /// A simple request for allocating memory                                 
   /// It is used as optimization to avoid divisions by stride                
   struct AllocationRequest {
      Size  mByteSize IF_SAFE(= 0);
      Count mElementCount IF_SAFE(= 0);

      IF_UNSAFE(constexpr AllocationRequest() {})
   };


   ///                                                                        
   ///   These methods are sought in each reflected type                      
   ///                                                                        
   /// The default constructor, wrapped in a lambda expression if available   
   /// Takes a pointer for a placement-new expression                         
   using FDefaultConstruct = void(*)(void*);

   /// Constructor by descriptor                                              
   /// Takes a pointer for a placement-new expression, and a Neat container   
   using FDescriptorConstruct = void(*)(void*, const Anyness::Many&);

   /// The copy/disown/clone constructor, wrapped in a lambda expression      
   /// Takes a pointer for a placement-new expression, and a source           
   using FCopyConstruct = void(*)(const void* from, void* to);

   /// The move/abandon constructor, wrapped in a lambda expression           
   /// Takes a pointer for a placement-new expression, and a source           
   using FMoveConstruct = void(*)(void* from, void* to);

   /// The destructor, wrapped in a lambda expression                         
   /// Takes the pointer to the instance for destruction                      
   using FDestroy = void(*)(void*);

   /// The == operator, wrapped in a lambda expression if available           
   /// Compares two instances for equality                                    
   using FCompare = bool(*)(const void*, const void*);

   /// The copy/disown/clone-assignment operator, wrapped in a lambda         
   using FCopy = void(*)(const void* from, void* to);

   /// The move/abandon-assignment operator, wrapped in a lambda expression   
   using FMove = void(*)(void* from, void* to);

   /// The class type function, wrapped in a lambda expression                
   /// Returns the typed memory block of the class instance                   
   using FResolve = Anyness::Block<>(*)(const void*);

   /// The hash getter, wrapped in a lambda expression                        
   /// Takes the pointer to the instance for hashing                          
   /// Returns the hash                                                       
   using FHash = Hash(*)(const void*);
   
   /// The reference function wrapped in a lambda                             
   /// Takes the pointer to the instance for referencing                      
   /// Returns the number of references after being referenced                
   /// (use 0 modifier to just get references)                                
   using FReference = Count(*)(void*, int modifier);

   /// A custom verb dispatcher, wrapped in a lambda expression               
   /// Takes the pointer to the instance that will dispatch, and a verb       
   /// There is a mutable and immutable version of this                       
   using FDispatchMutable = void(*)(void*, Flow::Verb&);
   using FDispatchConstant = void(*)(const void*, Flow::Verb&);
   using FVerbMutable = FDispatchMutable;
   using FVerbConstant = FDispatchConstant;

   using FTypeRetriever = DMeta(*)();
   using FTraitRetriever = TMeta(*)(int);

   using FDynamicCast = void*(*)(void*);


   ///                                                                        
   ///   Type-erased member variable reflection                               
   ///   Can represent arrays of elements, tag members as traits, etc.        
   ///                                                                        
   struct Member {
      LANGULUS(UNALLOCATABLE) true;
      LANGULUS(ACT_AS)        void;

      // Type of data                                                   
      // We can't get at reflection time, so we generate a lambda that  
      // retrieves it when required                                     
      FTypeRetriever mTypeRetriever {};
      // Get pointer to the member. Assumes argument points to an       
      // instance of the member owner.                                  
      FDynamicCast mValueRetriever {};
      // Number of elements in mData (in case of an array)              
      Count mCount = 1;
      // Trait tag                                                      
      // We can't get at reflection time, so we generate a lambda that  
      // retrieves it when required (TODO this is the first tag only)   
      FTraitRetriever mTraitRetriever {};

   public:
      constexpr Member() noexcept = default;
      constexpr Member(const Member&) noexcept = default;

      Member(const auto&);

      Member& operator = (const Member&) noexcept = default;

      bool operator == (const Member&) const noexcept;
      
      template<CT::Data T>
      auto As(const Byte*) const noexcept -> T const&;
      template<CT::Data T>
      auto As(Byte*) const noexcept -> T&;
      
      auto Get(Byte const*) const noexcept -> Byte const*;
      auto Get(Byte*)       const noexcept -> Byte*;

      DMeta GetType() const;
      TMeta GetTrait(int) const;

   private:
      template<class...T>
      static TMeta TraitSelector(int, Types<T...>&&);
   };

   using MemberList = ::std::vector<Member>;

   
   ///                                                                        
   ///   Used to reflect abilities                                            
   ///                                                                        
   struct Ability {
      LANGULUS(UNALLOCATABLE) true;

      // The verb ID                                                    
      VMeta mVerb {};

      using Signature = ::std::vector<DMeta>;

      // Functions to call, paired with their argument types            
      // For functions that can mutate the context                      
      using MutableOverloadList = ::std::unordered_map<Signature, FVerbMutable>;
      MutableOverloadList mOverloadsMutable {};
      
      // Functions to call, paired with their argument types            
      // For functions that can't mutate the context                    
      using ConstantOverloadList = ::std::unordered_map<Signature, FVerbConstant>;
      ConstantOverloadList mOverloadsConstant {};
      
   public:
      bool operator == (const Ability&) const noexcept;

      template<CT::Dense, CT::Data VERB, CT::Data...A>
      static Ability From() noexcept;
   };


   ///                                                                        
   ///   Used to reflect data coversions                                      
   ///                                                                        
   struct Converter {
      LANGULUS(UNALLOCATABLE) true;
      LANGULUS(ACT_AS)        void;

      // The data ID we're converting to                                
      DMeta mType {};
      // Address of function to call                                    
      FCopyConstruct mFunction {};
      
   public:
      constexpr bool operator == (const Converter&) const noexcept;

      template<CT::Decayed FROM, CT::Decayed TO>
      static Converter From(DMeta) noexcept;
   };

   using ConverterMap = ::std::unordered_map<DMeta, Converter>;


   ///                                                                        
   ///   Used to reflect a base for a type                                    
   ///                                                                        
   struct Base {
      LANGULUS(UNALLOCATABLE) true;
      LANGULUS(ACT_AS)        void;

      // Type of the base                                               
      DMeta mType;
      // CT::Number of bases that fit in the type                       
      Count mCount = 1;
      // Offset of the base, relative to the derived type               
      // @attention valid only if not mVirtualBase                      
      Offset mOffset = 0;
      // Used to map one type onto another                              
      // Usually true when base completely fills the derived type       
      bool mBinaryCompatible = false;
      // Whether or not this base is considered an imposed base or not  
      // Basically, imposed bases are not serialized and don't act in   
      // distance computation or dispatching                            
      // An imposed base can be added only manually                     
      bool mImposed = false;

      // True if base is virtual                                        
      bool mVirtualBase = false;
      // Only possible way to get pointer to a virtual base             
      FDynamicCast mDynamicCast = nullptr;

   public:
      bool operator == (const Base&) const noexcept;

      template<CT::Dense, CT::Dense BASE>
      static Base From() IF_UNSAFE(noexcept);
   };

   using BaseList = ::std::vector<Base>;


   ///                                                                        
   ///   Meta data                                                            
   ///                                                                        
   struct MetaData : Meta {
   private:
      struct PurposefullyIncompleteType;

      static_assert(    CT::Complete<int>,
         "Your compiler can't reliably detect incomplete types");
      static_assert(not CT::Complete<PurposefullyIncompleteType>,
         "Your compiler can't reliably detect incomplete types");

      using AbilityList = ::std::unordered_map<VMeta, Ability>;
      using MutableOverloadList = typename Ability::MutableOverloadList;
      using ConstantOverloadList = typename Ability::ConstantOverloadList;
      using NamedValueList = ::std::vector<CMeta>;

   public:
      friend struct Member;

      LANGULUS(NAME) "DMeta";
      LANGULUS_BASES(Meta);

      enum Distance : int {
         Infinite = ::std::numeric_limits<int>::max()
      };
      
      static constexpr Token DefaultToken = "NoData";

      MetaData(const Token& token)
         : Meta {token} {
         mTokenSanitized = ToLastToken(mToken);
         mTokenSanitized[0] = ::std::toupper(mTokenSanitized[0]);
      }

      Token Kind() const noexcept final { return Meta::Data; }

      // A sanitized last token (with a capital first letter)           
      ::std::string mTokenSanitized;
      // The origin type, with all qualifiers and sparseness removed    
      // Will be nullptr for incomplete types                           
      DMeta mOrigin {};
      // The type, when a single pointer is removed                     
      // Will be null if no pointers available                          
      DMeta mDeptr {};
      // The type, when a const and volatile qualifiers are removed     
      // Will be `this` if no qualifiers available                      
      DMeta mDecvq {};
      // Default concretization                                         
      // Used as redirection, when requesting the creation of asbtracts 
      FTypeRetriever mConcreteRetriever {};
      // Types with producers can be instantiated only by the invocation
      // of Verbs::Create in the context of the producer                
      FTypeRetriever mProducerRetriever {};
      // True if reflected data is sparse (a pointer)                   
      bool mIsSparse = false;
      // True if reflected data is constant                             
      // This comes only from the immediate type                        
      bool mIsConstant = false;
      // True if origin type is POD                                     
      // POD data can be batch-memcpy-ed, or binary-serialized          
      bool mIsPOD = false;
      // True if origin type is nullifiable                             
      // Nullifiable data can be batch-constructed via memset(0)        
      bool mIsNullifiable = false;
      // If origin type is abstract                                     
      bool mIsAbstract = false;
      // True if origin type is marked LANGULUS(DEEP) true              
      // Types marked deep can be iterated through                      
      bool mIsDeep = false;
      // True if origin type is allocatable (and thus clonable)         
      bool mIsUnallocatable = false;
      // True if origin type is executable (derived from Flow::Verb)    
      bool mIsExecutable = false;
      // Size of the reflected type (in bytes)                          
      Size mSize {};
      // Alignof (in bytes)                                             
      Size mAlignment {};
      // Minimal allocation, in bytes                                   
      Size mAllocationPage {};
      // Precomputed counts indexed by MSB (avoids division by stride)  
      Size mAllocationTable[sizeof(Size) * 8 + 1] {};
      // File extensions used by the origin type, separated by commas   
      Token mFileExtensions {};
      // Suffix for the origin type                                     
      Token mSuffix {};

      #if LANGULUS_FEATURE(MANAGED_MEMORY)
         PoolTactic mPoolTactic = PoolTactic::Default;

         private:
            // The pool chain for the type                              
            // Access it through GetPool() method, because it might not 
            // be exactly relevan to a convoluted/sparse type           
            mutable void* mPool {};

         public:
            template<class T>
            T*& GetPool() const noexcept;
      #endif

      // Default constructor wrapped in a lambda upon reflection        
      // @attention this always works with the origin type              
      FDefaultConstruct mDefaultConstructor {};
      // Descriptor constructor wrapped in a lambda upon reflection     
      // @attention this always works with the origin type              
      FDescriptorConstruct mDescriptorConstructor {};
      // Copy constructor wrapped in a lambda upon reflection           
      // @attention this always works with the origin type              
      FCopyConstruct mCopyConstructor {};
      // Refer constructor wrapped in a lambda upon reflection          
      // @attention this always works with the origin type              
      FCopyConstruct mReferConstructor {};
      // Disowned constructor wrapped in a lambda upon reflection       
      // @attention this always works with the origin type              
      FCopyConstruct mDisownConstructor {};
      // Cloned constructor wrapped in a lambda upon reflection         
      // @attention this always works with the origin type              
      FCopyConstruct mCloneConstructor {};
      // Move constructor wrapped in a lambda upon reflection           
      // @attention this always works with the origin type              
      FMoveConstruct mMoveConstructor {};
      // Abandon constructor wrapped in a lambda upon reflection        
      // @attention this always works with the origin type              
      FMoveConstruct mAbandonConstructor {};

      // Reference function (if any), wrapped in a lambda on reflection 
      // @attention this always works with the origin type              
      FReference mReference {};

      // Destructor wrapped in a lambda upon reflection                 
      // @attention this always works with the origin type              
      FDestroy mDestructor {};

      // Copy-assignment operator, wrapped in a lambda upon reflection  
      // @attention this always works with the origin type              
      FCopy mCopyAssigner {};
      // Refer-assignment operator, wrapped in a lambda upon reflection 
      // @attention this always works with the origin type              
      FCopy mReferAssigner {};
      // Disown-assignment operator, wrapped in a lambda upon reflection
      // @attention this always works with the origin type              
      FCopy mDisownAssigner {};
      // Clone-assignment operator, wrapped in a lambda upon reflection 
      // @attention this always works with the origin type              
      FCopy mCloneAssigner {};
      // Move-assignment, wrapped in a lambda upon reflection           
      // @attention this always works with the origin type              
      FMove mMoveAssigner {};
      // Abandon-assignment, wrapped in a lambda upon reflection        
      // @attention this always works with the origin type              
      FMove mAbandonAssigner {};

      // The == operator, wrapped in a lambda upon reflection           
      // @attention this always works with the origin type              
      FCompare mComparer {};

      // The ClassBlock method, wrapped in a lambda upon reflection     
      // @attention this always works with the origin type              
      FResolve mResolver {};

      // The GetHash() method, wrapped in a lambda                      
      // @attention this always works with the origin type              
      FHash mHasher {};

      // The Do verb, wrapped in a lambda (mutable context)             
      // @attention this always works with the origin type              
      FDispatchMutable mDispatcherMutable {};
      // The Do verb, wrapped in a lambda	(immutable context)           
      // @attention this always works with the origin type              
      FDispatchConstant mDispatcherConstant {};
      
      // List of reflected members of the origin type                   
      MemberList mMembers {};
      // List of reflected abilities of the origin type                 
      AbilityList mAbilities {};
      // List of reflected bases of the origin type                     
      BaseList mBases {};
      // List of reflected converters to/from the origin type           
      ConverterMap mConvertersTo {};
      ConverterMap mConvertersFrom {};
      // List of named values of the origin type                        
      NamedValueList mNamedValues {};

   protected:
      friend struct Base;

      template<CT::Fundamental>
      static void ReflectFundamentalType(MetaData&) noexcept;

      template<CT::Decayed>
      static void ReflectOriginType(MetaData&) noexcept;

      auto GetMemberInner(TMeta, DMeta, Offset&) const noexcept -> const Member*;
      auto GetMemberCountInner(TMeta, DMeta, Offset&) const noexcept -> Count;

      template<class, CT::Dense...Args>
      void SetBases(Types<Args...>) noexcept;

      template<CT::Dense, CT::Dense...Args>
      void SetAbilities(Types<Args...>) noexcept;

      template<class, class...Args>
      void SetConvertersTo(Types<Args...>) noexcept;
      template<class, class...Args>
      void SetConvertersFrom(Types<Args...>) noexcept;

      template<class>
      void AddConstant(auto&&);

   public:
      template<CT::Void>
      static consteval DMeta Of();

      template<CT::DataReference>
      static DMeta Of();

      template<CT::SparseData T>
      static DMeta Of() requires CT::InsertableAsSomethingElse<T>;
      template<CT::SparseData T>
      static DMeta Of() requires CT::NotInsertableAsSomethingElse<T>;

      template<CT::DenseData T>
      static DMeta Of() requires (CT::Convoluted<T> and CT::InsertableAsSomethingElse<T>);
      template<CT::DenseData T>
      static DMeta Of() requires (CT::Convoluted<T> and CT::NotInsertableAsSomethingElse<T>);

      template<CT::Decayed T>
      static DMeta Of() requires CT::InsertableAsSomethingElse<T>;
      template<CT::Decayed T>
      static DMeta Of() requires CT::NotInsertableAsSomethingElse<T>;

      DMeta GetMostConcrete() const noexcept;
      AllocationRequest RequestSize(Count) const noexcept;

      //                                                                
      // Base management                                                
      //                                                                
      bool GetBase(DMeta, Offset, Base&) const;
      template<CT::Data>
      bool GetBase(Offset, Base&) const;

      bool HasBase(DMeta) const;
      template<CT::Data>
      bool HasBase() const;

      bool HasDerivation(DMeta) const;
      template<CT::Data>
      bool HasDerivation() const;

      //                                                                
      // Member management                                              
      //                                                                
      auto GetMember(TMeta, DMeta = {}, Offset = 0) const noexcept -> const Member*;
      auto GetMemberCount(TMeta, DMeta = {}, Offset = 0) const noexcept -> Count;
      auto GetMemberCount() const noexcept -> Count;

      //                                                                
      // Ability management                                             
      //                                                                
      bool IsAbleTo(VMeta) const;
      template<CT::Data>
      bool IsAbleTo() const;

      template<bool MUTABLE>
      auto GetAbility(VMeta, DMeta) const;
      template<bool MUTABLE, CT::Data V>
      auto GetAbility(DMeta) const;
      template<bool MUTABLE, CT::Data V, CT::Data...A>
      auto GetAbility() const;

      //                                                                
      // Named value management                                         
      //                                                                
      Token GetNamedValueOf(const auto&) const;

      //                                                                
      // Morphisms and comparison                                       
      //                                                                
      FCopyConstruct GetConverter(DMeta) const;

      template<bool BINARY_COMPATIBLE = false, bool ADVANCED = false>
      bool CastsTo(DMeta) const;
      template<bool BINARY_COMPATIBLE = false>
      bool CastsTo(DMeta, Count) const;

      template<class, bool BINARY_COMPATIBLE = false, bool ADVANCED = false>
      bool CastsTo() const;
      template<class, bool BINARY_COMPATIBLE = false>
      bool CastsTo(Count) const;

      template<class>
      bool IsRelatedTo() const;
      bool IsRelatedTo(DMeta) const;

      template<class>
      Distance GetDistanceTo() const;
      Distance GetDistanceTo(DMeta) const;

      template<class, class...>
      constexpr bool Is() const;
      constexpr bool Is(DMeta) const noexcept;

      template<class, class...>
      constexpr bool IsSimilar() const;
      constexpr bool IsSimilar(DMeta) const noexcept;

      template<class, class...>
      constexpr bool IsExact() const;
      constexpr bool IsExact(DMeta) const noexcept;
   };

} // namespace Langulus::RTTI
