///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/Testing/CommonTypes.hpp>
#include <Langulus/MetaOf.hpp>


TEST_CASE_TEMPLATE("Unsigned integer RTTI interpretation", TestType,
   uint8_t, uint16_t, uint32_t, uint64_t
) {
   auto meta = MetaDataOf<TestType>();
   REQUIRE(meta != nullptr);

   REQUIRE(meta.CastsTo(MetaDataOf<AbstractNumber>()));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractInteger>()));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractUnsigned>()));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractUnsignedInteger>()));

   REQUIRE(meta.CastsTo(MetaDataOf<AbstractNumber>(), 1));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractInteger>(), 1));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractUnsigned>(), 1));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractUnsignedInteger>(), 1));

   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractReal>()));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSigned>()));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSignedInteger>()));

   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractReal>(), 1));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSigned>(), 1));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSignedInteger>(), 1));

   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractNumber>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractInteger>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractReal>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsigned>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSigned>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsignedInteger>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSignedInteger>(), 2));
}

TEST_CASE_TEMPLATE("Signed integer RTTI interpretation", TestType,
   int8_t, int16_t, int32_t, int64_t
) {
   auto meta = MetaDataOf<TestType>();
   REQUIRE(meta != nullptr);

   REQUIRE(meta.CastsTo(MetaDataOf<AbstractNumber>()));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractInteger>()));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractSigned>()));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractSignedInteger>()));

   REQUIRE(meta.CastsTo(MetaDataOf<AbstractNumber>(), 1));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractInteger>(), 1));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractSigned>(), 1));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractSignedInteger>(), 1));

   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractReal>()));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsigned>()));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsignedInteger>()));

   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractReal>(), 1));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsigned>(), 1));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsignedInteger>(), 1));

   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractNumber>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractInteger>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractReal>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSigned>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsigned>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSignedInteger>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsignedInteger>(), 2));
}

TEST_CASE_TEMPLATE("Real number RTTI interpretation", TestType,
   float, double
) {
   auto meta = MetaDataOf<TestType>();
   REQUIRE(meta != nullptr);

   REQUIRE(meta.CastsTo(MetaDataOf<AbstractNumber>()));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractReal>()));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractSigned>()));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractSignedReal>()));

   REQUIRE(meta.CastsTo(MetaDataOf<AbstractNumber>(), 1));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractReal>(), 1));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractSigned>(), 1));
   REQUIRE(meta.CastsTo(MetaDataOf<AbstractSignedReal>(), 1));

   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractInteger>()));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsigned>()));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsignedInteger>()));

   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractInteger>(), 1));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsigned>(), 1));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsignedInteger>(), 1));

   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractNumber>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractInteger>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractReal>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSigned>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsigned>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractSignedInteger>(), 2));
   REQUIRE_FALSE(meta.CastsTo(MetaDataOf<AbstractUnsignedInteger>(), 2));
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

TEST_CASE("MetaData::Is") {
   Sequence<N1::EmptyType, N1::Create, N2::EmptyType, N3::emptytype> sequence;

   for (unsigned i = 0; i < sequence.count; ++i) {
      // Compare horizontally                                        
      REQUIRE(sequence.meta   [i].Is(sequence.meta[i]));
      REQUIRE(sequence.meta   [i].Is(sequence.metac[i]));
      REQUIRE(sequence.meta   [i].Is(sequence.metav[i]));
      REQUIRE(sequence.meta   [i].Is(sequence.metap[i]));
      REQUIRE(sequence.meta   [i].Is(sequence.metacp[i]));
      REQUIRE(sequence.meta   [i].Is(sequence.metacpc[i]));
      REQUIRE(sequence.meta   [i].Is(sequence.metacpcp[i]));
                              
      REQUIRE(sequence.metac  [i].Is(sequence.meta[i]));
      REQUIRE(sequence.metac  [i].Is(sequence.metac[i]));
      REQUIRE(sequence.metac  [i].Is(sequence.metav[i]));
      REQUIRE(sequence.metac  [i].Is(sequence.metap[i]));
      REQUIRE(sequence.metac  [i].Is(sequence.metacp[i]));
      REQUIRE(sequence.metac  [i].Is(sequence.metacpc[i]));
      REQUIRE(sequence.metac  [i].Is(sequence.metacpcp[i]));
                              
      REQUIRE(sequence.metav  [i].Is(sequence.meta[i]));
      REQUIRE(sequence.metav  [i].Is(sequence.metac[i]));
      REQUIRE(sequence.metav  [i].Is(sequence.metav[i]));
      REQUIRE(sequence.metav  [i].Is(sequence.metap[i]));
      REQUIRE(sequence.metav  [i].Is(sequence.metacp[i]));
      REQUIRE(sequence.metav  [i].Is(sequence.metacpc[i]));
      REQUIRE(sequence.metav  [i].Is(sequence.metacpcp[i]));
                              
      REQUIRE(sequence.metap  [i].Is(sequence.meta[i]));
      REQUIRE(sequence.metap  [i].Is(sequence.metac[i]));
      REQUIRE(sequence.metap  [i].Is(sequence.metav[i]));
      REQUIRE(sequence.metap  [i].Is(sequence.metap[i]));
      REQUIRE(sequence.metap  [i].Is(sequence.metacp[i]));
      REQUIRE(sequence.metap  [i].Is(sequence.metacpc[i]));
      REQUIRE(sequence.metap  [i].Is(sequence.metacpcp[i]));
                              
      REQUIRE(sequence.metacp [i].Is(sequence.meta[i]));
      REQUIRE(sequence.metacp [i].Is(sequence.metac[i]));
      REQUIRE(sequence.metacp [i].Is(sequence.metav[i]));
      REQUIRE(sequence.metacp [i].Is(sequence.metap[i]));
      REQUIRE(sequence.metacp [i].Is(sequence.metacp[i]));
      REQUIRE(sequence.metacp [i].Is(sequence.metacpc[i]));
      REQUIRE(sequence.metacp [i].Is(sequence.metacpcp[i]));

      REQUIRE(sequence.metacpc[i].Is(sequence.meta[i]));
      REQUIRE(sequence.metacpc[i].Is(sequence.metac[i]));
      REQUIRE(sequence.metacpc[i].Is(sequence.metav[i]));
      REQUIRE(sequence.metacpc[i].Is(sequence.metap[i]));
      REQUIRE(sequence.metacpc[i].Is(sequence.metacp[i]));
      REQUIRE(sequence.metacpc[i].Is(sequence.metacpc[i]));
      REQUIRE(sequence.metacpc[i].Is(sequence.metacpcp[i]));


      REQUIRE(sequence.metacpcp[i].Is(sequence.meta[i]));
      REQUIRE(sequence.metacpcp[i].Is(sequence.metac[i]));
      REQUIRE(sequence.metacpcp[i].Is(sequence.metav[i]));
      REQUIRE(sequence.metacpcp[i].Is(sequence.metap[i]));
      REQUIRE(sequence.metacpcp[i].Is(sequence.metacp[i]));
      REQUIRE(sequence.metacpcp[i].Is(sequence.metacpc[i]));
      REQUIRE(sequence.metacpcp[i].Is(sequence.metacpcp[i]));

      for (unsigned j = 0; j < sequence.count; ++j) {
         if (i == j)
            continue;

         // Compare vertically                                       
         REQUIRE_FALSE(sequence.meta   [j].Is(sequence.meta[i]));
         REQUIRE_FALSE(sequence.meta   [j].Is(sequence.metac[i]));
         REQUIRE_FALSE(sequence.meta   [j].Is(sequence.metav[i]));
         REQUIRE_FALSE(sequence.meta   [j].Is(sequence.metap[i]));
         REQUIRE_FALSE(sequence.meta   [j].Is(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.meta   [j].Is(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.meta   [j].Is(sequence.metacpcp[i]));

         REQUIRE_FALSE(sequence.metac  [j].Is(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metac  [j].Is(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metac  [j].Is(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metac  [j].Is(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metac  [j].Is(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metac  [j].Is(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metac  [j].Is(sequence.metacpcp[i]));

         REQUIRE_FALSE(sequence.metav  [j].Is(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metav  [j].Is(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metav  [j].Is(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metav  [j].Is(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metav  [j].Is(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metav  [j].Is(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metav  [j].Is(sequence.metacpcp[i]));

         REQUIRE_FALSE(sequence.metap  [j].Is(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metap  [j].Is(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metap  [j].Is(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metap  [j].Is(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metap  [j].Is(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metap  [j].Is(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metap  [j].Is(sequence.metacpcp[i]));

         REQUIRE_FALSE(sequence.metacp [j].Is(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacp [j].Is(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacp [j].Is(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacp [j].Is(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metacp [j].Is(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metacp [j].Is(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacp [j].Is(sequence.metacpcp[i]));

         REQUIRE_FALSE(sequence.metacpc[j].Is(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpc[j].Is(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpc[j].Is(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacpc[j].Is(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metacpc[j].Is(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metacpc[j].Is(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacpc[j].Is(sequence.metacpcp[i]));

         REQUIRE_FALSE(sequence.metacpcp[j].Is(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpcp[j].Is(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpcp[j].Is(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacpcp[j].Is(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metacpcp[j].Is(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metacpcp[j].Is(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacpcp[j].Is(sequence.metacpcp[i]));
      }
   }
}

TEST_CASE("MetaData::IsSimilar") {
   static_assert(Same<void*, const void*>);
   static_assert(Same<void* const, const void*>);
   static_assert(Same<void* const, void*>);
   static_assert(Same<void**, const void**>);
   static_assert(Same<void** const, const void* const*>);
   static_assert(Same<void const* const* const, void**>);
   static_assert(Same<void const* const* const, void**, void** const, const void** const>);

   static_assert(not Same<int*, const float*>);
   static_assert(not Same<int* const, const float*>);
   static_assert(not Same<int* const, float*>);
   static_assert(not Same<int**, const float**>);
   static_assert(not Same<int** const, const float* const*>);
   static_assert(not Same<int const* const* const, float**>);
   static_assert(not Same<int const* const* const, float**, const int**>);

   WHEN("MetaData::IsSimilar is used to check similarity of incomplete types") {
      DMeta meta = MetaDataOf<IncompleteType*>();

      REQUIRE(meta.IsSame(MetaDataOf<IncompleteType*>()));
      REQUIRE(meta.IsSame(MetaDataOf<IncompleteType const*>()));
      REQUIRE(meta.IsSame(MetaDataOf<IncompleteType const* const>()));
   }

   WHEN("MetaData::IsSimilar is used to check similarity of complete types") {
      Sequence<N1::EmptyType, N1::Create, N2::EmptyType, N3::emptytype> sequence;
      DMeta voidp_meta = MetaDataOf<void*>();
      DMeta voidcp_meta = MetaDataOf<void const*>();
      DMeta voidcpc_meta = MetaDataOf<void const* const>();

      for (unsigned i = 0; i < sequence.count; ++i) {
         // Compare horizontally                                        
         REQUIRE      (sequence.meta   [i].IsSame(sequence.meta[i]));
         REQUIRE      (sequence.meta   [i].IsSame(sequence.metac[i]));
         REQUIRE      (sequence.meta   [i].IsSame(sequence.metav[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsSame(sequence.metap[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsSame(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsSame(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsSame(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsSame(voidp_meta));
         REQUIRE_FALSE(sequence.meta   [i].IsSame(voidcp_meta));
         REQUIRE_FALSE(sequence.meta   [i].IsSame(voidcpc_meta));
                                       
         REQUIRE      (sequence.metac  [i].IsSame(sequence.meta[i]));
         REQUIRE      (sequence.metac  [i].IsSame(sequence.metac[i]));
         REQUIRE      (sequence.metac  [i].IsSame(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsSame(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsSame(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsSame(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsSame(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsSame(voidp_meta));
         REQUIRE_FALSE(sequence.metac  [i].IsSame(voidcp_meta));
         REQUIRE_FALSE(sequence.metac  [i].IsSame(voidcpc_meta));

         REQUIRE      (sequence.metav  [i].IsSame(sequence.meta[i]));
         REQUIRE      (sequence.metav  [i].IsSame(sequence.metac[i]));
         REQUIRE      (sequence.metav  [i].IsSame(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsSame(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsSame(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsSame(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsSame(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsSame(voidp_meta));
         REQUIRE_FALSE(sequence.metav  [i].IsSame(voidcp_meta));
         REQUIRE_FALSE(sequence.metav  [i].IsSame(voidcpc_meta));
                                     
         REQUIRE_FALSE(sequence.metap  [i].IsSame(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metap  [i].IsSame(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metap  [i].IsSame(sequence.metav[i]));
         REQUIRE      (sequence.metap  [i].IsSame(sequence.metap[i]));
         REQUIRE      (sequence.metap  [i].IsSame(sequence.metacp[i]));
         REQUIRE      (sequence.metap  [i].IsSame(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metap  [i].IsSame(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metap  [i].IsSame(voidp_meta));
         REQUIRE_FALSE(sequence.metap  [i].IsSame(voidcp_meta));
         REQUIRE_FALSE(sequence.metap  [i].IsSame(voidcpc_meta));
                                       
         REQUIRE_FALSE(sequence.metacp [i].IsSame(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacp [i].IsSame(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacp [i].IsSame(sequence.metav[i]));
         REQUIRE      (sequence.metacp [i].IsSame(sequence.metap[i]));
         REQUIRE      (sequence.metacp [i].IsSame(sequence.metacp[i]));
         REQUIRE      (sequence.metacp [i].IsSame(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacp [i].IsSame(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metacp [i].IsSame(voidp_meta));
         REQUIRE_FALSE(sequence.metacp [i].IsSame(voidcp_meta));
         REQUIRE_FALSE(sequence.metacp [i].IsSame(voidcpc_meta));

         REQUIRE_FALSE(sequence.metacpc[i].IsSame(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpc[i].IsSame(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpc[i].IsSame(sequence.metav[i]));
         REQUIRE      (sequence.metacpc[i].IsSame(sequence.metap[i]));
         REQUIRE      (sequence.metacpc[i].IsSame(sequence.metacp[i]));
         REQUIRE      (sequence.metacpc[i].IsSame(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacpc[i].IsSame(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metacpc[i].IsSame(voidp_meta));
         REQUIRE_FALSE(sequence.metacpc[i].IsSame(voidcp_meta));
         REQUIRE_FALSE(sequence.metacpc[i].IsSame(voidcpc_meta));
         
         REQUIRE_FALSE(sequence.metacpcp[i].IsSame(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsSame(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsSame(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsSame(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsSame(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsSame(sequence.metacpc[i]));
         REQUIRE      (sequence.metacpcp[i].IsSame(sequence.metacpcp[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsSame(voidp_meta));
         REQUIRE_FALSE(sequence.metacpcp[i].IsSame(voidcp_meta));
         REQUIRE_FALSE(sequence.metacpcp[i].IsSame(voidcpc_meta));
         
         for (unsigned j = 0; j < sequence.count; ++j) {
            if (i == j)
               continue;

            // Compare vertically                                       
            REQUIRE_FALSE(sequence.meta   [j].IsSame(sequence.meta[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsSame(sequence.metac[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsSame(sequence.metav[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsSame(sequence.metap[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsSame(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsSame(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsSame(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metac  [j].IsSame(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsSame(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsSame(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsSame(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsSame(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsSame(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsSame(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metav  [j].IsSame(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsSame(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsSame(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsSame(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsSame(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsSame(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsSame(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metap  [j].IsSame(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsSame(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsSame(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsSame(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsSame(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsSame(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsSame(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacp [j].IsSame(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsSame(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsSame(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsSame(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsSame(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsSame(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsSame(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpc[j].IsSame(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsSame(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsSame(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsSame(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsSame(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsSame(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsSame(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpcp[j].IsSame(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsSame(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsSame(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsSame(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsSame(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsSame(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsSame(sequence.metacpcp[i]));
         }
      }
   }
}

TEST_CASE("MetaData::IsExact") {
   WHEN("MetaData::IsExact is used to check similarity of complete types") {
      Sequence<N1::EmptyType, N1::Create, N2::EmptyType, N3::emptytype> sequence;

      for (unsigned i = 0; i < sequence.count; ++i) {
         // Compare horizontally                                        
         REQUIRE      (sequence.meta   [i].IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.meta   [i].IsExact(sequence.metacpcp[i]));
                                       
         REQUIRE_FALSE(sequence.metac  [i].IsExact(sequence.meta[i]));
         REQUIRE      (sequence.metac  [i].IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metac  [i].IsExact(sequence.metacpcp[i]));
                                       
         REQUIRE_FALSE(sequence.metav  [i].IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsExact(sequence.metac[i]));
         REQUIRE      (sequence.metav  [i].IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metav  [i].IsExact(sequence.metacpcp[i]));
                                       
         REQUIRE_FALSE(sequence.metap  [i].IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metap  [i].IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metap  [i].IsExact(sequence.metav[i]));
         REQUIRE      (sequence.metap  [i].IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metap  [i].IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metap  [i].IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metap  [i].IsExact(sequence.metacpcp[i]));
                                       
         REQUIRE_FALSE(sequence.metacp [i].IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacp [i].IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacp [i].IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacp [i].IsExact(sequence.metap[i]));
         REQUIRE      (sequence.metacp [i].IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metacp [i].IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacp [i].IsExact(sequence.metacpcp[i]));

         REQUIRE_FALSE(sequence.metacpc[i].IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpc[i].IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpc[i].IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacpc[i].IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metacpc[i].IsExact(sequence.metacp[i]));
         REQUIRE      (sequence.metacpc[i].IsExact(sequence.metacpc[i]));
         REQUIRE_FALSE(sequence.metacpc[i].IsExact(sequence.metacpcp[i]));
         
         REQUIRE_FALSE(sequence.metacpcp[i].IsExact(sequence.meta[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsExact(sequence.metac[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsExact(sequence.metav[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsExact(sequence.metap[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsExact(sequence.metacp[i]));
         REQUIRE_FALSE(sequence.metacpcp[i].IsExact(sequence.metacpc[i]));
         REQUIRE      (sequence.metacpcp[i].IsExact(sequence.metacpcp[i]));
         
         for (unsigned j = 0; j < sequence.count; ++j) {
            if (i == j)
               continue;

            // Compare vertically                                       
            REQUIRE_FALSE(sequence.meta   [j].IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.meta   [j].IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metac  [j].IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metac  [j].IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metav  [j].IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metav  [j].IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metap  [j].IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metap  [j].IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacp [j].IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacp [j].IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpc[j].IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpc[j].IsExact(sequence.metacpcp[i]));

            REQUIRE_FALSE(sequence.metacpcp[j].IsExact(sequence.meta[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsExact(sequence.metac[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsExact(sequence.metav[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsExact(sequence.metap[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsExact(sequence.metacp[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsExact(sequence.metacpc[i]));
            REQUIRE_FALSE(sequence.metacpcp[j].IsExact(sequence.metacpcp[i]));
         }
      }
   }
}
