///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "Common.hpp"

using TypeDeepAlias = 
   One::Two::Three::TypeDeepIntoNamespaces;
using TemplatedAlias = 
   One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>;
using VeryComplexTemplatedAlias = 
   One::Two::Three::VeryComplexTemplate<
      One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>>;

class A; class B; class C; class D; class E; class F; class G; class H; class I;
class J; class K; class L; class M; class N; class O; class P; class Q; class R;
class S; class T; class U; class V; class W; class X; class Y; class Z; class _;

class a; class b; class c; class d; class e; class f; class g; class h; class i;
class j; class k; class l; class m; class n; class o; class p; class q; class r;
class s; class t; class u; class v; class w; class x; class y; class z;


namespace Langulus::RTTI::Inner
{
   template<class T>
   Token IsolateTypenameAtRuntime() {
      // Weirdly enough, the letter L caused problems... so here we     
      // have a non-constexpr equivalent to RTTI::Inner::IsolateTypename
      // to debug for problems                                          
      auto matcher = [](const char* lhs, const char* rhs) -> bool {
         while (*lhs and *rhs and *(lhs++) == *(rhs++));
         return *rhs == 0;
      };

      volatile auto name = RTTI::Inner::WrappedTypeName<T>();
      Logger::Info("IsolateTypenameAtRuntime::name: ", name);
      volatile int len = static_cast<int>(strlen(name));
      Logger::Info("IsolateTypenameAtRuntime::len: ", len);
      volatile auto helper_name = RTTI::Inner::WrappedTypeName<RTTI::Inner::Oddly_Specific_TypeASFNWEAFNOLAWFNWAFK>();
      Logger::Info("IsolateTypenameAtRuntime::helper_name: ", helper_name);
      volatile int helper_len = static_cast<int>(strlen(helper_name));
      Logger::Info("IsolateTypenameAtRuntime::helper_len: ", helper_len);
      REQUIRE(len > 0);
      REQUIRE(helper_len > 61);

      int left = 0;
      while (left < helper_len and left < len and helper_name[left] == name[left]
      and not matcher(helper_name + left, "Langulus::RTTI::Inner::Oddly_Specific_TypeASFNWEAFNOLAWFNWAFK"))
         ++left;
      Logger::Info("IsolateTypenameAtRuntime::left: ", left);

      int right = 1;
      while (right + 61 <= helper_len and right <= len
      and helper_name[helper_len - right] == name[len - right]
      and not matcher(helper_name + (helper_len - right - 61), "Langulus::RTTI::Inner::Oddly_Specific_TypeASFNWEAFNOLAWFNWAFK"))
         ++right;
      Logger::Info("IsolateTypenameAtRuntime::right: ", right);

      REQUIRE(len - right > left);
      auto ret = Token(name + left, name + len - right);
      Logger::Info("IsolateTypenameAtRuntime::ret: ", ret);
      return ret;
   }
}

SCENARIO("NameOf", "[nameof]") {
   WHEN("Taken the name of uint16_t") {
      auto name = NameOf<uint16_t>();
      REQUIRE(name == "uint16");
   }
      
   WHEN("Taken the name of uint16_t&") {
      auto name = NameOf<uint16_t&>();
      REQUIRE(name == "uint16");
   }

   WHEN("Taken the name of const uint16_t") {
      auto name = NameOf<const uint16_t>();
      REQUIRE(name == "const uint16");
   }

   WHEN("Taken the name of const uint16_t*") {
      auto name = NameOf<const uint16_t*>();
      REQUIRE(name == "const uint16*");
   }

   WHEN("Taken the name of const uint16_t**") {
      auto name = NameOf<const uint16_t**>();
      REQUIRE(name == "const uint16**");
   }
      
   WHEN("Taken the name of const uint16_t* const*") {
      auto name = NameOf<const uint16_t* const *>();
      REQUIRE(name == "const uint16* const*");
   }
      
   WHEN("Taken the name of const uint16_t* const* const") {
      auto name = NameOf<const uint16_t* const * const>();
      REQUIRE(name == "const uint16* const* const");
   }

   WHEN("Taken the name of uint16*") {
      auto name = NameOf<uint16_t*>();
      REQUIRE(name == "uint16*");
   }

   WHEN("Taken the name of enum Pi") {
      auto name = NameOf<Pi::Number>();
      REQUIRE(name == "Pi::Number");
   }

   #define DEFINE_SINGLE_LETTER_NAMEOF_TEST(WHAT) \
      WHEN("Taken the name of class " #WHAT " (corner cases)") { \
         auto name = NameOf<::WHAT>(); \
         Logger::Special("At runtime: ", std::string(RTTI::Inner::IsolateTypenameAtRuntime<::WHAT>())); \
         Logger::Special("Wrapped:  ",   std::string(RTTI::Inner::WrappedTypeName<::WHAT>())); \
         Logger::Special("Isolated: ",   std::string(RTTI::Inner::IsolateTypename<::WHAT>())); \
         REQUIRE(name == #WHAT); \
      }

   DEFINE_SINGLE_LETTER_NAMEOF_TEST(A)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(B)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(C)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(D)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(E)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(F)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(G)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(H)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(I)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(J)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(K)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(L)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(M)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(N)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(O)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(P)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(Q)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(R)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(S)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(T)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(U)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(V)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(W)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(X)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(Y)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(Z)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(_)

   DEFINE_SINGLE_LETTER_NAMEOF_TEST(a)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(b)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(c)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(d)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(e)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(f)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(g)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(h)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(i)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(j)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(k)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(l)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(m)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(n)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(o)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(p)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(q)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(r)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(s)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(t)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(u)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(v)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(w)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(x)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(y)
   DEFINE_SINGLE_LETTER_NAMEOF_TEST(z)

   WHEN("Taken the name of IncompleteType") {
      auto name = NameOf<IncompleteType>();
      REQUIRE(name == "IncompleteType");
   }

   WHEN("Taken the name of IncompleteType*") {
      auto name = NameOf<IncompleteType*>();
      REQUIRE(name == "IncompleteType*");
   }

   WHEN("Taken the name of const IncompleteType*") {
      auto name = NameOf<const IncompleteType*>();
      REQUIRE(name == "const IncompleteType*");
   }

   WHEN("Taken the name of const IncompleteType") {
      auto name = NameOf<const IncompleteType>();
      REQUIRE(name == "const IncompleteType");
   }
   
   WHEN("Taken the name of One::Two::Three::TypeDeepIntoNamespaces") {
      auto name = NameOf<One::Two::Three::TypeDeepIntoNamespaces>();
      REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces");
   }

   WHEN("Taken the name of TypeDeepAlias") {
      auto name = NameOf<TypeDeepAlias>();
      REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces");
   }

   WHEN("Taken the name of TypeDeepAlias*") {
      auto name = NameOf<TypeDeepAlias*>();
      REQUIRE(name == "One::Two::Three::TypeDeepIntoNamespaces*");
   }

   WHEN("Taken the name of const TypeDeepAlias*") {
      auto name = NameOf<const TypeDeepAlias*>();
      REQUIRE(name == "const One::Two::Three::TypeDeepIntoNamespaces*");
   }

   WHEN("Taken the name of const TypeDeepAlias*") {
      auto name = NameOf<const TypeDeepAlias>();
      REQUIRE(name == "const One::Two::Three::TypeDeepIntoNamespaces");
   }

   WHEN("Taken the name of One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>") {
      auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>>();
      REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>");
   }

   WHEN("Taken the name of Enum One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>::VeryDeeplyTemplatedEnum::YesYouGotThatRight") {
      auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>::VeryDeeplyTemplatedEnum::YesYouGotThatRight>();
      REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<char>::VeryDeeplyTemplatedEnum::YesYouGotThatRight");
   }

   WHEN("Taken the name of Enum One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::VeryDeeplyTemplatedEnum::YesYouGotThatRight") {
      auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::VeryDeeplyTemplatedEnum::YesYouGotThatRight>();
      REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>::VeryDeeplyTemplatedEnum::YesYouGotThatRight");
   }

   WHEN("Taken the name of Type One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::Nested<uint16_t>") {
      auto name = NameOf<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>::Nested<uint16_t>>();
      REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>::Nested<uint16>");
   }

   WHEN("Taken the name of TemplatedAlias") {
      auto name = NameOf<TemplatedAlias>();
      REQUIRE(name == "One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>");
   }

   WHEN("Taken the name of Type One::Two::Three::VeryComplexTemplate<TemplatedAlias>") {
      auto name = NameOf<One::Two::Three::VeryComplexTemplate<TemplatedAlias>>();
      REQUIRE(name == "One::Two::Three::VeryComplexTemplate<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>>");
   }

   WHEN("Taken the name of VeryComplexTemplatedAlias") {
      auto name = NameOf<VeryComplexTemplatedAlias>();
      REQUIRE(name == "One::Two::Three::VeryComplexTemplate<One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16>>");
   }

   WHEN("Taken the name of Langulus::Flow::Construct") {
      auto name = NameOf<Langulus::Flow::Construct>();
      REQUIRE(name == "Flow::Construct");
   }

   WHEN("Taken the name of Langulus::Flow::Constructconst") {
      auto name = NameOf<Langulus::Flow::Constructconst>();
      REQUIRE(name == "Flow::Constructconst");
   }

   WHEN("Taken the name of Langulus::Flow::constConstructconst") {
      auto name = NameOf<Langulus::Flow::constConstructconst>();
      REQUIRE(name == "Flow::constConstructconst");
   }

   WHEN("Taken the name of Langulus::Flow::constconst") {
      auto name = NameOf<Langulus::Flow::constconst>();
      REQUIRE(name == "Flow::constconst");
   }

   using Signature = void(*)(void*);

   WHEN("Taken the name of a function signature") {
      auto name = NameOf<Signature>();
      REQUIRE(name == "Function<void(void*)>*");
   }
}


struct TypeWithSuffix { LANGULUS(SUFFIX) "yeah"; };
struct TypeWithoutSuffix {};

SCENARIO("SuffixOf", "[metadata]") {
   WHEN("Generating a suffix for uint8_t") {
      constexpr auto token = SuffixOf<uint8_t>();
      REQUIRE(token == "u8");
   }

   WHEN("Generating a suffix for uint16_t") {
      constexpr auto token = SuffixOf<uint16_t>();
      REQUIRE(token == "u16");
   }

   WHEN("Generating a suffix for uint32_t") {
      constexpr auto token = SuffixOf<uint32_t>();

      if constexpr (CT::Same<uint32_t, unsigned int>)
         REQUIRE(token == "u");
      else
         REQUIRE(token == "u32");
   }
   
   WHEN("Generating a suffix for uint64_t") {
      constexpr auto token = SuffixOf<uint64_t>();

      if constexpr (CT::Same<uint64_t, unsigned int>)
         REQUIRE(token == "u");
      else
         REQUIRE(token == "u64");
   }

   WHEN("Generating a suffix for int8_t") {
      constexpr auto token = SuffixOf<int8_t>();
      REQUIRE(token == "i8");
   }

   WHEN("Generating a suffix for int16_t") {
      constexpr auto token = SuffixOf<int16_t>();
      REQUIRE(token == "i16");
   }

   WHEN("Generating a suffix for int32_t") {
      constexpr auto token = SuffixOf<int32_t>();

      if constexpr (CT::Same<int32_t, signed int>)
         REQUIRE(token == "i");
      else
         REQUIRE(token == "i32");
   }
   
   WHEN("Generating a suffix for int64_t") {
      constexpr auto token = SuffixOf<int64_t>();

      if constexpr (CT::Same<int64_t, signed int>)
         REQUIRE(token == "i");
      else
         REQUIRE(token == "i64");
   }

   WHEN("Generating a suffix for float") {
      constexpr auto token = SuffixOf<float>();

      if constexpr (CT::Same<float, Real>)
         REQUIRE(token == "");
      else
         REQUIRE(token == "f");
   }

   WHEN("Generating a suffix for double") {
      constexpr auto token = SuffixOf<double>();

      if constexpr (CT::Same<double, Real>)
         REQUIRE(token == "");
      else
         REQUIRE(token == "d");
   }

   WHEN("Generating a suffix for bool") {
      constexpr auto token = SuffixOf<bool>();
      REQUIRE(token == "b");
   }

   WHEN("Generating a suffix for a type with LANGULUS(SUFFIX)") {
      constexpr auto token = SuffixOf<TypeWithSuffix>();
      REQUIRE(token == "yeah");
   }

   WHEN("Generating a suffix for a type without LANGULUS(SUFFIX)") {
      constexpr auto token = SuffixOf<TypeWithoutSuffix>();
      REQUIRE(token == "");
   }
}