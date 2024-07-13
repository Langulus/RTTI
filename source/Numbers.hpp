///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Reflection.hpp"
#include "Intent.hpp"


namespace Langulus::CT
{
   namespace Inner
   {

      /// Analyze a single type for compile-time count trait                  
      /// The resulting count will be (bounded array extent) * any MemberCount
      /// constexpr static member, or constexpr size() method                 
      template<class T>
      consteval Count CountOfInner() noexcept {
         using DT = Decay<Deint<T>>;
         if constexpr (requires {{DT::MemberCount} -> Similar<Count>; })
            return DT::MemberCount * ExtentOf<Deint<T>>;
         else if constexpr (requires (DT a) {{a.size()} -> Similar<size_t>;}) {
            if constexpr (IsConstexpr([] { return DT {}.size(); }))
               return DT {}.size() * ExtentOf<Deint<T>>;
            else
               return ExtentOf<Deint<T>>;
         }
         else return ExtentOf<Deint<T>>;
      }

      template<class T1, class...TN>
      consteval Count CountOf() noexcept {
         if constexpr (sizeof...(TN))
            return CountOfInner<T1>() + (CountOfInner<TN>() + ...);
         else
            return CountOfInner<T1>();
      }

   } // namespace Langulus::CT::Inner

   /// Custom boolean concept (wrapped in another type)                       
   template<class...T>
   concept CustomBool = ((Typed<T> and Inner::CountOf<T>() == 1 and
         BuiltinBool<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom character concept (wrapped in another type)                     
   template<class...T>
   concept CustomCharacter = ((Typed<T> and Inner::CountOf<T>() == 1 and
         BuiltinCharacter<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom integer concept (wrapped in another type)                       
   template<class...T>
   concept CustomInteger = ((Typed<T> and Inner::CountOf<T>() == 1 and
         ::std::is_integral_v<Deref<TypeOf<T>>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom real concept (wrapped in another type)                          
   template<class...T>
   concept CustomReal = ((Typed<T> and Inner::CountOf<T>() == 1 and
         ::std::is_floating_point_v<Deref<TypeOf<T>>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);
   
   /// Custom number concept                                                  
   /// Any T that has underlying arithmetic type and is binary compatible     
   /// Unlike CT::BuiltinNumber, this one includes booleans and characters    
   template<class...T>
   concept CustomNumber = ((
      CustomBool<T> or CustomCharacter<T> or CustomInteger<T> or CustomReal<T>
   ) and ...);

   /// Any real number, custom or not                                         
   template<class...T>
   concept Real = ((BuiltinReal<T> or CustomReal<T>) and ...);

   /// Any integer, custom or not                                             
   template<class...T>
   concept Integer = ((BuiltinInteger<T> or CustomInteger<T>) and ...);
   
   /// Any signed integer, custom or not                                      
   template<class...T>
   concept SignedInteger = ((Signed<T> and Integer<T>) and ...);

   /// Any unsigned integer, custom or not                                    
   template<class...T>
   concept UnsignedInteger = ((Unsigned<T> and Integer<T>) and ...);

   /// Any bool, custom or not                                                
   template<class...T>
   concept Bool = ((BuiltinBool<T> or CustomBool<T>) and ...);

   /// Any character, custom or not                                           
   template<class...T>
   concept Character = ((BuiltinCharacter<T> or CustomCharacter<T>) and ...);

   /// Any number, custom or not                                              
   /// Excludes boolean and character types, unless wrapped in another type   
   template<class...T>
   concept Number = ((BuiltinNumber<T> or CustomNumber<T>) and ...);

   /// Single precision real number concept                                   
   template<class...T>
   concept Float = (Same<::Langulus::Float, T> and ...);

   /// Double precision real number concept                                   
   template<class...T>
   concept Double = (Same<::Langulus::Double, T> and ...);

   /// Size-related number concepts                                           
   /// These concepts include character and byte types                        
   template<class...T>
   concept SignedInteger8  = ((SignedInteger<T> and sizeof(Decay<T>) == 1) and ...);
   template<class...T>
   concept SignedInteger16 = ((SignedInteger<T> and sizeof(Decay<T>) == 2) and ...);
   template<class...T>
   concept SignedInteger32 = ((SignedInteger<T> and sizeof(Decay<T>) == 4) and ...);
   template<class...T>
   concept SignedInteger64 = ((SignedInteger<T> and sizeof(Decay<T>) == 8) and ...);

   template<class...T>
   concept UnsignedInteger16 = (((UnsignedInteger<T> or CT::Character<T>)
       and sizeof(Decay<T>) == 2) and ...);

   template<class...T>
   concept UnsignedInteger32 = (((UnsignedInteger<T> or CT::Character<T>)
       and sizeof(Decay<T>) == 4) and ...);

   template<class...T>
   concept UnsignedInteger64 = (((UnsignedInteger<T> or CT::Character<T>)
       and sizeof(Decay<T>) == 8) and ...);

   template<class...T>
   concept Integer16 = ((SignedInteger16<T> or UnsignedInteger16<T>) and ...);
   template<class...T>
   concept Integer32 = ((SignedInteger32<T> or UnsignedInteger32<T>) and ...);
   template<class...T>
   concept Integer64 = ((SignedInteger64<T> or UnsignedInteger64<T>) and ...);
        
} // namespace Langulus::CT
