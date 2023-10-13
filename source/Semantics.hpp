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

      /// An abstract semantic value                                          
      struct Semantic {
         LANGULUS(UNINSERTABLE) true;
         LANGULUS(UNALLOCATABLE) true;
      };

      /// An abstract copied value                                            
      struct Copied : Semantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = false;
         static constexpr bool Shallow = true;
      };

      /// An abstract moved value                                             
      struct Moved : Semantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = true;
         static constexpr bool Shallow = true;
      };

      /// An abstract abandoned value                                         
      struct Abandoned : Semantic {
         static constexpr bool Keep = false;
         static constexpr bool Move = true;
         static constexpr bool Shallow = true;
      };

      /// An abstract disowned value                                          
      struct Disowned : Semantic {
         static constexpr bool Keep = false;
         static constexpr bool Move = false;
         static constexpr bool Shallow = true;
      };

      /// An abstract cloned value                                            
      struct Cloned : Semantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = false;
         static constexpr bool Shallow = false;
      };
   }

   namespace CT
   {
      /// Checks if a type has special semantics                              
      template<class... T>
      concept Semantic = ((Decayed<T> and DerivedFrom<T, A::Semantic>) and ...);

      /// Checks if a type has no special semantics                           
      template<class... T>
      concept NotSemantic = ((not Semantic<T>) and ...);

      /// Check if a type is copied                                           
      template<class... T>
      concept Copied = ((Decayed<T> and DerivedFrom<T, A::Copied>) and ...);

      /// Check if a type is moved                                            
      template<class... T>
      concept Moved = ((Decayed<T> and DerivedFrom<T, A::Moved>) and ...);

      /// Check if a type is abandoned                                        
      template<class... T>
      concept Abandoned = ((Decayed<T> and DerivedFrom<T, A::Abandoned>) and ...);

      /// Check if a type is disowned                                         
      template<class... T>
      concept Disowned = ((Decayed<T> and DerivedFrom<T, A::Disowned>) and ...);

      /// Check if a type is cloned                                           
      template<class... T>
      concept Cloned = ((Decayed<T> and DerivedFrom<T, A::Cloned>) and ...);
   }
   
   
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
      
      /// Forward as copied, or collapse the semantic                         
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         if constexpr (not ::std::constructible_from<ALT_T, Copied<ALT_T>>
                       and ::std::copy_constructible<ALT_T>) {
            // Collapse the semantic, when applying it to fundamentals  
            // This way this wrapper is seamlessly integrated with the  
            // standard C++20 semantics                                 
            return static_cast<const ALT_T&>(mValue);
         }
         else return Copied<ALT_T> {mValue};
      }

      /// Forward as copied, never collapse                                   
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) ForwardPerfect() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
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
      explicit constexpr Moved(T&& value) noexcept
         : mValue {::std::forward<T>(value)} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }

      /// Forward as moved, or collapse the semantic                          
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         if constexpr (not ::std::constructible_from<ALT_T, Moved<ALT_T>>
                       and ::std::move_constructible<ALT_T>) {
            // Collapse the semantic, when applying it to fundamentals  
            // This way this wrapper is seamlessly integrated with the  
            // standard C++20 semantics                                 
            return ::std::forward<ALT_T>(mValue);
         }
         else return Moved<ALT_T> {::std::forward<ALT_T>(mValue)};
      }

      /// Forward as moved, never collapse                                    
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) ForwardPerfect() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
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
      explicit constexpr Abandoned(T&& value) noexcept
         : mValue {::std::forward<T>(value)} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as abandoned, never collapse                                
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         return Abandoned<ALT_T> {::std::forward<ALT_T>(mValue)};
      }

      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) ForwardPerfect() const noexcept {
         return Forward<ALT_T>();
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
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         return Disowned<ALT_T> {mValue};
      }

      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) ForwardPerfect() const noexcept {
         return Forward<ALT_T>();
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
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         return Cloned<ALT_T> {mValue};
      }

      template<class ALT_T = T>
      LANGULUS(INLINED)
      constexpr decltype(auto) ForwardPerfect() const noexcept {
         return Forward<ALT_T>();
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
   };
   
   /// Clone a value                                                          
   /// Does a deep-copy                                                       
   NOD() LANGULUS(INLINED)
   constexpr auto Clone(const CT::NotSemantic auto& value) noexcept {
      return Cloned<Decvq<Deref<decltype(value)>>> {value};
   }


   /// Create an element on the heap, using the provided semantic, by using   
   /// a placement new variant                                                
   ///   @attention assumes placement pointer is valid                        
   ///   @param placement - where to place the new instance                   
   ///   @param value - the constructor arguments and the semantic            
   ///   @return the instance on the heap                                     
   template<template<class> class S, CT::NotSemantic T> requires CT::Semantic<S<T>>
   LANGULUS(INLINED)
   auto SemanticNew(void* placement, S<T>&& value) {
      LANGULUS_ASSUME(DevAssumes, placement, "Invalid placement pointer");

      if constexpr (CT::Abstract<T>) {
         // Can't create abstract stuff                                 
         return Inner::Unsupported {};
      }
      else if constexpr (S<T>::Move) {
         if constexpr (not S<T>::Keep) {
            // Abandon                                                  
            if constexpr (requires { new T {Abandon(*value)}; })
               return new (placement) T {Abandon(*value)};
            else if constexpr (not CT::Destroyable<T>) {
               // If type is not destroyable (like fundamentals), then  
               // it is always acceptable to abandon them - just use    
               // the standard move-semantics                           
               if constexpr (requires { new T {::std::move(*value)}; })
                  return new (placement) T {::std::move(*value)};
               else
                  return Inner::Unsupported {};
            }
            else return Inner::Unsupported {};
         }
         else {
            // Move                                                     
            if constexpr (requires { new T {Move(*value)}; })
               return new (placement) T {Move(*value)};
            else if constexpr (requires { new T {::std::move(*value)}; })
               return new (placement) T {::std::move(*value)};
            else
               return Inner::Unsupported {};
         }
      }
      else if constexpr (not S<T>::Shallow) {
         // Clone                                                       
         if constexpr (requires { new T {Clone(*value)}; })
            return new (placement) T {Clone(*value)};
         else if constexpr (CT::POD<T>) {
            // If type is POD (like fundamentals, or trivials), then    
            // it is always acceptable to clone by memcpy               
            ::std::memcpy(placement, &(*value), sizeof(T));
            return reinterpret_cast<T*>(placement);
         }
         else return Inner::Unsupported {};
      }
      else if constexpr (not S<T>::Keep) {
         // Disown                                                      
         if constexpr (requires { new T {Disown(*value)}; })
            return new (placement) T {Disown(*value)};
         else
            return Inner::Unsupported {};
      }
      else {
         // Copy                                                        
         if constexpr (requires { new T {Copy(*value)}; })
            return new (placement) T {Copy(*value)};
         else if constexpr (requires { new T {*value}; })
            return new (placement) T {*value};
         else
            return Inner::Unsupported {};
      }
   }
   
   /// Create an element on the heap, using the provided semantic, by using   
   /// a placement new variant, relying on the reflected constructors         
   ///   @attention assumes type is valid, complete, and not abstract         
   ///   @attention assumes bit placement and TypeOf<S> are valid pointers    
   ///              that point to an instance of the provided type            
   ///   @param type - the reflected type to instantiate                      
   ///   @param placement - where to place the new instance                   
   ///   @param value - the constructor arguments and the semantic            
   LANGULUS(INLINED)
   void SemanticNewUnknown(RTTI::DMeta type, Byte* placement, CT::Semantic auto&& value) {
      LANGULUS_ASSUME(DevAssumes, type,      "Invalid type");
      LANGULUS_ASSUME(DevAssumes, placement, "Invalid placement pointer");
      LANGULUS_ASSUME(DevAssumes, *value,    "Invalid argument pointer");
      LANGULUS_ASSUME(DevAssumes, not type->mIsAbstract, "Type can't be abstract");

      using S = Decay<decltype(value)>;
      if constexpr (S::Move) {
         if (not S::Keep) {
            // Abandon                                                  
            if (type->mAbandonConstructor)
               type->mAbandonConstructor(*value, placement);
            else
               LANGULUS_THROW(Construct, "Can't abandon-construct T from S");
         }
         else {
            // Move                                                     
            if (type->mMoveConstructor)
               type->mMoveConstructor(*value, placement);
            else
               LANGULUS_THROW(Construct, "Can't move-construct T from S");
         }
      }
      else if constexpr (not S::Shallow) {
         // Clone                                                       
         if (type->mCloneConstructor)
            type->mCloneConstructor(*value, placement);
         else
            LANGULUS_THROW(Construct, "Can't clone-construct T from S");
      }
      else if constexpr (not S::Keep) {
         // Disown                                                      
         if (type->mDisownConstructor)
            type->mDisownConstructor(*value, placement);
         else
            LANGULUS_THROW(Construct, "Can't disown-construct T from S");
      }
      else {
         // Copy                                                        
         if (type->mCopyConstructor)
            type->mCopyConstructor(*value, placement);
         else
            LANGULUS_THROW(Construct, "Can't copy-construct T from S");
      }
   }

   /// Assign new value to an instance of T, using the provided semantic      
   ///   @param lhs - left hand side (what are we assigning to)               
   ///   @param rhs - right hand side (what are we assigning)                 
   ///   @return whatever the assignment operator returns                     
   template<template<class> class S, CT::NotSemantic T> requires CT::Semantic<S<T>>
   LANGULUS(INLINED)
   decltype(auto) SemanticAssign(T& lhs, S<T>&& rhs) {
      if constexpr (S<T>::Move) {
         if constexpr (not S<T>::Keep) {
            // Abandon                                                  
            if constexpr (requires(T& a) { a = Abandon(*rhs); })
               return (lhs = Abandon(*rhs));
            else if constexpr (not CT::Destroyable<T>) {
               // If type is not destroyable (like fundamentals), then  
               // it is always acceptable to abandon them - just use    
               // the standard move-semantics                           
               if constexpr (requires(T& a) { a = ::std::move(*rhs); })
                  return (lhs = ::std::move(*rhs));
               else
                  return Inner::Unsupported {};
            }
            else return Inner::Unsupported {};
         }
         else {
            // Move                                                     
            if constexpr (requires(T& a) { a = Move(*rhs); })
               return (lhs = Move(*rhs));
            else if constexpr (requires(T& a) { a = ::std::move(*rhs); })
               return (lhs = ::std::move(*rhs));
            else
               return Inner::Unsupported {};
         }
      }
      else {
         if constexpr (not S<T>::Shallow) {
            // Clone                                                    
            if constexpr (requires(T& a) { a = Clone(*rhs); })
               return (lhs = Clone(*rhs));
            else if constexpr (CT::POD<T>) {
               // If type is POD (like fundamentals, or trivials), then 
               // it is always acceptable to clone by memcpy            
               ::std::memcpy(&lhs, &(*rhs), sizeof(T));
               return (lhs);
            }
            else return Inner::Unsupported {};
         }
         else if constexpr (not S<T>::Keep) {
            // Disown                                                   
            if constexpr (requires(T& a) { a = Disown(*rhs); })
               return (lhs = Disown(*rhs));
            else
               return Inner::Unsupported {};
         }
         else {
            // Copy                                                     
            if constexpr (requires(T& a) { a = Copy(*rhs); })
               return (lhs = Copy(*rhs));
            else if constexpr (requires(T& a) { a = *rhs; })
               return (lhs = *rhs);
            else
               return Inner::Unsupported {};
         }
      }
   }

} // namespace Langulus

namespace Langulus::CT
{
   namespace Inner
   {

      /// Check if T is semantic-constructible by S                           
      template<template<class> class S, class T>
      concept SemanticMakable = Semantic<S<T>> and requires (T&& a) {
         {SemanticNew(nullptr, S<T> {Forward<T>(a)})} -> Supported;
      };

      /// Check if T is semantic-assignable                                   
      template<template<class> class S, class T>
      concept SemanticAssignable = Semantic<S<T>> and requires (T&& a) {
         {SemanticAssign(a, S<T> {Forward<T>(a)})} -> Supported;
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

      /// Check if the decayed T is copy-constructible                        
      template<class T>
      concept CopyMakable = SemanticMakable<Langulus::Copied, T>;

      /// Check if the decayed T is move-constructible                        
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

      /// Check if the decayed T is copy-assignable                           
      template<class T>
      concept CopyAssignable = SemanticAssignable<Langulus::Copied, T>;

      /// Check if the decayed T is move-assignable                           
      template<class T>
      concept MoveAssignable = SemanticAssignable<Langulus::Moved, T>;

   } // namespace Langulus::CT::Inner


   /// Check if origin T is semantic-constructible by semantic S              
   template<template<class> class S, class... T>
   concept SemanticMakable = Complete<Decay<T>...>
       and (Inner::SemanticMakable<S, Decay<T>> and ...);

   /// Check if origin T is semantic-assignable by semantic S                 
   template<template<class> class S, class... T>
   concept SemanticAssignable = Complete<Decay<T>...>
       and (Inner::SemanticAssignable<S, Decay<T>> and ...);


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

} // namespace Langulus::CT
