///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Byte.hpp"
#include "Meta.hpp"
#include <iterator>
#include <algorithm>
#include <string>

#if defined(_MSC_VER)
   #define BIG_CONSTANT(x) (x)
#else
   #define BIG_CONSTANT(x) (x##LLU)
#endif


namespace Langulus
{
   
   using Lowercase = ::std::string;

   /// Convert a token to a lowercase string                                  
   ///   @param token - the token to lowercase                                
   ///   @return the lowercase string                                         
   NOD() LANGULUS(INLINED) Lowercase ToLowercase(const Token& token) noexcept {
      Lowercase lc;
      ::std::transform(token.begin(), token.end(), std::back_inserter(lc),
         [](char c) { return static_cast<char>(::std::tolower(c)); }
      );
      return lc;
   }

   /// Default hash seed used in Langulus                                     
   constexpr uint32_t DefaultHashSeed = 19890212;

   namespace Inner
   {

      /// MurmurHash3 was written by Austin Appleby, and is placed in the     
      /// public domain                                                       
      /// https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
      /// Note - The x86 and x64 versions do _not_ produce the same results,  
      /// as the algorithms are optimized for their respective platforms.     
      /// You can still compile and run any of them on any platform, but your 
      /// performance with the non-native version will be less than optimal.  

      /// Block read - if your platform needs to do endian-swapping or can    
      /// only handle aligned reads, do the conversion here                   
      LANGULUS(INLINED)
      uint32_t getblock32(const uint32_t* p, int i) {
         return p[i];
      }

      LANGULUS(INLINED)
      uint64_t getblock64(const uint64_t* p, int i) {
         return p[i];
      }

      /// Finalization mix - force all bits of a hash block to avalanche      
      LANGULUS(INLINED)
      uint32_t fmix32(uint32_t h) {
         h ^= h >> 16;
         h *= 0x85ebca6b;
         h ^= h >> 13;
         h *= 0xc2b2ae35;
         h ^= h >> 16;
         return h;
      }

      LANGULUS(INLINED)
      uint64_t fmix64(uint64_t k) {
         k ^= k >> 33;
         k *= BIG_CONSTANT(0xff51afd7ed558ccd);
         k ^= k >> 33;
         k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
         k ^= k >> 33;
         return k;
      }

      /// 32-bit hasher optimized for x86                                     
      ///   @attention key memory must be aligned to 4 bytes                  
      ///   @tparam TAIL - set to true, if length is not aligned to 4 bytes   
      ///   @tparam SEED - the seed for the hash algorithm                    
      ///   @param key - the memory to hash                                   
      ///   @param len - the number of bytes in the key                       
      ///   @param seed - the seed for the hash                               
      ///   @param out - [out] the hash goes here (must be 4 bytes)           
      template<bool TAIL = true, uint32_t SEED = DefaultHashSeed>
      void MurmurHash3_x86_32(const void* key, int len, void* out) {
         const uint8_t* data = (const uint8_t*) key;
         const int nblocks = len / 4;
         uint32_t h1 = SEED;
         const uint32_t c1 = 0xcc9e2d51;
         const uint32_t c2 = 0x1b873593;
         
         // Body                                                        
         const uint32_t* blocks = (const uint32_t*) (data + nblocks * 4);
         for (int i = -nblocks; i; i++) {
            uint32_t k1 = getblock32(blocks, i);

            k1 *= c1;
            k1 = ::std::rotl(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = ::std::rotl(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
         }

         // Tail                                                        
         if constexpr (TAIL) {
            const uint8_t* tail = (const uint8_t*) (data + nblocks * 4);
            uint32_t k1 = 0;
            switch (len & 3) {
            case 3:
               k1 ^= tail[2] << 16;
               [[fallthrough]];
            case 2:
               k1 ^= tail[1] << 8;
               [[fallthrough]];
            case 1:
               k1 ^= tail[0];
               k1 *= c1;
               k1 = ::std::rotl(k1, 15);
               k1 *= c2;
               h1 ^= k1;
            };
         }

         // Finalization                                                
         h1 ^= len;
         h1 = fmix32(h1);
         *(uint32_t*) out = h1;
      }

      /// 128-bit hasher optimized for x86                                    
      ///   @attention key memory must be aligned to 16 bytes                 
      ///   @tparam TAIL - set to true, if length is not aligned to 16 bytes  
      ///   @tparam SEED - the seed for the hash algorithm                    
      ///   @param key - the memory to hash                                   
      ///   @param len - the number of bytes in the key                       
      ///   @param seed - the seed for the hash                               
      ///   @param out - [out] the hash goes here (must be 16 bytes)          
      template<bool TAIL = true, uint32_t SEED = DefaultHashSeed>
      void MurmurHash3_x86_128(const void* key, const int len, void* out) {
         const uint8_t* data = (const uint8_t*) key;
         const int nblocks = len / 16;

         uint32_t h1 = SEED;
         uint32_t h2 = SEED;
         uint32_t h3 = SEED;
         uint32_t h4 = SEED;

         const uint32_t c1 = 0x239b961b;
         const uint32_t c2 = 0xab0e9789;
         const uint32_t c3 = 0x38b34ae5;
         const uint32_t c4 = 0xa1e38b93;

         // Body                                                        
         const uint32_t* blocks = (const uint32_t*) (data + nblocks * 16);
         for (int i = -nblocks; i; i++) {
            uint32_t k1 = getblock32(blocks, i * 4 + 0);
            uint32_t k2 = getblock32(blocks, i * 4 + 1);
            uint32_t k3 = getblock32(blocks, i * 4 + 2);
            uint32_t k4 = getblock32(blocks, i * 4 + 3);

            k1 *= c1; 
            k1 = ::std::rotl(k1, 15);
            k1 *= c2; 
            h1 ^= k1;

            h1 = ::std::rotl(h1, 19);
            h1 += h2;
            h1 = h1 * 5 + 0x561ccd1b;

            k2 *= c2;
            k2 = ::std::rotl(k2, 16);
            k2 *= c3;
            h2 ^= k2;

            h2 = ::std::rotl(h2, 17);
            h2 += h3;
            h2 = h2 * 5 + 0x0bcaa747;

            k3 *= c3;
            k3 = ::std::rotl(k3, 17);
            k3 *= c4;
            h3 ^= k3;

            h3 = ::std::rotl(h3, 15);
            h3 += h4;
            h3 = h3 * 5 + 0x96cd1c35;

            k4 *= c4;
            k4 = ::std::rotl(k4, 18);
            k4 *= c1;
            h4 ^= k4;

            h4 = ::std::rotl(h4, 13);
            h4 += h1;
            h4 = h4 * 5 + 0x32ac3b17;
         }

         // Tail                                                        
         if constexpr (TAIL) {
            const uint8_t* tail = (const uint8_t*) (data + nblocks * 16);

            uint32_t k1 = 0;
            uint32_t k2 = 0;
            uint32_t k3 = 0;
            uint32_t k4 = 0;

            switch (len & 15) {
            case 15:
               k4 ^= tail[14] << 16;
               [[fallthrough]];
            case 14:
               k4 ^= tail[13] << 8;
               [[fallthrough]];
            case 13:
               k4 ^= tail[12] << 0;
               k4 *= c4;
               k4 = ::std::rotl(k4, 18);
               k4 *= c1;
               h4 ^= k4;
               [[fallthrough]];
            case 12:
               k3 ^= tail[11] << 24;
               [[fallthrough]];
            case 11:
               k3 ^= tail[10] << 16;
               [[fallthrough]];
            case 10:
               k3 ^= tail[9] << 8;
               [[fallthrough]];
            case  9:
               k3 ^= tail[8] << 0;
               k3 *= c3;
               k3 = ::std::rotl(k3, 17);
               k3 *= c4;
               h3 ^= k3;
               [[fallthrough]];
            case  8:
               k2 ^= tail[7] << 24;
               [[fallthrough]];
            case  7:
               k2 ^= tail[6] << 16;
               [[fallthrough]];
            case  6:
               k2 ^= tail[5] << 8;
               [[fallthrough]];
            case  5:
               k2 ^= tail[4] << 0;
               k2 *= c2;
               k2 = ::std::rotl(k2, 16);
               k2 *= c3;
               h2 ^= k2;
               [[fallthrough]];
            case  4:
               k1 ^= tail[3] << 24;
               [[fallthrough]];
            case  3:
               k1 ^= tail[2] << 16;
               [[fallthrough]];
            case  2:
               k1 ^= tail[1] << 8;
               [[fallthrough]];
            case  1:
               k1 ^= tail[0] << 0;
               k1 *= c1;
               k1 = ::std::rotl(k1, 15);
               k1 *= c2;
               h1 ^= k1;
            };
         }

         // Finalization                                                
         h1 ^= len;
         h2 ^= len;
         h3 ^= len;
         h4 ^= len;

         h1 += h2;
         h1 += h3;
         h1 += h4;
         h2 += h1;
         h3 += h1;
         h4 += h1;

         h1 = fmix32(h1);
         h2 = fmix32(h2);
         h3 = fmix32(h3);
         h4 = fmix32(h4);

         h1 += h2;
         h1 += h3;
         h1 += h4;
         h2 += h1;
         h3 += h1;
         h4 += h1;

         ((uint32_t*) out)[0] = h1;
         ((uint32_t*) out)[1] = h2;
         ((uint32_t*) out)[2] = h3;
         ((uint32_t*) out)[3] = h4;
      }

      /// 64-bit hasher optimized for x86                                     
      ///   @attention key memory must be aligned to 8 bytes                  
      ///   @tparam TAIL - set to true, if length is not aligned to 8 bytes   
      ///   @tparam SEED - the seed for the hash algorithm                    
      ///   @param key - the memory to hash                                   
      ///   @param len - the number of bytes in the key                       
      ///   @param seed - the seed for the hash                               
      ///   @param out - [out] the hash goes here (must be 8 bytes)           
      template<bool TAIL = true, uint32_t SEED = DefaultHashSeed>
      void MurmurHash2_x64_64(const void* key, int len, void* out) {
         const uint64_t m = BIG_CONSTANT(0xc6a4a7935bd1e995);
         const int r = 47;
         uint64_t h = uint64_t {SEED} ^ (len * m);
         const uint64_t* data = (const uint64_t*) key;
         const uint64_t* end = data + (len / 8);

         while (data != end) {
            uint64_t k = *data++;

            k *= m;
            k ^= k >> r;
            k *= m;

            h ^= k;
            h *= m;
         }

         if constexpr (TAIL) {
            const unsigned char* data2 = (const unsigned char*) data;
            switch (len & 7) {
            case 7:
               h ^= uint64_t(data2[6]) << 48;
               [[fallthrough]];
            case 6:
               h ^= uint64_t(data2[5]) << 40;
               [[fallthrough]];
            case 5:
               h ^= uint64_t(data2[4]) << 32;
               [[fallthrough]];
            case 4:
               h ^= uint64_t(data2[3]) << 24;
               [[fallthrough]];
            case 3:
               h ^= uint64_t(data2[2]) << 16;
               [[fallthrough]];
            case 2:
               h ^= uint64_t(data2[1]) << 8;
               [[fallthrough]];
            case 1:
               h ^= uint64_t(data2[0]);
               h *= m;
            };
         }

         h ^= h >> r;
         h *= m;
         h ^= h >> r;
         *(uint64_t*) out = h;
      }

      /// 128-bit hasher optimized for x64                                    
      ///   @attention key memory must be aligned to 16 bytes                 
      ///   @tparam TAIL - set to true, if length is not aligned to 16 bytes  
      ///   @tparam SEED - the seed for the hash algorithm                    
      ///   @param key - the memory to hash                                   
      ///   @param len - the number of bytes in the key                       
      ///   @param seed - the seed for the hash                               
      ///   @param out - [out] the hash goes here (must be 16 bytes)          
      template<bool TAIL = true, uint32_t SEED = DefaultHashSeed>
      void MurmurHash3_x64_128(const void* key, const int len, void* out) {
         const uint8_t* data = (const uint8_t*) key;
         const int nblocks = len / 16;

         uint64_t h1 = SEED;
         uint64_t h2 = SEED;

         const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
         const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

         // Body                                                        
         const uint64_t* blocks = (const uint64_t*) (data);
         for (int i = 0; i < nblocks; i++) {
            uint64_t k1 = getblock64(blocks, i * 2 + 0);
            uint64_t k2 = getblock64(blocks, i * 2 + 1);

            k1 *= c1;
            k1 = ::std::rotl(k1, 31);
            k1 *= c2;
            h1 ^= k1;

            h1 = ::std::rotl(h1, 27);
            h1 += h2;
            h1 = h1 * 5 + 0x52dce729;

            k2 *= c2;
            k2 = ::std::rotl(k2, 33);
            k2 *= c1;
            h2 ^= k2;

            h2 = ::std::rotl(h2, 31);
            h2 += h1;
            h2 = h2 * 5 + 0x38495ab5;
         }

         // Tail                                                        
         if constexpr (TAIL) {
            const uint8_t* tail = (const uint8_t*) (data + nblocks * 16);
            uint64_t k1 = 0;
            uint64_t k2 = 0;

            switch (len & 15) {
            case 15:
               k2 ^= ((uint64_t) tail[14]) << 48;
               [[fallthrough]];
            case 14:
               k2 ^= ((uint64_t) tail[13]) << 40;
               [[fallthrough]];
            case 13:
               k2 ^= ((uint64_t) tail[12]) << 32;
               [[fallthrough]];
            case 12:
               k2 ^= ((uint64_t) tail[11]) << 24;
               [[fallthrough]];
            case 11:
               k2 ^= ((uint64_t) tail[10]) << 16;
               [[fallthrough]];
            case 10:
               k2 ^= ((uint64_t) tail[9]) << 8;
               [[fallthrough]];
            case  9:
               k2 ^= ((uint64_t) tail[8]) << 0;
               k2 *= c2;
               k2 = ::std::rotl(k2, 33);
               k2 *= c1;
               h2 ^= k2;
               [[fallthrough]];
            case  8:
               k1 ^= ((uint64_t) tail[7]) << 56;
               [[fallthrough]];
            case  7:
               k1 ^= ((uint64_t) tail[6]) << 48;
               [[fallthrough]];
            case  6:
               k1 ^= ((uint64_t) tail[5]) << 40;
               [[fallthrough]];
            case  5:
               k1 ^= ((uint64_t) tail[4]) << 32;
               [[fallthrough]];
            case  4:
               k1 ^= ((uint64_t) tail[3]) << 24;
               [[fallthrough]];
            case  3:
               k1 ^= ((uint64_t) tail[2]) << 16;
               [[fallthrough]];
            case  2:
               k1 ^= ((uint64_t) tail[1]) << 8;
               [[fallthrough]];
            case  1:
               k1 ^= ((uint64_t) tail[0]) << 0;
               k1 *= c1;
               k1 = ::std::rotl(k1, 31);
               k1 *= c2;
               h1 ^= k1;
            };
         }

         // Finalization                                                
         h1 ^= len;
         h2 ^= len;

         h1 += h2;
         h2 += h1;

         h1 = fmix64(h1);
         h2 = fmix64(h2);

         h1 += h2;
         h2 += h1;

         ((uint64_t*) out)[0] = h1;
         ((uint64_t*) out)[1] = h2;
      }
   }

   /// Hash a sequence of bytes                                               
   ///   @tparam SEED - the seed for the hash algorithm                       
   ///   @tparam TAIL - true for a generalized hashing routine (internal)     
   ///   @param ptr - memory start                                            
   ///   @param len - number of bytes to hash                                 
   ///   @return the hash                                                     
   template<uint32_t SEED = DefaultHashSeed, bool TAIL = true>
   constexpr Hash HashBytes(void const* ptr, int len) noexcept {
      Hash result;
      if constexpr (sizeof(Hash) == 4)
         Inner::MurmurHash3_x86_32<TAIL, SEED>(ptr, len, &result);
      else if constexpr (sizeof(Hash) == 8)
         Inner::MurmurHash2_x64_64<TAIL, SEED>(ptr, len, &result);
      else if constexpr (sizeof(Hash) == 16)
         Inner::MurmurHash3_x64_128<TAIL, SEED>(ptr, len, &result);
      else
         LANGULUS_ERROR("Not implemented");
      return result;
   }

   namespace CT::Inner
   {

      /// Check if T can be hashed with HashOf                                
      template<class T>
      concept HasGetHashMethod = Complete<T> and requires (T& a) {
         {a.GetHash()} -> Same<Hash>;
      };

   } // namespace Langulus::CT


   /// Hash any hashable data, including fundamental types                    
   ///   @tparam FAKE - for internal use - if FAKE and evaluated to fail, it  
   ///                  will return CT::Unsupported; otherwise it will screm  
   ///                  a compile-time error at you                           
   ///   @tparam SEED - the seed for the hash algorithm                       
   ///   @tparam T - first type to hash (deducible)                           
   ///   @tparam MORE... - the rest of the hashed types (deducible)           
   ///   @param head, rest... - the data to hash                              
   ///   @return the hash                                                     
   template<bool FAKE = false, uint32_t SEED = DefaultHashSeed, class T, class... MORE>
   constexpr auto HashOf(const T& head, const MORE&... rest) {
      if constexpr (CT::Unsupported<T, MORE...>)
         return Inner::Unsupported {};
      else if constexpr (sizeof...(MORE)) {
         // Combine all data into a single array of hashes, and then    
         // hash that array as a whole                                  
         alignas(Bitness/8) const Hash coalesced[1 + sizeof...(MORE)] {
            HashOf<FAKE, SEED>(head),
            HashOf<FAKE, SEED>(rest)...
         };
         return HashBytes<SEED, false>(
            coalesced, static_cast<int>(sizeof(coalesced))
         );
      }
      else if constexpr (CT::Sparse<T>) {
         if constexpr (CT::Array<T>) {
            if constexpr (ExtentOf<T> == 1) {
               // Only one element in array, just use the first hash    
               return HashOf<FAKE, SEED>(head[0]);
            }
            else if constexpr (sizeof(Deext<T>) == 1 or ::std::is_fundamental_v<Deext<T>>) {
               // Array is made of POD-like elements, batch-hash them   
               return HashBytes<SEED>(head, static_cast<int>(sizeof(T)));
            }
            else {
               // Hash each element of the array individually, and then 
               // hash that array of hashes as a whole                  
               alignas(Bitness / 8) Hash coalesced[ExtentOf<T>];
               for (Count i = 0; i < ExtentOf<T>; ++i)
                  coalesced[i] = HashOf<FAKE, SEED>(head[i]);
               return HashBytes<SEED, false>(
                  coalesced, static_cast<int>(sizeof(coalesced))
               );
            }
         }
         else {
            // Hash pointer, never dereference it                       
            if (head == nullptr)
               return Hash {};

            return HashBytes<SEED, false>(
               &head, static_cast<int>(sizeof(T))
            );
         }
      }
      else if constexpr (CT::Exact<T, Hash>) {
         // Provided type is already a hash, just propagate it          
         return head;
      }
      else if constexpr (CT::Inner::HasGetHashMethod<T>) {
         // Hashable via a member GetHash() function                    
         return head.GetHash();
      }
      else if constexpr (CT::StdContainer<T>
      and requires (TypeOf<T>& a) {{HashOf<true, SEED>(a)} -> CT::Supported; }) {
         // Anything that contiguously iteratable is carried through    
         // HashOf for consistency, because different std library       
         // implementations might have different hashing algorithms.    
         // This should include string_view, string, vector, span, etc. 
         using TT = TypeOf<T>;
         if constexpr (CT::StdContiguousContainer<T>
         and (sizeof(TT) == 1 or ::std::is_fundamental_v<TT>)) {   //TODO if i use POD instead of fundamental here, std::string_view will be taken as byte array
            return HashBytes<SEED>(                                   // which uninadvertedly will fuck shit up, and it hints, that CT::POD should be
               head.data(),                                           // completely rethought to avoid any standard definition of POD (huh, probably that's why the
               static_cast<int>(head.size() * sizeof(TT))             // std::pod concept was deprecated in the first place, so there really ISN'T a definition at all)
            );
         }
         else {
            // Hash each individual element, then combine all hashes    
            ::std::vector<Hash> coalesced;
            for (auto& i : head)
               coalesced.emplace_back(HashOf<FAKE, SEED>(i));

            return HashBytes<SEED>(
               coalesced.data(),
               static_cast<int>(coalesced.size() * sizeof(Hash))
            );
         }
      }
      else if constexpr (CT::POD<T>) {
         // Explicitly marked POD types are always hashable, but be     
         // careful for POD types with padding - the junk inbetween     
         // members can interfere with the hash, giving unique          
         // hashes where the same hashes should be produced. In such    
         // cases it is recommended you add a custom GetHash() method   
         // to your type, or #pragma pack, in order to circumvent issue 
         // Warning: some types like std::string_view are actually      
         // qualified as POD by Langulus standards, and that's why POD  
         // is after the ::std::ranges::range<T> case                   
         return HashBytes<SEED, alignof(T) < Bitness / 8> (
            &head, static_cast<int>(sizeof(T)));
      }
      else if constexpr (requires (::std::hash<T> h, const T& i) { h(i); }) {
         // Hashable via std::hash (fallback for std containers)        
         // Beware, hashing functions coming from std::hash may have    
         // different implementations for different compilers, which    
         // will likely result in different ordering inside unordered   
         // containers. Nothing serious, unless you're pedantic like me 
         ::std::hash<T> hasher;
         return Hash {hasher(head)};
      }
      else {
         if constexpr (FAKE) return Inner::Unsupported {};
         else                LANGULUS_ERROR("Can't hash data");
      }
   }

} // namespace Langulus

// Let's not pollute the namespace
#undef BIG_CONSTANT

namespace Langulus::CT
{

   /// Check if the origin T can be hashed using HashOf                       
   template<class...T>
   concept Hashable = requires (T&...a) {
      {(HashOf<true>(a), ...)} -> Supported;
   };

} // namespace Langulus::CT

namespace std
{

   /// Extend std to capable of hashing anything with GetHash method          
   template<::Langulus::CT::Inner::HasGetHashMethod H>
   struct hash<H> {
      LANGULUS(INLINED)
      size_t operator()(const H& what) const noexcept {
         return what.GetHash().mHash;
      }
   };

   /// Make vectors of DMeta hashable                                         
   template<>
   struct hash<vector<::Langulus::RTTI::DMeta>> {
      LANGULUS(INLINED)
      size_t operator()(const vector<::Langulus::RTTI::DMeta>& k) const noexcept {
         return Langulus::HashOf(k).mHash;
      }
   };

   /*LANGULUS(INLINED)
   size_t hash<AMeta>::operator()(AMeta k) const noexcept {
      return Langulus::HashOf(k).mHash;
   }

   LANGULUS(INLINED)
   size_t hash<DMeta>::operator()(DMeta k) const noexcept {
      return Langulus::HashOf(k).mHash;
   }

   LANGULUS(INLINED)
   size_t hash<vector<DMeta>>::operator()(const vector<DMeta>& k) const noexcept {
      return Langulus::HashOf(k).mHash;
   }

   LANGULUS(INLINED)
   size_t hash<TMeta>::operator()(TMeta k) const noexcept {
      return Langulus::HashOf(k).mHash;
   }

   LANGULUS(INLINED)
   size_t hash<VMeta>::operator()(VMeta k) const noexcept {
      return Langulus::HashOf(k).mHash;
   }*/

} // namespace std

namespace Langulus::RTTI
{
   
   /// Get the constexpr hash of a type                                       
   ///	@return the hash of the type                                         
   /*template<CT::Data> LANGULUS(INLINED)
   constexpr Hash Meta::GenerateHash(const Token& name) noexcept {
      return HashBytes(name.data(), static_cast<int>(name.size()));
   }
   
   /// Get the generated hash, making any meta derivation CT::Hashable        
   ///	@return the hash of the type                                         
   LANGULUS(INLINED)
   const Hash& Meta::GetHash() const noexcept {
      return mHash;
   }*/
   
} // namespace Langulus::RTTI
