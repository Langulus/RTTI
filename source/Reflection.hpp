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
#include <Core/TypeList.hpp>
#include <Core/Utilities.hpp>
#include <vector>

/// Provide a custom token to your data type, instead of using CppNameOf      
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
#define LANGULUS_INFO() \
   public: static constexpr ::Langulus::Token CTTI_Info = 

/// You can provide file format associations by extensions                    
/// You can specify multiple extensions by separating them with commas        
/// When serializing data of this type to a file, the first reflected         
/// extension will be used, if no extension has been explicitly stated for    
/// the file upon opening it for writing                                      
/// When deserializing from a file, the type to deserialize will be deduced   
/// by checking the file extension with the database                          
#define LANGULUS_FILES() \
   public: static constexpr ::Langulus::Token CTTI_Files = 

/// You can version your types                                                
/// When deserializing, you will get an error on major version mismatch       
/// If not specified, major version is always 1                               
#define LANGULUS_VERSION_MAJOR() \
   public: static constexpr ::Langulus::Count CTTI_VersionMajor = 

/// You can version your types                                                
/// When deserializing, you will get a warning on minor version mismatch      
/// If not specified, minor version is always 0                               
#define LANGULUS_VERSION_MINOR() \
   public: static constexpr ::Langulus::Count CTTI_VersionMinor = 

/// You can mark types as deep by using LANGULUS(DEEP) true / false inside    
/// class, but to fit into CT::Deep concept, your type must also inherit, and 
/// be the same size as an Anyness::Block                                     
#define LANGULUS_DEEP() \
   public: static constexpr bool CTTI_Deep = 

/// You can mark types as POD (Plain Old Data) by using LANGULUS(POD) true or 
/// false inside class. POD types are batch-copied via memcpy, and do not     
/// call constructors or destructors when contained (unless nullifiable)      
#define LANGULUS_POD() \
   public: static constexpr bool CTTI_POD = 

/// You can mark types as nullifiable, by using LANGULUS(NULLIFIABLE) true or 
/// false inside class. Nullifiable classes are batch-constructed and         
/// batch-destroyed via memset(0)                                             
#define LANGULUS_NULLIFIABLE() \
   public: static constexpr bool CTTI_Nullifiable = 

#if LANGULUS_FEATURE(MANAGED_MEMORY)
   /// You can choose how a given type is pooled, if managed memory is        
   /// enabled. See RTTI::PoolTactic for options                              
   #define LANGULUS_POOL_TACTIC() \
      public: static constexpr ::Langulus::RTTI::PoolTactic CTTI_Pool = 
#endif

/// You can make types concretizable, by using LANGULUS(CONCRETE) Type        
/// When dynamically creating your abstract objects, the most concrete type   
/// in the chain will be used instead                                         
#define LANGULUS_CONCRETE() \
   public: using CTTI_Concrete = 

/// You can make types producible only by executing a Verbs::Create in the    
/// context of specific producer                                              
#define LANGULUS_PRODUCER() \
   public: using CTTI_Producer = 

/// You can make types not insertable to Anyness containers, such as some     
/// intermediate types, like Block::KnownPointer. These types will produce    
/// a compile-time error when a push is attempted. All reflected types are    
/// insertable by default                                                     
#define LANGULUS_UNINSERTABLE() \
   public: static constexpr bool CTTI_Uninsertable =

/// You can make types unallocatable by the memory manager. This serves not   
/// only as forcing the type to be either allocated by conventional C++ means,
/// or on the stack, but also optimizes away any memory manager searches,     
/// when inserting pointers (when managed memory is enabled)                  
#define LANGULUS_UNALLOCATABLE() \
   public: static constexpr bool CTTI_Unallocatable =

/// You can give names to specific values of a given type, such as enums      
/// These names can be used as constants when parsing code, and are used for  
/// serialization to code/text/debug                                          
#define LANGULUS_NAMED_VALUES(T) \
   public: static constexpr ::Langulus::RTTI::CMetaTriplet<T> CTTI_NamedValues[] =

/// You can give an optional suffix to your type                              
/// That suffix is actively used when serializing to/deserializing from text  
#define LANGULUS_SUFFIX() \
   public: static constexpr ::Langulus::Token CTTI_Suffix = 

/// You can define an allocation page (number of elements) by using           
/// LANGULUS(ALLOCATION_PAGE) X. When allocating memory for your type, X      
/// will be the minimum amount of elements to allocate, aligned to the        
/// nearest upper power-of-two amount of bytes. By default, allocation page   
/// size	is never lower than Alignment                                        
#define LANGULUS_ALLOCATION_PAGE() \
   public: static constexpr ::Langulus::Count CTTI_AllocationPage = 

/// Make a type abstract                                                      
#define LANGULUS_ABSTRACT() \
   public: static constexpr bool CTTI_Abstract = 

/// Start reflecting members                                                  
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
#define LANGULUS_BASES(...) \
   public: using CTTI_Bases = ::Langulus::TTypeList<__VA_ARGS__>

/// Reflect a list of verbs                                                   
#define LANGULUS_VERBS(...) \
   public: using CTTI_Verbs = ::Langulus::TTypeList<__VA_ARGS__>

/// Reflect a list of possible conversions                                    
/// These will be automatically used by Verbs::Interpret if available         
#if LANGULUS_COMPILER(CLANG) or LANGULUS_COMPILER(GCC)
   #define LANGULUS_CONVERSIONS(...) \
      public: using CTTI_Conversions = ::Langulus::TTypeList<void __VA_OPT__(,) __VA_ARGS__>
#else
   #define LANGULUS_CONVERSIONS(...) \
      public: using CTTI_Conversions = ::Langulus::TTypeList<void, __VA_ARGS__>
#endif

/// You can make types CT::Typed and retrieve their inner type using TypeOf   
/// by adding LANGULUS(TYPED) <inner type>; as a member                       
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
      concept Uninsertable = T::CTTI_Uninsertable;

      template<class T>
      concept Unallocatable = not Complete<T> or T::CTTI_Unallocatable;

      template<class T>
      concept POD = Complete<T> and (::std::is_trivial_v<T> or T::CTTI_POD);

      template<class T>
      concept Nullifiable = Complete<T> and T::CTTI_Nullifiable;

      template<class T>
      concept Concretizable = Complete<T> and requires {
         typename T::CTTI_Concrete;
      };

      template<class T>
      concept Producible = Complete<T> and requires {
         typename T::CTTI_Producer;
      };

      template<class T>
      concept Abstract = Complete<T>
         and (::std::is_abstract_v<T> or T::CTTI_Abstract);

      template<class T>
      concept DispatcherMutable = requires (T* a, ::Langulus::Flow::Verb& b) {
         {DenseCast(a).Do(b)};
      };

      template<class T>
      concept DispatcherConstant = requires (const T* a, ::Langulus::Flow::Verb& b) {
         {DenseCast(a).Do(b)};
      };
      
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
   concept Insertable = (not Inner::Uninsertable<Decay<T>> and ...);

   /// You can make types unallocatable by the memory manager. This serves    
   /// not only as forcing the type to be either allocated by conventional    
   /// C++ means, or on the stack, but also optimizes away any memory manager 
   /// searches, when inserting pointers, if managed memory is enabled        
   template<class... T>
   concept Unallocatable = ((Inner::Unallocatable<Decay<T>> and Dense<T>) and ...);

   template<class... T>
   concept Allocatable = ((not Inner::Unallocatable<Decay<T>> and Dense<T>) and ...);

   /// A POD (Plain Old Data) type is any type with a static member           
   /// T::CTTI_POD set to true. If no such member exists, the type is         
   /// assumed NOT POD by default, unless ::std::is_trivial.                  
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

} // namespace Langulus

namespace Langulus::CT
{

   /// Check if a type has an underlying type defined                         
   template<class... T>
   concept Typed = ((Inner::Typed<Decay<T>> and Data<TypeOf<T>>) and ...);

   /// Custom boolean concept (wrapped in another type)                       
   template<class... T>
   concept CustomBool = ((Typed<T> and
         BuiltinBool<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom character concept (wrapped in another type)                     
   template<class... T>
   concept CustomCharacter = ((Typed<T> and
         BuiltinCharacter<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom integer concept (wrapped in another type)                       
   /// Unlike CT::Integer, this includes character and boolean types          
   template<class... T>
   concept CustomInteger = ((Typed<T> and
         ::std::is_integral_v<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);

   /// Custom real concept (wrapped in another type)                          
   template<class... T>
   concept CustomReal = ((Typed<T> and
         ::std::is_floating_point_v<TypeOf<T>> and sizeof(T) == sizeof(TypeOf<T>)
      ) and ...);
   
   /// Custom number concept (either sparse or dense)                         
   /// Any T that has underlying arithmetic type and is binary compatible     
   /// Unlike CT::BuiltinNumber, this one includes booleans and characters    
   template<class... T>
   concept CustomNumber = ((
      CustomBool<T> or CustomCharacter<T> or CustomInteger<T> or CustomReal<T>
   ) and ...);

   /// Any real concept, custom or not (either sparse or dense)               
   template<class... T>
   concept Real = ((BuiltinReal<T> or CustomReal<T>) and ...);

   /// Dense real concept                                                     
   template<class... T>
   concept DenseReal = ((Real<T> and Dense<T>) and ...);

   /// Sparse real concept                                                    
   template<class... T>
   concept SparseReal = ((Real<T> and Sparse<T>) and ...);

   /// Any integer concept, custom or not (either sparse or dense)            
   template<class... T>
   concept Integer = ((BuiltinInteger<T> or CustomInteger<T>) and ...);

   /// Dense integer concept                                                  
   template<class... T>
   concept DenseInteger = ((Integer<T> and Dense<T>) and ...);

   /// Sparse integer concept                                                 
   template<class... T>
   concept SparseInteger = ((Integer<T> and Sparse<T>) and ...);
   
   /// Any signed integer concept, custom or not (either sparse or dense)     
   template<class... T>
   concept SignedInteger = ((Signed<T> and Integer<T>) and ...);

   /// Any dense signed integer concept, custom or not                        
   template<class... T>
   concept DenseSignedInteger = ((SignedInteger<T> and Dense<T>) and ...);

   /// Any sparse signed integer concept, custom or not                       
   template<class... T>
   concept SparseSignedInteger = ((SignedInteger<T> and Sparse<T>) and ...);

   /// Any unsigned integer concept, custom or not (either sparse or dense)   
   template<class... T>
   concept UnsignedInteger = ((Unsigned<T> and Integer<T>) and ...);

   /// Any dense unsigned integer concept, custom or not                      
   template<class... T>
   concept DenseUnsignedInteger = ((UnsignedInteger<T> and Dense<T>) and ...);

   /// Any sparse unsigned integer concept, custom or not                     
   template<class... T>
   concept SparseUnsignedInteger = ((UnsignedInteger<T> and Sparse<T>) and ...);

   /// Any bool concept, custom or not (either sparse or dense)               
   template<class... T>
   concept Bool = ((BuiltinBool<T> or CustomBool<T>) and ...);

   /// Dense bool concept                                                     
   template<class... T>
   concept DenseBool = ((Bool<T> and Dense<T>) and ...);

   /// Sparse bool concept                                                    
   template<class... T>
   concept SparseBool = ((Bool<T> and Sparse<T>) and ...);

   /// Any character concept, custom or not (either sparse or dense)          
   template<class... T>
   concept Character = ((BuiltinCharacter<T> or CustomCharacter<T>) and ...);

   /// Dense character concept                                                
   template<class... T>
   concept DenseCharacter = ((Character<T> and Dense<T>) and ...);

   /// Sparse character concept                                               
   template<class... T>
   concept SparseCharacter = ((Character<T> and Sparse<T>) and ...);

   /// Any number concept, custom or not (either sparse or dense)             
   /// Excludes boolean and character types, unless wrapped in another type   
   template<class... T>
   concept Number = ((BuiltinNumber<T> or CustomNumber<T>) and ...);
   
   /// Dense number concept                                                   
   /// Excludes boolean and character types, unless wrapped in another type   
   template<class... T>
   concept DenseNumber = ((Number<T> and Dense<T>) and ...);

   /// Sparse number concept                                                  
   /// Excludes boolean types and character types, unless wrapped in another  
   template<class... T>
   concept SparseNumber = ((Number<T> and Sparse<T>) and ...);

   /// Single precision real number concept                                   
   template<class... T>
   concept Float   = (Same<::Langulus::Float, T> and ...);

   /// Double precision real number concept                                   
   template<class... T>
   concept Double  = (Same<::Langulus::Double, T> and ...);

   /// Size-related number concepts                                           
   /// These concepts include character and byte types                        
   template<class... T>
   concept SignedInteger8  = ((CT::SignedInteger<T> and sizeof(Decay<T>) == 1) and ...);
   template<class... T>
   concept SignedInteger16 = ((CT::SignedInteger<T> and sizeof(Decay<T>) == 2) and ...);
   template<class... T>
   concept SignedInteger32 = ((CT::SignedInteger<T> and sizeof(Decay<T>) == 4) and ...);
   template<class... T>
   concept SignedInteger64 = ((CT::SignedInteger<T> and sizeof(Decay<T>) == 8) and ...);

   template<class... T>
   concept UnsignedInteger16 = (((CT::UnsignedInteger<T> or CT::Character<T>)
       and sizeof(Decay<T>) == 2) and ...);

   template<class... T>
   concept UnsignedInteger32 = (((CT::UnsignedInteger<T> or CT::Character<T>)
       and sizeof(Decay<T>) == 4) and ...);

   template<class... T>
   concept UnsignedInteger64 = (((CT::UnsignedInteger<T> or CT::Character<T>)
       and sizeof(Decay<T>) == 8) and ...);

   template<class... T>
   concept Integer16 = ((SignedInteger16<T> or UnsignedInteger16<T>) and ...);
   template<class... T>
   concept Integer32 = ((SignedInteger32<T> or UnsignedInteger32<T>) and ...);
   template<class... T>
   concept Integer64 = ((SignedInteger64<T> or UnsignedInteger64<T>) and ...);

   namespace Inner
   {
      template<class T>
      constexpr Count CountOf() noexcept {
         using DT = Decay<T>;
         if constexpr (requires {{DT::MemberCount} -> UnsignedInteger; })
            return DT::MemberCount * ExtentOf<T>;
         else if constexpr (requires (DT a) {{a.size()} -> UnsignedInteger;}) {
            if constexpr (IsConstexpr([] { return DT {}.size(); }))
               return DT {}.size() * ExtentOf<T>;
            else
               return ExtentOf<T>;
         }
         else return ExtentOf<T>;
      }

      template<class T>
      concept Vector = (Typed<T> and CountOf<T>() > 1
              and sizeof(T) == sizeof(TypeOf<T>) * CountOf<T>())
           or (ExtentOf<T>) > 1;

      template<class T>
      concept Scalar = not Vector<T>;
   }
         
   /// Vector concept                                                         
   /// Any type that is Typed and has CountOf that is at least 2, and         
   /// the T's size is exactly equal to sizeof(TypeOf<T>) * CountOf<T>        
   /// Additionally, bounded arrays with more than a single element are also  
   /// considered Vector                                                      
   template<class... T>
   concept Vector = (Inner::Vector<T> and ...);

   template<class... T>
   concept DenseVector = ((Dense<T> and Inner::Vector<T>) and ...);

   /// Scalar concept                                                         
   /// Any type that isn't a Vector type                                      
   template<class... T>
   concept Scalar = (Inner::Scalar<T> and ...);

   template<class... T>
   concept DenseScalar = ((Dense<T> and Inner::Scalar<T>) and ...);

} // namespace Langulus::CT


namespace Langulus
{

   /// Get the count of a counted type, or 0 if T is not counted              
   /// Any type with a static constexpr unsigned MemberCount is counted       
   template<class T>
   constexpr Count CountOf = CT::Inner::CountOf<T>();

   /// Casts a scalar to its underlying fundamental type (const)              
   /// If T::CTTI_InnerType exists, or if T is an enum, the inner type returns
   ///   @tparam T - type of the scalar/enum to cast                          
   ///   @param a - the scalar to cast                                        
   ///   @return a reference to the underlying type                           
   template<CT::Scalar T>
   NOD() LANGULUS(INLINED)
   constexpr decltype(auto) FundamentalCast(const T& a) noexcept {
      if constexpr (CT::Fundamental<T>) {
         // Already fundamental, just forward it                        
         return (a);
      }
      else if constexpr (CT::Typed<T> or CT::Enum<T>) {
         // Explicitly cast to a reference of the contained type, and   
         // nest down to the fundamentals                               
         return FundamentalCast(static_cast<const TypeOf<T>&>(DenseCast(a)));
      }
      else LANGULUS_ERROR("Shouldn't happen");
   }
   
   /// Casts a scalar to its underlying fundamental type                      
   /// If T::CTTI_InnerType exists, or if T is an enum, the inner type returns
   ///   @tparam T - type of the scalar/enum to cast                          
   ///   @param a - the scalar to cast                                        
   ///   @return a reference to the underlying type                           
   template<CT::Scalar T>
   NOD() LANGULUS(INLINED)
   constexpr decltype(auto) FundamentalCast(T& a) noexcept {
      if constexpr (CT::Fundamental<T>) {
         // Already fundamental, just forward it                        
         return (a);
      }
      else if constexpr (CT::Typed<T> or CT::Enum<T>) {
         // Explicitly cast to a reference of the contained type, and   
         // nest down to the fundamentals                               
         return FundamentalCast(static_cast<TypeOf<T>&>(DenseCast(a)));
      }
      else LANGULUS_ERROR("Shouldn't happen");
   }
    
   /// Returns the extent overlap of two arrays/non arrays                    
   /// An array to be considered array, it has to have more than one element  
   ///   @tparam LHS - left type                                              
   ///   @tparam RHS - right type                                             
   ///   @return the overlapping count:                                       
   ///           the smaller extent, if two arrays are provided;              
   ///           the bigger extent, if one of the arguments isn't an array    
   ///           1 if both arguments are not arrays                           
   template<class LHS, class RHS>
   NOD() constexpr Count OverlapExtents() noexcept {
      constexpr auto lhs = ExtentOf<LHS>;
      constexpr auto rhs = ExtentOf<RHS>;

      if constexpr (lhs > 1 and rhs > 1)
         return lhs < rhs ? lhs : rhs;
      else if constexpr (lhs > 1)
         return lhs;
      else if constexpr (rhs > 1)
         return rhs;
      else
         return 1;
   }

   #define OVERLAP_EXTENTS(l,r) OverlapExtents<decltype(l), decltype(r)>()
   
   /// Returns the count overlap of two vectors/scalars                       
   /// This is used to decide the output array size, for containing the       
   /// result of an arithmetic operation                                      
   ///   @tparam LHS - left type                                              
   ///   @tparam RHS - right type                                             
   ///   @return the overlapping count:                                       
   ///           the smaller extent, if two arrays are provided;              
   ///           the bigger extent, if one of the arguments isn't a vector    
   ///           1 if both arguments are not arrays                           
   template<class LHS, class RHS>
   NOD() constexpr Count OverlapCounts() noexcept {
      constexpr auto lhs = CountOf<LHS>;
      constexpr auto rhs = CountOf<RHS>;

      if constexpr (lhs > 1 and rhs > 1)
         return lhs < rhs ? lhs : rhs;
      else if constexpr (lhs > 1)
         return lhs;
      else if constexpr (rhs > 1)
         return rhs;
      else
         return 1;
   }

   namespace Inner
   {

      /// When given two types, choose the one that is most lossless in terms 
      /// of behavior, and capacity                                           
      ///  - if T1 or T2 is an array, an array of OverlapCount size will be   
      ///    given back. The array will be of the lossless decayed type       
      ///  - if both types are reals, the bigger real will be returned        
      ///  - if one of the type is a real, and the other an integer, the real 
      ///    will be returned                                                 
      ///  - if both types are integers with different signs, always returns  
      ///    the signed equivalent of the bigger integer                      
      ///  - if one of the types is not CT::Fundamental type, it will always  
      ///    be preferred, as it may have custom behavior                     
      ///  - if both types are not CT::Fundamental, the first type is always  
      ///    preferred (fallback)                                             
      ///   @attention this will discard any sparseness or other modifiers    
      template<class T1, class T2>
      constexpr auto Lossless() noexcept {
         constexpr auto size = OverlapCounts<T1, T2>();
         using LHS = Decay<TypeOf<T1>>;
         using RHS = Decay<TypeOf<T2>>;

         if constexpr (CT::Fundamental<LHS, RHS>) {
            // Both types are fundamental                               
            if constexpr (CT::Real<LHS, RHS>) {
               // Always prefer the bigger real number                  
               if constexpr (sizeof(LHS) >= sizeof(RHS))
                  return ::std::array<LHS, size> {};
               else
                  return ::std::array<RHS, size> {};
            }
            else if constexpr (CT::Real<LHS> and not CT::Real<RHS>) {
               // Always prefer real numbers                            
               return ::std::array<LHS, size> {};
            }
            else if constexpr (CT::Real<RHS> and not CT::Real<LHS>) {
               // Always prefer real numbers                            
               return ::std::array<RHS, size> {};
            }
            else if constexpr (CT::Signed<LHS> == CT::Signed<RHS>) {
               // Both are signed integers, so pick the bigger one      
               if constexpr (sizeof(LHS) >= sizeof(RHS))
                  return ::std::array<LHS, size> {};
               else
                  return ::std::array<RHS, size> {};
            }
            else if constexpr (CT::Signed<LHS>) {
               // LHS is signed, but RHS is not, so pick the signed one,
               // but also guarantee that size remains the bigger one   
               if constexpr (sizeof(LHS) >= sizeof(RHS))
                  return ::std::array<LHS, size> {};
               else
                  return ::std::array<::std::make_signed_t<RHS>, size> {};
            }
            else {
               // RHS is signed, but LHS is not, so pick the signed one,
               // but also guarantee that size remains the bigger one   
               if constexpr (sizeof(RHS) >= sizeof(LHS))
                  return ::std::array<RHS, size> {};
               else
                  return ::std::array<::std::make_signed_t<LHS>, size> {};
            }
         }
         else if constexpr (CT::Fundamental<LHS>) {
            // RHS isn't fundamental, so always prefer it               
            return ::std::array<RHS, size> {};
         }
         else {
            // Either both types aren't fundamental, or the RHS one is  
            // Just fallback to LHS                                     
            return ::std::array<LHS, size> {};
         }
      }
   }

   /// When given two types, choose the one that is most lossless             
   /// after an arithmetic operation is performed between them. If T1 or T2   
   /// is an array, an array of OverlapCount size will be given back.         
   ///   @attention this will discard any sparseness or other modifiers       
   template<class T1, class T2,
      class INNER = decltype(Inner::Lossless<T1, T2>()),
      Count SIZE = INNER {}.size()>
   using Lossless = Conditional<
      SIZE == 1, typename INNER::value_type, typename INNER::value_type [SIZE]
   >;

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