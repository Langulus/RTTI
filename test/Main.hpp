///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <RTTI/Meta.hpp>
#include <cstdint>
#include <cstddef>
#include <vector>

namespace Langulus::Flow
{
   struct Verb {};
   class Construct {};
   class Constructconst {};
   class constConstructconst {};
   class constconst {};
}

namespace Langulus::Anyness
{
   class Block {};
   class Trait : public Block {};
}

namespace Langulus::Traits
{
   class Tag : Anyness::Trait {};
}

using namespace Langulus;
using namespace Langulus::RTTI;

//#define LANGULUS_STD_BENCHMARK

#define CATCH_CONFIG_ENABLE_BENCHMARKING

using uint = unsigned int;
template<class T>
using some = std::vector<T>;

enum class Pi {
   Number = 314
};

struct IncompleteType;

namespace One::Two::Three {
   struct TypeDeepIntoNamespaces;

   template<class T>
   struct TemplatedTypeDeepIntoNamespaces {
      enum VeryDeeplyTemplatedEnum { YesYouGotThatRight };

      template<class MORE>
      struct Nested;
   };

   template<class T>
   struct VeryComplexTemplate;
}

namespace Verbs
{

   ///                                                                        
   /// A testing verb, similar to the ones used in Langulus::Flow             
   ///                                                                        
   struct Create : public Flow::Verb {
      LANGULUS(POSITIVE_VERB) "Create";
      LANGULUS(NEGATIVE_VERB) "Destroy";
      LANGULUS(POSITIVE_OPERATOR) " + ";
      LANGULUS(NEGATIVE_OPERATOR) " - ";
      LANGULUS(PRECEDENCE) 5;
      LANGULUS(INFO)
         "Used for allocating new elements. "
         "If the type you're creating has	a producer, "
         "you need to execute the verb in a matching producer, "
         "or that producer will be created automatically for you, if possible";

      /// Check if the verb is available in a type, and with given arguments  
      ///   @return true if verb is available in T with arguments A...        
      template<CT::Data T, CT::Data... A>
      static constexpr bool AvailableFor() noexcept {
         if constexpr (sizeof...(A) == 0)
            return requires (T & t, Verb & v) { t.Create(v); };
         else
            return requires (T & t, Verb & v, A... a) { t.Create(v, a...); };
      }

      /// Get the verb functor for the given type and arguments               
      ///   @return the function, or nullptr if not available                 
      template<CT::Data T, CT::Data... A>
      static constexpr auto Of() noexcept {
         if constexpr (!Create::AvailableFor<T, A...>()) {
            return nullptr;
         }
         else if constexpr (CT::Constant<T>) {
            return [](const void* context, Flow::Verb& verb, A... args) {
               auto typedContext = static_cast<const T*>(context);
               typedContext->Create(verb, args...);
            };
         }
         else {
            return [](void* context, Flow::Verb& verb, A... args) {
               auto typedContext = static_cast<T*>(context);
               typedContext->Create(verb, args...);
            };
         }
      }

      template<CT::Data T>
      static bool ExecuteIn(T&, Verb&);

      static bool ExecuteDefault(const Anyness::Block&, Verb&) {
         return true;
      }

      static bool ExecuteDefault(Anyness::Block&, Verb&) {
         return false;
      }

      static bool ExecuteStateless(Verb&) {
         return false;
      }
   };

}

struct ImplicitlyReflectedData {
   LANGULUS(POD) true;
   LANGULUS(FILES) "ASE";

   enum Named {
      One, Two, Three
   };

   LANGULUS_NAMED_VALUES(Named) {
      {"One", One},
      {"Two", Two},
      {"Three", Three}
   };

   Named v {One};

   inline bool operator == (const ImplicitlyReflectedData&) const noexcept = default;
};

class alignas(128) ImplicitlyReflectedDataWithTraits : public ImplicitlyReflectedData {
public:
   int member {664};
   bool anotherMember {};
   int anotherMemberArray [12] {};
   int* sparseMember {};

   inline operator int() const noexcept {
      return member;
   }

   void Create(Flow::Verb&) const {
      //++member;
   }

   void Create(Flow::Verb&) {
      ++member;
   }

   LANGULUS(NAME) "MyType";
   LANGULUS(INFO) "Info about MyType";
   LANGULUS(FILES) "txt, pdf";
   LANGULUS(VERSION_MAJOR) 2;
   LANGULUS(VERSION_MINOR) 1;
   LANGULUS(DEEP) true;
   LANGULUS(POD) true;
   LANGULUS(NULLIFIABLE) true;
   IF_LANGULUS_MANAGED_MEMORY(LANGULUS(POOL_TACTIC) PoolTactic::Size);
   LANGULUS(CONCRETE) ImplicitlyReflectedData;
   LANGULUS(UNINSERTABLE) true;
   LANGULUS(ALLOCATION_PAGE) 250;
   LANGULUS(ABSTRACT) true;
   LANGULUS_BASES(ImplicitlyReflectedData);
   LANGULUS_VERBS(Verbs::Create);
   LANGULUS_CONVERSIONS(int);

   LANGULUS_PROPERTIES_START(ImplicitlyReflectedDataWithTraits)
      LANGULUS_PROPERTY(member),
      LANGULUS_PROPERTY_TRAIT(anotherMember, Tag),
      LANGULUS_PROPERTY(anotherMemberArray),
      LANGULUS_PROPERTY(sparseMember)
   LANGULUS_PROPERTIES_END();
};

class alignas(128) Complex {
public:
   int member;
   bool anotherMember {};
   int anotherMemberArray [12] {};
   int* sparseMember {};

   LANGULUS(NAME) "ComplexType";
   LANGULUS(INFO) "Info about ComplexType";
   LANGULUS(VERSION_MAJOR) 2;
   LANGULUS(VERSION_MINOR) 1;
   IF_LANGULUS_MANAGED_MEMORY(LANGULUS(POOL_TACTIC) PoolTactic::Size);
   LANGULUS(ALLOCATION_PAGE) 250;

   LANGULUS_PROPERTIES_START(Complex)
      LANGULUS_PROPERTY(member),
      LANGULUS_PROPERTY_TRAIT(anotherMember, Tag),
      LANGULUS_PROPERTY(anotherMemberArray),
      LANGULUS_PROPERTY(sparseMember)
   LANGULUS_PROPERTIES_END();

   Complex(int stuff) : member(stuff) {}
   ~Complex() {
      if (sparseMember)
         delete sparseMember;
   }
};

class ContainsComplex {
   Complex mData;
};

struct Type {};

namespace N1 {
   struct Type {};
   struct Create {};
}

namespace N2 {
   struct Type {};
}

namespace N3 {
   struct type {};
}
