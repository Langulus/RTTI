///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "Config.hpp"
#include <Core/Types.hpp>
#include <Core/Utilities.hpp>
#include <vector>


namespace Langulus::RTTI
{
   struct DMeta;
   struct TMeta;
   struct VMeta;
   struct CMeta;
   struct AMeta;

   /// Specialize this for an out-of-body reflection experience               
   ///   @tparam T - type you desire to reflect                               
   template<CT::Decayed>
   struct ReflectionOf;

   /// Does a default RTTI generation                                         
   /// Useful to stop inheriting RTTI from base types                         
   struct Default {};
}

/// Reflection happens in three ways:                                         
///   1. Define a struct ReflectionOf<your type> in namespace Langulus::RTTI  
///      which contains the properties of the desired type (see list of       
///      available properties below). This is useful when you can't (or don't 
///      want to) modify types directly. Fundamental types are reflected      
///      that way.                                                            
///      You can forbit a type from being reflected by adding                 
///      LANGULUS(ACTS_AS) void; in the ReflectionOf definition.              
///   2. Define a public member struct RTTI in your type with the desired     
///      properties. For example: public: struct RTTI {...};                  
///      You can inherit, or predefine and reuse this RTTI struct anyway you  
///      like: public: using RTTI = SomePredefinedPattern;                    
///      You can forbid the type from being reflected by using RTTI = void;   
///      You can forbid any inheritance and regenerate RTTI by default using: 
///      public: using RTTI = Langulus::RTTI::Default;                        
///   3. Directly dump the desired properties in your type itself             
///      Useful when you want inheritance, but want to override only some     
///      aspects of the reflection.                                           
///                                                                           
/// Reflected properties propagate through the inheritance tree as usual.     
/// Here's a list of available properties:                                    
///                                                                           


/// Provide a custom token to your data type, instead of using CppNameOf      
///   @attention each name must be unique across all modules, including for   
///      instantiations of template types. A unique name will be automatically
///      generated by using the C++ declaration if no custom name is provided 
///   @attention the property will propagate to any derived class             
///      you will get compile-time error if base shares the same name         
///      when that base has been reflected using LANGULUS_BASES(...)          
///      otherwise a runtime meta conflict exception might happen             
#define LANGULUS_NAME() \
   public: static constexpr auto CTTI_Name = 

/// You can provide a custom token to your verbs, instead of using CppNameOf  
/// When verbs are reflected with this, their positive and negative tokens    
/// shall be the same. If you want them to be different, use POSITIVE and     
/// NEGATIVE names explicitly instead                                         
/// Once CTTI_Verb is provided, the type can no longer be reflected as data   
#define LANGULUS_VERB() \
   public: static constexpr ::Langulus::Token CTTI_Verb = 

/// You can provide a custom token to your trait                              
/// Once CTTI_Trait is provided, the type can no longer be reflected as data  
#define LANGULUS_TRAIT() \
   public: static constexpr ::Langulus::Token CTTI_Trait = 

/// You can provide a custom token to your constant                           
/// Once CTTI_Constant is provided, the type can no longer reflect as data    
#define LANGULUS_CONSTANT() \
   public: static constexpr ::Langulus::Token CTTI_Constant = 

/// You can provide a custom positive token for your verb                     
/// This has effect only when reflecting verbs, and if specified, then you    
/// should also specify the negative verb, too, or get a compile-time error   
/// If both negative and positive verbs are same, just use LANGULUS_VERB()    
/// Once CTTI_PositiveVerb is provided, the type can no longer reflect as data
#define LANGULUS_POSITIVE_VERB() \
   public: static constexpr ::Langulus::Token CTTI_PositiveVerb = 

/// You can provide a custom negative token for your verb                     
/// This has effect only when reflecting verbs, and if specified, then you    
/// should also specify the positive verb, too, or get a compile-time error   
/// If both negative and positive verbs are same, just use LANGULUS_VERB()    
/// Once CTTI_NegativeVerb is provided, the type can no longer reflect as data
#define LANGULUS_NEGATIVE_VERB() \
   public: static constexpr ::Langulus::Token CTTI_NegativeVerb = 

/// You can provide a custom operator for your verb                           
/// This is purely for syntax sugar                                           
/// This is used when both positive and negative operators are the same       
/// If you want them to be different, use POSITIVE and NEGATIVE names         
/// explicitly instead                                                        
#define LANGULUS_OPERATOR() \
   public: static constexpr ::Langulus::Token CTTI_Operator = 

/// You can provide a custom positive operator for your verb                  
/// This is purely for syntax sugar                                           
/// If positive operator is specified, you need to also specify the negative  
/// If both negative and positive verbs are same, just use LANGULUS_OPERATOR()
#define LANGULUS_POSITIVE_OPERATOR() \
   public: static constexpr ::Langulus::Token CTTI_PositiveOperator = 

/// You can provide a custom negative operator for your verb                  
/// This is purely for syntax sugar                                           
/// If negative operator is specified, you need to also specify the positive  
/// If both negative and positive verbs are same, just use LANGULUS_OPERATOR()
#define LANGULUS_NEGATIVE_OPERATOR() \
   public: static constexpr ::Langulus::Token CTTI_NegativeOperator = 

/// You can provide a custom negative operator for your verb                  
/// This is purely for syntax sugar                                           
/// If negative operator is specified, you need to also specify the positive  
/// If both negative and positive verbs are same, just use LANGULUS_OPERATOR()
#define LANGULUS_NEGATIVE_OPERATOR() \
   public: static constexpr ::Langulus::Token CTTI_NegativeOperator = 

/// You can provide precedence for verbs, used when parsing. Default          
/// precedence is zero, which means no precedence except order of appearance  
#define LANGULUS_PRECEDENCE() \
   public: static constexpr Real CTTI_Precedence = 

/// You can provide information string with your reflection                   
///   @attention the property will propagate to any derived class             
#define LANGULUS_INFO() \
   public: static constexpr ::Langulus::Token CTTI_Info = 

/// You can provide file format associations by extensions                    
/// You can specify multiple extensions by separating them with commas        
/// When serializing data of this type to a file, the first reflected         
/// extension will be used, if no extension has been explicitly stated for    
/// the file upon opening it for writing                                      
/// When deserializing from a file, the type to deserialize will be deduced   
/// by checking the file extension with the database                          
///   @attention the property will propagate to any derived class             
#define LANGULUS_FILES() \
   public: static constexpr ::Langulus::Token CTTI_Files = 

/// You can version your types                                                
/// When deserializing, you will get an error on major version mismatch       
/// If not specified, major version is always 1                               
///   @attention the property will propagate to any derived class             
#define LANGULUS_VERSION_MAJOR() \
   public: static constexpr ::Langulus::Count CTTI_VersionMajor = 

/// You can version your types                                                
/// When deserializing, you will get a warning on minor version mismatch      
/// If not specified, minor version is always 0                               
///   @attention the property will propagate to any derived class             
#define LANGULUS_VERSION_MINOR() \
   public: static constexpr ::Langulus::Count CTTI_VersionMinor = 

/// You can mark types as deep by using LANGULUS(DEEP) true / false inside    
/// class, but to fit into CT::Deep concept, your type must also inherit, and 
/// be the same size as an Anyness::Block                                     
///   @attention the property will propagate to any derived class             
#define LANGULUS_DEEP() \
   public: static constexpr bool CTTI_Deep = 

/// You can mark types as POD (Plain Old Data) by using LANGULUS(POD) true or 
/// false inside class. POD types are batch-copied via memcpy, and do not     
/// call constructors or destructors when contained (unless nullifiable)      
///   @attention the property will propagate to any derived class             
#define LANGULUS_POD() \
   public: static constexpr bool CTTI_POD = 

/// You can mark types as nullifiable, by using LANGULUS(NULLIFIABLE) true or 
/// false inside class. Nullifiable classes are batch-constructed and         
/// batch-destroyed via memset(0)                                             
///   @attention the property will propagate to any derived class             
#define LANGULUS_NULLIFIABLE() \
   public: static constexpr bool CTTI_Nullifiable = 

/// You can choose how a given type is pooled, if managed memory is           
/// enabled. See RTTI::PoolTactic for options. Used only if                   
/// LANGULUS_FEATURE(MANAGED_MEMORY) is enabled                               
///   @attention the property will propagate to any derived class             
#define LANGULUS_POOL_TACTIC() \
   public: static constexpr ::Langulus::RTTI::PoolTactic CTTI_Pool = 

/// You can make types concretizable, by using LANGULUS(CONCRETE) Type        
/// When dynamically creating your abstract objects, the most concrete type   
/// in the chain will be used instead                                         
///   @attention the property will propagate to any derived class             
#define LANGULUS_CONCRETE() \
   public: using CTTI_Concrete = 

/// You can make types producible only by executing a Verbs::Create in the    
/// context of specific producer                                              
///   @attention the property will propagate to any derived class             
#define LANGULUS_PRODUCER() \
   public: using CTTI_Producer = 

/// You can make types not reflectable by setting to void - useful for some   
/// intermediate types, like Handle. These types will produce a compile-time  
/// error when a reflection is attempted. All reflected types are reflectable 
/// by default as themselves. A binary compatible type is used for inserting  
/// CT::Block, CT::Trait and CT::Verb.                                        
///   @attention the property will propagate to any derived class             
#define LANGULUS_ACT_AS() \
   public: using CTTI_ActAs = 

/// You can make types unallocatable by the memory manager. This serves not   
/// only as forcing the type to be either allocated by conventional C++ means,
/// or on the stack, but also optimizes away any memory manager searches,     
/// when inserting pointers (when managed memory is enabled)                  
///   @attention the property will propagate to any derived class             
#define LANGULUS_UNALLOCATABLE() \
   public: static constexpr bool CTTI_Unallocatable =

/// You can give names to specific values of a given type, such as enums      
/// These names can be used as constants when parsing code, and are used for  
/// serialization to code/text/debug                                          
///   @attention the property will propagate to any derived class             
#define LANGULUS_NAMED_VALUES(...) \
   public: static constexpr auto CTTI_NamedValues = \
      ::Langulus::RTTI::CreateNamedValueTuple<__VA_ARGS__>()

/// You can give an optional suffix to your type                              
/// That suffix is actively used when serializing to/deserializing from text  
///   @attention the property will propagate to any derived class             
#define LANGULUS_SUFFIX() \
   public: static constexpr ::Langulus::Token CTTI_Suffix = 

/// You can define an allocation page (number of elements) by using           
/// LANGULUS(ALLOCATION_PAGE) X. When allocating memory for your type, X      
/// will be the minimum amount of elements to allocate, aligned to the        
/// nearest upper power-of-two amount of bytes. By default, allocation page   
/// size	is never lower than Alignment                                        
///   @attention the property will propagate to any derived class             
#define LANGULUS_ALLOCATION_PAGE() \
   public: static constexpr ::Langulus::Count CTTI_AllocationPage = 

/// Make a type abstract                                                      
///   @attention the property will propagate to any derived class             
#define LANGULUS_ABSTRACT() \
   public: static constexpr bool CTTI_Abstract = 

/// Reflecting members                                                        
///   @attention the list of members will be propagated to any derived class  
#define LANGULUS_MEMBERS(...) \
   public: using CTTI_Members = \
      decltype(::Langulus::RTTI::Inner::CreateMembersTuple<__VA_ARGS__>())

/// Reflect a list of bases                                                   
///   @attention the list of bases will be propagated to any derived class    
#define LANGULUS_BASES(...) \
   public: using CTTI_Bases = decltype(::Langulus::CreateTypeList<__VA_ARGS__>())

/// Reflect a list of verbs                                                   
///   @attention the list of verbs will be propagated to any derived class    
#define LANGULUS_VERBS(...) \
   public: using CTTI_Verbs = decltype(::Langulus::CreateTypeList<__VA_ARGS__>())

/// Reflect a list of possible conversions to list of possible types          
/// These will be automatically used by Verbs::Interpret if available         
///   @attention the list of conversions will be propagated to derived classes
#define LANGULUS_CONVERTS_TO(...) \
   public: using CTTI_ConvertTo = decltype(::Langulus::CreateTypeList<__VA_ARGS__>())

/// Reflect a list of possible conversions from a list of possible types      
/// These will be automatically used by Verbs::Interpret if available         
///   @attention the list of conversions will be propagated to derived classes
#define LANGULUS_CONVERTS_FROM(...) \
   public: using CTTI_ConvertFrom = decltype(::Langulus::CreateTypeList<__VA_ARGS__>())

/// You can make types CT::Typed and retrieve their inner type using TypeOf   
/// by adding LANGULUS(TYPED) <inner type>; as a member. You disable it by    
/// doing LANGULUS(TYPED) void;                                               
///   @attention the property will propagate to any derived class             
#define LANGULUS_TYPED() \
   public: using CTTI_InnerType = 


/// Compile-time checks and concepts associated with RTTI                     
namespace Langulus::CT
{
   namespace Inner
   {

      template<class T>
      consteval bool IsAbstract() {
         if constexpr (Complete<T> and Dense<T>) {
            if constexpr (requires { T::CTTI_Abstract; })
               return ::std::is_abstract_v<T> or T::CTTI_Abstract;
            else
               return ::std::is_abstract_v<T>;
         }
         else return false;
      }

      template<class T>
      consteval bool IsReflectable() {
         if constexpr (Complete<T> and Dense<T>) {
            if constexpr (requires { typename T::CTTI_ActAs; })
               return not CT::Void<typename T::CTTI_ActAs>;
            else
               return Complete<T>;
         }
         else return Complete<T>;
      }

   } // namespace Langulus::CT::Inner


   /// Check if all T are abstract (have at least one pure virtual function,  
   /// or are explicitly marked as LANGULUS(ABSTRACT) true). Sparse types are 
   /// never abstract!                                                        
   template<class...T>
   concept Abstract = (Inner::IsAbstract<T>() and ...);

   /// Check if any of the listed T is unallocatable                          
   /// You can make types unallocatable by the memory manager. This serves    
   /// not only as forcing the type to be either allocated by conventional    
   /// C++ means (on the heap or the stack), but also optimizes away any      
   /// memory manager searches, when inserting pointers while managed memory  
   /// is enabled. Raw function pointers are unallocatable by default         
   template<class...T>
   concept Unallocatable = not Complete<T...> or Function<T...>
        or ((Dense<T> and Decay<T>::CTTI_Unallocatable) or ...);

   /// Check if all of the types are allocatable                              
   template<class...T>
   concept Allocatable = ((not Unallocatable<T>) and ...);

   /// Check if all of the types are reflectable                              
   template<class...T>
   concept Reflectable = (Inner::IsReflectable<T>() and ...);

   namespace Inner
   {

      /// Check if T is typed, having either CTTI_InnerType or value_type as  
      /// member type declarations                                            
      ///   @attention the inner type must not be 'void', in order for T to   
      ///      be considered 'typed', as in not 'type-erased'                 
      template<class T>
      consteval bool IsTyped() {
         if constexpr (not Complete<Deref<T>>)
            return false;
         else if constexpr (requires { typename Deref<T>::CTTI_InnerType; })
            return Data<typename Deref<T>::CTTI_InnerType>;
         else if constexpr (requires { typename Deref<T>::value_type; })
            return Data<typename Deref<T>::value_type>;
         else
            return false;
      }

      /// Convenience function that wraps std::underlying_type_t for enums,   
      /// as well as any array, or anything with CTTI_InnerType that isn't    
      /// void, or has the value_type member type defined                     
      ///   - if T is an array, returns pointer of the array type             
      ///   - if T has CTTI_InnerType/value_type, return pointer of the type  
      ///   - if T is an enum, return pointer of the underlying type          
      ///   - otherwise just return a decayed T pointer                       
      template<class T>
      consteval auto GetUnderlyingType() {
         if constexpr (Array<T>)
            return (Deref<Deext<T>>*) nullptr;
         else {
            using DT = Decay<T>;
            if constexpr (not Complete<DT>)
               return (Deref<T>*) nullptr;
            else if constexpr (requires { typename DT::CTTI_InnerType; })
               return (Deref<typename DT::CTTI_InnerType>*) nullptr;
            else if constexpr (requires { typename DT::value_type; })
               return (Deref<typename DT::value_type>*) nullptr;
            else if constexpr (CT::Enum<DT>)
               return (Deref<::std::underlying_type_t<DT>>*) nullptr;
            else
               return (Deref<T>*) nullptr;
         }
      };
      
      /// Convenience function that returns the type of LANGULUS(ACT_AS)      
      /// Incomplete/lacking types return themselves (as decayed)             
      template<class T>
      consteval auto GetInsertableType() {
         if constexpr (not Complete<Decay<T>>)
            return (Decay<T>*) nullptr;
         else if constexpr (requires { typename Decay<T>::CTTI_ActAs; }) {
            using AS = typename Decay<T>::CTTI_ActAs;
            if constexpr (CT::Void<AS>)
               return (void*) nullptr;
            else {
               static_assert(CT::Decayed<AS>,
                  "Provided insertion type must be fully decayed");
               static_assert(sizeof(Decay<T>) == sizeof(AS),
                  "Provided insertion type must be binary compatible");
               return (AS*) nullptr;
            }
         }
         else return (Decay<T>*) nullptr;
      };

      /// Check if a type is POD (plain old data)                             
      ///   @tparam T - the type to check                                     
      ///   @return true if T is a POD type                                   
      template<class T>
      consteval bool IsPOD() {
         if constexpr (Complete<T>) {
            if constexpr (not Abstract<T>) {
               if constexpr (Dense<T> and requires { T::CTTI_POD; })
                  return T::CTTI_POD;
               if constexpr (Fundamental<T> or Sparse<T> or (
               ::std::is_trivial_v<T> and
               ::std::is_standard_layout_v<T> and
               ::std::is_destructible_v<T>))
                  return true;
               else
                  return false;
            }
            else return false;
         }
         else return false;
      };

      /// Check if a type is nullifiable                                      
      ///   @tparam T - the type to check                                     
      ///   @return true if T is nullifiable                                  
      template<class T>
      consteval bool IsNullifiable() {
         if constexpr (Complete<T>) {
            if constexpr (not Abstract<T>) {
               if constexpr (Dense<T> and requires { T::CTTI_Nullifiable; })
                  return T::CTTI_Nullifiable;
               else if constexpr (Fundamental<T> or Sparse<T>)
                  return true;
               else
                  return false;
            }
            else return false;
         }
         else return false;
      };

   } // namespace Langulus::CT::Inner

   /// Check if the origin T is resolvable at runtime                         
   template<class...T>
   concept Resolvable = Complete<T...> and requires (T&...a) {
         { (a.GetType(),  ...) } -> Exact<RTTI::DMeta>;
         { (a.GetBlock(), ...) } -> DerivedFrom<A::Block>;
      };

   /// Check if T is default-constructible                                    
   ///   @attention this includes even fundamentals that are not initialized  
   template<class...T>
   concept Defaultable = ((not Abstract<T>) and ...)
       and requires { (T {}, ...); };

   /// Check if T is noexcept default-constructible                           
   ///   @attention this includes even fundamentals that are not initialized  
   template<class...T>
   concept DefaultableNoexcept = Defaultable<T...>
       and (noexcept(T {}) and ...);

   /// Check if T requires its destructor being called                        
   template<class...T>
   concept Destroyable = Complete<T...> and ((
          not ::std::is_trivially_destructible_v<T>
          and ::std::is_destructible_v<T>
      ) and ...);

   /// A POD (Plain Old Data) type is any type with a static member           
   /// T::CTTI_POD set to true. If no such member exists, the type is         
   /// assumed NOT POD by default, unless ::std::is_trivial, which seems to   
   /// be inconsistent across compilers.                                      
   /// POD types improve construction, destruction, copying, and cloning      
   /// by using some batching runtime optimizations                           
   /// All POD types are also directly serializable to binary                 
   /// Use "LANGULUS(POD) true;" as member to tag POD types                   
   template<class...T>
   concept POD = sizeof...(T) > 0 and (Inner::IsPOD<T>() and ...);

   /// A nullifiable type is any type with a static member                    
   /// T::CTTI_Nullifiable set to true. If no such member exists, the type    
   /// is assumed NOT nullifiable by default, unless it is sparse             
   /// Nullifiable types improve default-construction by using some batching  
   /// runtime optimizations                                                  
   /// Use LANGULUS(NULLIFIABLE) true; as member to tag nullifiable types     
   template<class...T>
   concept Nullifiable = sizeof...(T) > 0 and (Inner::IsNullifiable<T>() and ...);
   
   /// A concretizable type is any type with a member type CTTI_Concrete      
   /// If no such member exists, the type is assumed NOT concretizable by     
   /// default. Concretizable types provide a default concretization for      
   /// when	allocating abstract types                                         
   /// Use LANGULUS(CONCRETIZABLE) macro as member to tag such types          
   template<class...T>
   concept Concretizable = Complete<T...> and (requires {
         typename Decay<T>::CTTI_Concrete;
      } and ...);

   /// A producible type is any type with a member type CTTI_Producer         
   /// If no such member exists, the type is assumed NOT producible by        
   /// default. Producible types can not be created at compile-time, and need 
   /// to be produced by executing Verbs::Create in the producer's context    
   /// Use LANGULUS(PRODUCER) macro as member to tag such types               
   template<class...T>
   concept Producible = Complete<T...> and (requires {
         typename Decay<T>::CTTI_Producer;
      } and ...);

   /// Check if a type has reflected named values                             
   template<class...T>
   concept HasNamedValues = Complete<T...> and (requires {
         (::std::tuple_size_v<decltype(T::CTTI_NamedValues)>) > 0;
      } and ...);

   /// Check if all T have a mutable dispatcher (have `Do(Verb&)` method)     
   template<class...T>
   concept DispatcherMutable = requires (Flow::Verb b, T...a) {
         {(a.Do(b), ...)};
      };

   /// Check if all T have a constant dispatcher (have `Do(Verb&) const`)     
   template<class...T>
   concept DispatcherConstant = requires (Flow::Verb b, const T...a) {
         {(a.Do(b), ...)};
      };

   /// Check if all T have a dispatcher, compatible with the cv-quality of T  
   template<class...T>
   concept Dispatcher = ((DispatcherMutable<T>
        or (Constant<T> and DispatcherConstant<T>)
      ) and ...);

} // namespace Langulus::CT

namespace Langulus
{

   /// Get the reflected concrete type                                        
   template<class T>
   using ConcreteOf = typename Decay<T>::CTTI_Concrete;

   /// Get the reflected producer type                                        
   template<class T>
   using ProducerOf = typename Decay<T>::CTTI_Producer;

   /// Get internal type of an enum, or anything reflected with the           
   /// LANGULUS(TYPED) member                                                 
   template<class T>
   using TypeOf = Deptr<decltype(CT::Inner::GetUnderlyingType<T>())>;

   /// Get the reflected insertable type, see LANGULUS(ACT_AS)                
   template<class T>
   using InsertableAs = Deptr<decltype(CT::Inner::GetInsertableType<T>())>;

   namespace CT
   {

      /// Check if all T have underlying type defined                         
      template<class...T>
      concept Typed = (Inner::IsTyped<T>() and ...);

      /// Check if all T has no underlying types defined                      
      template<class...T>
      concept Untyped = ((not Typed<T>) and ...);

      /// Check if the reflected insertable type is a redirection             
      template<class T>
      concept InsertableAsSomethingElse = Reflectable<T>
          and not CT::Exact<Decay<T>, InsertableAs<T>>;

      template<class T>
      concept NotInsertableAsSomethingElse = Reflectable<T>
          and CT::Exact<Decay<T>, InsertableAs<T>>;

   } // namespace namespace CT

   /// A type naming convention for standard number types, as well as         
   /// anything reflected with LANGULUS(SUFFIX)                               
   ///   @return the suffix depending on the template argument                
   template<CT::Dense T>
   consteval Token SuffixOf() {
      if constexpr (requires { T::CTTI_Suffix; })
         return T::CTTI_Suffix;
      else if constexpr (CT::Same<T, signed int>)
         return "i";
      else if constexpr (CT::Same<T, unsigned int>)
         return "u";
      else if constexpr (CT::Same<T, Real>)
         return "";
      else if constexpr (CT::Same<T, ::std::uint8_t>)
         return "u8";
      else if constexpr (CT::Same<T, ::std::uint16_t>)
         return "u16";
      else if constexpr (CT::Same<T, ::std::uint32_t>)
         return "u32";
      else if constexpr (CT::Same<T, ::std::uint64_t>)
         return "u64";
      else if constexpr (CT::Same<T, ::std::int8_t>)
         return "i8";
      else if constexpr (CT::Same<T, ::std::int16_t>)
         return "i16";
      else if constexpr (CT::Same<T, ::std::int32_t>)
         return "i32";
      else if constexpr (CT::Same<T, ::std::int64_t>)
         return "i64";
      else if constexpr (CT::Same<T, Float>)
         return "f";
      else if constexpr (CT::Same<T, Double>)
         return "d";
      else if constexpr (CT::Same<T, bool>)
         return "b";
      else
         return "";
   }

   namespace RTTI
   {

      /// The main boundary indentifier token                                 
      constexpr Token MainBoundary = "MAIN";

      /// Used for named constants reflections inside data types              
      ///   @tparam T - type of the constant                                  
      template<auto T>
      struct NamedValue {
         static constexpr auto Value = T;
         Token mInfo {};
      };

      /// Used for member reflections inside data types                       
      ///   @tparam HANDLE - a pointer to a member variable                   
      ///   @attention having just `auto HANDLE` as a template argument       
      ///      should generally be enough, and works as it should on MSVC,    
      ///      however it doesn't make unique template instantiations on      
      ///      Clang and causes very nasty bugs. So, we're forced to add      
      ///      a couple more template parameters, to ensure proper templating 
      template<auto HANDLE, class OWNER, class TYPE>
      struct NamedMember {
         using Member = decltype(HANDLE);
         static_assert(std::is_member_pointer_v<Member>,
            "Member must be a member pointer");
         using Owner = OWNER;
         using Type  = TYPE; 
         static constexpr TYPE OWNER::*Handle = HANDLE;

         Token mInfo {};

         constexpr NamedMember() = default;
         constexpr NamedMember(NamedValue<HANDLE>, TYPE OWNER::*) {}
      };

      template<auto HANDLE, class OWNER, class TYPE>
      NamedMember(NamedValue<HANDLE>, TYPE OWNER::*) -> NamedMember<HANDLE, OWNER, TYPE>;

      namespace Inner
      {

         /// Generate constexpr tuple with the members                        
         ///   @return a tuple of the desired member pointers                 
         template<auto...HANDLES>
         consteval auto CreateMembersTuple() {
            return Types<decltype(NamedMember(NamedValue<HANDLES> {}, HANDLES))...>{};
         }

      } // namespace Langulus::RTTI::Inner

      ///                                                                     
      /// Different pool tactics you can assign to your data types            
      /// Used primarily for advanced tweaking of a final product             
      /// Pooling works only if managed memory feature is enabled             
      ///                                                                     
      enum class PoolTactic {
         // Data instances will be pooled in the main pool chain,       
         // unless data was reflected from a boundary that is not MAIN  
         Main = 0,

         // Data instances will be pooled based on their size           
         // There will be pools dedicated for each allocation page size 
         // This effectively narrows the search for entries a bit       
         Size,

         // Data instances will be pooled based on their type           
         // Each meta definition will have its own pool chain           
         // This is the default pooling tactic for any meta data that   
         // is not reflected inside the RTTI::MainBoundary boundary.    
         // See LANGULUS_RTTI_BOUNDARY for more information on that.    
         Type,

         #if LANGULUS(DEBUG)
            Default = Type
         #else
            Default = Main
         #endif
      };

   } // namespace Langulus::RTTI

} // namespace Langulus

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   /// The Langulus::RTTI::Boundary symbol is intentionally left undefined,   
   /// so that it is mandatory for you to define it inside your executables   
   /// or mods. It's a simple compile-time string, that is attached upon data 
   /// reflection, so that RTTI can track from which library a type was       
   /// reflected, and thus unregister it when shared object is unloaded.      
   /// The boundary also affects pooling tactics, because if boundary is not  
   /// equal exactly to RTTI::MainBoundary, pooling will be PoolTactic::Type  
   /// by default, so that allocation that happen from external libraries can 
   /// be easily tracked                                                      
   #define LANGULUS_RTTI_BOUNDARY(a) \
      namespace Langulus::RTTI { \
         Token Boundary = a; \
      }
#else
   #define LANGULUS_RTTI_BOUNDARY(a)
#endif