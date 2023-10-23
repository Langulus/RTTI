///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
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
   struct Byte {
      LANGULUS(TYPED) ::std::uint8_t;
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

      ::std::uint8_t mValue {};

   public:
      constexpr Byte() noexcept = default;
      constexpr Byte(const Byte&) noexcept = default;
      constexpr Byte(Byte&&) noexcept = default;
      constexpr Byte(const ::std::uint8_t& a) noexcept
         : mValue {a} {}

      Byte& operator = (const Byte&) noexcept = default;
      Byte& operator = (Byte&&) noexcept = default;
      Byte& operator = (const ::std::uint8_t& a) noexcept {
         mValue = a;
         return *this;
      }

      /// All conversions are explicit only, to preserve type                 
      constexpr explicit operator const ::std::uint8_t& () const noexcept {
         return mValue;
      }
      constexpr explicit operator ::std::uint8_t& () noexcept {
         return mValue;
      }

      template<CT::BuiltinNumber T>
      constexpr explicit operator T () const noexcept
      requires (CT::Dense<T> and not CT::Same<T, ::std::uint8_t>) {
         return static_cast<T>(mValue);
      }

      NOD() constexpr Byte operator + (const Byte& rhs) const noexcept {
         return mValue + rhs.mValue;
      }
      NOD() constexpr Byte operator - (const Byte& rhs) const noexcept {
         return mValue - rhs.mValue;
      }
      NOD() constexpr Byte operator * (const Byte& rhs) const noexcept {
         return mValue * rhs.mValue;
      }
      NOD() constexpr Byte operator / (const Byte& rhs) const noexcept {
         return mValue / rhs.mValue;
      }
      NOD() constexpr Byte operator % (const Byte& rhs) const noexcept {
         return mValue % rhs.mValue;
      }
      NOD() constexpr Byte operator << (const Byte& rhs) const noexcept {
         return mValue << rhs.mValue;
      }
      NOD() constexpr Byte operator >> (const Byte& rhs) const noexcept {
         return mValue >> rhs.mValue;
      }
      NOD() constexpr Byte operator ^ (const Byte& rhs) const noexcept {
         return mValue ^ rhs.mValue;
      }
      NOD() constexpr Byte operator & (const Byte& rhs) const noexcept {
         return mValue & rhs.mValue;
      }
      NOD() constexpr Byte operator | (const Byte& rhs) const noexcept {
         return mValue | rhs.mValue;
      }

      constexpr Byte& operator += (const Byte& rhs) noexcept {
         mValue += rhs.mValue;
         return *this;
      }
      constexpr Byte& operator -= (const Byte& rhs) noexcept {
         mValue -= rhs.mValue;
         return *this;
      }
      constexpr Byte& operator *= (const Byte& rhs) noexcept {
         mValue *= rhs.mValue;
         return *this;
      }
      constexpr Byte& operator /= (const Byte& rhs) noexcept {
         mValue /= rhs.mValue;
         return *this;
      }
      constexpr Byte& operator %= (const Byte& rhs) noexcept {
         mValue %= rhs.mValue;
         return *this;
      }
      constexpr Byte& operator <<= (const Byte& rhs) noexcept {
         mValue <<= rhs.mValue;
         return *this;
      }
      constexpr Byte& operator >>= (const Byte& rhs) noexcept {
         mValue >>= rhs.mValue;
         return *this;
      }
      constexpr Byte& operator ^= (const Byte& rhs) noexcept {
         mValue ^= rhs.mValue;
         return *this;
      }
      constexpr Byte& operator &= (const Byte& rhs) noexcept {
         mValue &= rhs.mValue;
         return *this;
      }
      constexpr Byte& operator |= (const Byte& rhs) noexcept {
         mValue |= rhs.mValue;
         return *this;
      }

      NOD() constexpr bool operator == (const Byte&) const noexcept = default;
      NOD() constexpr bool operator <= (const Byte& rhs) const noexcept {
         return mValue <= rhs.mValue;
      }
      NOD() constexpr bool operator >= (const Byte& rhs) const noexcept {
         return mValue >= rhs.mValue;
      }
      NOD() constexpr bool operator < (const Byte& rhs) const noexcept {
         return mValue < rhs.mValue;
      }
      NOD() constexpr bool operator > (const Byte& rhs) const noexcept {
         return mValue > rhs.mValue;
      }
   };

   namespace CT
   {

      /// Byte concept                                                        
      template<class... T>
      concept Byte = (Same<::Langulus::Byte, T> and ...);

      template<class... T>
      concept UnsignedInteger8 = (((CT::UnsignedInteger<T> or CT::Character<T> or CT::Byte<T>)
         and sizeof(Decay<T>) == 1) and ...);

      template<class... T>
      concept Integer8 = ((SignedInteger8<T> or UnsignedInteger8<T>)  and ...);

      template<class... T>
      concept IntegerX = ((Integer8<T> or Integer16<T> or Integer32<T> or Integer64<T>) and ...);

   }
   
   /// Wrapper for memcpy                                                     
   ///   @tparam TO - destination memory type (deducible)                     
   ///   @tparam FROM - source memory type (deducible)                        
   ///   @param to - [out] destination memory                                 
   ///   @param from - source of data to copy                                 
   template<class TO, class FROM>
   LANGULUS(INLINED)
   void CopyMemory(TO* to, const FROM* from) noexcept {
      static_assert(CT::Void<TO>
                 or CT::Sparse<TO>
                 or CT::POD<TO>
                 or ::std::is_trivial_v<TO>, 
         "TO must be either pointer, reflected as POD, or trivial. "
         "(you can suppress this error by casting pointer to void*)");

      static_assert(CT::Void<TO> 
                 or(CT::Same<TO, FROM> and CT::Sparse<TO> == CT::Sparse<FROM>),
         "TO and FROM must be the exact same types"
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
   ///   @tparam TO - destination memory type (deducible)                     
   ///   @tparam FROM - source memory type (deducible)                        
   ///   @param to - [out] destination memory                                 
   ///   @param from - source of data to copy                                 
   ///   @param count - number of elements to copy                            
   ///   @attention count becomes bytecount, when TO is void                  
   template<class TO, class FROM>
   LANGULUS(INLINED)
   void CopyMemory(TO* to, const FROM* from, const Count& count) noexcept {
      static_assert(CT::Void<TO>
                 or CT::Sparse<TO>
                 or CT::POD<TO>
                 or ::std::is_trivial_v<TO>, 
         "TO must be either pointer, reflected as POD, or trivial. "
         "(you can suppress this error by casting pointer to void*)");

      static_assert(CT::Void<TO>
                 or(CT::Same<TO, FROM> and CT::Sparse<TO> == CT::Sparse<FROM>),
         "TO and FROM must be the exact same types"
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
   ///   @tparam TO - destination memory type (deducible)                     
   ///   @tparam FILLER - value to fill in with                               
   ///   @param to - [out] destination memory                                 
   template<int FILLER, class TO>
   LANGULUS(INLINED)
   void FillMemory(TO* to) noexcept {
      static_assert(CT::Void<TO>
                 or CT::Sparse<TO>
                 or CT::POD<TO>
                 or ::std::is_trivial_v<TO>,
         "TO must be either pointer, reflected as POD, or trivial. "
         "(you can suppress this error by casting to void*)");

      static_assert(FILLER
                 or CT::Nullifiable<TO>
                 or CT::Void<TO>
                 or CT::Sparse<TO>
                 or CT::Fundamental<TO>,
         "Filling with zeroes requires the type to be reflected as nullifiable, "
         "or be a pointer/fundamental (you can suppress this error by casting to void*)");

      if constexpr (CT::Void<TO>)
         LANGULUS_ERROR("Bytecount not specified when filling void pointer");
      
      ::std::memset(static_cast<void*>(to), FILLER, sizeof(TO));
   }
   
   /// Wrapper for memset                                                     
   ///   @tparam TO - destination memory type (deducible)                     
   ///   @tparam FILLER - value to fill in with                               
   ///   @param to - [out] destination memory                                 
   ///   @param count - number of elements to fill                            
   ///   @attention count becomes bytecount, when TO is void                  
   template<int FILLER, class TO>
   LANGULUS(INLINED)
   void FillMemory(TO* to, const Count& count) noexcept {
      static_assert(CT::Void<TO>
                 or CT::Sparse<TO>
                 or CT::POD<TO>
                 or ::std::is_trivial_v<TO>,
         "TO must be either pointer, reflected as POD, or trivial. "
         "(you can suppress this error by casting to void*)");

      static_assert(FILLER
                 or CT::Nullifiable<TO>
                 or CT::Void<TO>
                 or CT::Sparse<TO>
                 or CT::Fundamental<TO>,
         "Filling with zeroes requires the type to be reflected as nullifiable, "
         "or be a pointer/fundamental (you can suppress this error by casting to void*)");

      if constexpr (CT::Void<TO>)
         ::std::memset(static_cast<void*>(to), FILLER, count);
      else
         ::std::memset(static_cast<void*>(to), FILLER, sizeof(TO) * count);
   }

   /// Wrapper for memset 0                                                   
   ///   @tparam TO - destination memory type (deducible)                     
   ///   @param to - [out] destination memory                                 
   template<class TO>
   LANGULUS(INLINED)
   void ZeroMemory(TO* to) noexcept {
      return FillMemory<0>(to);
   }
      
   /// Wrapper for memset 0                                                   
   ///   @tparam TO - destination memory type (deducible)                     
   ///   @param to - [out] destination memory                                 
   ///   @param count - number of elements to fill                            
   ///   @attention count becomes bytecount, when TO is void                  
   template<class TO>
   LANGULUS(INLINED)
   void ZeroMemory(TO* to, const Count& count) noexcept {
      return FillMemory<0>(to, count);
   }
      
   /// Wrapper for memmove                                                    
   ///   @tparam TO - destination memory type (deducible)                     
   ///   @tparam FROM - source memory type (deducible)                        
   ///   @param to - [out] destination memory                                 
   ///   @param from - source of data to move                                 
   template<class TO, class FROM>
   LANGULUS(INLINED)
   void MoveMemory(TO* to, const FROM* from) noexcept {
      static_assert(CT::Void<TO>
                 or CT::Sparse<TO>
                 or CT::POD<TO>
                 or ::std::is_trivial_v<TO>,
         "TO must be either pointer, reflected as POD, or trivial. "
         "(You can suppress this error by casting pointer to void*)");

      static_assert(CT::Void<TO>
                 or(CT::Same<TO, FROM> and CT::Sparse<TO> == CT::Sparse<FROM>),
         "TO and FROM must be the exact same types"
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
   ///   @tparam TO - destination memory type (deducible)                     
   ///   @tparam FROM - source memory type (deducible)                        
   ///   @param to - [out] destination memory                                 
   ///   @param from - source of data to move                                 
   ///   @param count - number of elements to move                            
   ///   @attention count becomes bytecount, when TO is void                  
   template<class TO, class FROM>
   LANGULUS(INLINED)
   void MoveMemory(TO* to, const FROM* from, const Count& count) noexcept {
      static_assert(CT::Void<TO>
                 or CT::Sparse<TO>
                 or CT::POD<TO>
                 or ::std::is_trivial_v<TO>,
         "TO must be either pointer, reflected as POD, or trivial. "
         "(You can suppress this error by casting pointer to void*)");

      static_assert(CT::Void<TO>
                 or(CT::Same<TO, FROM> and CT::Sparse<TO> == CT::Sparse<FROM>),
         "TO and FROM must be the exact same types"
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
