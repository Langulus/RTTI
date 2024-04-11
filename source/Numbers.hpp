///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Reflection.hpp"
#include "Semantics.hpp"


namespace Langulus::CT
{

   /// Custom boolean concept (wrapped in another type)                       
   template<class...T>
   concept CustomBool = ((Typed<T> and
         BuiltinBool<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom character concept (wrapped in another type)                     
   template<class...T>
   concept CustomCharacter = ((Typed<T> and
         BuiltinCharacter<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom integer concept (wrapped in another type)                       
   /// Unlike CT::Integer, this includes character and boolean types          
   template<class...T>
   concept CustomInteger = ((Typed<T> and
         ::std::is_integral_v<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom real concept (wrapped in another type)                          
   template<class...T>
   concept CustomReal = ((Typed<T> and
         ::std::is_floating_point_v<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);
   
   /// Custom number concept (either sparse or dense)                         
   /// Any T that has underlying arithmetic type and is binary compatible     
   /// Unlike CT::BuiltinNumber, this one includes booleans and characters    
   template<class...T>
   concept CustomNumber = ((
      CustomBool<T> or CustomCharacter<T> or CustomInteger<T> or CustomReal<T>
   ) and ...);

   /// Any real concept, custom or not (either sparse or dense)               
   template<class...T>
   concept Real = ((BuiltinReal<T> or CustomReal<T>) and ...);

   /// Dense real concept                                                     
   template<class...T>
   concept DenseReal = ((Real<T> and Dense<T>) and ...);

   /// Sparse real concept                                                    
   template<class...T>
   concept SparseReal = ((Real<T> and Sparse<T>) and ...);

   /// Any integer concept, custom or not (either sparse or dense)            
   template<class...T>
   concept Integer = ((BuiltinInteger<T> or CustomInteger<T>) and ...);

   /// Dense integer concept                                                  
   template<class...T>
   concept DenseInteger = ((Integer<T> and Dense<T>) and ...);

   /// Sparse integer concept                                                 
   template<class...T>
   concept SparseInteger = ((Integer<T> and Sparse<T>) and ...);
   
   /// Any signed integer concept, custom or not (either sparse or dense)     
   template<class...T>
   concept SignedInteger = ((Signed<T> and Integer<T>) and ...);

   /// Any dense signed integer concept, custom or not                        
   template<class...T>
   concept DenseSignedInteger = ((SignedInteger<T> and Dense<T>) and ...);

   /// Any sparse signed integer concept, custom or not                       
   template<class...T>
   concept SparseSignedInteger = ((SignedInteger<T> and Sparse<T>) and ...);

   /// Any unsigned integer concept, custom or not (either sparse or dense)   
   template<class...T>
   concept UnsignedInteger = ((Unsigned<T> and Integer<T>) and ...);

   /// Any dense unsigned integer concept, custom or not                      
   template<class...T>
   concept DenseUnsignedInteger = ((UnsignedInteger<T> and Dense<T>) and ...);

   /// Any sparse unsigned integer concept, custom or not                     
   template<class...T>
   concept SparseUnsignedInteger = ((UnsignedInteger<T> and Sparse<T>) and ...);

   /// Any bool concept, custom or not, sparse or dense                       
   template<class...T>
   concept Bool = ((BuiltinBool<T> or CustomBool<T>) and ...);

   /// Dense bool concept                                                     
   template<class...T>
   concept DenseBool = ((Bool<T> and Dense<T>) and ...);

   /// Sparse bool concept                                                    
   template<class...T>
   concept SparseBool = ((Bool<T> and Sparse<T>) and ...);

   /// Any character concept, custom or not, sparse or dense                  
   template<class...T>
   concept Character = ((BuiltinCharacter<T> or CustomCharacter<T>) and ...);

   /// Dense character concept                                                
   template<class...T>
   concept DenseCharacter = ((Character<T> and Dense<T>) and ...);

   /// Sparse character concept                                               
   template<class...T>
   concept SparseCharacter = ((Character<T> and Sparse<T>) and ...);

   /// Any number concept, custom or not (either sparse or dense)             
   /// Excludes boolean and character types, unless wrapped in another type   
   template<class...T>
   concept Number = ((BuiltinNumber<T> or CustomNumber<T>) and ...);
   
   /// Dense number concept                                                   
   /// Excludes boolean and character types, unless wrapped in another type   
   template<class...T>
   concept DenseNumber = ((Number<T> and Dense<T>) and ...);

   /// Sparse number concept                                                  
   /// Excludes boolean types and character types, unless wrapped in another  
   template<class...T>
   concept SparseNumber = ((Number<T> and Sparse<T>) and ...);

   /// Single precision real number concept                                   
   template<class...T>
   concept Float = (Same<::Langulus::Float, T> and ...);

   /// Double precision real number concept                                   
   template<class...T>
   concept Double = (Same<::Langulus::Double, T> and ...);

   /// Size-related number concepts                                           
   /// These concepts include character and byte types                        
   template<class...T>
   concept SignedInteger8  = ((CT::SignedInteger<T> and sizeof(Decay<T>) == 1) and ...);
   template<class...T>
   concept SignedInteger16 = ((CT::SignedInteger<T> and sizeof(Decay<T>) == 2) and ...);
   template<class...T>
   concept SignedInteger32 = ((CT::SignedInteger<T> and sizeof(Decay<T>) == 4) and ...);
   template<class...T>
   concept SignedInteger64 = ((CT::SignedInteger<T> and sizeof(Decay<T>) == 8) and ...);

   template<class...T>
   concept UnsignedInteger16 = (((CT::UnsignedInteger<T> or CT::Character<T>)
       and sizeof(Decay<T>) == 2) and ...);

   template<class...T>
   concept UnsignedInteger32 = (((CT::UnsignedInteger<T> or CT::Character<T>)
       and sizeof(Decay<T>) == 4) and ...);

   template<class...T>
   concept UnsignedInteger64 = (((CT::UnsignedInteger<T> or CT::Character<T>)
       and sizeof(Decay<T>) == 8) and ...);

   template<class...T>
   concept Integer16 = ((SignedInteger16<T> or UnsignedInteger16<T>) and ...);
   template<class...T>
   concept Integer32 = ((SignedInteger32<T> or UnsignedInteger32<T>) and ...);
   template<class...T>
   concept Integer64 = ((SignedInteger64<T> or UnsignedInteger64<T>) and ...);

   namespace Inner
   {

      /// Analyze a single type for compile-time count trait                  
      /// The resulting count will be (bounded array extent) * any MemberCount
      /// constexpr static member, or constexpr size() method                 
      template<class T>
      consteval Count CountOfInner() noexcept {
         using DT = Decay<Desem<T>>;
         if constexpr (requires {{DT::MemberCount} -> UnsignedInteger; })
            return DT::MemberCount * ExtentOf<Desem<T>>;
         else if constexpr (requires (DT a) {{a.size()} -> UnsignedInteger;}) {
            if constexpr (IsConstexpr([] { return DT {}.size(); }))
               return DT {}.size() * ExtentOf<Desem<T>>;
            else
               return ExtentOf<Desem<T>>;
         }
         else return ExtentOf<Desem<T>>;
      }

      template<class T1, class...TN>
      consteval Count CountOf() noexcept {
         if constexpr (sizeof...(TN))
            return CountOfInner<T1>() + (CountOfInner<TN>() + ...);
         else
            return CountOfInner<T1>();
      }

   } // namespace Langulus::CT::Inner
         
   /// Check if all the provided types are considered Vector types            
   /// Any type that is Typed and has CountOf that is at least 2, and         
   /// the T's size is exactly equal to sizeof(TypeOf<T>) * CountOf<T>        
   /// Additionally, bounded arrays with more than a single element are also  
   /// considered Vector.                                                     
   ///   @attention any semantics are ignored, so that the usual semantics    
   ///      play well with custom vector types                                
   template<class...T>
   concept Vector = (((Typed<Desem<T>>
           and Inner::CountOf<Desem<T>>() > 1
           and sizeof(Desem<T>) == sizeof(TypeOf<Desem<T>>) * Inner::CountOf<Desem<T>>()
         ) or (ExtentOf<Desem<T>> > 1)
      ) and ...);

   template<class...T>
   concept DenseVector = ((Dense<Desem<T>> and Vector<T>) and ...);

   /// Scalar concept                                                         
   /// Any type that isn't a Vector type, and isn't a Semantic                
   ///   @attention any semantics are ignored, so that the usual semantics    
   ///      play well with custom vector types                                
   template<class...T>
   concept Scalar = ((not Vector<Desem<T>>) and ...);

   template<class...T>
   concept DenseScalar = ((Dense<Desem<T>> and Scalar<T>) and ...);

} // namespace Langulus::CT


namespace Langulus
{

   /// Get the count of a counted type                                        
   /// Any type with a static constexpr unsigned MemberCount, or a bounded    
   /// array, are multiplied and counted. Scalars always will give count of 1 
   /// If multiple types provided, their counts will be summed                
   template<class T1, class...TN>
   constexpr Count CountOf = CT::Inner::CountOf<T1, TN...>();

   /// Casts a scalar to its underlying fundamental type (const)              
   /// If non-void T::CTTI_InnerType exists, or if T is an enum, the inner    
   /// type returns                                                           
   ///   @tparam T - type of the scalar/enum to cast                          
   ///   @param a - the scalar to cast                                        
   ///   @return a reference to the underlying type                           
   template<CT::Scalar T> NOD() LANGULUS(INLINED)
   constexpr decltype(auto) FundamentalCast(const T& a) noexcept {
      using DT = Decay<Desem<T>>;
      if constexpr (CT::Fundamental<DT>) {
         // Already fundamental, just forward it                        
         return (a);
      }
      else if constexpr (CT::Typed<DT> or CT::Enum<DT>) {
         // Explicitly cast to a reference of the contained type, and   
         // nest down to the fundamentals                               
         return FundamentalCast(static_cast<const TypeOf<DT>&>(DenseCast(a)));
      }
      else LANGULUS_ERROR("Shouldn't happen");
   }
   
   /// Casts a scalar to its underlying fundamental type                      
   /// If non-void T::CTTI_InnerType exists, or if T is an enum, the inner    
   /// type returns                                                           
   ///   @tparam T - type of the scalar/enum to cast                          
   ///   @param a - the scalar to cast                                        
   ///   @return a reference to the underlying type                           
   template<CT::Scalar T> NOD() LANGULUS(INLINED)
   constexpr decltype(auto) FundamentalCast(T& a) noexcept {
      using DT = Decay<Desem<T>>;
      if constexpr (CT::Fundamental<DT>) {
         // Already fundamental, just forward it                        
         return (a);
      }
      else if constexpr (CT::Typed<DT> or CT::Enum<DT>) {
         // Explicitly cast to a reference of the contained type, and   
         // nest down to the fundamentals                               
         return FundamentalCast(static_cast<TypeOf<DT>&>(DenseCast(a)));
      }
      else LANGULUS_ERROR("Shouldn't happen");
   }
    
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
      constexpr auto lhs = ExtentOf<Desem<LHS>>;
      constexpr auto rhs = ExtentOf<Desem<RHS>>;

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
      constexpr auto lhs = CountOf<Desem<LHS>>;
      constexpr auto rhs = CountOf<Desem<RHS>>;

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
      ///   @attention this will discard any semantics                        
      template<class T1, class T2>
      consteval auto Lossless() noexcept {
         constexpr auto size = OverlapCounts<T1, T2>();
         using LHS = Decay<TypeOf<Desem<T1>>>;
         using RHS = Decay<TypeOf<Desem<T2>>>;

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