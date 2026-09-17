///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Meta.hpp"
#include "DefinitionData.hpp"


namespace Langulus::Fractalloc
{
   struct Allocator;
}

namespace Langulus::RTTI
{
   namespace Inner
   {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      ///                                                                     
      /// These rely on the definition limits to pack an ID into the smallest 
      /// possible space, but also uses some additional bits to encode some   
      /// often used information about the definition. The handle still has   
      /// to be transformed into a pointer for more advanced uses, but in     
      /// general it is likely to avoid an indirection altogether at the      
      /// cost of a bitwise operation, making it a bit more cache-friendly,   
      /// and worth experimenting with                                        
      template<uint S>
      struct Structured;

      /// Encodes most frequently used properties                             
      #pragma pack(push, 1)
      template<> struct Structured<1> {
      protected:
         union {
            struct {
               // The set of the main properties                        
               bool sparse     : 1;
               bool constant   : 1;
               bool deep       : 1;
               bool pod        : 1;
               bool nullable   : 1;
               bool referenced : 1;
               bool resolvable : 1;
               bool dispatcher : 1;
            };
            uint8_t all {};
         };
      };
      #pragma pack(pop)
      //static_assert(sizeof(Structured<1>) == 1);

      /// Encodes most frequently used properties and the size up to 255 bytes
      #pragma pack(push, 1)
      template<> struct Structured<2> {
      protected:
         union {
            struct {
               // Stores the size up to 255 bytes                       
               // A value of zero means size is bigger, and a lookup    
               // into the definition is required. The size of the type 
               // is probably the most used property                    
               uint8_t size;

               // The set of the main properties                        
               bool sparse     : 1;
               bool constant   : 1;
               bool deep       : 1;
               bool pod        : 1;
               bool nullable   : 1;
               bool referenced : 1;
               bool resolvable : 1;
               bool dispatcher : 1;
            };
            uint16_t all {};
         };
      };
      #pragma pack(pop)
      //static_assert(sizeof(Structured<2>) == 2);

      /// This is the most commonly used packing tactic, until proven not     
      /// sufficient. It contains the most packed properties and should be    
      /// the fastest, due to the smallest chance of an indirection           
      /// Packing strategy that can't exceed 2^(8*ID_SIZE)-2 possible types   
      ///   @tparam ID_SIZE the size reserved for unique ID                   
      ///   @tparam PT_SIZE the size reserved for properties                  
      #pragma pack(push, 1)
      template<uint ID_SIZE, uint PT_SIZE>
      struct MetaDataStructured_XY : MetaPacked<ID_SIZE>, Structured<PT_SIZE> {
      protected:
         using Structured<PT_SIZE>::sparse;
         using Structured<PT_SIZE>::constant;
         using Structured<PT_SIZE>::deep;
         using Structured<PT_SIZE>::pod;
         using Structured<PT_SIZE>::nullable;
         using Structured<PT_SIZE>::referenced;
         using Structured<PT_SIZE>::resolvable;
         using Structured<PT_SIZE>::dispatcher;
         using Structured<PT_SIZE>::all;

         auto GetDefinition() const noexcept -> DefinitionData const*;

      public:
         using CTTI_Abstract = No;
         using Base = MetaPacked<ID_SIZE>;

         constexpr MetaDataStructured_XY() noexcept = default;
         constexpr MetaDataStructured_XY(MetaDataStructured_XY const&) noexcept = default;
         constexpr MetaDataStructured_XY(MetaDataStructured_XY&&) noexcept = default;
         constexpr MetaDataStructured_XY(nullptr_t) noexcept;
         constexpr MetaDataStructured_XY(DefinitionData const*) noexcept;

         constexpr MetaDataStructured_XY& operator = (MetaDataStructured_XY const&) noexcept = default;
         constexpr MetaDataStructured_XY& operator = (MetaDataStructured_XY&&) noexcept = default;
         constexpr MetaDataStructured_XY& operator = (nullptr_t) noexcept;
         constexpr MetaDataStructured_XY& operator = (DefinitionData const*) noexcept;

         bool Is(const MetaDataStructured_XY&) const noexcept;
         constexpr bool IsExact(const MetaDataStructured_XY&) const noexcept;
         constexpr bool IsSame(const MetaDataStructured_XY&) const noexcept;
         constexpr bool operator == (const MetaDataStructured_XY&) const noexcept;

         constexpr auto GetSize()     const noexcept -> size_t;
         auto GetAlignment()          const noexcept -> pot_t;
         auto GetName()               const noexcept -> Token;
         auto GetInfo()               const noexcept -> Token;
         auto GetCppName()            const noexcept -> Token;
         auto GetHash()               const noexcept -> Hash;
         auto GetFiles()              const noexcept -> Token;
         auto GetSuffix()             const noexcept -> Token;
         auto GetBoundaries()         const noexcept -> Definition::BoundarySet const&;
         auto GetVersionMajor()       const noexcept -> uint;
         auto GetVersionMinor()       const noexcept -> uint;
         auto GetMinAllocation()      const noexcept -> pot_t;
         auto GetAllocationTable()    const noexcept -> size_t const*;

         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            auto GetMinPoolsize()     const noexcept -> pot_t;
            auto GetPoolTactic()      const noexcept -> PoolTactic;
         #endif

         constexpr auto GetIndirections() const noexcept -> size_t;
         constexpr auto GetIndirections(bool& custom_pointer_encountered) const noexcept -> size_t;
         constexpr auto GetPointerSpecification() const noexcept -> PointerSpecification;
         constexpr bool IsDense()     const noexcept;
         constexpr bool IsSparse()    const noexcept;
         constexpr bool IsConstant()  const noexcept;
         constexpr bool IsMutable()   const noexcept;
         constexpr bool IsDeep()      const noexcept;
         constexpr bool IsPOD()       const noexcept;
         constexpr bool IsNullable()  const noexcept;
         constexpr bool IsAbstract()  const noexcept;
         constexpr bool IsExecutable()const noexcept;
         constexpr bool HasGetHashMethod() const noexcept;

         auto GetDereffer()           const noexcept -> DefinitionData::FBinary;
         auto GetDestructor()         const noexcept -> DefinitionData::FUnary;
         auto GetReferencer()         const noexcept -> DefinitionData::FReference;
         auto GetResolver()           const noexcept -> DefinitionData::FResolve;
         auto GetDefaultConstructor() const noexcept -> DefinitionData::FUnary;
         auto GetDescribeConstructor()const noexcept -> DefinitionData::FDescribe;
         auto GetReferConstructor()   const noexcept -> DefinitionData::FBinary;
         auto GetReferAssigner()      const noexcept -> DefinitionData::FBinary;
         auto GetMoveConstructor()    const noexcept -> DefinitionData::FBinary;
         auto GetMoveAssigner()       const noexcept -> DefinitionData::FBinary;
         auto GetAbandonConstructor() const noexcept -> DefinitionData::FBinary;
         auto GetAbandonAssigner()    const noexcept -> DefinitionData::FBinary;
         auto GetDisownConstructor()  const noexcept -> DefinitionData::FBinary;
         auto GetDisownAssigner()     const noexcept -> DefinitionData::FBinary;
         auto GetCloneConstructor()   const noexcept -> DefinitionData::FBinary;
         auto GetCloneAssigner()      const noexcept -> DefinitionData::FBinary;
         auto GetCopyConstructor()    const noexcept -> DefinitionData::FBinary;
         auto GetCopyAssigner()       const noexcept -> DefinitionData::FBinary;
         auto GetComparer()           const noexcept -> DefinitionData::FCompare;
         auto GetComparerEqual()      const noexcept -> DefinitionData::FCompareEqual;
         auto GetHasher()             const noexcept -> DefinitionData::FHash;
         auto GetDispatcher()         const noexcept -> DefinitionData::FDispatch;
         
         auto GetDeptr(size_t = 1)    const -> MetaDataStructured_XY;
         auto GetOrigin()             const -> MetaDataStructured_XY;
         auto GetDecvqAll()           const -> MetaDataStructured_XY;
         auto GetDecvq()              const -> MetaDataStructured_XY;
         auto AddPtr()                const -> MetaDataStructured_XY;
         auto AddConst()              const -> MetaDataStructured_XY;
         auto GetConcrete()           const -> MetaDataStructured_XY;
         auto GetProducer()           const -> MetaDataStructured_XY;
              
         auto GetBases()              const noexcept -> DefinitionData::BaseList const&;
         auto GetVerbs()              const noexcept -> DefinitionData::VerbList const&;
         auto GetMembers()            const noexcept -> DefinitionData::MemberList const&;
         auto GetNamedValues()        const noexcept -> DefinitionData::ValuesList const&;
         auto GetMorphismsTo()        const noexcept -> DefinitionData::MorphismList const&;
         //auto GetMorphismsFrom()      const noexcept -> DefinitionData::MorphismList const&;
         auto GetMorphism(MetaDataStructured_XY) const noexcept -> DefinitionData::Morphism;

         IF_SAFE(LglsNoSideEffects explicit operator bool() const noexcept);
      };
      #pragma pack(pop)
      
      /*static_assert(sizeof(MetaDataStructured_XY<1, 1>) == 2);
      static_assert(sizeof(MetaDataStructured_XY<2, 1>) == 3);
      static_assert(sizeof(MetaDataStructured_XY<3, 1>) == 4);
      static_assert(sizeof(MetaDataStructured_XY<1, 2>) == 3);
      static_assert(sizeof(MetaDataStructured_XY<2, 2>) == 4);
      static_assert(sizeof(MetaDataStructured_XY<3, 2>) == 5);*/
   #endif

      ///                                                                     
      /// A naked pointer to a definition. Probably (not likely) the fastest, 
      /// but most memory-inefficient on 64bit systems                        
      struct MetaDataNaked : MetaNaked<DefinitionData> {
         using CTTI_Abstract = No;
         using Base = MetaNaked;
         using Base::Base;
         using Base::operator =;
         using Base::operator bool;

         bool Is(const MetaDataNaked&) const noexcept;
         bool IsSame(const MetaDataNaked&) const noexcept;

         auto GetSize()               const noexcept -> size_t;
         auto GetAlignment()          const noexcept -> pot_t;
         auto GetFiles()              const noexcept -> Token;
         auto GetSuffix()             const noexcept -> Token;
         auto GetMinAllocation()      const noexcept -> pot_t;
         auto GetAllocationTable()    const noexcept -> size_t const*;

         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            auto GetMinPoolsize()     const noexcept -> pot_t;
            auto GetPoolTactic()      const noexcept -> PoolTactic;
         #endif
                                      
         auto GetIndirections()       const noexcept -> size_t;
         auto GetIndirections(bool& custom_pointer_encountered) const noexcept -> size_t;
         auto GetPointerSpecification() const noexcept -> PointerSpecification;
         bool IsDense()               const noexcept;
         bool IsSparse()              const noexcept;
         bool IsConstant()            const noexcept;
         bool IsMutable()             const noexcept;
         bool IsDeep()                const noexcept;
         bool IsPOD()                 const noexcept;
         bool IsNullable()            const noexcept;
         bool IsAbstract()            const noexcept;
         bool IsExecutable()          const noexcept;
         bool HasGetHashMethod()      const noexcept;

         auto GetDereffer()           const noexcept -> DefinitionData::FBinary;
         auto GetDestructor()         const noexcept -> DefinitionData::FUnary;
         auto GetReferencer()         const noexcept -> DefinitionData::FReference;
         auto GetResolver()           const noexcept -> DefinitionData::FResolve;
         auto GetDefaultConstructor() const noexcept -> DefinitionData::FUnary;
         auto GetDescribeConstructor()const noexcept -> DefinitionData::FDescribe;
         auto GetReferConstructor()   const noexcept -> DefinitionData::FBinary;
         auto GetReferAssigner()      const noexcept -> DefinitionData::FBinary;
         auto GetMoveConstructor()    const noexcept -> DefinitionData::FBinary;
         auto GetMoveAssigner()       const noexcept -> DefinitionData::FBinary;
         auto GetAbandonConstructor() const noexcept -> DefinitionData::FBinary;
         auto GetAbandonAssigner()    const noexcept -> DefinitionData::FBinary;
         auto GetDisownConstructor()  const noexcept -> DefinitionData::FBinary;
         auto GetDisownAssigner()     const noexcept -> DefinitionData::FBinary;
         auto GetCloneConstructor()   const noexcept -> DefinitionData::FBinary;
         auto GetCloneAssigner()      const noexcept -> DefinitionData::FBinary;
         auto GetCopyConstructor()    const noexcept -> DefinitionData::FBinary;
         auto GetCopyAssigner()       const noexcept -> DefinitionData::FBinary;
         auto GetComparer()           const noexcept -> DefinitionData::FCompare;
         auto GetComparerEqual()      const noexcept -> DefinitionData::FCompareEqual;
         auto GetHasher()             const noexcept -> DefinitionData::FHash;
         auto GetDispatcher()         const noexcept -> DefinitionData::FDispatch;

         auto GetDeptr(size_t = 1)    const -> MetaDataNaked;
         auto GetOrigin()             const -> MetaDataNaked;
         auto GetDecvqAll()           const -> MetaDataNaked;
         auto GetDecvq()              const -> MetaDataNaked;
         auto AddPtr()                const -> MetaDataNaked;
         auto AddConst()              const -> MetaDataNaked;
         auto GetConcrete()           const -> MetaDataNaked;
         auto GetProducer()           const -> MetaDataNaked;

         auto GetBases()              const noexcept -> DefinitionData::BaseList const&;
         auto GetVerbs()              const noexcept -> DefinitionData::VerbList const&;
         auto GetMembers()            const noexcept -> DefinitionData::MemberList const&;
         auto GetNamedValues()        const noexcept -> DefinitionData::ValuesList const&;
         auto GetMorphismsTo()        const noexcept -> DefinitionData::MorphismList const&;
         //auto GetMorphismsFrom()      const noexcept -> DefinitionData::MorphismList const&;
         auto GetMorphism(MetaDataNaked) const noexcept -> DefinitionData::Morphism;
      };
   }
}

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   #include "MetaDataStructured.inl"
#endif

#include "MetaDataNaked.inl"

#if LANGULUS_FEATURE(LOGGING)
namespace fmt
{
   using uint = ::Langulus::uint;

   ///                                                                        
   /// Extend FMT to be capable of logging data types                         
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
   template<uint ID_SIZE, uint PT_SIZE>
   struct formatter<::Langulus::RTTI::Inner::MetaDataStructured_XY<ID_SIZE, PT_SIZE>> {
      using M = ::Langulus::RTTI::Inner::MetaDataStructured_XY<ID_SIZE, PT_SIZE>;

      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {return ctx.begin();}

      template<class CONTEXT>
      auto format(M const& c, CONTEXT& ctx) const {
         return format_to(ctx.out(), "{}", c.GetName());
      }
   };
   #endif
   
   template<>
   struct formatter<::Langulus::RTTI::Inner::MetaDataNaked> {
      using M = ::Langulus::RTTI::Inner::MetaDataNaked;

      template<class CONTEXT>
      constexpr auto parse(CONTEXT& ctx) {return ctx.begin();}

      template<class CONTEXT>
      auto format(M const& c, CONTEXT& ctx) const {
         return format_to(ctx.out(), "{}", c.GetName());
      }
   };
}
#endif