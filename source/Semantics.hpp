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

      ///                                                                     
      /// An abstract semantic                                                
      ///                                                                     
      struct Semantic {
         LANGULUS(ABSTRACT) true;
         LANGULUS(UNINSERTABLE) true;
         LANGULUS(UNALLOCATABLE) true;
         LANGULUS(REFLECTABLE) false;
      };

      /// An abstract shallow semantic                                        
      /// Shallow semantics are generally not propagated through indirections 
      struct ShallowSemantic : Semantic {
         static constexpr bool Shallow = true;
      };

      /// An abstract deep semantic                                           
      /// Deep semantics are propagated through indirection layers            
      struct DeepSemantic : Semantic {
         static constexpr bool Shallow = false;
      };

      /// An abstract refer semantic                                          
      /// It doesn't copy anything but the container state, and references    
      /// data. For pointers, the Referred semantic collapses to the Copy     
      /// semantic                                                            
      struct Referred : ShallowSemantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = false;
      };
      
      /// An abstract shallow-copy semantic                                   
      /// A shallowc-copy is like a Clone semantic, but doesn't clone past    
      /// indirection layers                                                  
      struct Copied : ShallowSemantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = false;
      };

      /// An abstract move semantic                                           
      /// Moving transfers ownership, and ensures, that the source is reset   
      /// and left in a safe, consistent state after the move                 
      struct Moved : ShallowSemantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = true;
      };

      /// An abstract abandon-move semantic                                   
      /// Unlike the Move semantic, this doesn't guarantee, that the moved    
      /// instance is in a consistent and safe state. Instead, it makes sure  
      /// only that it is safely destructible, to save on some performance.   
      struct Abandoned : ShallowSemantic {
         static constexpr bool Keep = false;
         static constexpr bool Move = true;
      };

      /// An abstract disowned-refer semantic                                 
      /// Similar to Refer semantic, but doesn't actually reference data,     
      /// producing what's essentially a cheap view over the data (unsafe)    
      struct Disowned : ShallowSemantic {
         static constexpr bool Keep = false;
         static constexpr bool Move = false;
      };

      /// An abstract clone semantic (a.k.a. deep-copy semantic)              
      /// Copies all elements across all indirection layers                   
      struct Cloned : DeepSemantic {
         static constexpr bool Keep = true;
         static constexpr bool Move = false;
      };

   } // namespace Langulus::A

   struct Describe;

   namespace CT
   {

      /// Checks if all T are semantic                                        
      template<class...T>
      concept Semantic = sizeof...(T) > 0
          and (DerivedFrom<T, A::Semantic> and ...);

      /// Checks if all T are NOT semantic                                    
      template<class...T>
      concept NotSemantic = sizeof...(T) > 0
          and ((not Semantic<T>) and ...);

      /// Checks if all T are shallow-semantic                                
      template<class...T>
      concept ShallowSemantic = sizeof...(T) > 0
          and (DerivedFrom<T, A::ShallowSemantic> and ...);

      /// Checks if all T are deep-semantic                                   
      template<class...T>
      concept DeepSemantic = sizeof...(T) > 0
          and (DerivedFrom<T, A::DeepSemantic> and ...);

      /// Check if all T are refer-semantic                                   
      template<class...T>
      concept Referred = sizeof...(T) > 0
          and (DerivedFrom<T, A::Referred> and ...);
      
      /// Check if all T are copy-semantic                                    
      template<class...T>
      concept Copied = sizeof...(T) > 0
          and (DerivedFrom<T, A::Copied> and ...);

      /// Check if all T are move-semantic                                    
      template<class...T>
      concept Moved = sizeof...(T) > 0
          and (DerivedFrom<T, A::Moved> and ...);

      /// Check if all T are abandon-semantic                                 
      template<class...T>
      concept Abandoned = sizeof...(T) > 0
          and (DerivedFrom<T, A::Abandoned> and ...);

      /// Check if all T are disown-semantic                                  
      template<class...T>
      concept Disowned = sizeof...(T) > 0
          and (DerivedFrom<T, A::Disowned> and ...);

      /// Check if all T are clone-semantic                                   
      template<class...T>
      concept Cloned = sizeof...(T) > 0
          and (DerivedFrom<T, A::Cloned> and ...);

   } // namespace Langulus::CT

   
   ///                                                                        
   /// Referred value intermediate type, use in constructors and assignments  
   /// to refer to data explicitly                                            
   ///   @tparam T - the type to refer                                        
   template<class T>
   struct Referred : A::Referred {
   private:
      const T& mValue;

   public:
      LANGULUS(TYPED) T;

      Referred() = delete;
      explicit constexpr Referred(const Referred&) noexcept = default;
      explicit constexpr Referred(Referred&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Referred(const T& value) noexcept
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as refered                                                  
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T> LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Referred<ALT_T> {mValue};
      }

      /// Refer something else                                                
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto&& value) noexcept {
         using ALT = Decvq<Deref<decltype(value)>>;
         if constexpr (CT::Semantic<ALT>)
            return Referred<TypeOf<ALT>> {*value};
         else
            return Referred<ALT> {value};
      }

      template<class ALT>
      using As = Conditional<CT::Semantic<ALT>, Referred<TypeOf<ALT>>, Referred<ALT>>;

      LANGULUS(INLINED)
      const T& operator * () const noexcept { return mValue; }

      LANGULUS(INLINED)
      auto operator -> () const noexcept {
         if constexpr (CT::Sparse<T>)
            return mValue;
         else
            return &mValue;
      }

      /// Implicitly collapse the semantic                                    
      /// This way this wrapper is seamlessly integrated with the standard    
      /// C++20 copy semantics                                                
      LANGULUS(INLINED)
      constexpr operator const T& () const& noexcept {
         return mValue;
      }
      LANGULUS(INLINED)
      constexpr operator const T& () & noexcept {
         return mValue;
      }
      LANGULUS(INLINED)
      constexpr operator const T& () && noexcept {
         return mValue;
      }
   };
   
   /// Refer a value                                                          
   NOD() LANGULUS(INLINED) 
   constexpr auto Refer(auto&& value) noexcept {
      using ALT = Decvq<Deref<decltype(value)>>;
      if constexpr (CT::Semantic<ALT>)
         return Referred<TypeOf<ALT>> {*value};
      else
         return Referred<ALT> {value};
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
      constexpr Copied(const Copied&) noexcept = default;
      explicit constexpr Copied(Copied&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Copied(const T& value) noexcept
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as copied                                                   
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T> LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Copied<ALT_T> {mValue};
      }

      /// Copy something else                                                 
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto&& value) noexcept {
         using ALT = Decvq<Deref<decltype(value)>>;
         if constexpr (CT::Semantic<ALT>)
            return Copied<TypeOf<ALT>> {*value};
         else
            return Copied<ALT> {value};
      }

      template<class ALT>
      using As = Conditional<CT::Semantic<ALT>, Copied<TypeOf<ALT>>, Copied<ALT>>;

      LANGULUS(INLINED)
      const T& operator * () const noexcept { return mValue; }

      LANGULUS(INLINED)
      auto operator -> () const noexcept {
         if constexpr (CT::Sparse<T>)
            return mValue;
         else
            return &mValue;
      }

      /// Implicitly collapse the semantic, when applying it to POD/Sparse,   
      /// since Refer is isomorphic to Copy in those cases                    
      LANGULUS(INLINED)
      constexpr operator const T& () const noexcept
      requires (CT::POD<T> or CT::Sparse<T>) {
         return mValue;
      }

      /// Used by DecayCast                                                   
      LANGULUS(INLINED)
      constexpr const T& DecayCast() const noexcept {
         return mValue;
      }
   };
   
   /// Copy a value                                                           
   NOD() LANGULUS(INLINED) 
   constexpr auto Copy(auto&& value) noexcept {
      using ALT = Decvq<Deref<decltype(value)>>;
      if constexpr (CT::Semantic<ALT>)
         return Copied<TypeOf<ALT>> {*value};
      else
         return Copied<ALT> {value};
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
      constexpr Moved(const Moved& r) noexcept
         : mValue {::std::forward<T>(r.mValue)} {}
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
      template<class ALT_T = T> LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Moved<ALT_T> {::std::forward<ALT_T>(mValue)};
      }

      /// Move something else                                                 
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto&& value) noexcept {
         using ALT = Decvq<Deref<decltype(value)>>;
         if constexpr (CT::Semantic<ALT>) {
            return Moved<TypeOf<ALT>> {
               ::std::forward<TypeOf<ALT>>(*value)};
         }
         else {
            return Moved<ALT> {
               ::std::forward<ALT>(value)};
         }
      }

      template<class ALT>
      using As = Conditional<CT::Semantic<ALT>, Moved<TypeOf<ALT>>, Moved<ALT>>;

      LANGULUS(INLINED)
      T& operator * () const noexcept { return mValue; }

      LANGULUS(INLINED)
      auto operator -> () const noexcept {
         if constexpr (CT::Sparse<T>)
            return mValue;
         else
            return &mValue;
      }

      /// Implicitly collapse the semantic                                    
      /// This way this wrapper is seamlessly integrated with the standard    
      /// C++20 move-semantics                                                
      LANGULUS(INLINED)
      constexpr operator T&& () const noexcept {
         return ::std::forward<T>(mValue);
      }

      /// Used by DecayCast                                                   
      LANGULUS(INLINED)
      constexpr T& DecayCast() const noexcept {
         return mValue;
      }
   };
   
   /// Move a value                                                           
   NOD() LANGULUS(INLINED)
   constexpr auto Move(auto&& value) noexcept {
      using ALT = Decvq<Deref<decltype(value)>>;
      if constexpr (CT::Semantic<ALT>)
         return Moved<TypeOf<ALT>> {::std::forward<TypeOf<ALT>>(*value)};
      else
         return Moved<ALT> {::std::forward<ALT>(value)};
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
      constexpr Abandoned(const Abandoned& r) noexcept
         : mValue {::std::forward<T>(r.mValue)} {}
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
      template<class ALT_T = T> LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Abandoned<ALT_T> {::std::forward<ALT_T>(mValue)};
      }

      /// Abandon something else                                              
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto&& value) noexcept {
         using ALT = Decvq<Deref<decltype(value)>>;
         if constexpr (CT::Semantic<ALT>) {
            return Abandoned<TypeOf<ALT>> {
               ::std::forward<TypeOf<ALT>>(*value)};
         }
         else {
            return Abandoned<ALT> {
               ::std::forward<ALT>(value)};
         }
      }

      template<class ALT>
      using As = Conditional<CT::Semantic<ALT>, Abandoned<TypeOf<ALT>>, Abandoned<ALT>>;

      LANGULUS(INLINED)
      T& operator * () const noexcept { return mValue; }

      LANGULUS(INLINED)
      auto operator -> () const noexcept {
         if constexpr (CT::Sparse<T>)
            return mValue;
         else
            return &mValue;
      }

      /// Implicitly collapse the semantic                                    
      /// This way this wrapper is seamlessly integrated with the standard    
      /// C++20 move-semantics                                                
      LANGULUS(INLINED)
      constexpr operator T&& () const noexcept {
         return ::std::forward<T>(mValue);
      }

      /// Used by DecayCast                                                   
      LANGULUS(INLINED)
      constexpr T& DecayCast() const noexcept {
         return mValue;
      }
   };
   
   /// Abandon a value                                                        
   /// Same as Move, but resets only mandatory data inside source after move  
   /// essentially saving up on a couple of instructions                      
   NOD() LANGULUS(INLINED)
   constexpr auto Abandon(auto&& value) noexcept {
      using ALT = Decvq<Deref<decltype(value)>>;
      if constexpr (CT::Semantic<ALT>) {
         return Abandoned<TypeOf<ALT>> {
            ::std::forward<TypeOf<ALT>>(*value)};
      }
      else {
         return Abandoned<ALT> {
            ::std::forward<ALT>(value)};
      }
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
      constexpr Disowned(const Disowned&) noexcept = default;
      explicit constexpr Disowned(Disowned&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Disowned(const T& value) noexcept
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }

      /// Forward as disowned, never collapse                                 
      ///   @tparam ALT_T - optional type to static_cast to, when forwarding  
      template<class ALT_T = T> LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Disowned<ALT_T> {mValue};
      }

      /// Disown something else                                               
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto&& value) noexcept {
         using ALT = Decvq<Deref<decltype(value)>>;
         if constexpr (CT::Semantic<ALT>)
            return Disowned<TypeOf<ALT>> {*value};
         else
            return Disowned<ALT> {value};
      }

      template<class ALT>
      using As = Conditional<CT::Semantic<ALT>, Disowned<TypeOf<ALT>>, Disowned<ALT>>;
      
      LANGULUS(INLINED)
      const T& operator * () const noexcept { return mValue; }

      LANGULUS(INLINED)
      auto operator -> () const noexcept {
         if constexpr (CT::Sparse<T>)
            return mValue;
         else
            return &mValue;
      }

      /// Implicitly collapse the semantic, when applying it to PODs,         
      /// since they never have ownership either way                          
      LANGULUS(INLINED)
      constexpr operator const T& () const noexcept requires CT::POD<T> {
         return mValue;
      }

      /// Used by DecayCast                                                   
      LANGULUS(INLINED)
      constexpr const T& DecayCast() const noexcept {
         return mValue;
      }
   };
   
   /// Disown a value                                                         
   /// Same as a shallow-copy, but never references, saving some instructions 
   NOD() LANGULUS(INLINED)
   constexpr auto Disown(auto&& value) noexcept {
      using ALT = Decvq<Deref<decltype(value)>>;
      if constexpr (CT::Semantic<ALT>)
         return Disowned<TypeOf<ALT>> {*value};
      else
         return Disowned<ALT> {value};
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
      constexpr Cloned(const Cloned&) noexcept = default;
      explicit constexpr Cloned(Cloned&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Cloned(const T& value) noexcept
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }

      /// Forward as cloned, bever collapse                                   
      template<class ALT_T = T> LANGULUS(INLINED)
      constexpr decltype(auto) Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>,
            "Can't nest semantics");
         static_assert(CT::Similar<T, ALT_T> or CT::DerivedFrom<T, ALT_T>,
            "Can't forward as this type");
         return Cloned<ALT_T> {mValue};
      }

      /// Clone something else                                                
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto&& value) noexcept {
         using ALT = Decvq<Deref<decltype(value)>>;
         if constexpr (CT::Semantic<ALT>)
            return Cloned<TypeOf<ALT>> {*value};
         else
            return Cloned<ALT> {value};
      }

      template<class ALT>
      using As = Conditional<CT::Semantic<ALT>, Cloned<TypeOf<ALT>>, Cloned<ALT>>;
      
      LANGULUS(INLINED)
      const T& operator * () const noexcept { return mValue; }

      LANGULUS(INLINED)
      auto operator -> () const noexcept {
         if constexpr (CT::Sparse<T>)
            return mValue;
         else
            return &mValue;
      }

      /// Implicitly collapse the semantic, when applying it to PODs,         
      /// since they are always cloned upon copy                              
      LANGULUS(INLINED)
      constexpr operator const T& () const noexcept requires CT::POD<T> {
         return mValue;
      }

      /// Used by DecayCast                                                   
      LANGULUS(INLINED)
      constexpr const T& DecayCast() const noexcept {
         return mValue;
      }
   };
   
   /// Clone a value                                                          
   /// Does a deep-copy                                                       
   NOD() LANGULUS(INLINED)
   constexpr auto Clone(auto&& value) noexcept {
      using ALT = Decvq<Deref<decltype(value)>>;
      if constexpr (CT::Semantic<ALT>)
         return Cloned<TypeOf<ALT>> {*value};
      else
         return Cloned<ALT> {value};
   }


   ///                                                                        
   /// Descriptor intermediate type, use in constructors to enable descriptor 
   /// construction. The inner type is always Anyness::Neat                   
   struct Describe : A::Semantic {
   protected:
      const Anyness::Neat& mValue;

   public:
      Describe() = delete;
      constexpr Describe(const Describe&) noexcept = default;
      explicit constexpr Describe(Describe&&) noexcept = default;

      LANGULUS(INLINED)
      explicit constexpr Describe(const Anyness::Neat& value) noexcept
         : mValue {value} {}

      /// The describe semantic completely ignores nesting, only propagates   
      /// itself                                                              
      LANGULUS(INLINED)
      static constexpr decltype(auto) Nest(auto&& value) noexcept {
         using ALT = Decvq<Deref<decltype(value)>>;
         if constexpr (CT::Similar<ALT, Describe>)
            return Forward<ALT>(value);
         else if constexpr (CT::Semantic<ALT> and CT::Similar<TypeOf<ALT>, Anyness::Neat>)
            return Describe {*value};
         else if constexpr (CT::Similar<ALT, Anyness::Neat>)
            return Describe {value};
         else
            LANGULUS_ERROR("Can't nest provided type as a Describe semantic");
      }

      LANGULUS(INLINED)
      const Anyness::Neat& operator *  () const noexcept { return  mValue; }

      LANGULUS(INLINED)
      const Anyness::Neat* operator -> () const noexcept { return &mValue; }
   };

   
   namespace CT
   {

      ///                                                                     
      ///   Semantic type traits                                              
      ///                                                                     
      ///   These concepts are strict requirements, and are true only if the  
      /// corresponding constructors/assigners are implicitly/explicitly      
      /// defined. No fallbacks!                                              
      ///                                                                     

      /// Check if all T have semantic constructors for S                     
      ///   @tparam S - the semantic                                          
      ///   @tparam T... - the types                                          
      template<template<class> class S, class...T>
      concept HasSemanticConstructor = Complete<T...> and ((
            Semantic<S<T>> and ::std::constructible_from<T, S<T>>
         ) and ...);

      /// Check if all TypeOf<S> have semantic constructors for S             
      ///   @tparam S - the semantic and type                                 
      template<class...S>
      concept HasSemanticConstructorAlt = Complete<TypeOf<S>...> and ((
            Semantic<S> and ::std::constructible_from<TypeOf<S>, S>
         ) and ...);

      /// Check if all T have a disown-constructor                            
      /// Disowning does a shallow copy without referencing contents,         
      /// generating a 'view' of the data that is without ownership.          
      template<class...T>
      concept HasDisownConstructor =
         (HasSemanticConstructor<Langulus::Disowned, T> and ...);

      /// Check if all Decay<T> have a clone-constructor                      
      /// Does a deep copy                                                    
      template<class...T>
      concept HasCloneConstructor =
         (HasSemanticConstructor<Langulus::Cloned, T> and ...);

      /// Check if all T have a abandon-constructor                           
      /// Does a move, but doesn't fully reset source (optimization)          
      template<class...T>
      concept HasAbandonConstructor =
         (HasSemanticConstructor<Langulus::Abandoned, T> and ...);

      /// Check if all T have a refer-constructor                             
      /// Refering does a shallow copy while referencing contents, providing  
      /// ownership.                                                          
      /// T has refer-constructor as long as it is std::copy_constuctible     
      template<class...T>
      concept HasReferConstructor = Complete<T...>
          and ((HasSemanticConstructor<Langulus::Referred, T>
           or ::std::copy_constructible<T>) and ...);
      
      /// Check if all T have a copy-constructor (don't mistake it for a      
      /// std::copy_constructible!)                                           
      /// Does a shallow copy _of the contents_ (like shallow cloning).       
      template<class...T>
      concept HasCopyConstructor =
         (HasSemanticConstructor<Langulus::Copied, T> and ...);

      /// Check if all T have a move-constructor                              
      /// Does a move, fully resetting source                                 
      /// T has move-constructor as long as it is std::move_constuctible      
      template<class...T>
      concept HasMoveConstructor = Complete<T...> and ((Sparse<T>
           or HasSemanticConstructor<Langulus::Moved, T>
           or ::std::move_constructible<T>) and ...);


      /// Check if all T have a semantic-assigner for S                       
      ///   @tparam S - the semantic                                          
      ///   @tparam T... - the types                                          
      template<template<class> class S, class...T>
      concept HasSemanticAssign = Complete<T...> and ((Semantic<S<T>>
          and ::std::assignable_from<T&, S<T>&&>) and ...);

      /// Check if all TypeOf<S> has semantic-assigner for S                  
      ///   @tparam S - the semantic and type                                 
      template<class...S>
      concept HasSemanticAssignAlt = Complete<TypeOf<S>...> and ((Semantic<S>
          and ::std::assignable_from<TypeOf<S>&, S&&>) and ...);

      /// Check if all T have a disown-assigner                               
      /// Disowning does a shallow copy without referencing contents,         
      /// generating a 'view' of the data that is without ownership.          
      template<class...T>
      concept HasDisownAssign =
         (HasSemanticAssign<Langulus::Disowned, T> and ...);

      /// Check if all Decay<T> have a clone-assigner                         
      /// Does a deep copy                                                    
      template<class...T>
      concept HasCloneAssign =
         (HasSemanticAssign<Langulus::Cloned, T> and ...);

      /// Check if all T have an abandon-assigner                             
      /// Does a move, but doesn't fully reset source (optimization)          
      template<class...T>
      concept HasAbandonAssign =
         (HasSemanticAssign<Langulus::Abandoned, T> and ...);

      /// Check if all T have refer-assigner                                  
      /// Refering does a shallow copy while referencing contents, providing  
      /// ownership.                                                          
      /// T has a refer-assigner as long as std::copy_assignable<T> holds     
      template<class...T>
      concept HasReferAssign = Complete<T...>
          and ((HasSemanticAssign<Langulus::Referred, T>
           or ::std::is_copy_assignable_v<T>) and ...);
      
      /// Check if all T have a copy-assigner (don't mistake it for a         
      /// std::copy_assignable!)                                              
      /// Does a shallow copy _of the contents_ (like shallow cloning).       
      template<class...T>
      concept HasCopyAssign =
         (HasSemanticAssign<Langulus::Copied, T> and ...);

      /// Check if all T have a move-assigner                                 
      /// Does a move, fully resetting source                                 
      /// T has a move-assigner as long as std::is_move_assignable_v<T> holds 
      /// @attention you can't have move semantics, if a type has its         
      ///   destructor deleted - every time you move an instance, the old one 
      ///   has to be deleted later.                                          
      template<class...T>
      concept HasMoveAssign = Complete<T...> and ((::std::destructible<T>) and ...)
          and ((HasSemanticAssign<Langulus::Moved, T>
           or ::std::is_move_assignable_v<T>) and ...);

   } // namespace Langulus::CT



   /// Create an instance of T at the provided memory, using placement new    
   ///   @attention assumes placement pointer is valid                        
   ///   @attention when S is a deep semantic, like Cloned, this function     
   ///              assumes that the 'placement' pointer always points to a   
   ///              dense place, where the dacayed T is cloned in             
   ///   @param placement - where to place the new instance                   
   ///   @param value - the constructor arguments and the semantic            
   ///   @return the instance on the heap                                     
   template<bool FAKE = false, template<class> class S, CT::NotSemantic T>
   requires CT::Semantic<S<T>> LANGULUS(INLINED)
   constexpr auto SemanticNew(void* placement, S<T>&& value) {
      using SS = S<T>;
      LANGULUS_ASSUME(DevAssumes, placement, "Invalid placement pointer");

      if constexpr (CT::Abstract<T>) {
         // Can't instantiate abstract type                             
         if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't instantiate abstract type");
      }
      else if constexpr (CT::Reference<T>) {
         // Can't instantiate as a reference                            
         if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't SemanticNew at a reference");
      }
      else if constexpr (SS::Move) {
         if constexpr (not SS::Keep) {
            // Abandon                                                  
            if constexpr (CT::HasAbandonConstructor<T>)
               return new (placement) T (Forward<SS>(value));
            else if constexpr (CT::POD<T> and CT::HasMoveConstructor<T>)
               return new (placement) T (Move(*value));
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR(
                  "Can't abandon-construct destructible type"
                  " - explicit abandon-constructor is required");
         }
         else {
            // Move                                                     
            if constexpr (CT::HasMoveConstructor<T>)
               return new (placement) T (Forward<SS>(value));
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't move-construct type");
         }
      }
      else if constexpr (not SS::Shallow) {
         // Clone                                                       
         using DT = Decay<T>;

         if constexpr (not CT::Void<DT>) {
            if constexpr (CT::HasCloneConstructor<DT>)
               return new (placement) DT (Clone(DenseCast(*value)));
            else if constexpr (CT::POD<DT> and ::std::copy_constructible<DT>)
               return new (placement) DT (DenseCast(*value));
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't clone-construct type");
         }
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't clone-construct a void type");
      }
      else if constexpr (not SS::Keep) {
         // Disown                                                      
         if constexpr (CT::HasDisownConstructor<T>)
            return new (placement) T (Forward<SS>(value));
         else if constexpr (CT::POD<T> and ::std::copy_constructible<T>)
            return new (placement) T (*value);
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't disown-construct type");
      }
      else if constexpr (CT::Copied<SS>) {
         // Copy                                                        
         if constexpr (CT::HasCopyConstructor<T>)
            return new (placement) T (Forward<SS>(value));
         else if constexpr (CT::POD<T> and ::std::copy_constructible<T>)
            return new (placement) T (*value);
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't copy-construct type");
      }
      else if constexpr (CT::Referred<SS>) {
         // Refer                                                       
         if constexpr (CT::HasReferConstructor<T>)
            return new (placement) T (Forward<SS>(value));
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't refer-construct type");
      }
      else LANGULUS_ERROR("Unsupported shallow semantic");
   }

   /// Assign new value to an instance of T, using the provided semantic      
   ///   @attention when S is a deep semantic, like Cloned, this function     
   ///              will DenseCast 'lhs' and 'rhs', and copy only dense data  
   ///   @param lhs - left hand side (what are we assigning to)               
   ///   @param rhs - right hand side (what are we assigning)                 
   ///   @return whatever the assignment operator returns                     
   template<bool FAKE = false, template<class> class S, CT::NotSemantic T>
   requires CT::Semantic<S<T>> LANGULUS(INLINED)
   constexpr decltype(auto) SemanticAssign(Decvq<T>& lhs, S<T>&& rhs) {
      using MT = Decvq<T>;
      using SS = S<T>;

      if constexpr (not CT::Complete<MT>) {
         // Can't assign to an incomplete type                          
         if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't SemanticAssign to an incomplete type");
      }
      else if constexpr (CT::Reference<MT>) {
         // Can't reassign a reference                                  
         if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't SemanticAssign at a reference");
      }
      else if constexpr (SS::Move) {
         if constexpr (not SS::Keep) {
            // Abandon                                                  
            if constexpr (CT::HasAbandonAssign<T>)
               return (lhs = Forward<SS>(rhs));
            else if constexpr (CT::POD<T> and CT::HasMoveAssign<T>)
               return (lhs = Move(*rhs));
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR(
                  "Can't abandon-assign destructible type"
                  " - explicit abandon-assigner is required");
         }
         else {
            // Move                                                     
            if constexpr (CT::HasMoveAssign<T>)
               return (lhs = Forward<SS>(rhs));
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't move-assign type");
         }
      }
      else if constexpr (not SS::Shallow) {
         // Clone                                                       
         using DT = Decay<T>;

         if constexpr (not CT::Void<DT>) {
            if constexpr (CT::Mutable<decltype(DenseCast(lhs))>) {
               if constexpr (CT::HasCloneAssign<DT>)
                  return (DenseCast(lhs) = Clone(DenseCast(*rhs)));
               else if constexpr (CT::POD<DT> and ::std::is_copy_assignable_v<DT>)
                  return (DenseCast(lhs) = DenseCast(*rhs));
               else if constexpr (FAKE)
                  return Inner::Unsupported {};
               else
                  LANGULUS_ERROR("Can't clone-assign type");
            }
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't clone-assign type - lhs is not mutable");
         }
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't clone-assign void");
      }
      else if constexpr (not SS::Keep) {
         // Disown                                                      
         if constexpr (CT::HasDisownAssign<T>)
            return (lhs = Forward<SS>(rhs));
         else if constexpr (CT::POD<T> and ::std::is_copy_assignable_v<T>)
            return (lhs = *rhs);
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't disown-assign type");
      }
      else if constexpr (CT::Copied<SS>) {
         // Copy                                                        
         if constexpr (CT::HasCopyAssign<T>)
            return (lhs = Forward<SS>(rhs));
         else if constexpr (CT::POD<T> and ::std::is_copy_assignable_v<T>)
            return (lhs = *rhs);
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't copy-assign type");
      }
      else if constexpr (CT::Referred<SS>) {
         // Refer                                                       
         if constexpr (CT::HasReferAssign<T>)
            return (lhs = Forward<SS>(rhs));
         else if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't refer-assign type");
      }
      else LANGULUS_ERROR("Unsupported shallow semantic");
   }

   /// Deduce the proper semantic type, based on whether T already has a      
   /// specified semantic, or is an rvalue (&&) or not                        
   /// If it is, then we use move semantics; if it isn't - we use refer       
   /// semantics (which can fallback to copy semantics)                       
   template<class T>
   using SemanticOf = Conditional<CT::Semantic<T>, Decay<T>, 
      Conditional<::std::is_rvalue_reference_v<T> and CT::Mutable<Deref<T>>,
         Moved<Deref<T>>, Referred<Deref<T>>>>;

   /// Remove the semantic from a type, or just return the type, if not       
   /// wrapped inside a semantic                                              
   template<class T>
   using Desem = Conditional<CT::Semantic<T>, TypeOf<T>, T>;

   namespace CT
   {
   
      /// Check if T is constructible with each of the provided arguments     
      ///   @attention that this differs from std::constructible_from, by     
      ///      attempting each argument separately                            
      ///   @attention this also includes aggregate type construction, so it  
      ///      will return true if first member is constructible with each A  
      template<class T, class...A>
      concept MakableFrom = ((::std::constructible_from<T, A&&>) and ...);

      /// Check if T is assignable with each of the provided arguments        
      template<class T, class...A>
      concept AssignableFrom = ((
            requires (T t, A&& a) { t = Forward<A>(a); }
         ) and ...);


      ///                                                                     
      ///   Makables                                                          
      ///                                                                     
      ///   These concepts are bit looser on requirements, compared to their  
      /// Has*Constructor counterparts, to allow for fallbacks in places where
      /// they are required. A type may not explicitly HasAbandonConstructor, 
      /// and yet be AbandonMakable, because it is still movable, for example.
      ///                                                                     

      /// Check if all T are semantic-makable by S                            
      /// T can be semantic-makable even if not semantic-constructible,       
      /// as long as T and S are compatible with standard C++20 semantics     
      ///   @tparam S - the semantic                                          
      ///   @tparam T... - the types                                          
      template<template<class> class S, class...T>
      concept SemanticMakable = Semantic<S<T>...> and (requires {
            {SemanticNew<true>(nullptr, Fake<S<T>&&>())} -> Supported;
         } and ...);

      /// Check if all TypeOf<S> are semantic-makable by S                    
      /// T can be semantic-makable even if not semantic-constructible,       
      /// as long as T and S are compatible with standard C++20 semantics     
      ///   @tparam S - the semantic and type                                 
      template<class...S>
      concept SemanticMakableAlt = Semantic<S...> and (requires {
            {SemanticNew<true>(nullptr, Fake<S&&>())} -> Supported;
         } and ...);

      /// Check if all T are disown-makable                                   
      /// Disowning does a shallow copy without referencing contents,         
      /// generating a 'view' of the data that is without ownership.          
      /// If POD, T can be disown-makable even if not disown-constructible,   
      /// as long as it is std::copy_constuctible                             
      template<class...T>
      concept DisownMakable = (SemanticMakable<Langulus::Disowned, T> and ...);

      /// Check if all Decay<T> are clone-makable                             
      /// Does a deep copy                                                    
      /// If POD, Decay<T> can be clone-makable even if not                   
      /// clone-constructible, as long as it is std::copy_constuctible        
      template<class...T>
      concept CloneMakable = (SemanticMakable<Langulus::Cloned, T> and ...);

      /// Check if all T are abandon-makable                                  
      /// Does a move, but doesn't fully reset source (optimization)          
      /// T can be abandon-makable even if not abandon-constructible,         
      /// as long as it is std::move_constuctible                             
      template<class...T>
      concept AbandonMakable = (SemanticMakable<Langulus::Abandoned, T> and ...);

      /// Check if all T are refer-makable                                    
      /// Refering does a shallow copy while referencing contents, providing  
      /// ownership.                                                          
      /// T can be refer-makable as long as it is std::copy_constuctible      
      template<class...T>
      concept ReferMakable = (SemanticMakable<Langulus::Referred, T> and ...);
      
      /// Check if all T are copy-makable                                     
      /// Does a shallow copy _of the contents_ (like shallow cloning).       
      /// If POD, T can be copy-makable even if not copy-constructible, as    
      /// long as it is std::copy_constuctible                                
      template<class...T>
      concept CopyMakable = (SemanticMakable<Langulus::Copied, T> and ...);

      /// Check if all T are move-makable                                     
      /// Does a move, fully resetting source                                 
      /// T is move-makable as long as it is std::move_constuctible           
      template<class...T>
      concept MoveMakable = (SemanticMakable<Langulus::Moved, T> and ...);


      /// Check if all T are semantic-assignable by S                         
      /// T can be semantic-assignable even if not having an explicit assigner
      /// as long as T and S are compatible with C++20 semantics              
      ///   @tparam S - the semantic                                          
      ///   @tparam T... - the types                                          
      template<template<class> class S, class...T>
      concept SemanticAssignable = Semantic<S<T>...> and (requires {
            {SemanticAssign<true>(Fake<T&>(), Fake<S<T>&&>())} -> Supported;
         } and ...);

      /// Check if all TypeOf<S> are semantic-assignable by S                 
      /// T can be semantic-assignable even if not having an explicit assigner
      /// as long as T and S are compatible with standard C++20 semantics     
      ///   @tparam S - the semantic and type                                 
      template<class...S>
      concept SemanticAssignableAlt = Semantic<S...> and (requires {
            {SemanticAssign<true>(Fake<TypeOf<S>&>(), Fake<S&&>())} -> Supported;
         } and ...);

      /// Check if all T are disown-assignable                                
      /// Disowning does a shallow copy without referencing contents,         
      /// generating a 'view' of the data that is without ownership.          
      /// If POD, T can be disown-assignable even if not having an explicit   
      /// disown-assignment, as long as std::copy_assignable<T> holds         
      template<class...T>
      concept DisownAssignable = (SemanticAssignable<Langulus::Disowned, T> and ...);

      /// Check if all Decay<T> are clone-assignable                          
      /// Does a deep copy                                                    
      /// If POD, Decay<T> can be clone-assignable even if not having an      
      /// explicit clone-assignment, as long as std::copy_assignable<T> holds 
      template<class...T>
      concept CloneAssignable = (SemanticAssignable<Langulus::Cloned, T> and ...);

      /// Check if all T are abandon-assignable                               
      /// Does a move, but doesn't fully reset source (optimization)          
      /// T can be abandon-assignable even if not having an explicit          
      /// abandon-assignment, as long as std::move_assignable<T> holds        
      template<class...T>
      concept AbandonAssignable = (SemanticAssignable<Langulus::Abandoned, T> and ...);

      /// Check if all T are refer-assignable                                 
      /// Refering does a shallow copy while referencing contents, providing  
      /// ownership.                                                          
      /// T can be refer-assignable as long as std::copy_assignable<T> holds  
      template<class...T>
      concept ReferAssignable = (SemanticAssignable<Langulus::Referred, T> and ...);
      
      /// Check if all T are copy-assignable                                  
      /// Does a shallow copy _of the contents_ (like shallow cloning).       
      /// If POD, T can be copy-assignable even if not having an explicit     
      /// copy-assigner, as long as std::copy_assignable<T> holds             
      template<class...T>
      concept CopyAssignable = (SemanticAssignable<Langulus::Copied, T> and ...);

      /// Check if all T are move-assignable                                  
      /// Does a move, fully resetting source                                 
      /// T is move-assignable as long as std::move_assignable<T> holds       
      /// @attention you can't have move semantics, if a type has its         
      ///   destructor deleted - every time you move an instance, the old one 
      ///   has to be deleted later.                                          
      template<class...T>
      concept MoveAssignable = (SemanticAssignable<Langulus::Moved, T> and ...);


      /// Check if the T is descriptor-makable                                
      template<class...T>
      concept DescriptorMakable = not Abstract<T...> and not Enum<T...>
          and requires (const Anyness::Neat& a) { (T (Describe {a}), ...); };

      /// Check if the T is noexcept-descriptor-makable                       
      template<class...T>
      concept DescriptorMakableNoexcept = DescriptorMakable<T...>
          and (noexcept ( T (Describe {Fake<const Anyness::Neat&>()})) and ...);

   } // namespace Langulus::CT


   /// Downcasts a typed wrapper to the contained element, if cast operator   
   /// to TypeOf<T>& is available                                             
   ///   @param what - the instance to decay                                  
   ///   @return a reference to the the inner data                            
   NOD() LANGULUS(INLINED)
   constexpr auto& DecayCast(auto&& what) noexcept {
      using T = decltype(what);
      if constexpr (CT::Typed<T>) {
         using TT = TypeOf<T>;
         if constexpr (requires { what.DecayCast(); })
            return what.DecayCast();
         else if constexpr (requires { what.operator TT&& (); })
            return what.operator TT&& ();
         else if constexpr (requires { what.operator TT& (); })
            return what.operator TT& ();
         else if constexpr (requires { what.operator const TT& (); })
            return what.operator const TT& ();
         else
            LANGULUS_ERROR("No cast operator available for decaying to inner type");
      }
      else return what;
   }
   
   /// Decay a semantic to the contained instance                             
   ///   @param what - the instance to decay                                  
   ///   @return a reference (preferably) or a copy of the inner data         
   NOD() LANGULUS(INLINED)
   constexpr auto& DesemCast(auto&& what) noexcept {
      using T = decltype(what);
      if constexpr (CT::Semantic<T>)
         return DecayCast(Forward<T>(what));
      else
         return what;
   }

} // namespace Langulus
