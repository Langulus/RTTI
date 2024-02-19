///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Semantics.hpp"


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
   ///   @tparam TAG - the attribute                                          
   ///                                                                        
   template<class DATA, class TAG>
   struct alignas(alignof(DATA)) Tag : A::Semantic {
      static constexpr bool CTTI_TagTag = true;

      static_assert(not CT::Array<DATA>,
         "Carry the extent of the array outside the tag: "
         "`Tag<bool[64], Trait> mMember` becomes `Tag<bool, Trait> mMember[64]` "
         " - this is required for reflection to work properly"
      );

      DATA mData;

      using DataType = DATA;
      using TagType  = TAG;

      LANGULUS(INLINED)
      constexpr Tag() requires CT::Inner::Defaultable<DATA>
         : mData {} {}

      LANGULUS(INLINED)
      constexpr Tag(const Tag& t) requires CT::Inner::CopyMakable<DATA>
         : mData {t.mData} {}

      LANGULUS(INLINED)
      constexpr Tag(Tag&& t) requires CT::Inner::MoveMakable<DATA>
         : mData {::std::move(t.mData)} {}

      template<class T>
      requires CT::MakableFrom<DATA, T> LANGULUS(INLINED)
      constexpr Tag(T&& t)
         : mData {Forward<T>(t)} {}

      LANGULUS(INLINED)
      constexpr operator DATA& () noexcept {
         static_assert(sizeof (Tag) == sizeof (DATA));
         static_assert(alignof(Tag) == alignof(DATA));
         return mData;
      }

      LANGULUS(INLINED)
      constexpr operator DATA const& () const noexcept {
         static_assert(sizeof (Tag) == sizeof (DATA));
         static_assert(alignof(Tag) == alignof(DATA));
         return mData;
      }

      constexpr Tag& operator = (const Tag& rhs) 
      requires CT::Inner::CopyAssignable<DATA> {
         mData = rhs.mData;
         return *this;
      }

      constexpr Tag& operator = (Tag&& rhs)
      requires CT::Inner::MoveAssignable<DATA> {
         mData = ::std::move(rhs.mData);
         return *this;
      }

      template<class T>
      requires CT::AssignableFrom<DATA, T> LANGULUS(INLINED)
      constexpr Tag& operator = (T&& rhs) {
         mData = Forward<T>(rhs);
         return *this;
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

} // namespace Langulus::RTTI

namespace Langulus::CT
{

   template<class...T>
   concept Tagged = (requires { Decay<T>::CTTI_TagTag; } and ...);

   template<class...T>
   concept NotTagged = ((not Tagged<T>) and ...);

} // namespace Langulus::CT
