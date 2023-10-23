///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Meta.hpp"
#include "Byte.hpp"
#include <unordered_map>
#include <algorithm>
#include <string>


namespace Langulus::RTTI
{

   /// A simple request for allocating memory                                 
   /// It is used as optimization to avoid divisions by stride                
   struct AllocationRequest {
      Size mByteSize IF_SAFE(= 0);
      Count mElementCount IF_SAFE(= 0);

      IF_UNSAFE(constexpr AllocationRequest() {})
   };

   template<class T>
   constexpr Size GetAllocationPageOf() noexcept;

   using Lowercase = ::std::string;

   /// Convert a token to a lowercase string                                  
   ///   @param token - the token to lowercase                                
   ///   @return the lowercase string                                         
   NOD() inline Lowercase ToLowercase(const Token& token) noexcept {
      Lowercase lc {token};
      ::std::transform(lc.begin(), lc.end(), lc.begin(),
         [](char c) { return static_cast<char>(::std::tolower(c)); }
      );
      return lc;
   }
   
   ///                                                                        
   ///   These methods are sought in each reflected type                      
   ///                                                                        
   /// The default constructor, wrapped in a lambda expression if available   
   /// Takes a pointer for a placement-new expression                         
   using FDefaultConstruct = void(*)(void*);

   /// Constructor by descriptor                                              
   /// Takes a pointer for a placement-new expression, and a Neat container   
   using FDescriptorConstruct = void(*)(void*, const Anyness::Neat&);

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
   using FResolve = Anyness::Block(*)(const void*);

   /// The hash getter, wrapped in a lambda expression                        
   /// Takes the pointer to the instance for hashing                          
   /// Returns the hash                                                       
   using FHash = Hash(*)(const void*);

   /// A custom verb dispatcher, wrapped in a lambda expression               
   /// Takes the pointer to the instance that will dispatch, and a verb       
   /// There is a mutable and immutable version of this                       
   using FDispatchMutable = void(*)(void*, Flow::Verb&);
   using FDispatchConstant = void(*)(const void*, Flow::Verb&);
   using FVerbMutable = FDispatchMutable;
   using FVerbConstant = FDispatchConstant;

   using FTypeRetriever = DMeta(*)();
   using FTraitRetriever = TMeta(*)();


   ///                                                                        
   ///   Used to reflect a member variable                                    
   ///   You can reflect arrays of elements, tag members as traits, etc.      
   ///                                                                        
   struct Member {
      LANGULUS(UNALLOCATABLE) true;
      LANGULUS(UNINSERTABLE) true;

      // Type of data                                                   
      // We can't get at reflection time, so we generate a lambda that  
      // retrieves it when required                                     
      FTypeRetriever mTypeRetriever {};
      // Member offset. This is relative to the type it is offsetted    
      // in! If accessed through a derived type, that offset might      
      // be wrong! Type must be resolved first!                         
      Offset mOffset {};
      // Number of elements in mData (in case of an array)              
      Count mCount {1};
      // Trait tag                                                      
      // We can't get at reflection time, so we generate a lambda that  
      // retrieves it when required                                     
      FTraitRetriever mTraitRetriever {};
      // Member token                                                   
      Token mName {};

   public:
      template<CT::Data OWNER, CT::Data DATA>
      NOD() static Member From(DATA OWNER::* member, const Token&);
      template<CT::Decayed TRAIT, CT::Data OWNER, CT::Data DATA>
      NOD() static Member FromTagged(DATA OWNER::* member, const Token&);

      NOD() bool operator == (const Member&) const noexcept;
      
      template<CT::Data T>
      NOD() bool Is() const;
      NOD() bool Is(DMeta) const;
      template<CT::Decayed T>
      NOD() bool TraitIs() const;
      NOD() bool TraitIs(TMeta) const;
      
      template<CT::Data T>
      NOD() const T& As(const Byte*) const noexcept;
      template<CT::Data T>
      NOD() T& As(Byte*) const noexcept;
      
      NOD() constexpr const Byte* Get(const Byte*) const noexcept;
      NOD() constexpr Byte* Get(Byte*) const noexcept;

      NOD() DMeta GetType() const;
      NOD() TMeta GetTrait() const;
   };

   using MemberList = ::std::span<const Member>;

   
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
      NOD() bool operator == (const Ability&) const noexcept;

      template<CT::Dense T, CT::Data VERB, CT::Data... A>
      NOD() static Ability From() noexcept;
   };


   ///                                                                        
   ///   Meta constant, used to reflect named values for enums                
   ///                                                                        
   struct MetaConst final : public Meta {
      LANGULUS(NAME) "CMeta";
      LANGULUS_BASES(Meta);

      static constexpr Token DefaultToken = "NoConst";

      MetaType GetMetaType() const noexcept final { return Meta::Constant; }

      DMeta mValueType {};
      const void* mPtrToValue {};

   protected:
      template<CT::Data T>
      static constexpr Token GetReflectedToken() noexcept;

      NOD() constexpr bool Is(CMeta) const noexcept;

      constexpr bool operator == (const MetaConst&) const noexcept;
   };

   using NamedValueList = ::std::vector<CMeta>;
   using AbilityList = ::std::unordered_map<VMeta, Ability>;
   using MutableOverloadList = typename Ability::MutableOverloadList;
   using ConstantOverloadList = typename Ability::ConstantOverloadList;


   ///                                                                        
   ///   Used to reflect data coversions                                      
   ///                                                                        
   struct Converter {
      LANGULUS(UNALLOCATABLE) true;
      LANGULUS(UNINSERTABLE) true;

      // The data ID we're converting to                                
      DMeta mDestrinationType {};
      // Address of function to call                                    
      FCopyConstruct mFunction {};
      
   public:
      NOD() constexpr bool operator == (const Converter&) const noexcept;

      template<class T, class TO>
      NOD() static Converter From() noexcept;
   };

   using ConverterList = ::std::unordered_map<DMeta, Converter>;


   ///                                                                        
   ///   Used to reflect a base for a type                                    
   ///                                                                        
   struct Base {
      LANGULUS(UNALLOCATABLE) true;
      LANGULUS(UNINSERTABLE) true;

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
      NOD() static Base From() IF_UNSAFE(noexcept);
   };

   using BaseList = ::std::span<const Base>;


   ///                                                                        
   ///   Meta data                                                            
   ///                                                                        
   struct MetaData final : public Meta {
   private:
      struct PurposefullyIncompleteType;

      static_assert(CT::Complete<int>,
         "Can't reliably detect incomplete types");
      static_assert(!CT::Complete<PurposefullyIncompleteType>,
         "Can't reliably detect incomplete types");

   public:
      friend struct Member;
      LANGULUS(NAME) "DMeta";
      LANGULUS_BASES(Meta);

      enum Distance : int {
         Infinite = ::std::numeric_limits<int>::max()
      };
      
      static constexpr Token DefaultToken = "NoData";

      MetaType GetMetaType() const noexcept final { return Meta::Data; }

      // List of reflected members of the origin type                   
      MemberList mMembers {};
      // List of reflected abilities of the origin type                 
      AbilityList mAbilities {};
      // List of reflected bases of the origin type                     
      BaseList mBases {};
      // List of reflected converters of the origin type                
      ConverterList mConverters {};
      // List of named values of the origin type                        
      NamedValueList mNamedValues {};
      // The origin type, with all qualifiers and sparseness removed    
      // Will be nullptr for incomplete types                           
      DMeta mOrigin {};
      // The type, when a single pointer is removed                     
      // It is nullptr, if the type isn't sparse                        
      DMeta mDeptr {};
      // The type, when a const and volatile qualifiers are removed     
      // It is nullptr, if the type doesn't have any qualifiers         
      DMeta mDecvq {};
      // Default concretization                                         
      // Used as redirection, when requesting the creation of asbtracts 
      DMeta mConcrete {};
      // Types with producers can be instantiated only by the invokation
      // of Verbs::Create in the context of the producer                
      DMeta mProducer {};
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
      // True if origin type satisifes the CT::Deep concept             
      bool mIsDeep = false;
      // True if origin type is not insertable into containers          
      bool mIsUninsertable = false;
      // True if origin type is allocatable (and thus clonable)         
      bool mIsUnallocatable = false;
      // Size of the reflected type (in bytes)                          
      Size mSize {};
      // Alignof (in bytes)                                             
      Size mAlignment {};
      // Minimal allocation, in bytes                                   
      Size mAllocationPage {};
      // Precomputed counts indexed by MSB (avoids division by stride)  
      Size mAllocationTable[sizeof(Size) * 8] {};
      // File extensions used by the origin type, separated by commas   
      Token mFileExtensions {};
      // Suffix for the origin type                                     
      Token mSuffix {};

      #if LANGULUS_FEATURE(MANAGED_MEMORY)
         // The pooling tactic used for the type                        
         PoolTactic mPoolTactic = PoolTactic::Default;
         private:
            // The pool chain for the type                              
            // Access it through GetPool() method, because it might not 
            // be exactly relevan to a convoluted/sparse type           
            mutable void* mPool {};

         public:
            template<class T>
            NOD() T*& GetPool() const noexcept;
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

      // Destructor wrapped in a lambda upon reflection                 
      // @attention this always works with the origin type              
      FDestroy mDestructor {};

      // Copy-assignment operator, wrapped in a lambda upon reflection  
      // @attention this always works with the origin type              
      FCopy mCopyAssigner {};
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

   protected:
      friend struct Base;

      template<CT::Fundamental T>
      static void ReflectFundamentalType(MetaData&) noexcept;

      template<CT::Decayed T>
      static void ReflectOriginType(MetaData&) noexcept;

      NOD() const Member* GetMemberInner(TMeta, DMeta, Offset&) const noexcept;
      NOD() Count GetMemberCountInner(TMeta, DMeta, Offset&) const noexcept;

      template<class T, CT::Dense... Args>
      void SetBases(TTypeList<Args...>) noexcept;

      template<CT::Dense T, CT::Dense... Args>
      void SetAbilities(TTypeList<Args...>) noexcept;

      template<class T, class... Args>
      void SetConverters(TTypeList<Args...>) noexcept;

   public:
      template<CT::Void T>
      NOD() static constexpr DMeta Of();
      template<CT::DataReference T>
      NOD() static DMeta Of();
      template<CT::SparseData T>
      NOD() static DMeta Of();
      template<CT::DenseData T>
      NOD() static DMeta Of() requires (CT::Convoluted<T>);
      template<CT::DenseData T>
      NOD() static DMeta Of() requires (CT::Decayed<T>);

      NOD() DMeta GetMostConcrete() const noexcept;
      NOD() AllocationRequest RequestSize(const Count&) const noexcept;

      //                                                                
      // Base management                                                
      //                                                                
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
      // Member management                                              
      //                                                                
      NOD() const Member* GetMember(TMeta, DMeta = nullptr, Offset = 0) const noexcept;
      NOD() Count GetMemberCount(TMeta, DMeta = nullptr, Offset = 0) const noexcept;
      NOD() Count GetMemberCount() const noexcept;

      //                                                                
      // Ability management                                             
      //                                                                
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
      // Named value management                                         
      //                                                                
      template<class T>
      NOD() Token GetNamedValueOf(const T&) const;

      //                                                                
      // Morphisms and comparison                                       
      //                                                                
      template<class T>
      FCopyConstruct GetConverter() const noexcept;
      FCopyConstruct GetConverter(DMeta) const noexcept;

      template<bool ADVANCED = false, bool BINARY_COMPATIBLE = false>
      NOD() bool CastsTo(DMeta) const;
      template<bool BINARY_COMPATIBLE = false>
      NOD() bool CastsTo(DMeta, Count) const;

      template<CT::Data T, bool ADVANCED = false, bool BINARY_COMPATIBLE = false>
      NOD() bool CastsTo() const;
      template<CT::Data T, bool BINARY_COMPATIBLE = false>
      NOD() bool CastsTo(Count) const;

      template<CT::Data T>
      NOD() bool IsRelatedTo() const;
      NOD() bool IsRelatedTo(DMeta) const;

      template<CT::Data T>
      NOD() Distance GetDistanceTo() const;
      NOD() Distance GetDistanceTo(DMeta) const;

      template<CT::Data, CT::Data...>
      NOD() constexpr bool Is() const;
      NOD() constexpr bool Is(DMeta) const noexcept;

      template<CT::Data, CT::Data...>
      NOD() constexpr bool IsSimilar() const;
      NOD() constexpr bool IsSimilar(DMeta) const noexcept;

      template<CT::Data, CT::Data...>
      NOD() constexpr bool IsExact() const;
      NOD() constexpr bool IsExact(DMeta) const noexcept;

      constexpr bool operator == (const MetaData&) const noexcept;
   };

} // namespace Langulus::RTTI
