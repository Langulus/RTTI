///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Config.hpp"
#include <Core/Types.hpp>
#include <Core/Utilities.hpp>
#include <vector>


/// Provide a custom token to your data type, instead of using CppNameOf      
///   @attention each name must be unique, including for instantiations of    
///              template types. A unique name will be generated by using the 
///              C++ declaration, if no custom name was provided              
///   @attention the property will propagate to any derived class             
///              you will get compile-time error if base shares the same name,
///              if that base has been reflected using LANGULUS_BASES(...)    
///              otherwise a runtime meta conflict exception might happen     
#define LANGULUS_NAME() \
   public: static constexpr ::Langulus::Token CTTI_Name = 

/// You can provide a custom token to your verbs, instead of using CppNameOf  
/// When verbs are reflected with this, their positive and negative tokens    
/// shall be the same. If you want them to be different, use POSITIVE and     
/// NEGATIVE names explicitly instead                                         
#define LANGULUS_VERB() \
   public: static constexpr ::Langulus::Token CTTI_Verb = 

/// You can provide a custom token to your trait                              
#define LANGULUS_TRAIT() \
   public: static constexpr ::Langulus::Token CTTI_Trait = 

/// You can provide a custom token to your constant                           
#define LANGULUS_CONSTANT() \
   public: static constexpr ::Langulus::Token CTTI_Constant = 

/// You can provide a custom positive token for your verb                     
/// This has effect only when reflecting verbs, and if specified, then you    
/// should also specify the negative verb, too, or get a compile-time error   
/// If both negative and positive verbs are same, just use LANGULUS_VERB()    
#define LANGULUS_POSITIVE_VERB() \
   public: static constexpr ::Langulus::Token CTTI_PositiveVerb = 

/// You can provide a custom negative token for your verb                     
/// This has effect only when reflecting verbs, and if specified, then you    
/// should also specify the positive verb, too, or get a compile-time error   
/// If both negative and positive verbs are same, just use LANGULUS_VERB()    
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

#if LANGULUS_FEATURE(MANAGED_MEMORY)
   /// You can choose how a given type is pooled, if managed memory is        
   /// enabled. See RTTI::PoolTactic for options                              
   ///   @attention the property will propagate to any derived class          
   #define LANGULUS_POOL_TACTIC() \
      public: static constexpr ::Langulus::RTTI::PoolTactic CTTI_Pool = 
#endif

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

/// You can make types not insertable to Anyness containers, such as some     
/// intermediate types, like Block::KnownPointer. These types will produce    
/// a compile-time error when a push is attempted. All reflected types are    
/// insertable by default                                                     
///   @attention the property will propagate to any derived class             
#define LANGULUS_UNINSERTABLE() \
   public: static constexpr bool CTTI_Uninsertable =

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
#define LANGULUS_NAMED_VALUES(T) \
   public: static constexpr ::Langulus::RTTI::CMetaTriplet<T> CTTI_NamedValues[] =

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

/// Start reflecting members                                                  
///   @attention the list of members will be propagated to any derived class  
#define LANGULUS_PROPERTIES_START(SELF) \
   using Self = SELF; \
   public: inline static const Langulus::RTTI::Member CTTI_Members [] = {

/// Reflect a property with a trait tag                                       
#define LANGULUS_PROPERTY_TRAIT(name, trait) \
   ::Langulus::RTTI::Member::FromTagged<::Langulus::Traits::trait>(&Self::name, #name)

/// Reflect a property without trait tag                                      
#define LANGULUS_PROPERTY(name) \
   ::Langulus::RTTI::Member::From(&Self::name, #name)

/// End reflecting members                                                    
#define LANGULUS_PROPERTIES_END() }

/// Reflect a list of bases                                                   
///   @attention the list of bases will be propagated to any derived class    
#define LANGULUS_BASES(...) \
   public: using CTTI_Bases = decltype(::Langulus::CreateTypeList<__VA_ARGS__>())

/// Reflect a list of verbs                                                   
///   @attention the list of verbs will be propagated to any derived class    
#define LANGULUS_VERBS(...) \
   public: using CTTI_Verbs = decltype(::Langulus::CreateTypeList<__VA_ARGS__>())

/// Reflect a list of possible conversions                                    
/// These will be automatically used by Verbs::Interpret if available         
///   @attention the list of conversions will be propagated to derived classes
#define LANGULUS_CONVERSIONS(...) \
   public: using CTTI_Conversions = decltype(::Langulus::CreateTypeList<__VA_ARGS__>())

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
      concept Reflectable = requires {
         {T::Reflect()} -> Same<::Langulus::RTTI::MetaData>;
      };

      template<class T>
      concept Abstract = not Complete<T> or ::std::is_abstract_v<T> or T::CTTI_Abstract;

      template<class T>
      concept Uninsertable = T::CTTI_Uninsertable;

      template<class T>
      concept Unallocatable = not Complete<T> or T::CTTI_Unallocatable;

      template<class T>
      concept Destroyable = not ::std::is_trivially_destructible_v<T>
                            and ::std::is_destructible_v<T>;

      template<class T>
      concept POD = Complete<T> and not Abstract<T> and (
         T::CTTI_POD or ::std::is_trivial_v<T> or not Destroyable<T>);


      template<class T>
      concept Nullifiable = Complete<T>
                    and not Abstract<T>
                    and T::CTTI_Nullifiable;

      template<class T>
      concept Concretizable = Complete<T> and requires {
         typename T::CTTI_Concrete;
      };

      template<class T>
      concept Producible = Complete<T> and requires {
         typename T::CTTI_Producer;
      };

      template<class T>
      concept Defaultable = not Abstract<T> and requires { T {}; };

      template<class T>
      concept DefaultableNoexcept = Defaultable<T> and noexcept(T {});
      
      template<class T>
      concept DispatcherMutable  = requires (      T a, Flow::Verb b) { {a.Do(b)}; };

      template<class T>
      concept DispatcherConstant = requires (const T a, Flow::Verb b) { {a.Do(b)}; };
      
      template<class T>
      concept Typed = Complete<T> and (requires {typename T::CTTI_InnerType;}
                                   or  requires {typename T::value_type;});
      
      template<class T>
      concept HasNamedValues = Complete<T> and requires {
         T::CTTI_NamedValues;
      };

      /// Convenience function that wraps std::underlying_type_t for enums,   
      /// as well as any array, or anything with CTTI_InnerType or            
      /// value_type member type defined                                      
      ///   - if T is an array, returns pointer of the array type             
      ///   - if T is Typed, return pointer of the type                       
      ///   - if T is an enum, return pointer of the underlying type          
      ///   - otherwise just return a decayed T pointer                       
      template<class T>
      constexpr auto GetUnderlyingType() noexcept {
         if constexpr (Array<T>)
            return (Deref<Deext<T>>*) nullptr;
         else {
            using DT = Decay<T>;
            if constexpr (Typed<DT>) {
               if constexpr (requires {typename DT::CTTI_InnerType; })
                  return (typename DT::CTTI_InnerType*) nullptr;
               else
                  return (typename DT::value_type*) nullptr;
            }
            else if constexpr (CT::Enum<DT>)
               return (::std::underlying_type_t<DT>*) nullptr;
            else
               return (Deref<T>*) nullptr;
         }
      };

   } // namespace Langulus::CT::Inner

   
   /// Check if the origin T is default-constructible                         
   template<class... T>
   concept Defaultable = Complete<Decay<T>...>
      and (Inner::Defaultable<Decay<T>> and ...);

   template<class... T>
   concept DefaultableNoexcept = Complete<Decay<T>...>
      and (Inner::DefaultableNoexcept<Decay<T>> and ...);

   /// Check if the origin T requires its destructor being called             
   template<class... T>
   concept Destroyable = Complete<Decay<T>...>
      and (Inner::Destroyable<Decay<T>> and ...);

   /// A reflected type is a type that has a public Reflection field          
   /// This field is automatically added when using LANGULUS(REFLECT) macro   
   /// inside the type you want to reflect                                    
   template<class... T>
   concept Reflectable = (Inner::Reflectable<Decay<T>> and ...);

   /// An uninsertable type is any type with a true static member             
   /// T::CTTI_Uninsertable. All types are insertable by default              
   /// Useful to mark some intermediate types, that are not supposed to be    
   /// inserted in containers                                                 
   template<class... T>
   concept Uninsertable = (Inner::Uninsertable<Decay<T>> and ...);

   template<class... T>
   concept Insertable = not Uninsertable<T...>;

   /// You can make types unallocatable by the memory manager. This serves    
   /// not only as forcing the type to be either allocated by conventional    
   /// C++ means, or on the stack, but also optimizes away any memory manager 
   /// searches, when inserting pointers, if managed memory is enabled        
   template<class... T>
   concept Unallocatable = ((Inner::Unallocatable<Decay<T>>) and ...);

   template<class... T>
   concept Allocatable = not Unallocatable<T...>;

   /// A POD (Plain Old Data) type is any type with a static member           
   /// T::CTTI_POD set to true. If no such member exists, the type is         
   /// assumed NOT POD by default, unless ::std::is_trivial, which seems to   
   /// inconsistent across compilers.                                         
   /// POD types improve construction, destruction, copying, and cloning      
   /// by using some batching runtime optimizations                           
   /// All POD types are also directly serializable to binary                 
   /// Use LANGULUS(POD) macro as member to tag POD types                     
   template<class... T>
   concept POD = (Inner::POD<Decay<T>> and ...);

   /// A nullifiable type is any type with a static member                    
   /// T::CTTI_Nullifiable set to true. If no such member exists, the type    
   /// is assumed NOT nullifiable by default, unless it is sparse             
   /// Nullifiable types improve default-construction by using some batching  
   /// runtime optimizations                                                  
   /// Use LANGULUS(NULLIFIABLE) macro as member to tag nullifiable types     
   template<class... T>
   concept Nullifiable = (Inner::Nullifiable<Decay<T>> and ...);

   /// A concretizable type is any type with a member type CTTI_Concrete      
   /// If no such member exists, the type is assumed NOT concretizable by     
   /// default. Concretizable types provide a default concretization for      
   /// when	allocating abstract types                                         
   /// Use LANGULUS(CONCRETIZABLE) macro as member to tag such types          
   template<class... T>
   concept Concretizable = (Inner::Concretizable<Decay<T>> and ...);

   /// Get the reflected concrete type                                        
   template<class T>
   using ConcreteOf = typename Decay<T>::CTTI_Concrete;

   /// A producible type is any type with a member type CTTI_Producer         
   /// If no such member exists, the type is assumed NOT producible by        
   /// default. Producible types can not be created at compile-time, and need 
   /// to be produced by executing Verbs::Create in the producer's context    
   /// Use LANGULUS(PRODUCER) macro as member to tag such types               
   template<class... T>
   concept Producible = (Inner::Producible<Decay<T>> and ...);
   
   /// Check if a type has reflected named values                             
   template<class... T>
   concept HasNamedValues = (Inner::HasNamedValues<Decay<T>> and ...);

   /// Get the reflected producer type                                        
   template<class T>
   using ProducerOf = typename Decay<T>::CTTI_Producer;

   /// Check if T is abstract (has at least one pure virtual function, or is  
   /// explicitly marked as LANGULUS(ABSTRACT)). Sparse types are never       
   /// abstract                                                               
   template<class... T>
   concept Abstract = (Inner::Abstract<Decay<T>> and ...);

   /// Check if all T have a mutable dispatcher (have `Do(Verb&)` method)     
   template<class... T>
   concept DispatcherMutable = (Inner::DispatcherMutable<T> and ...);

   /// Check if all T have a constant dispatcher (have `Do(Verb&) const`)     
   template<class... T>
   concept DispatcherConstant = (Inner::DispatcherConstant<T> and ...);

   /// Check if all T have a dispatcher, compatible with the cv-quality of T  
   template<class... T>
   concept Dispatcher = ((DispatcherMutable<T> or (Constant<T> and DispatcherConstant<T>)) and ...);

} // namespace Langulus::CT

namespace Langulus
{

   /// Get internal type of an enum, or anything reflected with the           
   /// LANGULUS(TYPED) member                                                 
   template<class T>
   using TypeOf = Deptr<decltype(CT::Inner::GetUnderlyingType<T>())>;

   namespace CT
   {
      /// Check if a type has an underlying type defined                      
      template<class... T>
      concept Typed = ((Inner::Typed<Decay<T>> and Data<TypeOf<T>>) and ...);
   }

   /// A type naming convention for standard number types, as well as         
   /// anything reflected with LANGULUS(SUFFIX)                               
   ///   @return the suffix depending on the template argument                
   template<CT::Dense T>
   constexpr Token SuffixOf() {
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

      /// Triplet used for named constants reflections inside data types      
      ///   @tparam T - type of the constant                                  
      template<class T>
      struct CMetaTriplet {
         Token mToken;
         T mValue;
         Token mInfo {};
      };

   #if LANGULUS_FEATURE(MANAGED_MEMORY)
      ///                                                                     
      /// Different pool tactics you can assign to your data types            
      /// Used primarily for advanced tweaking of a final product             
      /// Pooling works only if managed memory feature is enabled             
      ///                                                                     
      enum class PoolTactic {
         // Data instances will be pooled in the default pool chain,    
         // unless data was reflected from a boundary that is not MAIN  
         Default = 0,

         // Data instances will be pooled based on their size           
         // There will be pools dedicated for each allocation page size 
         // This effectively narrows the search for entries a bit       
         Size,

         // Data instances will be pooled based on their type           
         // Each meta definition will have its own pool chain           
         // This is the default pooling tactic for any meta data that   
         // is not reflected inside the "MAIN" boundary. See            
         // LANGULUS_RTTI_BOUNDARY for more information on that.        
         Type
      };
   #endif

   } // namespace Langulus::RTTI

} // namespace Langulus

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   /// The Langulus::RTTI::Boundary symbol is intentionally left undefined,   
   /// so that it is mandatory for you to define it inside your executables   
   /// or mods. It's a simple compile-time string, that is attached upon data 
   /// reflection, so that RTTI can track from which library a type was       
   /// reflected, and thus unregister it when shared object is unloaded.      
   /// The boundary also affects pooling tactics, because if boundary is not  
   /// equal exactly to "MAIN", pooling will be PoolTactic::Type by default,  
   /// so that allocation that happen from external libraries can be easily   
   /// tracked.                                                               
   #define LANGULUS_RTTI_BOUNDARY(a) \
      namespace Langulus::RTTI { \
         Token Boundary = a; \
      }
#else
   #define LANGULUS_RTTI_BOUNDARY(a)
#endif