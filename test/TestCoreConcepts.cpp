///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "Common.hpp"


//TODO IF_CONSTEXPR()
//TODO ArgumentOf
//TODO ReturnOf
//TODO Deref
//TODO Deptr
//TODO Decvq
//TODO Deext
//TODO Decay


///                                                                           
/// CT::TypeSwap                                                              
///                                                                           
TEST_CASE("Testing TypeSwap", "[type navigation]") {
   static_assert(CT::Exact<TypeSwap<int, float>, float>);
   static_assert(CT::Exact<TypeSwap<int[2], float>, float[2]>);
   static_assert(CT::Exact<TypeSwap<int&, float>, float&>);
   static_assert(CT::Exact<TypeSwap<int&&, float>, float&&>);
   static_assert(CT::Exact<TypeSwap<volatile int, float>, volatile float>);
   static_assert(CT::Exact<TypeSwap<int*, float>, float*>);
   static_assert(CT::Exact<TypeSwap<int***, float>, float***>);
   static_assert(CT::Exact<TypeSwap<int***[5], float>, float***[5]>);
   static_assert(CT::Exact<TypeSwap<const int***[5], float>, const float***[5]>);
   static_assert(CT::Exact<TypeSwap<int**const*const, float>, float**const*const>);
   static_assert(CT::Exact<TypeSwap<int const**const*const&, float>, float const**const*const&>);
}

//TODO CT::Complete
//TODO CT::Same
//TODO CT::Similar
//TODO CT::Exact
//TODO CT::SameAsOneOf
//TODO CT::SimilarAsOneOf
//TODO CT::ExactAsOneOf
//TODO CT::Unsupported
//TODO CT::Supported
//TODO CT::Array
//TODO CT::Function
//TODO CT::Sparse
//TODO CT::Dense
//TODO CT::Constant
//TODO CT::Volatile
//TODO CT::Convoluted
//TODO CT::Mutable
//TODO CT::Signed
//TODO CT::DenseSigned
//TODO CT::SparseSigned
//TODO CT::Unsigned
//TODO CT::DenseUnsigned
//TODO CT::SparseUnsigned
//TODO CT::BuiltinBool
//TODO CT::BuiltinCharacter
//TODO CT::StringLiteral
//TODO CT::StringPointer
//TODO CT::String
//TODO CT::StdContainer
//TODO CT::StdContiguousContainer
//TODO CT::BuiltinInteger
//TODO CT::BuiltinSignedInteger
//TODO CT::BuiltinUnsignedInteger
//TODO CT::BuiltinReal
//TODO CT::BuiltinNumber
//TODO CT::DenseBuiltinNumber
//TODO CT::SparseBuiltinNumber
//TODO CT::Convertible
//TODO CT::Sortable
//TODO CT::Comparable
//TODO CT::Enum
//TODO CT::Fundamental
//TODO CT::Arithmetic
//TODO CT::Referencable
//TODO CT::Swappable
//TODO CT::SwappableNoexcept
//TODO CT::DerivedFrom
//TODO CT::Related
//TODO CT::BinaryCompatible
//TODO CT::VirtuallyDerivedFrom
//TODO CT::Reference
//TODO CT::Slab
//TODO CT::Decayed
//TODO CT::NotDecayed
//TODO CT::Void

///                                                                           
/// CT::TypeErased                                                            
///                                                                           
TEST_CASE("Testing CT::TypeErased", "[concepts]") {
   static_assert(not CT::TypeErased<bool>);
   static_assert(    CT::TypeErased<void>);
   static_assert(    CT::TypeErased<const void>);
   static_assert(    CT::TypeErased<volatile void>);
   static_assert(not CT::TypeErased<void*>);
   static_assert(not CT::TypeErased<bool, void, void*>);
   static_assert(    CT::TypeErased<void, void, void>);
}

//TODO CT::Data
//TODO CT::DenseData
//TODO CT::SparseData
//TODO CT::DataReference
//TODO CT::Nullptr
//TODO CT::Conditional
//TODO CT::ExtentOf
//TODO CT::DecvqAll
