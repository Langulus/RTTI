///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Semantics.hpp"


namespace Langulus::CT
{

   template<class...T>
   concept Tagged = (requires { Decay<T>::CTTI_TagTag; } and ...);

   template<class...T>
   concept NotTagged = ((not Tagged<T>) and ...);

} // namespace Langulus::CT

namespace Langulus::RTTI
{

   ///                                                                        
   ///   Tag a member with some property                                      
   ///                                                                        
   ///   Aims to be sheddable, zero-overhead and seamless - technically, it   
   /// just emulates attributes. Used extensively when reflecting members     
   /// that need to have specific properties. The alternative would be, to    
   /// actually add [[attributes]] and use post-build steps, to parse the C++ 
   /// files and generate reflection from that. Which, needless to say, is    
   /// just too damn tedious.                                                 
   ///   Tags are also very useful to infer the trait type, when seeking      
   /// values from the hierarchy.                                             
   ///                                                                        
   ///   @tparam DATA - type to apply the attributes to                       
   ///   @tparam TAGS - the attributes                                        
   ///                                                                        
   #pragma pack(push, 1)
   template<class DATA, class...TAGS>
   struct Tag {
      LANGULUS(TYPED) DATA;

      static constexpr bool CTTI_TagTag = true;

      static_assert(not CT::Array<DATA>,
         "Carry the extent of the array outside the tag: "
         "`Tag<bool[64], Trait> mMember` becomes `Tag<bool, Trait> mMember[64]` "
         " - this is required for reflection to work properly");

      DATA mData;

      using DataType = DATA;
      using Tags = Types<TAGS...>;

      constexpr Tag() = default;

      template<CT::NotTagged T>
      requires CT::MakableFrom<DATA, T> LANGULUS(INLINED)
      constexpr Tag(T&& t)
         : mData {Forward<T>(t)} {}

      constexpr Tag(const Tag& other)
         : mData {other.mData} {}

      constexpr Tag(Tag&& other)
         : mData {::std::move(other.mData)} {}

      constexpr Tag& operator = (const Tag& rhs) {
         mData = rhs.mData;
         return *this;
      }

      constexpr Tag& operator = (Tag&& rhs) {
         mData = ::std::move(rhs.mData);
         return *this;
      }

      template<CT::NotTagged T>
      requires CT::AssignableFrom<DATA, T> LANGULUS(INLINED)
      constexpr Tag& operator = (T&& rhs) {
         mData = Forward<T>(rhs);
         return *this;
      }

      LANGULUS(INLINED)
      constexpr operator DATA& () const noexcept {
         static_assert(sizeof (Tag) == sizeof (DATA));
         return const_cast<DATA&>(mData);
      }

      LANGULUS(INLINED)
      constexpr decltype(auto) operator & () noexcept {
         if constexpr (requires { mData.operator & (); })
            return mData.operator & ();
         else
            return &mData;
      }

      LANGULUS(INLINED)
      constexpr decltype(auto) operator & () const noexcept {
         if constexpr (requires { mData.operator & (); })
            return mData.operator & ();
         else
            return &mData;
      }

      LANGULUS(INLINED)
      constexpr decltype(auto) operator * () noexcept {
         if constexpr (requires { mData.operator * (); })
            return mData.operator * ();
         else
            return (mData);
      }

      LANGULUS(INLINED)
      constexpr decltype(auto) operator * () const noexcept {
         if constexpr (requires { mData.operator * (); })
            return mData.operator * ();
         else
            return (mData);
      }

      LANGULUS(INLINED)
      constexpr decltype(auto) operator -> () noexcept {
         if constexpr (requires { mData.operator -> (); })
            return mData.operator -> ();
         else if constexpr (CT::Dense<DATA>)
            return &mData;
         else
            return mData;
      }

      LANGULUS(INLINED)
      constexpr decltype(auto) operator -> () const noexcept {
         if constexpr (requires { mData.operator -> (); })
            return mData.operator -> ();
         else if constexpr (CT::Dense<DATA>)
            return &mData;
         else
            return mData;
      }
   };
   #pragma pack(pop)

} // namespace Langulus::RTTI