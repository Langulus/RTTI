///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Numbers.hpp"


namespace Langulus
{

   ///                                                                        
   ///   A byte                                                               
   ///                                                                        
   ///   std::byte is shitty, this one's better. It preserves arithmetic      
   /// operations on the byte. These operations counteract integer promotion, 
   /// the result is always truncated back down to a byte.                    
   ///                                                                        
   #pragma pack(push, 1)
   struct Byte {
      using Type = ::std::uint8_t;
      LANGULUS(TYPED) Type;
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

      Type mValue {};

   public:
      constexpr Byte() noexcept = default;
      constexpr Byte(const Byte&) noexcept = default;
      constexpr Byte(Byte&&) noexcept = default;

      LANGULUS(INLINED)
      constexpr Byte(const Type& a) noexcept
         : mValue {a} {}

      Byte& operator = (const Byte&) noexcept = default;
      Byte& operator = (Byte&&) noexcept = default;

      LANGULUS(INLINED)
      Byte& operator = (const Type& a) noexcept {
         mValue = a;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr explicit operator Type& () const noexcept {
         return const_cast<Type&>(mValue);
      }

      template<CT::BuiltinNumber T> LANGULUS(INLINED)
      constexpr explicit operator T () const noexcept
      requires (CT::Dense<T> and not CT::Same<T, Type>) {
         return static_cast<T>(mValue);
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator + (const Byte& rhs) const noexcept {
         return mValue + rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator - (const Byte& rhs) const noexcept {
         return mValue - rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator * (const Byte& rhs) const noexcept {
         return mValue * rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator / (const Byte& rhs) const noexcept {
         return mValue / rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator % (const Byte& rhs) const noexcept {
         return mValue % rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator << (const Byte& rhs) const noexcept {
         return mValue << rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator >> (const Byte& rhs) const noexcept {
         return mValue >> rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator ^ (const Byte& rhs) const noexcept {
         return mValue ^ rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator & (const Byte& rhs) const noexcept {
         return mValue & rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr Byte operator | (const Byte& rhs) const noexcept {
         return mValue | rhs.mValue;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator += (const Byte& rhs) noexcept {
         mValue += rhs.mValue;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator -= (const Byte& rhs) noexcept {
         mValue -= rhs.mValue;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator *= (const Byte& rhs) noexcept {
         mValue *= rhs.mValue;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator /= (const Byte& rhs) noexcept {
         mValue /= rhs.mValue;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator %= (const Byte& rhs) noexcept {
         mValue %= rhs.mValue;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator <<= (const Byte& rhs) noexcept {
         mValue <<= rhs.mValue;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator >>= (const Byte& rhs) noexcept {
         mValue >>= rhs.mValue;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator ^= (const Byte& rhs) noexcept {
         mValue ^= rhs.mValue;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator &= (const Byte& rhs) noexcept {
         mValue &= rhs.mValue;
         return *this;
      }

      LANGULUS(INLINED)
      constexpr Byte& operator |= (const Byte& rhs) noexcept {
         mValue |= rhs.mValue;
         return *this;
      }

      NOD() LANGULUS(INLINED)
      constexpr bool operator == (const Byte&) const noexcept = default;

      NOD() LANGULUS(INLINED)
      constexpr bool operator <= (const Byte& rhs) const noexcept {
         return mValue <= rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr bool operator >= (const Byte& rhs) const noexcept {
         return mValue >= rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr bool operator < (const Byte& rhs) const noexcept {
         return mValue < rhs.mValue;
      }

      NOD() LANGULUS(INLINED)
      constexpr bool operator > (const Byte& rhs) const noexcept {
         return mValue > rhs.mValue;
      }

      /// Prefix operators                                                    
      Byte& operator ++ () noexcept {
         ++mValue;
         return *this;
      }
      Byte& operator -- () noexcept {
         --mValue;
         return *this;
      }

      /// Suffix operators                                                    
      NOD() Byte operator ++ (int) noexcept {
         return mValue++;
      }

      NOD() Byte operator -- (int) noexcept {
         return mValue--;
      }
   };
   #pragma pack(pop)

   namespace CT
   {

      /// Built-in byte concept                                               
      template<class...T>
      concept BuiltinByte = sizeof...(T) > 0 and ((
            SimilarAsOneOf<Deref<T>, ::Langulus::Byte, ::std::byte>
         ) and ...);

      /// Custom byte concept (wrapped in another type)                       
      template<class...T>
      concept CustomByte = ((Typed<T> and
            BuiltinByte<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
         ) and ...);

      /// Byte concept                                                        
      template<class...T>
      concept Byte = ((BuiltinByte<T> or CustomByte<T>) and ...);

      /// Any unsigned character, byte or integer, sized exactly 1 byte       
      template<class...T>
      concept UnsignedInteger8 = sizeof...(T) > 0
          and (((UnsignedInteger<T> or Character<T> or Byte<T>)
          and sizeof(Decay<T>) == 1) and ...);

      /// Any signed character, byte or integer, sized exactly 1 byte         
      template<class...T>
      concept Integer8 = sizeof...(T) > 0
          and ((SignedInteger8<T> or UnsignedInteger8<T>) and ...);

      template<class...T>
      concept IntegerX = sizeof...(T) > 0
          and ((Integer8<T> or Integer16<T> or Integer32<T> or Integer64<T>) and ...);

   } // namespace Langulus::CT

   
   /// Wrapper for memcpy                                                     
   ///   @param to - [out] destination memory                                 
   ///   @param from - source of data to copy                                 
   template<class TO, class FROM> LANGULUS(INLINED)
   void CopyMemory(TO* to, const FROM* from) noexcept {
      static_assert(CT::Void<TO> or CT::Sparse<TO> or CT::POD<TO>, 
         "TO must be either pointer, reflected as POD, or trivial "
         "(you can suppress this error by casting pointer to void*)");

      static_assert(CT::Void<TO> or CT::Similar<TO, FROM>,
         "TO and FROM must be similar types "
         "(you can suppress this error by casting pointer to void*)");

      if constexpr (CT::Void<TO>)
         LANGULUS_ERROR("Bytecount not specified when copying void pointers");

      ::std::memcpy(
         static_cast<void*>(to),
         static_cast<const void*>(from),
         sizeof(TO)
      );
   }

   /// Wrapper for memcpy                                                     
   ///   @param to - [out] destination memory                                 
   ///   @param from - source of data to copy                                 
   ///   @param count - number of elements to copy                            
   ///   @attention count becomes bytecount, when TO is void                  
   template<class TO, class FROM> LANGULUS(INLINED)
   void CopyMemory(TO* to, const FROM* from, const Count& count) noexcept {
      static_assert(CT::Void<TO> or CT::Sparse<TO> or CT::POD<TO>,
         "TO must be either pointer, reflected as POD, or trivial "
         "(you can suppress this error by casting pointer to void*)");

      static_assert(CT::Void<TO> or CT::Similar<TO, FROM>,
         "TO and FROM must be similar types "
         "(you can suppress this error by casting pointer to void*)");

      if constexpr (CT::Void<TO>) {
         ::std::memcpy(
            static_cast<void*>(to),
            static_cast<const void*>(from),
            count
         );
      }
      else {
         ::std::memcpy(
            static_cast<void*>(to),
            static_cast<const void*>(from),
            sizeof(TO) * count
         );
      }
   }
   
   /// Wrapper for memset                                                     
   ///   @tparam FILLER - value to fill in with                               
   ///   @param to - [out] destination memory                                 
   template<int FILLER, class TO> LANGULUS(INLINED)
   void FillMemory(TO* to) noexcept {
      static_assert(FILLER or CT::Nullifiable<TO> or CT::Void<TO>,
         "Filling with zeroes requires the type to be reflected as nullifiable, "
         "or be a pointer/fundamental (you can suppress this error by casting to void*)");

      if constexpr (CT::Void<TO>)
         LANGULUS_ERROR("Bytecount not specified when filling void pointer");
      
      ::std::memset(static_cast<void*>(to), FILLER, sizeof(TO));
   }
   
   /// Wrapper for memset                                                     
   ///   @tparam FILLER - value to fill in with                               
   ///   @param to - [out] destination memory                                 
   ///   @param count - number of elements to fill                            
   ///   @attention count becomes bytecount, when TO is void                  
   template<int FILLER, class TO> LANGULUS(INLINED)
   void FillMemory(TO* to, const Count& count) noexcept {
      static_assert(FILLER or CT::Nullifiable<TO> or CT::Void<TO>,
         "Filling with zeroes requires the type to be reflected as nullifiable, "
         "or be a pointer/fundamental (you can suppress this error by casting to void*)");

      if constexpr (CT::Void<TO>)
         ::std::memset(static_cast<void*>(to), FILLER, count);
      else
         ::std::memset(static_cast<void*>(to), FILLER, sizeof(TO) * count);
   }

   /// Wrapper for memset 0                                                   
   ///   @param to - [out] destination memory                                 
   template<class TO> LANGULUS(INLINED)
   void ZeroMemory(TO* to) noexcept {
      return FillMemory<0>(to);
   }
      
   /// Wrapper for memset 0                                                   
   ///   @param to - [out] destination memory                                 
   ///   @param count - number of elements to fill                            
   ///   @attention count becomes bytecount, when TO is void                  
   template<class TO> LANGULUS(INLINED)
   void ZeroMemory(TO* to, const Count& count) noexcept {
      return FillMemory<0>(to, count);
   }
      
   /// Wrapper for memset 0, with overlapping memory check                    
   ///   @param from - [out] source memory                                    
   ///   @param to - destination memory                                       
   ///   @param count - number of elements to fill                            
   ///   @attention count becomes bytecount, when TO is void                  
   template<class TO> LANGULUS(INLINED)
   void ZeroMemoryOverlapped(TO* from, const TO* to, const Count& count) noexcept {
      if (from >= to + count or from + count <= to) {
         // No overlap                                                  
         // From: [][][][][][]                                          
         // To:                 [][][][][][]                            
         // Zero: [][][][][][]                                          
         ZeroMemory(from, count);
      }
      else {
         // Zero only the difference                                    
         if (from > to) {
            // From:         [][][][][][][][][][][][]                   
            // To:   [][][][][][][][][][][][]                           
            // Zero:                         [][][][]                   
            const auto diff = from - to;
            ZeroMemory(from + count - diff, diff);
         }
         else {
            // From: [][][][][][][][][][][][]                           
            // To:           [][][][][][][][][][][][]                   
            // Zero: [][][][]                                           
            ZeroMemory(from, to - from);
         }
      }
   }
      
   /// Wrapper for memmove                                                    
   ///   @param to - [out] destination memory                                 
   ///   @param from - source of data to move                                 
   template<class TO, class FROM> LANGULUS(INLINED)
   void MoveMemory(TO* to, const FROM* from) noexcept {
      static_assert(CT::Void<TO> or CT::Sparse<TO> or CT::POD<TO>,
         "TO must be either pointer, reflected as POD, or trivial "
         "(You can suppress this error by casting pointer to void*)");

      static_assert(CT::Void<TO> or CT::Similar<TO, FROM>,
         "TO and FROM must be similar types "
         "(you can suppress this error by casting pointer to void*)");

      if constexpr (CT::Void<TO>)
         LANGULUS_ERROR("Bytecount not specified when filling void pointer");

      ::std::memmove(
         static_cast<void*>(to),
         static_cast<const void*>(from),
         sizeof(TO)
      );

      #if LANGULUS(PARANOID)
         TODO() // zero old memory, but beware - `from` and `to` might overlap
      #endif
   }

   /// Wrapper for memmove                                                    
   ///   @param to - [out] destination memory                                 
   ///   @param from - source of data to move                                 
   ///   @param count - number of elements to move                            
   ///   @attention count becomes bytecount, when TO is void                  
   template<class TO, class FROM> LANGULUS(INLINED)
   void MoveMemory(TO* to, const FROM* from, const Count& count) noexcept {
      static_assert(CT::Void<TO> or CT::Sparse<TO> or CT::POD<TO>,
         "TO must be either pointer, reflected as POD, or trivial "
         "(You can suppress this error by casting pointer to void*)");

      static_assert(CT::Void<TO> or CT::Similar<TO, FROM>,
         "TO and FROM must be similar types "
         "(you can suppress this error by casting pointer to void*)");

      if constexpr (CT::Void<TO>) {
         ::std::memmove(
            static_cast<void*>(to),
            static_cast<const void*>(from),
            count
         );
      }
      else {
         ::std::memmove(
            static_cast<void*>(to),
            static_cast<const void*>(from),
            sizeof(TO) * count
         );
      }

      #if LANGULUS(PARANOID)
         TODO() // zero old memory, but beware - `from` and `to` might overlap
      #endif
   }

} // namespace Langulus
