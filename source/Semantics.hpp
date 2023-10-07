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
      explicit constexpr Copied(const T& value) noexcept
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as copied                                                   
      template<class ALT_T = T>
      NOD() constexpr Copied<ALT_T> Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         return Copied<ALT_T>{mValue};
      }

      /// Copy something else                                                 
      template<class ALT_T>
      NOD() static constexpr Copied<ALT_T> Nest(const ALT_T& value) noexcept {
         return Copied<ALT_T>{value};
      }

      template<class ALT_T>
      using Nested = Copied<ALT_T>;

      LANGULUS(INLINED)
      const T& operator * () const noexcept {
         return mValue;
      }

      LANGULUS(INLINED)
      const T* operator -> () const noexcept {
         return &mValue;
      }
   };
   
   /// Copy a value                                                           
   template<CT::NotSemantic T>
   NOD() LANGULUS(INLINED) 
   constexpr auto Copy(const T& item) noexcept {
      return Copied<T>{item};
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

      static_assert(not ::std::is_const_v<T>,
         "T must be mutable in order to be moved");

      Moved() = delete;
      Moved(const Moved&) = delete;
      explicit constexpr Moved(Moved&&) noexcept = default;
      explicit constexpr Moved(T&& value) noexcept
         : mValue {::std::forward<T>(value)} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }

      /// Forward as moved                                                    
      template<class ALT_T = T>
      NOD() constexpr Moved<ALT_T> Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         return Moved<ALT_T>{::std::forward<ALT_T>(mValue)};
      }

      /// Move something else                                                 
      template<class ALT_T>
      NOD() static constexpr Moved<ALT_T> Nest(ALT_T& value) noexcept {
         return Moved<ALT_T>{::std::move(value)};
      }

      template<class ALT_T>
      NOD() static constexpr Moved<ALT_T> Nest(ALT_T&& value) noexcept {
         return Moved<ALT_T>{::std::forward<ALT_T>(value)};
      }

      template<class ALT_T>
      using Nested = decltype(Nest(Fake<ALT_T>()));

      LANGULUS(INLINED)
      T& operator * () const noexcept {
         return mValue;
      }

      LANGULUS(INLINED)
      T* operator -> () const noexcept {
         return &mValue;
      }
   };
   
   /// Move data                                                              
   template<CT::NotSemantic T>
   NOD() LANGULUS(INLINED)
   constexpr auto Move(T&& a) noexcept {
      return Moved<T> {::std::forward<T>(a)};
   }

   /// Move data                                                              
   template<CT::NotSemantic T>
   NOD() LANGULUS(INLINED)
   constexpr auto Move(T& a) noexcept {
      return Moved<T> {::std::move(a)};
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

      static_assert(not ::std::is_const_v<T>,
         "T must be mutable in order to be abandoned");

      Abandoned() = delete;
      Abandoned(const Abandoned&) = delete;
      explicit constexpr Abandoned(Abandoned&&) noexcept = default;
      explicit constexpr Abandoned(T&& value) noexcept 
         : mValue {::std::forward<T>(value)} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as abandoned                                                
      template<class ALT_T = T>
      NOD() constexpr Abandoned<ALT_T> Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         return Abandoned<ALT_T>{::std::forward<ALT_T>(mValue)};
      }

      /// Abandon something else                                              
      template<class ALT_T>
      NOD() static constexpr Abandoned<ALT_T> Nest(ALT_T& value) noexcept {
         return Abandoned<ALT_T>{::std::move(value)};
      }

      template<class ALT_T>
      NOD() static constexpr Abandoned<ALT_T> Nest(ALT_T&& value) noexcept {
         return Abandoned<ALT_T>{::std::forward<ALT_T>(value)};
      }

      template<class ALT_T>
      using Nested = decltype(Nest(Fake<ALT_T>()));

      LANGULUS(INLINED)
      T& operator * () const noexcept {
         return mValue;
      }

      LANGULUS(INLINED)
      T* operator -> () const noexcept {
         return &mValue;
      }
   };
   
   /// Abandon a value                                                        
   /// Same as Move, but resets only mandatory data inside source after move  
   /// essentially saving up on a couple of instructions                      
   template<CT::NotSemantic T>
   NOD() LANGULUS(INLINED)
   constexpr auto Abandon(T&& a) noexcept {
      return Abandoned<T>{::std::forward<T>(a)};
   }

   /// Abandon a value                                                        
   /// Same as Move, but resets only mandatory data inside source after move  
   /// essentially saving up on a couple of instructions                      
   template<CT::NotSemantic T>
   NOD() LANGULUS(INLINED)
   constexpr auto Abandon(T& a) noexcept {
      return Abandoned<T>{::std::move(a)};
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
      explicit constexpr Disowned(const T& value) noexcept 
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as disowned                                                 
      template<class ALT_T = T>
      NOD() constexpr Disowned<ALT_T> Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         return Disowned<ALT_T>{mValue};
      }

      /// Disown something else                                               
      template<class ALT_T>
      NOD() static constexpr Disowned<ALT_T> Nest(const ALT_T& value) noexcept {
         return Disowned<ALT_T>{value};
      }

      template<class ALT_T>
      using Nested = Disowned<ALT_T>;

      LANGULUS(INLINED)
      const T& operator * () const noexcept {
         return mValue;
      }

      LANGULUS(INLINED)
      const T* operator -> () const noexcept {
         return &mValue;
      }
   };
   
   /// Disown a value                                                         
   /// Same as a shallow-copy, but never references, saving some instructions 
   template<CT::NotSemantic T>
   NOD() LANGULUS(INLINED)
   constexpr auto Disown(const T& item) noexcept {
      return Disowned<T>{item};
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
      explicit constexpr Cloned(const T& value) noexcept
         : mValue {value} {
         static_assert(CT::NotSemantic<T>, "Can't nest semantics");
      }
      
      /// Forward as cloned                                                   
      template<class ALT_T = T>
      NOD() constexpr Cloned<ALT_T> Forward() const noexcept {
         static_assert(CT::NotSemantic<ALT_T>, "Can't nest semantics");
         return Cloned<ALT_T>{mValue};
      }

      /// Clone something else                                                
      template<class ALT_T>
      NOD() static constexpr Cloned<ALT_T> Nest(const ALT_T& value) noexcept {
         return Cloned<ALT_T>{value};
      }

      template<class ALT_T>
      using Nested = Cloned<ALT_T>;
      
      LANGULUS(INLINED)
      const T& operator * () const noexcept {
         return mValue;
      }

      LANGULUS(INLINED)
      const T* operator -> () const noexcept {
         return &mValue;
      }
   };
   
   /// Clone a value                                                          
   template<CT::NotSemantic T>
   NOD() LANGULUS(INLINED)
   constexpr auto Clone(const T& item) noexcept {
      return Cloned<T>{item};
   }


   /// Create an element on the stack, using the provided semantic            
   ///   @tparam T - the type to instantiate                                  
   ///   @tparam S - the semantic to use (deducible)                          
   ///   @param value - the constructor argument and the semantic             
   ///   @return the instance on the stack                                    
   template<CT::NotSemantic T, CT::Semantic S>
   NOD() LANGULUS(INLINED)
   auto SemanticMake(S&& value) {
      static_assert(CT::Exact<TypeOf<S>, T>,
         "S type must be exactly T (build-time optimization)");

      if constexpr (CT::Complete<T>) {
         using A = TypeOf<S>;

         if constexpr (S::Move) {
            if constexpr (not S::Keep and ::std::constructible_from<T, Abandoned<A>&&>)
               return T {Abandon(*value)};
            else if constexpr (::std::constructible_from<T, Moved<A>&&>)
               return T {Move(*value)};
            else if constexpr (::std::constructible_from<T, A&&>)
               return T {::std::move(*value)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return T {Copy(*value)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return T {*value};
            else
               return Inner::Unsupported {};
         }
         else {
            if constexpr (not S::Shallow and ::std::constructible_from<T, Cloned<A>&&>)
               return T {Clone(*value)};
            else if constexpr (not S::Keep and ::std::constructible_from<T, Disowned<A>&&>)
               return T {Disown(*value)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return T {Copy(*value)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return T {*value};
            else
               return Inner::Unsupported {};
         }
      }
      else return Inner::Unsupported {};
   }

   /// Create an element on the heap, using the provided semantic             
   ///   @tparam T - the type to instantiate                                  
   ///   @tparam S - the semantic to use (deducible)                          
   ///   @param value - the constructor arguments and the semantic            
   ///   @return the instance on the heap                                     
   template<CT::NotSemantic T, CT::Semantic S>
   NOD() LANGULUS(INLINED)
   auto SemanticNew(S&& value) {
      static_assert(CT::Exact<TypeOf<S>, T>,
         "S type must be exactly T (build-time optimization)");

      if constexpr (CT::Complete<T>) {
         using A = TypeOf<S>;

         if constexpr (S::Move) {
            if constexpr (not S::Keep and ::std::constructible_from<T, Abandoned<A>&&>)
               return new T {Abandon(*value)};
            else if constexpr (::std::constructible_from<T, Moved<A>&&>)
               return new T {Move(*value)};
            else if constexpr (::std::constructible_from<T, A&&>)
               return new T {::std::move(*value)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return new T {Copy(*value)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return new T {*value};
            else
               return Inner::Unsupported {};
         }
         else {
            if constexpr (not S::Shallow and ::std::constructible_from<T, Cloned<A>&&>)
               return new T {Clone(*value)};
            else if constexpr (not S::Keep and ::std::constructible_from<T, Disowned<A>&&>)
               return new T {Disown(*value)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return new T {Copy(*value)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return new T {*value};
            else
               return Inner::Unsupported {};
         }
      }
      else return Inner::Unsupported {};
   }

   /// Create an element on the heap, using the provided semantic, by using   
   /// a placement new variant                                                
   ///   @attention assumes placement pointer is valid                        
   ///   @tparam T - the type to instantiate                                  
   ///   @tparam S - the semantic to use (deducible)                          
   ///   @param placement - where to place the new instance                   
   ///   @param value - the constructor arguments and the semantic            
   ///   @return the instance on the heap                                     
   template<CT::NotSemantic T, CT::Semantic S>
   LANGULUS(INLINED)
   auto SemanticNew(void* placement, S&& value) {
      static_assert(CT::Exact<TypeOf<S>, T>,
         "S type must be exactly T (build-time optimization)");

      if constexpr (CT::Complete<T>) {
         LANGULUS_ASSUME(DevAssumes, placement, "Invalid placement pointer");

         using A = TypeOf<S>;

         if constexpr (S::Move) {
            if constexpr (not S::Keep and ::std::constructible_from<T, Abandoned<A>&&>)
               return new (placement) T {Abandon(*value)};
            else if constexpr (::std::constructible_from<T, Moved<A>&&>)
               return new (placement) T {Move(*value)};
            else if constexpr (::std::constructible_from<T, A&&>)
               return new (placement) T {::std::move(*value)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return new (placement) T {Copy(*value)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return new (placement) T {*value};
            else
               return Inner::Unsupported {};
         }
         else {
            if constexpr (not S::Shallow and ::std::constructible_from<T, Cloned<A>&&>)
               return new (placement) T {Clone(*value)};
            else if constexpr (not S::Keep and ::std::constructible_from<T, Disowned<A>&&>)
               return new (placement) T {Disown(*value)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return new (placement) T {Copy(*value)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return new (placement) T {*value};
            else
               return Inner::Unsupported {};
         }
      }
      else return Inner::Unsupported {};
   }
   
   /// Create an element on the heap, using the provided semantic, by using   
   /// a placement new variant, relying on the reflected constructors         
   ///   @attention assumes type is valid and complete                        
   ///   @attention assumes bit placement and TypeOf<S> are valid pointers    
   ///              that point to an instance of the provided type            
   ///   @tparam S - the semantic to use (deducible)                          
   ///   @param type - the reflected type to instantiate                      
   ///   @param placement - where to place the new instance                   
   ///   @param value - the constructor arguments and the semantic            
   template<CT::Semantic S>
   LANGULUS(INLINED)
   void SemanticNewUnknown(RTTI::DMeta type, Byte* placement, S&& value) {
      static_assert(
         CT::Exact<TypeOf<S>, Byte*> or
         CT::Exact<TypeOf<S>, const Byte*>,
         "Bad argument"
      );

      LANGULUS_ASSUME(DevAssumes, type, "Invalid type");
      LANGULUS_ASSUME(DevAssumes, placement, "Invalid placement pointer");
      LANGULUS_ASSUME(DevAssumes, *value, "Invalid argument pointer");

      if constexpr (S::Shallow) {
         // Abandon/Disown/Move/Copy                                    
         if constexpr (S::Move) {
            if constexpr (not S::Keep) {
               if (type->mAbandonConstructor) {
                  type->mAbandonConstructor(*value, placement);
                  return;
               }
            }
            else if (type->mMoveConstructor) {
               type->mMoveConstructor(*value, placement);
               return;
            }
         }
         else {
            if constexpr (not S::Keep) {
               if (type->mDisownConstructor) {
                  type->mDisownConstructor(*value, placement);
                  return;
               }
            }
         }

         if (type->mCopyConstructor)
            type->mCopyConstructor(*value, placement);
         else
            LANGULUS_THROW(Construct, "Can't abandon/disown/move/copy-construct T from S");
      }
      else {
         // Clone/Copy                                                  
         if (type->mCloneConstructor)
            type->mCloneConstructor(*value, placement);
         else if (type->mCopyConstructor)
            type->mCopyConstructor(*value, placement);
         else
            LANGULUS_THROW(Construct, "Can't clone/copy-construct T from S");
      }
   }

   /// Assign new value to an instance of T, using the provided semantic      
   ///   @tparam T - the type to assign to (deducible)                        
   ///   @tparam S - the semantic to use (deducible)                          
   ///   @param lhs - left hand side (what are we assigning to)               
   ///   @param rhs - right hand side (what are we assigning)                 
   ///   @return whatever the assignment operator returns                     
   template<CT::NotSemantic T, CT::Semantic S>
   LANGULUS(INLINED)
   decltype(auto) SemanticAssign(T& lhs, S&& rhs) {
      static_assert(CT::Exact<TypeOf<S>, T>,
         "S type must be exactly T (build-time optimization)");

      if constexpr (S::Move) {
         if constexpr (not S::Keep and requires(T& a) { a = Abandon(*rhs); })
            return (lhs = Abandon(*rhs));
         else if constexpr (requires(T& a) { a = Move(*rhs); })
            return (lhs = Move(*rhs));
         else if constexpr (requires(T& a) { a = ::std::move(*rhs); })
            return (lhs = ::std::move(*rhs));
         else if constexpr (requires(T& a) { a = Copy(*rhs); })
            return (lhs = Copy(*rhs));
         else if constexpr (requires(T& a) { a = *rhs; })
            return (lhs = *rhs);
         else
            return Inner::Unsupported {};
      }
      else {
         if constexpr (not S::Shallow and requires(T& a) { a = Clone(*rhs); })
            return (lhs = Clone(*rhs));
         else if constexpr (not S::Keep and requires(T& a) { a = Disown(*rhs); })
            return (lhs = Disown(*rhs));
         else if constexpr (requires(T& a) { a = Copy(*rhs); })
            return (lhs = Copy(*rhs));
         else if constexpr (requires(T& a) { a = *rhs; })
            return (lhs = *rhs);
         else
            return Inner::Unsupported {};
      }
   }

} // namespace Langulus

namespace Langulus::CT
{
   namespace Inner
   {
      /// Check if T is disown-constructible                                  
      template<class T>
      concept DisownMakable = requires (const T& a) {
         {SemanticMake<T>(Disown(a))} -> Exact<T>;
      };

      /// Check if T is disown-assignable if mutable                          
      template<class T>
      concept DisownAssignable = requires (T& a) {
         {SemanticAssign(a, Disown(a))} -> Exact<T&>;
      };

      /// Check if T is clone-constructible                                   
      template<class T>
      concept CloneMakable = requires (const T& a) {
         {SemanticMake<T>(Clone(a))} -> Exact<T>;
      };

      /// Check if T is clone-assignable if mutable                           
      template<class T>
      concept CloneAssignable = requires (T& a) {
         {SemanticAssign(a, Clone(a))} -> Exact<T&>;
      };

      /// Check if T is abandon-constructible                                 
      template<class T>
      concept AbandonMakable = requires (T& a) {
         {SemanticMake<T>(Abandon(a))} -> Exact<T>;
      };

      /// Check if T is abandon-assignable if mutable                         
      template<class T>
      concept AbandonAssignable = requires (T& a) {
         {SemanticAssign(a, Abandon(a))} -> Exact<T&>;
      };

      /// Check if the decayed T is copy-constructible                        
      template<class T>
      concept CopyMakable = requires (const T& a) {
         {SemanticMake<T>(Copy(a))} -> Exact<T>;
      };

      /// Check if the decayed T is copy-assignable                           
      template<class T>
      concept CopyAssignable = requires (T& a) {
         {SemanticAssign(a, Copy(a))} -> Exact<T&>;
      };

      /// Check if the decayed T is move-constructible                        
      template<class T>
      concept MoveMakable = requires (T& a) {
         {SemanticMake<T>(Move(a))} -> Exact<T>;
      };

      /// Check if the decayed T is move-assignable                           
      template<class T>
      concept MoveAssignable = requires (T& a) {
         {SemanticAssign(a, Move(a))} -> Exact<T&>;
      };

      /// Check if T is semantic-constructible by S                           
      template<class S, class T>
      concept SemanticMakable = Semantic<S> and requires (T a) {
         {SemanticMake<T>(S {a})} -> Exact<T>;
      };

      /// Check if T is semantic-assignable if mutable                        
      template<class S, class T>
      concept SemanticAssignable = Semantic<S> and requires (T a) {
         {SemanticAssign(a, S {a})} -> Exact<T&>;
      };

   } // namespace Langulus::CT::Inner


   /// Check if origin T is disown-constructible                              
   template<class... T>
   concept DisownMakable = Complete<Decay<T>...>
       and (Inner::DisownMakable<Decay<T>> and ...);

   /// Check if origin T is disown-assignable if mutable                      
   template<class... T>
   concept DisownAssignable = Complete<Decay<T>...>
       and (Inner::DisownAssignable<Decay<T>> and ...);

   /// Check if origin T is clone-constructible                               
   template<class... T>
   concept CloneMakable = Complete<Decay<T>...>
       and (Inner::CloneMakable<Decay<T>> and ...);

   /// Check if origin T is clone-assignable if mutable                       
   template<class... T>
   concept CloneAssignable = Complete<Decay<T>...>
       and (Inner::CloneAssignable<Decay<T>> and ...);

   /// Check if origin T is abandon-constructible                             
   template<class... T>
   concept AbandonMakable = Complete<Decay<T>...>
       and (Inner::AbandonMakable<Decay<T>> and ...);

   /// Check if origin T is abandon-assignable if mutable                     
   template<class... T>
   concept AbandonAssignable = Complete<Decay<T>...>
       and (Inner::AbandonAssignable<Decay<T>> and ...);

   /// Check if origin T is copy-constructible                                
   template<class... T>
   concept CopyMakable = Complete<Decay<T>...>
       and (Inner::CopyMakable<Decay<T>> and ...);

   /// Check if origin T is copy-assignable                                   
   template<class... T>
   concept CopyAssignable = Complete<Decay<T>...>
       and (Inner::CopyAssignable<Decay<T>> and ...);
         
   /// Check if origin T is move-constructible                                
   template<class... T>
   concept MoveMakable = Complete<Decay<T>...>
       and (Inner::MoveMakable<Decay<T>> and ...);

   /// Check if origin T is move-assignable                                   
   template<class... T>
   concept MoveAssignable = Complete<Decay<T>...>
       and (Inner::MoveAssignable<Decay<T>> and ...);

   /// Check if origin T is semantic-constructible by semantic S              
   template<class S, class... T>
   concept SemanticMakable = Complete<Decay<T>...>
       and (Inner::SemanticMakable<S, Decay<T>> and ...);

   /// Check if origin T is semantic-assignable by semantic S, if mutable     
   template<class S, class... T>
   concept SemanticAssignable = Complete<Decay<T>...>
       and (Inner::SemanticAssignable<S, Decay<T>> and ...);

} // namespace Langulus::CT
