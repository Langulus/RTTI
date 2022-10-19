///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Reflection.hpp"

namespace Langulus
{

   ///                                                                        
   ///   A byte                                                               
   ///                                                                        
   ///   std::byte is shitty, this one's better. It preserves arithmetic      
   /// operations on the byte. These operations never do integer promotion,   
   /// the result is always truncated back down to a byte.                    
   ///                                                                        
   struct Byte {
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

      using MemberType = ::std::uint8_t;
      static constexpr Count MemberCount {1};

      MemberType mValue {};

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

      /// All conversions are explicit only, to preserve type                 
      constexpr explicit operator ::std::uint8_t& () noexcept {
         return mValue;
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

      NOD() constexpr Byte& operator += (const Byte& rhs) noexcept {
         mValue += rhs.mValue;
         return *this;
      }
      NOD() constexpr Byte& operator -= (const Byte& rhs) noexcept {
         mValue -= rhs.mValue;
         return *this;
      }
      NOD() constexpr Byte& operator *= (const Byte& rhs) noexcept {
         mValue *= rhs.mValue;
         return *this;
      }
      NOD() constexpr Byte& operator /= (const Byte& rhs) noexcept {
         mValue /= rhs.mValue;
         return *this;
      }
      NOD() constexpr Byte& operator %= (const Byte& rhs) noexcept {
         mValue %= rhs.mValue;
         return *this;
      }
      NOD() constexpr Byte& operator <<= (const Byte& rhs) noexcept {
         mValue <<= rhs.mValue;
         return *this;
      }
      NOD() constexpr Byte& operator >>= (const Byte& rhs) noexcept {
         mValue >>= rhs.mValue;
         return *this;
      }
      NOD() constexpr Byte& operator ^= (const Byte& rhs) noexcept {
         mValue ^= rhs.mValue;
         return *this;
      }
      NOD() constexpr Byte& operator &= (const Byte& rhs) noexcept {
         mValue &= rhs.mValue;
         return *this;
      }
      NOD() constexpr Byte& operator |= (const Byte& rhs) noexcept {
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

} // namespace Langulus
