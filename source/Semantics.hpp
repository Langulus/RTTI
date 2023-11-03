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
#include "Byte.hpp"
#include "Assumptions.hpp"


namespace Langulus
{
   
   /// A namespace dedicated to abstract entities                             
   namespace A
   {

      /// An abstract semantic                                                
      struct Semantic {
         LANGULUS(ABSTRACT) true;
         LANGULUS(UNINSERTABLE) true;
         LANGULUS(UNALLOCATABLE) true;
      };

      /// An abstract shallow semantic                                        
      struct ShallowSemantic : Semantic {
         static constexpr bool Shallow = true;
      };

      /// An abstract deep semantic                                           
      struct DeepSemantic : Semantic {
         static constexpr bool Shallow = false;
      };

      /// An abstract shallow-copy semantic                                   
      struct Copied : ShallowSemantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = false;
      };

      /// An abstract shallow-move semantic                                   
      struct Moved : ShallowSemantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = true;
      };

      /// An abstract shallow-abandon-move semantic                           
      struct Abandoned : ShallowSemantic {
         static constexpr bool Keep = false;
         static constexpr bool Move = true;
      };

      /// An abstract shallow-disowned-copy semantic                          
      struct Disowned : ShallowSemantic {
         static constexpr bool Keep = false;
         static constexpr bool Move = false;
      };

      /// An abstract clone semantic (a.k.a. deep-copy semantic)              
      struct Cloned : DeepSemantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = false;
      };

   } // namespace Langulus::A

   struct Describe;

   namespace CT
   {

      /// Checks if a type is a semantic                                      
      template<class... T>
      concept Semantic = ((Decayed<T> and DerivedFrom<T, A::Semantic>) and ...);

      /// Checks if a type is a shallow semantic                              
      template<class... T>
      concept ShallowSemantic = ((Decayed<T> and DerivedFrom<T, A::ShallowSemantic>) and ...);

      /// Checks if a type is a deep semantic                                 
      template<class... T>
      concept DeepSemantic = ((Decayed<T> and DerivedFrom<T, A::DeepSemantic>) and ...);

      /// Checks if a type is not a semantic                                  
      template<class... T>
      concept NotSemantic = ((not Semantic<T> and not CT::Same<T, Describe>) and ...);

      /// Check if a type is a shallow-copy semantic                          
      template<class... T>
      concept Copied = ((Decayed<T> and DerivedFrom<T, A::Copied>) and ...);

      /// Check if a type is a shallow-move semantic                          
      template<class... T>
      concept Moved = ((Decayed<T> and DerivedFrom<T, A::Moved>) and ...);

      /// Check if a type is a shallow-abandon-move semantic                  
      template<class... T>
      concept Abandoned = ((Decayed<T> and DerivedFrom<T, A::Abandoned>) and ...);

      /// Check if a type is shallow-disowned-copy                            
      template<class... T>
      concept Disowned = ((Decayed<T> and DerivedFrom<T, A::Disowned>) and ...);

      /// Check if a type is clone (deep-copy) semantic                       
      template<class... T>
      concept Cloned = ((Decayed<T> and DerivedFrom<T, A::Cloned>) and ...);

   } // namespace Langulus::CT

   
   ///                                                                        
   /// Copied value intermediate type, use in constructors and assignments    
   /// to shallow-copy container explicitly                                   
   ///   @tparam T - the type to copy                                         
   template<class T>
   struct Copied : A::Copied {
   private:
      const T& mValue;

   public:
      LANGULUS(TYPED) T;

      Copied() = delete;
      Copied(const Copied&) = delete;
      explicit constexpr Copied(Copied&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Copied(const T& value) noexcept
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as copied                                                   
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Copied<ALT_T> {mValue};
      }

      /// Copy something else                                                 
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(const auto& value) noexcept {
         return Copied<Decvq<Deref<decltype(value)>>> {value};
      }

      template<class ALT_T>
      using Nested = Copied<ALT_T>;

      LANGULUS(INLINED)
      const T& operator *  () const noexcept { return mValue; }

      LANGULUS(INLINED)
      const T* operator -> () const noexcept { return &mValue; }

      /// Implicitly collapse the semantic, when applying it to fundamentals  
      /// This way this wrapper is seamlessly integrated with the standard    
      /// C++20 semantics                                                     
      LANGULUS(INLINED)
      constexpr operator const T& () const noexcept {
         return mValue;
      }
   };
   
   /// Copy a value                                                           
   NOD() LANGULUS(INLINED) 
   constexpr auto Copy(const CT::NotSemantic auto& value) noexcept {
      return Copied<Decvq<Deref<decltype(value)>>> {value};
   }
   

   ///                                                                        
   /// Moved value intermediate type, use in constructors and assignments     
   /// to move data explicitly                                                
   ///   @tparam T - the type to move                                         
   template<class T>
   struct Moved : A::Moved {
   protected:
      T&& mValue;

   public:
      LANGULUS(TYPED) T;

      static_assert(CT::Mutable<T>,
         "T must be mutable in order to be moved");

      Moved() = delete;
      Moved(const Moved&) = delete;
      explicit constexpr Moved(Moved&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Moved(T& value) noexcept
         : mValue {::std::move(value)} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }

      LANGULUS(INLINED)
      explicit constexpr Moved(T&& value) noexcept
         : mValue {::std::forward<T>(value)} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }

      /// Forward as moved                                                    
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Moved<ALT_T> {::std::forward<ALT_T>(mValue)};
      }

      /// Move something else                                                 
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto& value) noexcept {
         return Moved<Decvq<Deref<decltype(value)>>> {::std::move(value)};
      }

      /// Move something else                                                 
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto&& value) noexcept {
         using ALT_T = Decvq<Deref<decltype(value)>>;
         return Moved<ALT_T> {::std::forward<ALT_T>(value)};
      }

      template<class ALT_T>
      using Nested = decltype(Nest(Fake<ALT_T>()));

      LANGULUS(INLINED)
      T& operator *  () const noexcept { return mValue; }

      LANGULUS(INLINED)
      T* operator -> () const noexcept { return &mValue; }

      /// Implicitly collapse the semantic, when applying it to fundamentals  
      /// This way this wrapper is seamlessly integrated with the standard    
      /// C++20 semantics                                                     
      LANGULUS(INLINED)
      constexpr operator T&& () const noexcept {
         return ::std::forward<T>(mValue);
      }
   };
   
   /// Move a value                                                           
   NOD() LANGULUS(INLINED)
   constexpr auto Move(CT::NotSemantic auto&& value) noexcept {
      using T = Decvq<Deref<decltype(value)>>;
      return Moved<T> {::std::forward<T>(value)};
   }

   /// Move a value                                                           
   NOD() LANGULUS(INLINED)
   constexpr auto Move(CT::NotSemantic auto& value) noexcept {
      using T = Decvq<Deref<decltype(value)>>;
      return Moved<T> {::std::move(value)};
   }


   ///                                                                        
   /// Abandoned value intermediate type, can be used in constructors and     
   /// assignments to provide a guarantee, that the value shall not be used   
   /// after that function, so we can save up on resetting it fully           
   /// For example, you can construct an Any with an abandoned Any, which is  
   /// same as move-construction, but the abandoned Any shall have only its   
   /// mEntry reset, instead of the entire container                          
   ///   @tparam T - the type to abandon                                      
   template<class T>
   struct Abandoned : A::Abandoned {
   protected:
      T&& mValue;

   public:
      LANGULUS(TYPED) T;

      static_assert(CT::Mutable<T>,
         "T must be mutable in order to be abandoned");

      Abandoned() = delete;
      Abandoned(const Abandoned&) = delete;
      explicit constexpr Abandoned(Abandoned&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Abandoned(T& value) noexcept
         : mValue {::std::move(value)} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      LANGULUS(INLINED)
      explicit constexpr Abandoned(T&& value) noexcept
         : mValue {::std::forward<T>(value)} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as abandoned, never collapse                                
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Abandoned<ALT_T> {::std::forward<ALT_T>(mValue)};
      }

      /// Abandon something else                                              
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto& value) noexcept {
         using ALT_T = Decvq<Deref<decltype(value)>>;
         return Abandoned<ALT_T> {::std::move(value)};
      }

      /// Abandon something else                                              
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto&& value) noexcept {
         using ALT_T = Decvq<Deref<decltype(value)>>;
         return Abandoned<ALT_T> {::std::forward<ALT_T>(value)};
      }

      template<class ALT_T>
      using Nested = decltype(Nest(Fake<ALT_T>()));

      LANGULUS(INLINED)
      T& operator *  () const noexcept { return mValue; }

      LANGULUS(INLINED)
      T* operator -> () const noexcept { return &mValue; }

      /// Implicitly collapse the semantic, when applying it to trivially     
      /// destructible types, since abandoning them is same as moving         
      LANGULUS(INLINED)
      constexpr operator const T& () const noexcept requires (not CT::Inner::Destroyable<T>) {
         return mValue;
      }
   };
   
   /// Abandon a value                                                        
   /// Same as Move, but resets only mandatory data inside source after move  
   /// essentially saving up on a couple of instructions                      
   NOD() LANGULUS(INLINED)
   constexpr auto Abandon(CT::NotSemantic auto&& value) noexcept {
      using T = Decvq<Deref<decltype(value)>>;
      return Abandoned<T> {::std::forward<T>(value)};
   }

   /// Abandon a value                                                        
   /// Same as Move, but resets only mandatory data inside source after move  
   /// essentially saving up on a couple of instructions                      
   NOD() LANGULUS(INLINED)
   constexpr auto Abandon(CT::NotSemantic auto& value) noexcept {
      using T = Decvq<Deref<decltype(value)>>;
      return Abandoned<T> {::std::move(value)};
   }


   ///                                                                        
   /// Disowned value intermediate type, use in constructors and assignments  
   /// to copy container without gaining ownership                            
   ///   @tparam T - the type to disown                                       
   template<class T>
   struct Disowned : A::Disowned {
   protected:
      const T& mValue;

   public:
      LANGULUS(TYPED) T;

      Disowned() = delete;
      Disowned(const Disowned&) = delete;
      explicit constexpr Disowned(Disowned&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Disowned(const T& value) noexcept
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as disowned, never collapse                                 
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Disowned<ALT_T> {mValue};
      }

      /// Disown something else                                               
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(const auto& value) noexcept {
         return Disowned<Decvq<Deref<decltype(value)>>> {value};
      }

      template<class ALT_T>
      using Nested = Disowned<ALT_T>;

      LANGULUS(INLINED)
      const T& operator *  () const noexcept { return mValue; }

      LANGULUS(INLINED)
      const T* operator -> () const noexcept { return &mValue; }

      /// Implicitly collapse the semantic, when applying it to PODs,         
      /// since they never have ownership                                     
      LANGULUS(INLINED)
      constexpr operator const T& () const noexcept requires CT::Inner::POD<T> {
         return mValue;
      }
   };
   
   /// Disown a value                                                         
   /// Same as a shallow-copy, but never references, saving some instructions 
   NOD() LANGULUS(INLINED)
   constexpr auto Disown(const CT::NotSemantic auto& value) noexcept {
      return Disowned<Decvq<Deref<decltype(value)>>> {value};
   }
   

   ///                                                                        
   /// Cloned value intermediate type, use in constructors and assignments    
   /// to clone container, doing a deep copy instead of default shallow copy  
   ///   @tparam T - the type to clone                                        
   template<class T>
   struct Cloned : A::Cloned {
   protected:
      const T& mValue;

   public:
      LANGULUS(TYPED) T;

      Cloned() = delete;
      Cloned(const Cloned&) = delete;
      explicit constexpr Cloned(Cloned&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Cloned(const T& value) noexcept
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as cloned, bever collapse                                   
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Cloned<ALT_T> {mValue};
      }

      /// Clone something else                                                
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(const auto& value) noexcept {
         return Cloned<Decvq<Deref<decltype(value)>>> {value};
      }

      template<class ALT_T>
      using Nested = Cloned<ALT_T>;
      
      LANGULUS(INLINED)
      const T& operator *  () const noexcept { return mValue; }

      LANGULUS(INLINED)
      const T* operator -> () const noexcept { return &mValue; }

      /// Implicitly collapse the semantic, when applying it to PODs,         
      /// since they are always cloned upon copy                              
      LANGULUS(INLINED)
      constexpr operator const T& () const noexcept requires CT::Inner::POD<T> {
         return mValue;
      }
   };
   
   /// Clone a value                                                          
   /// Does a deep-copy                                                       
   NOD() LANGULUS(INLINED)
   constexpr auto Clone(const CT::NotSemantic auto& value) noexcept {
      return Cloned<Decvq<Deref<decltype(value)>>> {value};
   }
      

   ///                                                                        
   /// Descriptor intermediate type, use in constructors to enable descriptor 
   /// construction. The inner type is always Neat                            
   struct Describe {
   protected:
      const Anyness::Neat& mValue;

   public:
      Describe() = delete;
      Describe(const Describe&) = delete;
      explicit constexpr Describe(Describe&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Describe(const Anyness::Neat& value) noexcept
         : mValue {value} {}
      
      LANGULUS(INLINED)
      const Anyness::Neat& operator *  () const noexcept { return mValue; }

      LANGULUS(INLINED)
      const Anyness::Neat* operator -> () const noexcept { return &mValue; }
   };


   /// Create an element on the heap, using the provided semantic, by using   
   /// a placement new variant                                                
   ///   @attention assumes placement pointer is valid                        
   ///   @param placement - where to place the new instance                   
   ///   @param value - the constructor arguments and the semantic            
   ///   @return the instance on the heap                                     
   template<bool FAKE = false, template<class> class S, CT::NotSemantic T>
   requires CT::Semantic<S<T>>
   LANGULUS(INLINED)
   auto SemanticNew(void* placement, S<T>&& value) {
      LANGULUS_ASSUME(DevAssumes, placement, "Invalid placement pointer");

      if constexpr (CT::Inner::Abstract<T>) {
         // Can't instantiate abstract type                             
         if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't instantiate abstract type");
      }
      else if constexpr (S<T>::Move) {
         if constexpr (not S<T>::Keep) {
            // Abandon                                                  
            if constexpr (requires { new T (Abandon(*value)); })
               return new (placement) T (Abandon(*value));
            else if constexpr (not CT::Inner::Destroyable<T>) {
               // If type is not destroyable (like fundamentals), then  
               // it is always acceptable to abandon them - just use    
               // the standard move-semantics                           
               if constexpr (requires { new T (::std::move(*value)); })
                  return new (placement) T (::std::move(*value));
               else if constexpr (FAKE)
                  return Inner::Unsupported {};
               else
                  LANGULUS_ERROR("Can't abandon/move-construct non-destructible type");
            }
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR(
                  "Can't abandon-construct destructible type"
                  " - explicit abandon-constructor is required");
         }
         else {
            // Move                                                     
            if constexpr (requires { new T (Move(*value)); })
               return new (placement) T (Move(*value));
            else if constexpr (requires { new T (::std::move(*value)); })
               return new (placement) T (::std::move(*value));
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't move-construct type");
         }
      }
      else if constexpr (not S<T>::Shallow) {
         // Clone                                                       
         if constexpr (requires { new T (Clone(*value)); })
            return new (placement) T (Clone(*value));
         else if constexpr (CT::Inner::POD<T>) {
            // If type is POD (like fundamentals, or trivials), then    
            // it is always acceptable to clone by memcpy               
            ::std::memcpy(placement, &(*value), sizeof(T));
            return reinterpret_cast<T*>(placement);
         }
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't clone-construct type");
      }
      else if constexpr (not S<T>::Keep) {
         // Disown                                                      
         if constexpr (requires { new T (Disown(*value)); })
            return new (placement) T (Disown(*value));
         else if constexpr (CT::Inner::POD<T>) {
            // If type is POD (like fundamentals, or trivials), then    
            // it is always acceptable to disown by memcpy, because     
            // PODs don't have ownership anyways                        
            ::std::memcpy(placement, &(*value), sizeof(T));
            return reinterpret_cast<T*>(placement);
         }
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't disown-construct type");
      }
      else {
         // Copy                                                        
         if constexpr (requires { new T (Copy(*value)); })
            return new (placement) T (Copy(*value));
         else if constexpr (requires { new T (*value); })
            return new (placement) T (*value);
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't copy-construct type");
      }
   }

   /// Assign new value to an instance of T, using the provided semantic      
   ///   @param lhs - left hand side (what are we assigning to)               
   ///   @param rhs - right hand side (what are we assigning)                 
   ///   @return whatever the assignment operator returns                     
   template<bool FAKE = false, template<class> class S, CT::NotSemantic T>
   requires CT::Semantic<S<T>>
   LANGULUS(INLINED)
   decltype(auto) SemanticAssign(T& lhs, S<T>&& rhs) {
      if constexpr (S<T>::Move) {
         if constexpr (not S<T>::Keep) {
            // Abandon                                                  
            if constexpr (requires(T& a) { a = Abandon(*rhs); })
               return (lhs = Abandon(*rhs));
            else if constexpr (not CT::Inner::Destroyable<T>) {
               // If type is not destroyable (like fundamentals), then  
               // it is always acceptable to abandon them - just use    
               // the standard move-semantics                           
               if constexpr (requires(T& a) { a = ::std::move(*rhs); })
                  return (lhs = ::std::move(*rhs));
               else if constexpr (FAKE)
                  return Inner::Unsupported {};
               else
                  LANGULUS_ERROR("Can't abandon/move-assign non-destructible type");
            }
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR(
                  "Can't abandon-assign destructible type"
                  " - explicit abandon-assigner is required");
         }
         else {
            // Move                                                     
            if constexpr (requires(T& a) { a = Move(*rhs); })
               return (lhs = Move(*rhs));
            else if constexpr (requires(T& a) { a = ::std::move(*rhs); })
               return (lhs = ::std::move(*rhs));
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't move-assign type");
         }
      }
      else {
         if constexpr (not S<T>::Shallow) {
            // Clone                                                    
            if constexpr (requires(T& a) { a = Clone(*rhs); })
               return (lhs = Clone(*rhs));
            else if constexpr (CT::Inner::POD<T>) {
               // If type is POD (like fundamentals, or trivials), then 
               // it is always acceptable to clone by memcpy            
               ::std::memcpy(&lhs, &(*rhs), sizeof(T));
               return (lhs);
            }
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't clone-assign type");
         }
         else if constexpr (not S<T>::Keep) {
            // Disown                                                   
            if constexpr (requires(T& a) { a = Disown(*rhs); })
               return (lhs = Disown(*rhs));
            else if constexpr (CT::Inner::POD<T>) {
               // If type is POD (like fundamentals, or trivials), then 
               // it is always acceptable to disown by memcpy, because  
               // PODs don't have ownership anyways                     
               ::std::memcpy(&lhs, &(*rhs), sizeof(T));
               return (lhs);
            }
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't disown-assign type");
         }
         else {
            // Copy                                                     
            if constexpr (requires(T& a) { a = Copy(*rhs); })
               return (lhs = Copy(*rhs));
            else if constexpr (requires(T& a) { a = *rhs; })
               return (lhs = *rhs);
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't copy-assign type");
         }
      }
   }

} // namespace Langulus

namespace Langulus::CT
{
   namespace Inner
   {
      //TODO make non shallow semantics always check Decay<T>, even if inner?
      // could help generalizing and eliminating the need for separate
      // ShallowSemantic and DeepSemantic signatures in constructors/assignments
      /// Check if T is semantic-constructible by S                           
      template<template<class> class S, class T>
      concept SemanticMakable = Semantic<S<T>> and requires (T&& a) {
         {SemanticNew<true>(nullptr, S<T> {Forward<T>(a)})} -> Supported;
      };
      template<class S>
      concept SemanticMakableAlt = Semantic<S> and requires (TypeOf<S>&& a) {
         {SemanticNew<true>(nullptr, S {Forward<TypeOf<S>>(a)})} -> Supported;
      };

      /// Check if T is semantic-assignable                                   
      template<template<class> class S, class T>
      concept SemanticAssignable = Semantic<S<T>> and requires (T&& a) {
         {SemanticAssign<true>(a, S<T> {Forward<T>(a)})} -> Supported;
      };
      template<class S>
      concept SemanticAssignableAlt = Semantic<S> and requires (TypeOf<S>&& a) {
         {SemanticAssign<true>(a, S {Forward<TypeOf<S>>(a)})} -> Supported;
      };


      /// Check if T is disown-constructible                                  
      template<class T>
      concept DisownMakable = SemanticMakable<Langulus::Disowned, T>;

      /// Check if T is clone-constructible                                   
      template<class T>
      concept CloneMakable = SemanticMakable<Langulus::Cloned, T>;

      /// Check if T is abandon-constructible                                 
      template<class T>
      concept AbandonMakable = SemanticMakable<Langulus::Abandoned, T>;

      /// Check if T is copy-constructible                                    
      template<class T>
      concept CopyMakable = SemanticMakable<Langulus::Copied, T>;

      /// Check if T is move-constructible                                    
      template<class T>
      concept MoveMakable = SemanticMakable<Langulus::Moved, T>;


      /// Check if T is disown-assignable if mutable                          
      template<class T>
      concept DisownAssignable = SemanticAssignable<Langulus::Disowned, T>;

      /// Check if T is clone-assignable if mutable                           
      template<class T>
      concept CloneAssignable = SemanticAssignable<Langulus::Cloned, T>;

      /// Check if T is abandon-assignable if mutable                         
      template<class T>
      concept AbandonAssignable = SemanticAssignable<Langulus::Abandoned, T>;

      /// Check if the T is copy-assignable                                   
      template<class T>
      concept CopyAssignable = SemanticAssignable<Langulus::Copied, T>;

      /// Check if the T is move-assignable                                   
      template<class T>
      concept MoveAssignable = SemanticAssignable<Langulus::Moved, T>;


      /// Check if the T is descriptor-constructible                          
      template<class T>
      concept DescriptorMakable = not Abstract<T> and not Enum<T>
         and requires { T {Describe {Fake<const Anyness::Neat&>()}}; };

      /// Check if the T is noexcept-descriptor-constructible                 
      template<class T>
      concept DescriptorMakableNoexcept = DescriptorMakable<T>
         and noexcept ( T {Describe {Fake<const Anyness::Neat&>()}}  );

   } // namespace Langulus::CT::Inner


   /// Check if origin T is semantic-constructible by semantic S              
   template<template<class> class S, class... T>
   concept SemanticMakable = Complete<Decay<T>...>
       and (Inner::SemanticMakable<S, Decay<T>> and ...);

   template<class... S>
   concept SemanticMakableAlt = Complete<Decay<TypeOf<S>>...>
       and (Inner::SemanticMakableAlt<S> and ...);

   /// Check if origin T is semantic-assignable by semantic S                 
   template<template<class> class S, class... T>
   concept SemanticAssignable = Complete<Decay<T>...>
       and (Inner::SemanticAssignable<S, Decay<T>> and ...);

   template<class... S>
   concept SemanticAssignableAlt = Complete<Decay<TypeOf<S>>...>
       and (Inner::SemanticAssignableAlt<S> and ...);


   /// Check if origin T is disown-constructible                              
   template<class... T>
   concept DisownMakable = (SemanticMakable<Langulus::Disowned, T> and ...);

   /// Check if origin T is clone-constructible                               
   template<class... T>
   concept CloneMakable = (SemanticMakable<Langulus::Cloned, T> and ...);

   /// Check if origin T is abandon-constructible                             
   template<class... T>
   concept AbandonMakable = (SemanticMakable<Langulus::Abandoned, T> and ...);
   
   /// Check if origin T is copy-constructible                                
   template<class... T>
   concept CopyMakable = (SemanticMakable<Langulus::Copied, T> and ...);

   /// Check if origin T is move-constructible                                
   template<class... T>
   concept MoveMakable = (SemanticMakable<Langulus::Moved, T> and ...);


   /// Check if origin T is disown-assignable if mutable                      
   template<class... T>
   concept DisownAssignable = (SemanticAssignable<Langulus::Disowned, T> and ...);

   /// Check if origin T is clone-assignable if mutable                       
   template<class... T>
   concept CloneAssignable = (SemanticAssignable<Langulus::Cloned, T> and ...);

   /// Check if origin T is abandon-assignable if mutable                     
   template<class... T>
   concept AbandonAssignable = (SemanticAssignable<Langulus::Abandoned, T> and ...);

   /// Check if origin T is copy-assignable                                   
   template<class... T>
   concept CopyAssignable = (SemanticAssignable<Langulus::Copied, T> and ...);

   /// Check if origin T is move-assignable                                   
   template<class... T>
   concept MoveAssignable = (SemanticAssignable<Langulus::Moved, T> and ...);


   /// Check if the origin T is descriptor-constructible                      
   template<class... T>
   concept DescriptorMakable = Complete<Decay<T>...>
      and (Inner::DescriptorMakable<Decay<T>> and ...);

   /// Check if the origin T is noexcept-descriptor-constructible             
   template<class... T>
   concept DescriptorMakableNoexcept = Complete<Decay<T>...>
      and (Inner::DescriptorMakableNoexcept<Decay<T>> and ...);

} // namespace Langulus::CT
