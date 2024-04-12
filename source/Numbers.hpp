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
   template<class...T>
   concept CustomInteger = ((Typed<T> and
         ::std::is_integral_v<Deref<TypeOf<T>>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom real concept (wrapped in another type)                          
   template<class...T>
   concept CustomReal = ((Typed<T> and
         ::std::is_floating_point_v<Deref<TypeOf<T>>> and sizeof(T) == sizeof(TypeOf<T>)
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
     
} // namespace Langulus::CT
