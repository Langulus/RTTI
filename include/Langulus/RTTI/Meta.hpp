///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Definition.hpp"

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   #include <array>
#endif


namespace Langulus::RTTI
{
   class Registry;
}

namespace Langulus::RTTI::Inner
{

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   ///                                                                        
   /// Relies on the definition limits to pack an ID into the smallest        
   /// possible space. We would never have 64bit worth of type definitions    
   /// in a program either way. If somehow you do, then you're probably doing 
   /// something wrong. The handle has to be transformed into a pointer, so   
   /// this requires an additional level of indirection                       
   ///   @tparam BYTESIZE the size of the handle in bytes                     
   #pragma pack(push, 1)
   template<unsigned BYTESIZE>
   struct MetaPacked {
      using CTTI_Abstract  = Yup;
      using CTTI_POD       = Yup;
      using CTTI_Nullable  = Yup;
      using CTTI_Meta      = Yup;

   protected:
      friend class RTTI::Registry;
      using Block = ::std::array<uint8_t, BYTESIZE>;
      static constexpr Block Zero {0};
      Block mHandle {0};

      union convert {
         size_t id_unprocessed;
         Block id_processed;

         explicit convert(const size_t& t) : id_unprocessed {t} {}
         explicit convert(const Block&  t) : id_unprocessed {0} {
            id_processed = t;
         }
      };

   public:
      constexpr MetaPacked() noexcept = default;
      constexpr MetaPacked(const MetaPacked&) noexcept = default;
      constexpr MetaPacked(MetaPacked&&) noexcept = default;
      constexpr MetaPacked(size_t id) noexcept {
         static_assert(sizeof(size_t) >= BYTESIZE);
         mHandle = convert(id).id_processed;
      }

      constexpr MetaPacked& operator = (const MetaPacked&) noexcept = default;
      constexpr MetaPacked& operator = (MetaPacked&&) noexcept = default;
      constexpr MetaPacked& operator = (size_t id) noexcept {
         static_assert(sizeof(size_t) >= BYTESIZE);
         mHandle = convert(id).id_processed;
         return *this;
      }

      LglsNoSideEffects
      constexpr explicit operator bool() const noexcept {
         return mHandle != Zero;
      }

      constexpr bool operator == (const MetaPacked& rhs) const noexcept {
         return mHandle == rhs.mHandle;
      }

      constexpr size_t GetID() const noexcept {
         if constexpr (BYTESIZE == 1)
            return static_cast<size_t>(mHandle[0]);
         else
            return convert(mHandle).id_unprocessed;
      }
   };
   #pragma pack(pop)
#endif


   ///                                                                        
   /// A naked pointer to a definition. Probably the fastest, but most        
   /// memory-inefficient on 64bit systems                                    
   ///   @tparam T the type of the meta (data/tag/verb/const)                 
   template<class T>
   struct MetaNaked {
      using CTTI_Abstract  = Yup;
      using CTTI_POD       = Yup;
      using CTTI_Nullable  = Yup;
      using CTTI_Meta      = Yup;

   protected:
      const T* mDefinition = nullptr;

   public:
      constexpr MetaNaked() noexcept = default;
      constexpr MetaNaked(const MetaNaked&) noexcept = default;
      constexpr MetaNaked(MetaNaked&&) noexcept = default;
      constexpr MetaNaked(nullptr_t) noexcept {}
      constexpr MetaNaked(const T* d) noexcept
         : mDefinition {d} {}

      constexpr MetaNaked& operator = (const MetaNaked&) noexcept = default;
      constexpr MetaNaked& operator = (MetaNaked&&) noexcept = default;
      constexpr MetaNaked& operator = (nullptr_t) noexcept {
         mDefinition = nullptr;
         return *this;
      }
      constexpr MetaNaked& operator = (const T* d) noexcept {
         mDefinition = d;
         return *this;
      }

      LglsNoSideEffects
      constexpr explicit operator bool() const noexcept {
         return mDefinition != nullptr;
      }

      constexpr bool IsExact(const MetaNaked& rhs) const noexcept {
         return mDefinition == rhs.mDefinition;
      }

      constexpr bool operator == (const MetaNaked& rhs) const noexcept {
         return mDefinition == rhs.mDefinition;
      }

      auto GetDefinition() const noexcept -> T const* {
         return mDefinition;
      }

      /// Get the name of the type, the result of NameOf                      
      auto GetName() const noexcept -> Token {
         if (mDefinition)
            return mDefinition->mNameOf;
         return T::InvalidName;
      }
   
      /// Get the info of the type, the result of InfoOf                      
      auto GetInfo() const noexcept -> Token {
         if (mDefinition)
            return mDefinition->mInfoOf;
         return {};
      }
   
      /// Get the name of the type as it appearch in C++                      
      auto GetCppName() const noexcept -> Token {
         if (mDefinition)
            return mDefinition->mCppNameOf;
         return {};
      }

      /// Get the type hash                                                   
      auto GetHash() const noexcept -> Hash {
         if (mDefinition)
            return mDefinition->mHash;
         return {};
      }
   
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      /// Get the active type boundaries                                      
      auto GetBoundaries() const noexcept -> Definition::BoundarySet const& {
         if (mDefinition)
            return mDefinition->mBoundaries;

         static const Definition::BoundarySet fallback;
         return fallback;
      }
   #endif

      /// Get the major version                                               
      unsigned GetVersionMajor() const noexcept {
         if (mDefinition)
            return mDefinition->mVersionMajor;
         return 0;
      }

      /// Get the minor version                                               
      unsigned GetVersionMinor() const noexcept {
         if (mDefinition)
            return mDefinition->mVersionMinor;
         return 0;
      }
   };
}
