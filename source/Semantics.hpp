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

         /// Semantic types are ephemeral and should never be reassignable    
         constexpr Semantic() noexcept = default;
         constexpr Semantic(const Semantic&) noexcept = default;
         constexpr Semantic(Semantic&&) noexcept = default;

         auto operator = (const Semantic&) = delete;
         auto operator = (Semantic&&) = delete;
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
      template<class...T>
      concept Semantic = (DerivedFrom<T, A::Semantic> and ...);

      /// Checks if a type is not a semantic                                  
      template<class...T>
      concept NotSemantic = not Semantic<T...>;

      /// Checks if a type is a shallow semantic                              
      template<class...T>
      concept ShallowSemantic = (DerivedFrom<T, A::ShallowSemantic> and ...);

      /// Checks if a type is a deep semantic                                 
      template<class...T>
      concept DeepSemantic = (DerivedFrom<T, A::DeepSemantic> and ...);

      /// Check if a type is a shallow-copy semantic                          
      template<class...T>
      concept Copied = (DerivedFrom<T, A::Copied> and ...);

      /// Check if a type is a shallow-move semantic                          
      template<class...T>
      concept Moved = (DerivedFrom<T, A::Moved> and ...);

      /// Check if a type is a shallow-abandon-move semantic                  
      template<class...T>
      concept Abandoned = (DerivedFrom<T, A::Abandoned> and ...);

      /// Check if a type is shallow-disowned-copy                            
      template<class...T>
      concept Disowned = (DerivedFrom<T, A::Disowned> and ...);

      /// Check if a type is clone (deep-copy) semantic                       
      template<class...T>
      concept Cloned = (DerivedFrom<T, A::Cloned> and ...);

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
      constexpr Copied(const Copied&) noexcept = default;
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

      /// Implicitly collapse the semantic, when applying it to fundamentals  
      /// This way this wrapper is seamlessly integrated with the standard    
      /// C++20 semantics                                                     
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

      /// Implicitly collapse the semantic, when applying it to fundamentals  
      /// This way this wrapper is seamlessly integrated with the standard    
      /// C++20 semantics                                                     
      LANGULUS(INLINED)
      constexpr operator T&& () const noexcept requires (not CT::Inner::Destroyable<T>) {
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
      /// since they never have ownership                                     
      LANGULUS(INLINED)
      constexpr operator const T& () const noexcept requires CT::Inner::POD<T> {
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
      constexpr operator const T& () const noexcept requires CT::Inner::POD<T> {
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
      LANGULUS_ASSUME(DevAssumes, placement, "Invalid placement pointer");

      if constexpr (CT::Inner::Abstract<T>) {
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
      else if constexpr (not S<T>::Shallow and not CT::Void<Decay<T>>) {
         // Clone                                                       
         using DT = Decay<T>;
         if constexpr (requires { new DT (Clone(DenseCast(*value))); })
            return new (placement) DT (Clone(DenseCast(*value)));
         else if constexpr (CT::Inner::POD<DT>) {
            // If type is POD (like fundamentals, or trivials), then    
            // it is always acceptable to clone by memcpy               
            ::std::memcpy(placement, &DenseCast(*value), sizeof(DT));
            return reinterpret_cast<DT*>(placement);
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
   ///   @attention when S is a deep semantic, like Cloned, this function     
   ///              will DenseCast 'lhs' and 'rhs', and copy only dense data  
   ///   @param lhs - left hand side (what are we assigning to)               
   ///   @param rhs - right hand side (what are we assigning)                 
   ///   @return whatever the assignment operator returns                     
   template<bool FAKE = false, template<class> class S, CT::NotSemantic T, class MT = Decvq<T>>
   requires CT::Semantic<S<T>> LANGULUS(INLINED)
   constexpr decltype(auto) SemanticAssign(MT& lhs, S<T>&& rhs) {
      if constexpr (CT::Reference<MT>) {
         // Can't reassign a reference                                  
         if constexpr (FAKE)
            return Inner::Unsupported {};
         else
            LANGULUS_ERROR("Can't SemanticAssign at a reference");
      }
      else if constexpr (S<T>::Move) {
         if constexpr (not S<T>::Keep) {
            // Abandon                                                  
            if constexpr (requires(MT a) { a = Abandon(*rhs); })
               return (lhs = Abandon(*rhs));
            else if constexpr (not CT::Inner::Destroyable<T>) {
               // If type is not destroyable (like fundamentals), then  
               // it is always acceptable to abandon them - just use    
               // the standard move-semantics                           
               if constexpr (requires(MT& a) { a = ::std::move(*rhs); })
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
            if constexpr (requires(MT& a) { a = Move(*rhs); })
               return (lhs = Move(*rhs));
            else if constexpr (requires(MT& a) { a = ::std::move(*rhs); })
               return (lhs = ::std::move(*rhs));
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't move-assign type");
         }
      }
      else {
         if constexpr (not S<T>::Shallow and not CT::Void<Decay<T>>) {
            // Clone                                                    
            using DT = Decay<T>;
            if constexpr (CT::Mutable<decltype(DenseCast(lhs))>) {
               if constexpr (requires(DT& a) { a = Clone(DenseCast(*rhs)); })
                  return (DenseCast(lhs) = Clone(DenseCast(*rhs)));
               else if constexpr (CT::Inner::POD<DT>) {
                  // If type is POD (like fundamentals, or trivials),   
                  // then it is always acceptable to clone by memcpy    
                  ::std::memcpy(&DenseCast(lhs), &DenseCast(*rhs), sizeof(DT));
                  return (lhs);
               }
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
         else if constexpr (not S<T>::Keep) {
            // Disown                                                   
            if constexpr (requires(MT& a) { a = Disown(*rhs); })
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
            if constexpr (requires(MT& a) { a = Copy(*rhs); })
               return (lhs = Copy(*rhs));
            else if constexpr (requires(MT& a) { a = *rhs; })
               return (lhs = *rhs);
            else if constexpr (FAKE)
               return Inner::Unsupported {};
            else
               LANGULUS_ERROR("Can't copy-assign type");
         }
      }
   }

   /// Deduce the proper semantic type, based on whether T already has a      
   /// specified semantic, or is an rvalue (&&) or not                        
   template<class T>
   using SemanticOf = Conditional<CT::Semantic<T>, Decay<T>, 
      Conditional<::std::is_rvalue_reference_v<T> and CT::Mutable<Deref<T>>,
         Moved<Deref<T>>, Copied<Deref<T>>>>;

   /// Remove the semantic from a type, or just return the type, if not       
   /// wrapped inside a semantic                                              
   template<class T>
   using Desem = Conditional<CT::Semantic<T>, TypeOf<T>, T>;

} // namespace Langulus

namespace Langulus::CT
{
   
   /// Check if T is constructible with each of the provided arguments        
   /// Notice, that this differs from std::constructible_from, by             
   /// attempting each argument separately                                    
   template<class T, class...A>
   concept MakableFrom = (::std::constructible_from<T, A&&> and ...);

   /// Check if T is assignable with each of the provided arguments           
   template<class T, class...A>
   concept AssignableFrom = (::std::assignable_from<T, A&&> and ...);

   namespace Inner
   {

      /// Check if T is semantic-constructible by S                           
      template<template<class> class S, class...T>
      concept SemanticMakable = Semantic<S<T>...> and (requires {
             {SemanticNew<true>(nullptr, Fake<S<T>&&>())} -> Supported;
          } and ...);

      template<class...S>
      concept SemanticMakableAlt = Semantic<S...> and (requires {
             {SemanticNew<true>(nullptr, Fake<S&&>())} -> Supported;
          } and ...);

      /// Check if T is semantic-assignable                                   
      template<template<class> class S, class...T>
      concept SemanticAssignable = Semantic<S<T>...> and (requires {
             {SemanticAssign<true>(Fake<T&>(), Fake<S<T>&&>())} -> Supported;
          } and ...);

      template<class...S>
      concept SemanticAssignableAlt = Semantic<S...> and (requires {
             {SemanticAssign<true>(Fake<TypeOf<S>&>(), Fake<S&&>())} -> Supported;
          } and ...);


      /// Check if T is disown-constructible                                  
      template<class...T>
      concept DisownMakable = (SemanticMakable<Langulus::Disowned, T> and ...);

      /// Check if T is clone-constructible                                   
      template<class...T>
      concept CloneMakable = (SemanticMakable<Langulus::Cloned, T> and ...);

      /// Check if T is abandon-constructible                                 
      template<class...T>
      concept AbandonMakable = (SemanticMakable<Langulus::Abandoned, T> and ...);

      /// Check if T is copy-constructible                                    
      template<class...T>
      concept CopyMakable = (SemanticMakable<Langulus::Copied, T> and ...);

      /// Check if T is move-constructible                                    
      template<class...T>
      concept MoveMakable = (SemanticMakable<Langulus::Moved, T> and ...);


      /// Check if T is disown-assignable if mutable                          
      template<class...T>
      concept DisownAssignable = (SemanticAssignable<Langulus::Disowned, T> and ...);

      /// Check if T is clone-assignable if mutable                           
      template<class...T>
      concept CloneAssignable = (SemanticAssignable<Langulus::Cloned, T> and ...);

      /// Check if T is abandon-assignable if mutable                         
      template<class...T>
      concept AbandonAssignable = (SemanticAssignable<Langulus::Abandoned, T> and ...);

      /// Check if the T is copy-assignable                                   
      template<class...T>
      concept CopyAssignable = (SemanticAssignable<Langulus::Copied, T> and ...);

      /// Check if the T is move-assignable                                   
      template<class...T>
      concept MoveAssignable = (SemanticAssignable<Langulus::Moved, T> and ...);


      /// Check if the T is descriptor-constructible                          
      template<class...T>
      concept DescriptorMakable = not Abstract<T...> and not Enum<T...>
          and requires (const Anyness::Neat& a) { (T {Describe {a}}, ...); };

      /// Check if the T is noexcept-descriptor-constructible                 
      template<class...T>
      concept DescriptorMakableNoexcept = DescriptorMakable<T...>
          and (noexcept ( T {Describe {Fake<const Anyness::Neat&>()}}) and ...);

   } // namespace Langulus::CT::Inner


   /// Check if origin T is semantic-constructible by semantic S              
   template<template<class> class S, class...T>
   concept SemanticMakable = Complete<Decay<T>...>
       and Inner::SemanticMakable<S, Decay<T>...>;

   template<class...S>
   concept SemanticMakableAlt = Complete<Decay<TypeOf<S>>...>
       and Inner::SemanticMakableAlt<S...>;

   /// Check if origin T is semantic-assignable by semantic S                 
   template<template<class> class S, class...T>
   concept SemanticAssignable = Complete<Decay<T>...>
       and Inner::SemanticAssignable<S, Decay<T>...>;

   template<class...S>
   concept SemanticAssignableAlt = Complete<Decay<TypeOf<S>>...>
       and Inner::SemanticAssignableAlt<S...>;


   /// Check if origin T is disown-constructible                              
   template<class...T>
   concept DisownMakable = SemanticMakable<Langulus::Disowned, T...>;

   /// Check if origin T is clone-constructible                               
   template<class...T>
   concept CloneMakable = SemanticMakable<Langulus::Cloned, T...>;

   /// Check if origin T is abandon-constructible                             
   template<class...T>
   concept AbandonMakable = SemanticMakable<Langulus::Abandoned, T...>;
   
   /// Check if origin T is copy-constructible                                
   template<class...T>
   concept CopyMakable = SemanticMakable<Langulus::Copied, T...>;

   /// Check if origin T is move-constructible                                
   template<class...T>
   concept MoveMakable = SemanticMakable<Langulus::Moved, T...>;


   /// Check if origin T is disown-assignable if mutable                      
   template<class...T>
   concept DisownAssignable = SemanticAssignable<Langulus::Disowned, T...>;

   /// Check if origin T is clone-assignable if mutable                       
   template<class...T>
   concept CloneAssignable = SemanticAssignable<Langulus::Cloned, T...>;

   /// Check if origin T is abandon-assignable if mutable                     
   template<class...T>
   concept AbandonAssignable = SemanticAssignable<Langulus::Abandoned, T...>;

   /// Check if origin T is copy-assignable                                   
   template<class...T>
   concept CopyAssignable = SemanticAssignable<Langulus::Copied, T...>;

   /// Check if origin T is move-assignable                                   
   template<class...T>
   concept MoveAssignable = SemanticAssignable<Langulus::Moved, T...>;


   /// Check if the origin T is descriptor-constructible                      
   template<class...T>
   concept DescriptorMakable = Complete<Decay<T>...>
       and Inner::DescriptorMakable<Decay<T>...>;

   /// Check if the origin T is noexcept-descriptor-constructible             
   template<class...T>
   concept DescriptorMakableNoexcept = Complete<Decay<T>...>
       and Inner::DescriptorMakableNoexcept<Decay<T>...>;

   namespace Inner
   {

      /// Unfolds T, if it is a bounded array, or std::range, and returns     
      /// a nullptr pointer of the type, contained inside. Nested for ranges  
      /// containing other ranges, or arrays containing ranges                
      ///   @tparam T - type to unfold                                        
      ///   @return a pointer of the most inner type                          
      template<class T>
      constexpr auto Unfold() noexcept {
         if constexpr (CT::Sparse<T>) {
            if constexpr (CT::Array<T>)
               return Unfold<Deext<T>>();
            else
               return (Deref<T>*) nullptr;
         }
         else if constexpr (::std::ranges::range<T>)
            return Unfold<TypeOf<T>>();
         else 
            return (Deref<T>*) nullptr;
      }

   } // namespace Langulus::CT::Inner

} // namespace Langulus::CT

namespace Langulus
{

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
            return what.operator TT && ();
         else if constexpr (requires { what.operator TT& (); })
            return what.operator TT & ();
         else if constexpr (requires { what.operator const TT& (); })
            return what.operator const TT & ();
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
      else return what;
   }

   /// Nest-unfold any bounded array or std::range, and get most inner type   
   template<class T>
   using Unfold = Deptr<decltype(CT::Inner::Unfold<T>())>;

   namespace CT::Inner
   {

      /// Check if T is constructible with each of the provided arguments,    
      /// either directly, or by unfolding that argument                      
      template<class T, class...A>
      concept UnfoldMakableFrom = ((
               ::std::constructible_from<T, A>
            or ::std::constructible_from<T, Langulus::Unfold<A>>
         ) and ...);

      /// Check if T is insertable to containers, either directly, or while   
      /// wrapped in a semantic                                               
      template<class T1, class...TAIL>
      concept UnfoldInsertable = Insertable<Desem<T1>, Desem<TAIL>...>;

   } // namespace Langulus::CT::Inner

} // namespace Langulus
