///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Main.hpp"


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
   template<class TYPE>
   struct Block {};
   class Many  : public Block<> {};
   class Trait : public Block<> {};
}

namespace Langulus::Traits
{
   class Tag : Anyness::Trait {
      LANGULUS(TRAIT) "Tag";
   };
}

/// An empty trivial type                                                     
class ImplicitlyConstructible {};

/// A simple aggregate type                                                   
struct AggregateType {
   int m1, m2, m3, m4;
   bool m5;
};

/// Explicitly deleted destructor                                             
class NonDestructible {
   ~NonDestructible() = delete;
};

/// Has an explicit destructor                                                
class Destructible {
public:
   char* someptr {};

   ~Destructible() {
      if (someptr)
         delete someptr;
   }
};

/// Default-constructible, but only privately                                 
class PrivatelyConstructible {
   LANGULUS(POD) false;
private:
   PrivatelyConstructible() = default;
   PrivatelyConstructible(const PrivatelyConstructible&) = default;
   PrivatelyConstructible(PrivatelyConstructible&&) = default;
};

/// Had no explicit intent constructors and assigners                         
/// Has only implicit refer & move constructors and assigners                 
class NonIntentConstructible {
public:
   NonIntentConstructible(CT::NoIntent auto&&) {}
};

/// Has explicit copy, move, refer, clone, abandon, disown constructors       
/// Has implicit refer & move constructors, too                               
/// Has no explicit semantic assigners, only implicit refer & move            
class PartiallyIntentConstructible {
public:
   template<template<class> class S, class T>
   PartiallyIntentConstructible(S<T>&&) requires CT::Intent<S<T>> {}
};

/// Has all intent constructors + implicit refer & move ones                  
/// Has no explicit intent assigners, only implicit refer & move ones         
/// Making constructor explicit makes sure, that no implicit intent assign    
/// happens                                                                   
class AllIntentConstructible {
public:
   LANGULUS(POD) false;
   explicit AllIntentConstructible(CT::Intent auto&&) {}
};

/// Has all intent constructors + implicit refer & move ones                  
/// Has no explicit intent assigners, only implicit refer & move ones         
/// Making constructor implicit also allows for intent assignments            
class AllIntentConstructibleImplicit {
public:
   LANGULUS(POD) false;
   AllIntentConstructibleImplicit(CT::Intent auto&&) {}
};

/// Has all semantic constructors and assigners + implicit refer & move ones  
class AllIntentConstructibleAndAssignable {
public:
   LANGULUS(POD) false;
   AllIntentConstructibleAndAssignable(CT::Intent auto&&) {}
   AllIntentConstructibleAndAssignable& operator = (CT::Intent auto&&) { return *this; }
};

/// Has explicit descriptor constructor, and implicit refer & move ones       
/// Has no explicit intent assigners, only implicit refer & move              
class DescriptorConstructible {
public:
   DescriptorConstructible(Describe) {}
};


enum class Pi {
   Number = 314
};

struct IncompleteType;

struct NotReflectableType {
   LANGULUS(ACT_AS) void;
};

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

      static bool ExecuteDefault(const Anyness::Many&, Verb&) {
         return true;
      }

      static bool ExecuteDefault(Anyness::Many&, Verb&) {
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

   enum Named {One, Two, Three};
   LANGULUS_NAMED_VALUES(One, Two, Three);
   LANGULUS(TYPED) Named;

   Named v = One;

   inline bool operator == (const ImplicitlyReflectedData&) const noexcept = default;
};

struct ConvertibleData : ImplicitlyReflectedData {
   int member {664};

   inline operator int() const noexcept {
      return member;
   }

   ConvertibleData() = default;
   explicit ConvertibleData(Pi)
      : member {314} {}

   LANGULUS_BASES(ImplicitlyReflectedData);
   LANGULUS_CONVERTS_TO(int);
   LANGULUS_CONVERTS_FROM(Pi);
   LANGULUS_NAMED_VALUES();
};

struct CheckingWhatConverterGetsInherited : ConvertibleData {
   LANGULUS(NAME) "CheckingWhatConverterGetsInherited";

   using ConvertibleData::ConvertibleData;
};

class alignas(128) ImplicitlyReflectedDataWithTraits : public ConvertibleData {
public:
   RTTI::Tag<bool, Traits::Tag> anotherMember {};
   int anotherMemberArray [12] {};
   int* sparseMember {};

   void Create(Flow::Verb&) const {
      //++member;
   }

   void Create(Flow::Verb&) {
      ++member;
   }

   using ConvertibleData::ConvertibleData;

   LANGULUS(NAME) "MyType";
   LANGULUS(INFO) "Info about MyType";
   LANGULUS(FILES) "txt, pdf";
   LANGULUS(VERSION_MAJOR) 2;
   LANGULUS(VERSION_MINOR) 1;
   LANGULUS(DEEP) true;
   LANGULUS(POD) true;
   LANGULUS(NULLIFIABLE) true;
   LANGULUS(POOL_TACTIC) PoolTactic::Size;
   LANGULUS(CONCRETE) ImplicitlyReflectedData;
   LANGULUS(ALLOCATION_PAGE) 250;
   LANGULUS(ABSTRACT) true;
   LANGULUS_BASES(ConvertibleData);
   LANGULUS_VERBS(Verbs::Create);

   using Self = ImplicitlyReflectedDataWithTraits;
   LANGULUS_MEMBERS(
      &Self::anotherMember,
      &Self::anotherMemberArray,
      &Self::sparseMember
   );
};

/// Doesn't have implicit copy/move, so it is abandon-makable by explicit move
/// but not abandon-assignable                                                
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
   LANGULUS(POOL_TACTIC) PoolTactic::Size;
   LANGULUS(ALLOCATION_PAGE) 250;

   using Self = Complex;
   LANGULUS_MEMBERS(
      &Self::member,
      &Self::anotherMember,
      &Self::anotherMemberArray,
      &Self::sparseMember
   );

   Complex(const Complex& s)
      : member(s.member) {}
   Complex(Complex&& s)
      : member(s.member) {}
   Complex(int stuff)
      : member(stuff) {}

   ~Complex() {
      if (sparseMember)
         delete sparseMember;
   }
};

struct AnotherTypeWithSimilarilyNamedValues {
   enum Named {One = 501, Two, Three};
   LANGULUS_NAMED_VALUES(One, Two, Three);
   LANGULUS(NAME) "YetAnotherNamedType";

   int v = One;

   inline bool operator == (const AnotherTypeWithSimilarilyNamedValues&) const noexcept = default;
};

struct CheckingWhatGetsInherited : ImplicitlyReflectedDataWithTraits {
   LANGULUS(NAME) "CheckingWhatGetsInherited";

   using ImplicitlyReflectedDataWithTraits::ImplicitlyReflectedDataWithTraits;
};

class ContainsComplex {
   Complex mData;
};

/// A complex aggregate type                                                  
struct AggregateTypeComplex {
   int m1, m2, m3, m4;
   bool m5;
   Complex mData;
};

class ForcefullyPod {
   LANGULUS(POD) true;
   Complex mData;
};

struct Type {};

struct TypeErasedContainer {
   LANGULUS(TYPED) void;
};

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

enum class TypedEnum : int16_t {
   E1, E2, E3
};

/// Built-in abstract type                                                    
struct ImpureVirtual {
   virtual ~ImpureVirtual() {}
};

/// Built-in abstract type                                                    
struct PureVirtual {
   PureVirtual() = delete;
   virtual ~PureVirtual() {}

   PureVirtual(void*) {}

   virtual auto PureVirtualMethod() -> Offset = 0;
};

/// Proper type, reflected as abstract                                        
struct ForcedAbstract {
   LANGULUS(ABSTRACT) true;
};

struct VirtuallyDerived : virtual ImpureVirtual {

};


struct ReferConstructibleButNotAssignable {
   int m;
   ReferConstructibleButNotAssignable(Referred<ReferConstructibleButNotAssignable>&& a) : m {a->m} {}
   ReferConstructibleButNotAssignable& operator = (const ReferConstructibleButNotAssignable&) = delete;
   ReferConstructibleButNotAssignable& operator = (ReferConstructibleButNotAssignable&&) = delete;
};

struct CopyConstructibleButNotAssignable {
   int m;
   CopyConstructibleButNotAssignable(Copied<CopyConstructibleButNotAssignable>&& a) : m {a->m} {}
   CopyConstructibleButNotAssignable& operator = (const CopyConstructibleButNotAssignable&) = delete;
   CopyConstructibleButNotAssignable& operator = (CopyConstructibleButNotAssignable&&) = delete;
};

struct MoveConstructibleButNotAssignable {
   int m;
   MoveConstructibleButNotAssignable(Moved<MoveConstructibleButNotAssignable>&& a) : m {a->m} {}
   MoveConstructibleButNotAssignable& operator = (const MoveConstructibleButNotAssignable&) = delete;
   MoveConstructibleButNotAssignable& operator = (MoveConstructibleButNotAssignable&&) = delete;
};

struct AbandonConstructibleButNotAssignable {
   int m;
   AbandonConstructibleButNotAssignable(Abandoned<AbandonConstructibleButNotAssignable>&& a) : m {a->m} {}
   AbandonConstructibleButNotAssignable& operator = (const AbandonConstructibleButNotAssignable&) = delete;
   AbandonConstructibleButNotAssignable& operator = (AbandonConstructibleButNotAssignable&&) = delete;
};

struct DisownConstructibleButNotAssignable {
   int m;
   DisownConstructibleButNotAssignable(Disowned<DisownConstructibleButNotAssignable>&& a) : m {a->m} {}
   DisownConstructibleButNotAssignable& operator = (const DisownConstructibleButNotAssignable&) = delete;
   DisownConstructibleButNotAssignable& operator = (DisownConstructibleButNotAssignable&&) = delete;
};

struct CloneConstructibleButNotAssignable {
   int m;
   CloneConstructibleButNotAssignable(Cloned<CloneConstructibleButNotAssignable>&& a) : m {a->m} {}
   CloneConstructibleButNotAssignable& operator = (const CloneConstructibleButNotAssignable&) = delete;
   CloneConstructibleButNotAssignable& operator = (CloneConstructibleButNotAssignable&&) = delete;
};