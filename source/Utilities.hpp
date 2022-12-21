///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Reflection.hpp"

namespace Langulus
{

   template<class T, CT::Semantic S>
   LANGULUS(ALWAYSINLINE)
   NOD() T SemanticMake(S&& value) {
      using A = typename S::Type;

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
            LANGULUS_ERROR("Can't abandon/move/copy-construct T from S");
      }
      else {
         if constexpr (!S::Keep && ::std::constructible_from<T, Disowned<A>&&>)
            return T {Disown(value.mValue)};
         else if constexpr (::std::constructible_from<T, Copied<A>>)
            return T {Copy(value.mValue)};
         else if constexpr (::std::constructible_from<T, const A&>)
            return T {value.mValue};
         else
            LANGULUS_ERROR("Can't disown/copy-construct T from S");
      }
   }

   template<class T, CT::Semantic S>
   LANGULUS(ALWAYSINLINE)
   NOD() T* SemanticNew(S&& value) {
      using A = typename S::Type;

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
            LANGULUS_ERROR("Can't abandon/move/copy-construct T from S");
      }
      else {
         if constexpr (!S::Keep && ::std::constructible_from<T, Disowned<A>&&>)
            return new T {Disown(value.mValue)};
         else if constexpr (::std::constructible_from<T, Copied<A>>)
            return new T {Copy(value.mValue)};
         else if constexpr (::std::constructible_from<T, const A&>)
            return new T {value.mValue};
         else
            LANGULUS_ERROR("Can't disown/copy-construct T from S");
      }
   }

   template<class T, CT::Semantic S>
   LANGULUS(ALWAYSINLINE)
   T* SemanticNew(void* placement, S&& value) {
      using A = typename S::Type;

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
            LANGULUS_ERROR("Can't abandon/move/copy-construct T from S");
      }
      else {
         if constexpr (!S::Keep && ::std::constructible_from<T, Disowned<A>&&>)
            return new (placement) T {Disown(value.mValue)};
         else if constexpr (::std::constructible_from<T, Copied<A>>)
            return new (placement) T {Copy(value.mValue)};
         else if constexpr (::std::constructible_from<T, const A&>)
            return new (placement) T {value.mValue};
         else
            LANGULUS_ERROR("Can't disown/copy-construct T from S");
      }
   }

   template<class T, CT::Semantic S>
   LANGULUS(ALWAYSINLINE)
   decltype(auto) SemanticAssign(T& lhs, S&& rhs) {
      using A = typename S::Type;

      if constexpr (S::Move) {
         if constexpr (!S::Keep && requires(T a) { a = Abandon(rhs.mValue); })
            return lhs = Abandon(rhs.mValue);
         else if constexpr (requires(T a) { a = Move(rhs.mValue); })
            return lhs = Move(rhs.mValue);
         else if constexpr (requires(T a) { a = ::std::move(rhs.mValue); })
            return lhs = ::std::move(rhs.mValue);
         else if constexpr (requires(T a) { a = Copy(rhs.mValue); })
            return lhs = Copy(rhs.mValue);
         else if constexpr (requires(T a) { a = rhs.mValue; })
            return lhs = rhs.mValue;
         else
            LANGULUS_ERROR("Can't abandon/move/copy-assign T from S");
      }
      else {
         if constexpr (!S::Keep && requires(T a) { a = Disown(rhs.mValue); })
            return lhs = Disown(rhs.mValue);
         else if constexpr (requires(T a) { a = Copy(rhs.mValue); })
            return lhs = Copy(rhs.mValue);
         else if constexpr (requires(T a) { a = rhs.mValue; })
            return lhs = rhs.mValue;
         else
            LANGULUS_ERROR("Can't disown/copy-assign T from S");
      }
   }

} // namespace Langulus
