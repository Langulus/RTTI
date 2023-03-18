///                                                                           
/// Langulus::Core                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Reflection.hpp"
#include "Byte.hpp"

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
      concept Semantic = Decayed<T...> && (DerivedFrom<T, A::Semantic> && ...);

      /// Checks if a type has no special semantics                           
      template<class... T>
      concept NotSemantic = !Semantic<T...>;

      /// Check if a type is copied                                           
      template<class... T>
      concept Copied = Decayed<T...> && (DerivedFrom<T, A::Copied> && ...);

      /// Check if a type is moved                                            
      template<class... T>
      concept Moved = Decayed<T...> && (DerivedFrom<T, A::Moved> && ...);

      /// Check if a type is abandoned                                        
      template<class... T>
      concept Abandoned = Decayed<T...> && (DerivedFrom<T, A::Abandoned> && ...);

      /// Check if a type is disowned                                         
      template<class... T>
      concept Disowned = Decayed<T...> && (DerivedFrom<T, A::Disowned> && ...);

      /// Check if a type is cloned                                           
      template<class... T>
      concept Cloned = Decayed<T...> && (DerivedFrom<T, A::Cloned> && ...);
   }
   
   
   ///                                                                        
   /// Copied value intermediate type, use in constructors and assignments    
   /// to shallow-copy container explicitly                                   
   ///   @tparam T - the type to copy                                         
   template<class T>
   struct Copied : A::Copied {
      LANGULUS(TYPED) T;

      const T& mValue;

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
   };
   
   /// Copy a value                                                           
   template<CT::NotSemantic T>
   LANGULUS(INTRINSIC)
   NOD() constexpr auto Copy(const T& item) noexcept {
      return Copied<T>{item};
   }
   

   ///                                                                        
   /// Moved value intermediate type, use in constructors and assignments     
   /// to move data explicitly                                                
   ///   @tparam T - the type to move                                         
   template<class T>
   struct Moved : A::Moved {
      LANGULUS(TYPED) T;

      static_assert(!::std::is_const_v<T>,
         "T must be mutable in order to be moved");

      T&& mValue;

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
      using Nested = decltype(Nest(Uneval<ALT_T>()));
   };
   
   /// Move data                                                              
   template<CT::NotSemantic T>
   LANGULUS(INTRINSIC)
   NOD() constexpr auto Move(T&& a) noexcept {
      return Moved<T>{::std::forward<T>(a)};
   }

   /// Move data                                                              
   template<CT::NotSemantic T>
   LANGULUS(INTRINSIC)
   NOD() constexpr auto Move(T& a) noexcept {
      return Moved<T>{::std::move(a)};
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
      LANGULUS(TYPED) T;

      static_assert(!::std::is_const_v<T>,
         "T must be mutable in order to be abandoned");

      T&& mValue;

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
      using Nested = decltype(Nest(Uneval<ALT_T>()));
   };
   
   /// Abandon a value                                                        
   /// Same as Move, but resets only mandatory data inside source after move  
   /// essentially saving up on a couple of instructions                      
   template<CT::NotSemantic T>
   LANGULUS(INTRINSIC)
   NOD() constexpr auto Abandon(T&& a) noexcept {
      return Abandoned<T>{::std::forward<T>(a)};
   }

   /// Abandon a value                                                        
   /// Same as Move, but resets only mandatory data inside source after move  
   /// essentially saving up on a couple of instructions                      
   template<CT::NotSemantic T>
   LANGULUS(INTRINSIC)
   NOD() constexpr auto Abandon(T& a) noexcept {
      return Abandoned<T>{::std::move(a)};
   }


   ///                                                                        
   /// Disowned value intermediate type, use in constructors and assignments  
   /// to copy container without gaining ownership                            
   ///   @tparam T - the type to disown                                       
   template<class T>
   struct Disowned : A::Disowned {
      LANGULUS(TYPED) T;

      const T& mValue;

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
   };
   
   /// Disown a value                                                         
   /// Same as a shallow-copy, but never references, saving some instructions 
   template<CT::NotSemantic T>
   LANGULUS(INTRINSIC)
   NOD() constexpr auto Disown(const T& item) noexcept {
      return Disowned<T>{item};
   }
   

   ///                                                                        
   /// Cloned value intermediate type, use in constructors and assignments    
   /// to clone container, doing a deep copy instead of default shallow copy  
   ///   @tparam T - the type to clone                                        
   template<class T>
   struct Cloned : A::Cloned {
      LANGULUS(TYPED) T;

      const T& mValue;

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
   };
   
   /// Clone a value                                                          
   template<CT::NotSemantic T>
   LANGULUS(INTRINSIC)
   NOD() constexpr auto Clone(const T& item) noexcept {
      return Cloned<T>{item};
   }


   /// Create an element on the stack, using the provided semantic            
   ///   @tparam T - the type to instantiate                                  
   ///   @tparam S - the semantic to use (deducible)                          
   ///   @param value - the constructor argument and the semantic             
   ///   @return the instance on the stack                                    
   template<class T, CT::Semantic S>
   NOD() LANGULUS(ALWAYSINLINE)
   auto SemanticMake(S&& value) {
      if constexpr (CT::Complete<T>) {
         using A = TypeOf<S>;

         if constexpr (S::Move) {
            if constexpr (!S::Keep && ::std::constructible_from<T, Abandoned<A>&&>)
               return T {Abandon(value.mValue)};
            else if constexpr (::std::constructible_from<T, Moved<A>&&>)
               return T {Move(value.mValue)};
            else if constexpr (::std::constructible_from<T, A&&>)
               return T {::std::move(value.mValue)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return T {Copy(value.mValue)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return T {value.mValue};
            else
               return Inner::Unsupported {};
         }
         else {
            if constexpr (!S::Shallow && ::std::constructible_from<T, Cloned<A>&&>)
               return T {Clone(value.mValue)};
            else if constexpr (!S::Keep && ::std::constructible_from<T, Disowned<A>&&>)
               return T {Disown(value.mValue)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return T {Copy(value.mValue)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return T {value.mValue};
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
   template<class T, CT::Semantic S>
   NOD() LANGULUS(ALWAYSINLINE)
   auto SemanticNew(S&& value) {
      if constexpr (CT::Complete<T>) {
         using A = TypeOf<S>;

         if constexpr (S::Move) {
            if constexpr (!S::Keep && ::std::constructible_from<T, Abandoned<A>&&>)
               return new T {Abandon(value.mValue)};
            else if constexpr (::std::constructible_from<T, Moved<A>&&>)
               return new T {Move(value.mValue)};
            else if constexpr (::std::constructible_from<T, A&&>)
               return new T {::std::move(value.mValue)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return new T {Copy(value.mValue)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return new T {value.mValue};
            else
               return Inner::Unsupported {};
         }
         else {
            if constexpr (!S::Shallow && ::std::constructible_from<T, Cloned<A>&&>)
               return new T {Clone(value.mValue)};
            else if constexpr (!S::Keep && ::std::constructible_from<T, Disowned<A>&&>)
               return new T {Disown(value.mValue)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return new T {Copy(value.mValue)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return new T {value.mValue};
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
   template<class T, CT::Semantic S>
   LANGULUS(ALWAYSINLINE)
   auto SemanticNew(void* placement, S&& value) {
      if constexpr (CT::Complete<T>) {
         LANGULUS_ASSUME(DevAssumes, placement, "Invalid placement pointer");

         using A = TypeOf<S>;

         if constexpr (S::Move) {
            if constexpr (!S::Keep && ::std::constructible_from<T, Abandoned<A>&&>)
               return new (placement) T {Abandon(value.mValue)};
            else if constexpr (::std::constructible_from<T, Moved<A>&&>)
               return new (placement) T {Move(value.mValue)};
            else if constexpr (::std::constructible_from<T, A&&>)
               return new (placement) T {::std::move(value.mValue)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return new (placement) T {Copy(value.mValue)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return new (placement) T {value.mValue};
            else
               return Inner::Unsupported {};
         }
         else {
            if constexpr (!S::Shallow && ::std::constructible_from<T, Cloned<A>&&>)
               return new (placement) T {Clone(value.mValue)};
            else if constexpr (!S::Keep && ::std::constructible_from<T, Disowned<A>&&>)
               return new (placement) T {Disown(value.mValue)};
            else if constexpr (::std::constructible_from<T, Copied<A>>)
               return new (placement) T {Copy(value.mValue)};
            else if constexpr (::std::constructible_from<T, const A&>)
               return new (placement) T {value.mValue};
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
   LANGULUS(ALWAYSINLINE)
   void SemanticNewUnknown(RTTI::DMeta type, Byte* placement, S&& value) {
      static_assert(
         CT::Exact<TypeOf<S>, Byte*> ||
         CT::Exact<TypeOf<S>, const Byte*>,
         "Bad argument"
      );

      LANGULUS_ASSUME(DevAssumes, type, "Invalid type");
      LANGULUS_ASSUME(DevAssumes, placement, "Invalid placement pointer");
      LANGULUS_ASSUME(DevAssumes, value.mValue, "Invalid argument pointer");

      if constexpr (S::Shallow) {
         // Abandon/Disown/Move/Copy                                    
         if constexpr (S::Move) {
            if constexpr (!S::Keep) {
               if (type->mAbandonConstructor) {
                  type->mAbandonConstructor(value.mValue, placement);
                  return;
               }
            }
            else if (type->mMoveConstructor) {
               type->mMoveConstructor(value.mValue, placement);
               return;
            }
         }
         else {
            if constexpr (!S::Keep) {
               if (type->mDisownConstructor) {
                  type->mDisownConstructor(value.mValue, placement);
                  return;
               }
            }
         }

         if (type->mCopyConstructor)
            type->mCopyConstructor(value.mValue, placement);
         else
            LANGULUS_THROW(Construct, "Can't abandon/disown/move/copy-construct T from S");
      }
      else {
         // Clone/Copy                                                  
         if (type->mCloneConstructor)
            type->mCloneConstructor(value.mValue, placement);
         else if (type->mCopyConstructor)
            type->mCopyConstructor(value.mValue, placement);
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
   template<class T, CT::Semantic S>
   LANGULUS(ALWAYSINLINE)
   decltype(auto) SemanticAssign(T& lhs, S&& rhs) {
      if constexpr (S::Move) {
         if constexpr (!S::Keep && requires(T& a) { a = Abandon(rhs.mValue); })
            return (lhs = Abandon(rhs.mValue));
         else if constexpr (requires(T& a) { a = Move(rhs.mValue); })
            return (lhs = Move(rhs.mValue));
         else if constexpr (requires(T& a) { a = ::std::move(rhs.mValue); })
            return (lhs = ::std::move(rhs.mValue));
         else if constexpr (requires(T& a) { a = Copy(rhs.mValue); })
            return (lhs = Copy(rhs.mValue));
         else if constexpr (requires(T& a) { a = rhs.mValue; })
            return (lhs = rhs.mValue);
         else
            return Inner::Unsupported {};
      }
      else {
         if constexpr (!S::Shallow && requires(T& a) { a = Clone(rhs.mValue); })
            return (lhs = Clone(rhs.mValue));
         else if constexpr (!S::Keep && requires(T& a) { a = Disown(rhs.mValue); })
            return (lhs = Disown(rhs.mValue));
         else if constexpr (requires(T& a) { a = Copy(rhs.mValue); })
            return (lhs = Copy(rhs.mValue));
         else if constexpr (requires(T& a) { a = rhs.mValue; })
            return (lhs = rhs.mValue);
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
      concept SemanticMakable = Semantic<S> && requires (T a) {
         {SemanticMake<T>(S {a})} -> Exact<T>;
      };

      /// Check if T is semantic-assignable if mutable                        
      template<class S, class T>
      concept SemanticAssignable = Semantic<S> && requires (T a) {
         {SemanticAssign(a, S {a})} -> Exact<T&>;
      };

   } // namespace Inner

   /// Check if T is disown-constructible, disregards density                 
   template<class... T>
   concept DisownMakable = Complete<Decay<T>...> && (Inner::DisownMakable<Decay<T>> && ...);

   /// Check if T is disown-assignable if mutable, disregards density         
   template<class... T>
   concept DisownAssignable = Complete<Decay<T>...> && (Inner::DisownAssignable<Decay<T>> && ...);

   /// Check if T is clone-constructible, disregards density                  
   template<class... T>
   concept CloneMakable = Complete<Decay<T>...> && (Inner::CloneMakable<Decay<T>> && ...);

   /// Check if T is clone-assignable if mutable, disregards density          
   template<class... T>
   concept CloneAssignable = Complete<Decay<T>...> && (Inner::CloneAssignable<Decay<T>> && ...);

   /// Check if T is abandon-constructible, disregards density                
   template<class... T>
   concept AbandonMakable = Complete<Decay<T>...> && (Inner::AbandonMakable<Decay<T>> && ...);

   /// Check if T is abandon-assignable if mutable, disregards density        
   template<class... T>
   concept AbandonAssignable = Complete<Decay<T>...> && (Inner::AbandonAssignable<Decay<T>> && ...);

   /// Check if the decayed T is copy-constructible, disregards density       
   template<class... T>
   concept CopyMakable = Complete<Decay<T>...> && (Inner::CopyMakable<Decay<T>> && ...);

   /// Check if the decayed T is copy-assignable, disregards density          
   template<class... T>
   concept CopyAssignable = Complete<Decay<T>...> && (Inner::CopyAssignable<Decay<T>> && ...);
         
   /// Check if the decayed T is move-constructible, disregards density       
   template<class... T>
   concept MoveMakable = Complete<Decay<T>...> && (Inner::MoveMakable<Decay<T>> && ...);

   /// Check if the decayed T is move-assignable, disregards density          
   template<class... T>
   concept MoveAssignable = Complete<Decay<T>...> && (Inner::MoveAssignable<Decay<T>> && ...);


   /// Check if T is semantic-constructible by S, disregards density          
   template<class S, class... T>
   concept SemanticMakable = Complete<Decay<T>...> && (Inner::SemanticMakable<S, Decay<T>> && ...);

   /// Check if T is semantic-assignable if mutable, disregards density       
   template<class S, class... T>
   concept SemanticAssignable = Complete<Decay<T>...> && (Inner::SemanticAssignable<S, Decay<T>> && ...);

} // namespace Langulus::CT
