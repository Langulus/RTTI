///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Byte.hpp"


namespace Langulus
{
   namespace CT
   {

      /// Check if all the provided types are considered Vector types         
      /// Any type that is Typed and has CountOf that is at least 2, and      
      /// the T's size is exactly equal to sizeof(TypeOf<T>) * CountOf<T>     
      /// Additionally, bounded arrays with more than a single element are    
      /// also considered Vector.                                             
      template<class...T>
      concept Vector = (((Typed<Deint<T>>
              and Inner::CountOf<T>() > 1
              and sizeof(Deint<T>) == sizeof(TypeOf<Deint<T>>) * Inner::CountOf<T>()
            ) or (ExtentOf<Deint<T>> > 1)
         ) and ...);

      /// Scalar concept - any fundamental or custom number type, regardless  
      /// if wrapped inside an intent. Bounded arrays of those with           
      /// ExtentOf == 1 are also considered scalars.                          
      template<class...T>
      concept Scalar = ((Inner::CountOf<T>() == 1
            and (Byte<Deext<Deint<T>>> or Integer<Deext<Deint<T>>>
             or  Real<Deext<Deint<T>>> or Character<Deext<Deint<T>>>)
         ) and ...);

   } // namespace Langulus::CT

   
   /// Get the count of a counted type                                        
   /// Any type with a static constexpr unsigned MemberCount, or a bounded    
   /// array, are multiplied and counted. Scalars always will give count of 1 
   /// If multiple types provided, their counts will be summed up             
   template<class T1, class...TN>
   constexpr Count CountOf = CT::Inner::CountOf<T1, TN...>();

   /// Casts a scalar to its underlying fundamental type (const)              
   /// If non-void T::CTTI_InnerType exists, or if T is an enum, the inner    
   /// type returns                                                           
   ///   @tparam T - type of the scalar/enum to cast                          
   ///   @param a - the scalar to cast                                        
   ///   @return a reference to the underlying type                           
   template<CT::Scalar T, bool FAKE = false> NOD() LANGULUS(INLINED)
   constexpr decltype(auto) FundamentalCast(const T& a) noexcept {
      using DT = Decay<Deint<T>>;
      if constexpr (CT::Fundamental<DT>) {
         // Already fundamental, just forward it                        
         return (a);
      }
      else if constexpr ((CT::Typed<DT> or CT::Enum<DT>)
      and requires { static_cast<const TypeOf<DT>&>(DenseCast(a)); }) {
         // Explicitly cast to a reference of the contained type, and   
         // nest down to the fundamentals                               
         return FundamentalCast(static_cast<const TypeOf<DT>&>(DenseCast(a)));
      }
      else if constexpr (FAKE)
         return Inner::Unsupported {};
      else
         LANGULUS_ERROR("Can't perform FundamentalCast");
   }
   
   /// Casts a scalar to its underlying fundamental type                      
   /// If non-void T::CTTI_InnerType exists, or if T is an enum, the inner    
   /// type returns                                                           
   ///   @tparam T - type of the scalar/enum to cast                          
   ///   @param a - the scalar to cast                                        
   ///   @return a reference to the underlying type                           
   template<CT::Scalar T, bool FAKE = false> NOD() LANGULUS(INLINED)
   constexpr decltype(auto) FundamentalCast(T& a) noexcept {
      using DT = Decay<Deint<T>>;
      if constexpr (CT::Fundamental<DT>) {
         // Already fundamental, just forward it                        
         return (a);
      }
      else if constexpr ((CT::Typed<DT> or CT::Enum<DT>)
      and requires { static_cast<TypeOf<DT>&>(DenseCast(a)); }) {
         // Explicitly cast to a reference of the contained type, and   
         // nest down to the fundamentals                               
         return FundamentalCast(static_cast<TypeOf<DT>&>(DenseCast(a)));
      }
      else if constexpr (FAKE)
         return Inner::Unsupported {};
      else
         LANGULUS_ERROR("Can't perform FundamentalCast");
   }

   namespace CT
   {

      /// Check if an instance of T can be converted to a fundamental         
      template<class...T>
      concept CastsToFundamental = (
         Supported<decltype(FundamentalCast<T, true>(Fake<T&>()))> and ...);

   } // namespace Langulus::CT
    
   /// Returns the extent overlap of two arrays/non arrays                    
   /// An array to be considered array, it has to have more than one element  
   ///   @tparam LHS - left type                                              
   ///   @tparam RHS - right type                                             
   ///   @return the overlapping count:                                       
   ///           the smaller extent, if two arrays are provided;              
   ///           the bigger extent, if one of the arguments isn't an array    
   ///           1 if both arguments are not arrays                           
   template<class LHS, class RHS>
   consteval Count OverlapExtents() noexcept {
      constexpr auto lhs = ExtentOf<Deint<LHS>>;
      constexpr auto rhs = ExtentOf<Deint<RHS>>;

      if constexpr (lhs > 1 and rhs > 1)
         return lhs < rhs ? lhs : rhs;
      else if constexpr (lhs > 1)
         return lhs;
      else if constexpr (rhs > 1)
         return rhs;
      else
         return 1;
   }

   #define OVERLAP_EXTENTS(l,r) OverlapExtents<decltype(l), decltype(r)>()
   
   /// Returns the count overlap of two vectors/scalars                       
   /// This is used to decide the output array size, for containing the       
   /// result of an arithmetic operation                                      
   ///   @tparam LHS - left type                                              
   ///   @tparam RHS - right type                                             
   ///   @return the overlapping count:                                       
   ///           the smaller extent, if two arrays are provided;              
   ///           the bigger extent, if one of the arguments isn't a vector    
   ///           1 if both arguments are not arrays                           
   template<class LHS, class RHS>
   consteval Count OverlapCounts() noexcept {
      constexpr auto lhs = CountOf<Deint<LHS>>;
      constexpr auto rhs = CountOf<Deint<RHS>>;

      if constexpr (lhs > 1 and rhs > 1)
         return lhs < rhs ? lhs : rhs;
      else if constexpr (lhs > 1)
         return lhs;
      else if constexpr (rhs > 1)
         return rhs;
      else
         return 1;
   }

   namespace Inner
   {

      /// When given two types, choose the one that is most lossless in terms 
      /// of behavior, and capacity                                           
      ///  - if T1 or T2 is an array, an array of OverlapCount size will be   
      ///    given back. The array will be of the lossless decayed type       
      ///  - if both types are reals, the bigger real will be returned        
      ///  - if one of the type is a real, and the other an integer, the real 
      ///    will be returned                                                 
      ///  - if both types are integers with different signs, always returns  
      ///    the signed equivalent of the bigger integer                      
      ///  - if one of the types is not CT::Fundamental type, it will always  
      ///    be preferred, as it may have custom behavior                     
      ///  - if both types are not CT::Fundamental, the first type is always  
      ///    preferred (fallback)                                             
      ///   @attention this will discard any sparseness or other modifiers    
      ///   @attention this will shed any intents                             
      template<class T1, class T2>
      consteval auto Lossless() noexcept {
         constexpr auto size = OverlapCounts<T1, T2>();
         using LHS = Decay<TypeOf<Deint<T1>>>;
         using RHS = Decay<TypeOf<Deint<T2>>>;

         if constexpr (CT::Fundamental<LHS, RHS>) {
            // Both types are fundamental                               
            if constexpr (CT::Real<LHS, RHS>) {
               // Always prefer the bigger real number                  
               if constexpr (sizeof(LHS) >= sizeof(RHS))
                  return ::std::array<LHS, size> {};
               else
                  return ::std::array<RHS, size> {};
            }
            else if constexpr (CT::Real<LHS> and not CT::Real<RHS>) {
               // Always prefer real numbers                            
               return ::std::array<LHS, size> {};
            }
            else if constexpr (CT::Real<RHS> and not CT::Real<LHS>) {
               // Always prefer real numbers                            
               return ::std::array<RHS, size> {};
            }
            else if constexpr (CT::Signed<LHS> == CT::Signed<RHS>) {
               // Both are signed integers, so pick the bigger one      
               if constexpr (sizeof(LHS) >= sizeof(RHS))
                  return ::std::array<LHS, size> {};
               else
                  return ::std::array<RHS, size> {};
            }
            else if constexpr (CT::Signed<LHS>) {
               // LHS is signed, but RHS is not, so pick the signed one,
               // but also guarantee that size remains the bigger one   
               if constexpr (sizeof(LHS) >= sizeof(RHS))
                  return ::std::array<LHS, size> {};
               else
                  return ::std::array<::std::make_signed_t<RHS>, size> {};
            }
            else {
               // RHS is signed, but LHS is not, so pick the signed one,
               // but also guarantee that size remains the bigger one   
               if constexpr (sizeof(RHS) >= sizeof(LHS))
                  return ::std::array<RHS, size> {};
               else
                  return ::std::array<::std::make_signed_t<LHS>, size> {};
            }
         }
         else if constexpr (CT::Fundamental<LHS>) {
            // RHS isn't fundamental, so always prefer it               
            return ::std::array<RHS, size> {};
         }
         else {
            // Either both types aren't fundamental, or the RHS one is  
            // Just fallback to LHS                                     
            return ::std::array<LHS, size> {};
         }
      }

      /// Nest the above function for all types in a variadic template        
      template<class T1, class T2, class...TN>
      consteval auto LosslessNestedInner() noexcept {
         using T1T2 = decltype(Lossless<T1, T2>());

         if constexpr (sizeof...(TN))
            return LosslessNestedInner<T1T2, TN...>();
         else
            return T1T2 {};
      }

      /// Nest the above function for all types in a variadic template        
      template<class T1, class...TN>
      consteval auto LosslessNested() noexcept {
         if constexpr (sizeof...(TN) == 0)
            return ::std::array<Decay<TypeOf<T1>>, CountOf<T1>> {};
         else
            return LosslessNestedInner<T1, TN...>();
      }

   } // namespace Langulus::Inner

   /// Given any number of types, choose the one that is most lossless        
   /// after an arithmetic operation is performed between them. If any type   
   /// is an array, an array of OverlapCount size will be given back.         
   ///   @attention this will discard any sparseness or other modifiers       
   template<class T1, class...TN>
   using Lossless = Conditional<
         CountOf<decltype(Inner::LosslessNested<T1, TN...>())> == 1,
          TypeOf<decltype(Inner::LosslessNested<T1, TN...>())>,
          TypeOf<decltype(Inner::LosslessNested<T1, TN...>())>
               [CountOf<decltype(Inner::LosslessNested<T1, TN...>())>]
      >;

} // namespace Langulus
