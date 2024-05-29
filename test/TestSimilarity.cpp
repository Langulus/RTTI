///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "Common.hpp"


/// A freestanding type compatibility check                                   
/// Purely cosmetic, to avoid typing `template` before member function        
template<CT::Data T, bool ADVANCED = false> LANGULUS(INLINED)
bool CastsTo(DMeta from) {
   return from->template CastsTo<T, ADVANCED>();
}

/// A freestanding type compatibility check                                   
/// Purely cosmetic, to avoid typing `template` before member function        
template<CT::Data T> LANGULUS(INLINED)
bool CastsTo(DMeta from, Count count) {
   return from->template CastsTo<T>(count);
}

TEMPLATE_TEST_CASE("Unsigned integer RTTI interpretation", "[metadata]",
   uint8_t, uint16_t, uint32_t, uint64_t
) {
   GIVEN("An unsigned integer type") {
      auto meta = MetaData::Of<TestType>();
      REQUIRE(meta != nullptr);

      WHEN("Interpreted as another type") {
         REQUIRE(CastsTo<A::Number>(meta));
         REQUIRE(CastsTo<A::Integer>(meta));
         REQUIRE(CastsTo<A::Unsigned>(meta));
         REQUIRE(CastsTo<A::UnsignedInteger>(meta));

         REQUIRE(CastsTo<A::Number>(meta, 1));
         REQUIRE(CastsTo<A::Integer>(meta, 1));
         REQUIRE(CastsTo<A::Unsigned>(meta, 1));
         REQUIRE(CastsTo<A::UnsignedInteger>(meta, 1));

         REQUIRE_FALSE(CastsTo<A::Real>(meta));
         REQUIRE_FALSE(CastsTo<A::Signed>(meta));
         REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta));

         REQUIRE_FALSE(CastsTo<A::Real>(meta, 1));
         REQUIRE_FALSE(CastsTo<A::Signed>(meta, 1));
         REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 1));

         REQUIRE_FALSE(CastsTo<A::Number>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Integer>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Real>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Signed>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 2));
      }
   }
}

TEMPLATE_TEST_CASE("Signed integer RTTI interpretation", "[metadata]",
   int8_t, int16_t, int32_t, int64_t
) {
   GIVEN("A signed integer type") {
      auto meta = MetaData::Of<TestType>();
      REQUIRE(meta != nullptr);

      WHEN("Interpreted as another type") {
         REQUIRE(CastsTo<A::Number>(meta));
         REQUIRE(CastsTo<A::Integer>(meta));
         REQUIRE(CastsTo<A::Signed>(meta));
         REQUIRE(CastsTo<A::SignedInteger>(meta));

         REQUIRE_FALSE(CastsTo<A::Real>(meta));
         REQUIRE_FALSE(CastsTo<A::Unsigned>(meta));
         REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta));

         REQUIRE(CastsTo<A::Number>(meta, 1));
         REQUIRE(CastsTo<A::Integer>(meta, 1));
         REQUIRE(CastsTo<A::Signed>(meta, 1));
         REQUIRE(CastsTo<A::SignedInteger>(meta, 1));

         REQUIRE_FALSE(CastsTo<A::Real>(meta, 1));
         REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 1));
         REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 1));

         REQUIRE_FALSE(CastsTo<A::Number>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Integer>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Real>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Signed>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 2));
      }
   }
}

TEMPLATE_TEST_CASE("Real number RTTI interpretation", "[metadata]",
   float, double
) {
   GIVEN("A real number type") {
      auto meta = MetaData::Of<TestType>();
      REQUIRE(meta != nullptr);

      WHEN("Interpreted as another type") {
         REQUIRE(CastsTo<A::Number>(meta));
         REQUIRE(CastsTo<A::Real>(meta));
         REQUIRE(CastsTo<A::Signed>(meta));

         REQUIRE_FALSE(CastsTo<A::Integer>(meta));
         REQUIRE_FALSE(CastsTo<A::Unsigned>(meta));
         REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta));
         REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta));

         REQUIRE(CastsTo<A::Number>(meta, 1));
         REQUIRE(CastsTo<A::Real>(meta, 1));
         REQUIRE(CastsTo<A::Signed>(meta, 1));

         REQUIRE_FALSE(CastsTo<A::Integer>(meta, 1));
         REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 1));
         REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 1));
         REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 1));

         REQUIRE_FALSE(CastsTo<A::Number>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Integer>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Real>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Unsigned>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::Signed>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::UnsignedInteger>(meta, 2));
         REQUIRE_FALSE(CastsTo<A::SignedInteger>(meta, 2));
      }
   }
}

template<class... T>
struct Sequence {
   static constexpr auto count = sizeof...(T);
   DMeta meta[count]     {MetaDataOf<T>()...};
   DMeta metac[count]    {MetaDataOf<const T>()...};
   DMeta metav[count]    {MetaDataOf<volatile T>()...};
   DMeta metap[count]    {MetaDataOf<T*>()...};
   DMeta metacp[count]   {MetaDataOf<const T*>()...};
   DMeta metacpc[count]  {MetaDataOf<const T* const>()...};
   DMeta metacpcp[count] {MetaDataOf<const T* const*>()...};
};

TEST_CASE("MetaData::Is", "[is]") {
   WHEN("MetaData::Is is used to check similarity of complete types") {
      Sequence<N1::Type, N1::Create, N2::Type, N3::type> sequence;

      for (unsigned i = 0; i < sequence.count; ++i) {
         // Compare horizontally                                        
         REQUIRE(sequence.meta   [i]->Is(sequence.meta[i]));
         REQUIRE(sequence.meta   [i]->Is(sequence.metac[i]));
         REQUIRE(sequence.meta   [i]->Is(sequence.metav[i]));
         REQUIRE(sequence.meta   [i]->Is(sequence.metap[i]));
         REQUIRE(sequence.meta   [i]->Is(sequence.metacp[i]));
         REQUIRE(sequence.meta   [i]->Is(sequence.metacpc[i]));
         REQUIRE(sequence.meta   [i]->Is(sequence.metacpcp[i]));
                                 
         REQUIRE(sequence.metac  [i]->Is(sequence.meta[i]));
         REQUIRE(sequence.metac  [i]->Is(sequence.metac[i]));
         REQUIRE(sequence.metac  [i]->Is(sequence.metav[i]));
         REQUIRE(sequence.metac  [i]->Is(sequence.metap[i]));
         REQUIRE(sequence.metac  [i]->Is(sequence.metacp[i]));
         REQUIRE(sequence.metac  [i]->Is(sequence.metacpc[i]));
         REQUIRE(sequence.metac  [i]->Is(sequence.metacpcp[i]));
                                 
         REQUIRE(sequence.metav  [i]->Is(sequence.meta[i]));
         REQUIRE(sequence.metav  [i]->Is(sequence.metac[i]));
         REQUIRE(sequence.metav  [i]->Is(sequence.metav[i]));
         REQUIRE(sequence.metav  [i]->Is(sequence.metap[i]));
         REQUIRE(sequence.metav  [i]->Is(sequence.metacp[i]));
         REQUIRE(sequence.metav  [i]->Is(sequence.metacpc[i]));
         REQUIRE(sequence.metav  [i]->Is(sequence.metacpcp[i]));
                                 
         REQUIRE(sequence.metap  [i]->Is(sequence.meta[i]));
         REQUIRE(sequence.metap  [i]->Is(sequence.metac[i]));
         REQUIRE(sequence.metap  [i]->Is(sequence.metav[i]));
         REQUIRE(sequence.metap  [i]->Is(sequence.metap[i]));
         REQUIRE(sequence.metap  [i]->Is(sequence.metacp[i]));
         REQUIRE(sequence.metap  [i]->Is(sequence.metacpc[i]));
         REQUIRE(sequence.metap  [i]->Is(sequence.metacpcp[i]));
                                 
         REQUIRE(sequence.metacp [i]->Is(sequence.meta[i]));
         REQUIRE(sequence.metacp [i]->Is(sequence.metac[i]));
         REQUIRE(sequence.metacp [i]->Is(sequence.metav[i]));
         REQUIRE(sequence.metacp [i]->Is(sequence.metap[i]));
         REQUIRE(sequence.metacp [i]->Is(sequence.metacp[i]));
         REQUIRE(sequence.metacp [i]->Is(sequence.metacpc[i]));
         REQUIRE(sequence.metacp [i]->Is(sequence.metacpcp[i]));

         REQUIRE(sequence.metacpc[i]->Is(sequence.meta[i]));
         REQUIRE(sequence.metacpc[i]->Is(sequence.metac[i]));
         REQUIRE(sequence.metacpc[i]->Is(sequence.metav[i]));
         REQUIRE(sequence.metacpc[i]->Is(sequence.metap[i]));
         REQUIRE(sequence.metacpc[i]->Is(sequence.metacp[i]));
         REQUIRE(sequence.metacpc[i]->Is(sequence.metacpc[i]));
         REQUIRE(sequence.metacpc[i]->Is(sequence.metacpcp[i]));


         REQUIRE(sequence.metacpcp[i]->Is(sequence.meta[i]));
         REQUIRE(sequence.metacpcp[i]->Is(sequence.metac[i]));
         REQUIRE(sequence.metacpcp[i]->Is(sequence.metav[i]));
         REQUIRE(sequence.metacpcp[i]->Is(sequence.metap[i]));
         REQUIRE(sequence.metacpcp[i]->Is(sequence.metacp[i]));
         REQUIRE(sequence.metacpcp[i]->Is(sequence.metacpc[i]));
         REQUIRE(sequence.metacpcp[i]->Is(sequence.metacpcp[i]));

         for (unsigned j = 0; j < sequence.count; ++j) {
            if (i == j)
               continue;

            // Compare vertically                                       
            REQUIRE_FALSE(sequence.meta   [j]->Is(sequence.meta[i]));
            REQUIRE_FALSE(sequence.meta   [j]->Is(sequence.metac[i]));
            REQUIRE_FALSE(sequence.meta   [j]->Is(sequence.metav[i]));
            REQUIRE_FALSE(sequence.meta   [j]->Is(sequence.metap[i]));
            REQUIRE_FALSE(sequence.meta   [j]->Is(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.meta   [j]->Is(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.meta   [j]->Is(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metac  [j]->Is(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metac  [j]->Is(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metac  [j]->Is(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metac  [j]->Is(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metac  [j]->Is(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metac  [j]->Is(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metac  [j]->Is(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metav  [j]->Is(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metav  [j]->Is(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metav  [j]->Is(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metav  [j]->Is(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metav  [j]->Is(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metav  [j]->Is(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metav  [j]->Is(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metap  [j]->Is(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metap  [j]->Is(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metap  [j]->Is(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metap  [j]->Is(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metap  [j]->Is(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metap  [j]->Is(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metap  [j]->Is(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacp [j]->Is(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacp [j]->Is(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacp [j]->Is(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacp [j]->Is(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacp [j]->Is(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacp [j]->Is(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacp [j]->Is(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpc[j]->Is(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->Is(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->Is(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->Is(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->Is(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->Is(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->Is(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpcp[j]->Is(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->Is(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->Is(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->Is(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->Is(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->Is(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->Is(sequence.metacpcp[i]));
         }
      }
   }
}

TEST_CASE("MetaData::IsSimilar", "[is]") {
   static_assert(CT::Similar<void*, const void*>);
   static_assert(CT::Similar<void* const, const void*>);
   static_assert(CT::Similar<void* const, void*>);
   static_assert(CT::Similar<void**, const void**>);
   static_assert(CT::Similar<void** const, const void* const*>);
   static_assert(CT::Similar<void const* const* const, void**>);
   static_assert(CT::Similar<void const* const* const, void**, void** const, const void** const>);

   static_assert(not CT::Similar<int*, const float*>);
   static_assert(not CT::Similar<int* const, const float*>);
   static_assert(not CT::Similar<int* const, float*>);
   static_assert(not CT::Similar<int**, const float**>);
   static_assert(not CT::Similar<int** const, const float* const*>);
   static_assert(not CT::Similar<int const* const* const, float**>);
   static_assert(not CT::Similar<int const* const* const, float**, const int**>);

   WHEN("MetaData::IsSimilar is used to check similarity of incomplete types") {
      //DMeta meta1 = MetaDataOf<IncompleteType>(); // shouldn't build
      DMeta meta = MetaDataOf<IncompleteType*>();

      REQUIRE(meta->IsSimilar<IncompleteType*>());
      REQUIRE(meta->IsSimilar<const IncompleteType*>());
      REQUIRE(meta->IsSimilar<const IncompleteType* const>());
   }

   WHEN("MetaData::IsSimilar is used to check similarity of complete types") {
      Sequence<N1::Type, N1::Create, N2::Type, N3::type> sequence;

      for (unsigned i = 0; i < sequence.count; ++i) {
         // Compare horizontally                                        
         REQUIRE      (sequence.meta   [i]->IsSimilar(sequence.meta[i]));
         REQUIRE      (sequence.meta   [i]->IsSimilar(sequence.metac[i]));
         REQUIRE      (sequence.meta   [i]->IsSimilar(sequence.metav[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsSimilar(sequence.metap[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsSimilar(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsSimilar(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsSimilar(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsSimilar<void*>());
         REQUIRE_FALSE(sequence.meta   [i]->IsSimilar<void*, const void*, const void* const>());
                                       
         REQUIRE      (sequence.metac  [i]->IsSimilar(sequence.meta[i]));
         REQUIRE      (sequence.metac  [i]->IsSimilar(sequence.metac[i]));
         REQUIRE      (sequence.metac  [i]->IsSimilar(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsSimilar(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsSimilar(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsSimilar(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsSimilar(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsSimilar<void*>());
         REQUIRE_FALSE(sequence.metac  [i]->IsSimilar<void*, const void*, const void* const>());

         REQUIRE      (sequence.metav  [i]->IsSimilar(sequence.meta[i]));
         REQUIRE      (sequence.metav  [i]->IsSimilar(sequence.metac[i]));
         REQUIRE      (sequence.metav  [i]->IsSimilar(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsSimilar(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsSimilar(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsSimilar(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsSimilar(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsSimilar<void*>());
         REQUIRE_FALSE(sequence.metav  [i]->IsSimilar<void*, const void*, const void* const>());
                                     
         REQUIRE_FALSE(sequence.metap  [i]->IsSimilar(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metap  [i]->IsSimilar(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metap  [i]->IsSimilar(sequence.metav[i]));
         REQUIRE      (sequence.metap  [i]->IsSimilar(sequence.metap[i]));
         REQUIRE      (sequence.metap  [i]->IsSimilar(sequence.metacp[i]));
         REQUIRE      (sequence.metap  [i]->IsSimilar(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metap  [i]->IsSimilar(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metap  [i]->IsSimilar<void*>());
         REQUIRE_FALSE(sequence.metap  [i]->IsSimilar<void*, const void*, const void* const>());
                                       
         REQUIRE_FALSE(sequence.metacp [i]->IsSimilar(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacp [i]->IsSimilar(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacp [i]->IsSimilar(sequence.metav[i]));
         REQUIRE      (sequence.metacp [i]->IsSimilar(sequence.metap[i]));
         REQUIRE      (sequence.metacp [i]->IsSimilar(sequence.metacp[i]));
         REQUIRE      (sequence.metacp [i]->IsSimilar(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacp [i]->IsSimilar(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metacp [i]->IsSimilar<void*>());
         REQUIRE_FALSE(sequence.metacp [i]->IsSimilar<void*, const void*, const void* const>());

         REQUIRE_FALSE(sequence.metacpc[i]->IsSimilar(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpc[i]->IsSimilar(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpc[i]->IsSimilar(sequence.metav[i]));
         REQUIRE      (sequence.metacpc[i]->IsSimilar(sequence.metap[i]));
         REQUIRE      (sequence.metacpc[i]->IsSimilar(sequence.metacp[i]));
         REQUIRE      (sequence.metacpc[i]->IsSimilar(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacpc[i]->IsSimilar(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metacpc[i]->IsSimilar<void*>());
         REQUIRE_FALSE(sequence.metacpc[i]->IsSimilar<void*, const void*, const void* const>());
         
         REQUIRE_FALSE(sequence.metacpcp[i]->IsSimilar(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsSimilar(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsSimilar(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsSimilar(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsSimilar(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsSimilar(sequence.metacpc[i]));
         REQUIRE      (sequence.metacpcp[i]->IsSimilar(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsSimilar<void*>());
         REQUIRE_FALSE(sequence.metacpcp[i]->IsSimilar<void*, const void*, const void* const>());
         
         for (unsigned j = 0; j < sequence.count; ++j) {
            if (i == j)
               continue;

            // Compare vertically                                       
            REQUIRE_FALSE(sequence.meta   [j]->IsSimilar(sequence.meta[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsSimilar(sequence.metac[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsSimilar(sequence.metav[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsSimilar(sequence.metap[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsSimilar(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsSimilar(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsSimilar(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metac  [j]->IsSimilar(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsSimilar(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsSimilar(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsSimilar(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsSimilar(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsSimilar(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsSimilar(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metav  [j]->IsSimilar(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsSimilar(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsSimilar(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsSimilar(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsSimilar(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsSimilar(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsSimilar(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metap  [j]->IsSimilar(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsSimilar(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsSimilar(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsSimilar(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsSimilar(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsSimilar(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsSimilar(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacp [j]->IsSimilar(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsSimilar(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsSimilar(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsSimilar(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsSimilar(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsSimilar(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsSimilar(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpc[j]->IsSimilar(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsSimilar(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsSimilar(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsSimilar(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsSimilar(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsSimilar(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsSimilar(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpcp[j]->IsSimilar(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsSimilar(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsSimilar(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsSimilar(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsSimilar(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsSimilar(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsSimilar(sequence.metacpcp[i]));
         }
      }
   }
}

TEST_CASE("MetaData::IsExact", "[is]") {
   WHEN("MetaData::IsExact is used to check similarity of complete types") {
      Sequence<N1::Type, N1::Create, N2::Type, N3::type> sequence;

      for (unsigned i = 0; i < sequence.count; ++i) {
         // Compare horizontally                                        
         REQUIRE      (sequence.meta   [i]->IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.meta   [i]->IsExact(sequence.metacpcp[i]));
                                       
         REQUIRE_FALSE(sequence.metac  [i]->IsExact(sequence.meta[i]));
         REQUIRE      (sequence.metac  [i]->IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metac  [i]->IsExact(sequence.metacpcp[i]));
                                       
         REQUIRE_FALSE(sequence.metav  [i]->IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsExact(sequence.metac[i]));
         REQUIRE      (sequence.metav  [i]->IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metav  [i]->IsExact(sequence.metacpcp[i]));
                                       
         REQUIRE_FALSE(sequence.metap  [i]->IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metap  [i]->IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metap  [i]->IsExact(sequence.metav[i]));
         REQUIRE      (sequence.metap  [i]->IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metap  [i]->IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metap  [i]->IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metap  [i]->IsExact(sequence.metacpcp[i]));
                                       
         REQUIRE_FALSE(sequence.metacp [i]->IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacp [i]->IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacp [i]->IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacp [i]->IsExact(sequence.metap[i]));
         REQUIRE      (sequence.metacp [i]->IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metacp [i]->IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacp [i]->IsExact(sequence.metacpcp[i]));

         REQUIRE_FALSE(sequence.metacpc[i]->IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpc[i]->IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpc[i]->IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacpc[i]->IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metacpc[i]->IsExact(sequence.metacp[i]));
         REQUIRE      (sequence.metacpc[i]->IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacpc[i]->IsExact(sequence.metacpcp[i]));
         
         REQUIRE_FALSE(sequence.metacpcp[i]->IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metacpcp[i]->IsExact(sequence.metacpc[i]));
         REQUIRE      (sequence.metacpcp[i]->IsExact(sequence.metacpcp[i]));
         
         for (unsigned j = 0; j < sequence.count; ++j) {
            if (i == j)
               continue;

            // Compare vertically                                       
            REQUIRE_FALSE(sequence.meta   [j]->IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.meta   [j]->IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metac  [j]->IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metac  [j]->IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metav  [j]->IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metav  [j]->IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metap  [j]->IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metap  [j]->IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacp [j]->IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacp [j]->IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpc[j]->IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpc[j]->IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpcp[j]->IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpcp[j]->IsExact(sequence.metacpcp[i]));
         }
      }
   }
}
