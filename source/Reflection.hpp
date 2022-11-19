///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once

/// You can provide a custom token to your data type, instead of using NameOf 
#define LANGULUS_NAME() \
   public: static constexpr ::Langulus::Token CTTI_Name = 

/// You can provide a custom token to your verbs, instead of using NameOf     
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
/// class, but to fit into CT::Deep concept, your type must also inherit Block
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

/// You can choose how a given type is pooled, if managed memory is enabled   
/// See RTTI::PoolTactic for options                                          
#define LANGULUS_POOL_TACTIC() \
   public: static constexpr ::Langulus::RTTI::PoolTactic CTTI_Pool = 

/// You can make types concretizable, by using LANGULUS(CONCRETE) Type        
/// When dynamically creating your abstract objects, the most concrete type   
/// in the chain will be used instead                                         
#define LANGULUS_CONCRETE() \
   public: using CTTI_Concrete = 

/// You can make types producible only by executing a Verbs::Create in        
/// a provided type                                                           
#define LANGULUS_PRODUCER() \
   public: using CTTI_Producer = 

/// You can make types not insertable to Anyness containers, such as some     
/// intermediate types, like Block::KnownPointer. These types will produce    
/// a compile-time error when a push is attempted. All reflected types are    
/// insertable by default                                                     
#define LANGULUS_UNINSERTABLE() \
   public: static constexpr bool CTTI_Uninsertable =

/// You can give names to specific values of a given type, such as enums      
/// These names can be used as constants when parsing code, and are used for  
/// serialization to code/text/debug                                          
#define LANGULUS_NAMED_VALUES(T) \
   public: static constexpr ::Langulus::RTTI::CMetaTriplet<T> CTTI_NamedValues[] =

/// You can give an optional suffix to your type                              
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
#define LANGULUS_CONVERSIONS(...) \
   public: using CTTI_Conversions = ::Langulus::TTypeList<__VA_ARGS__>

#include "DataState.hpp"
#include "NameOf.hpp"
#include <vector>

LANGULUS_EXCEPTION(Meta);


/// Compile-time checks and concepts associated with RTTI                     
namespace Langulus::CT
{

   namespace Inner
   {
      template<class T>
      concept Reflectable = requires {
         {Decay<T>::Reflect()} -> Same<::Langulus::RTTI::MetaData>;
      };
   }

   /// A reflected type is a type that has a public Reflection field          
   /// This field is automatically added when using LANGULUS(REFLECT) macro   
   /// inside the type you want to reflect                                    
   template<class... T>
   concept Reflectable = (Inner::Reflectable<T> && ...);

   namespace Inner
   {
      template<class T>
      concept Uninsertable = Decay<T>::CTTI_Uninsertable;
   }

   /// An uninsertable type is any type with a true static member             
   /// T::CTTI_Uninsertable. All types are insertable by default              
   /// Useful to mark some intermediate types, that are not supposed to be    
   /// inserted in containers                                                 
   template<class... T>
   concept Uninsertable = (Inner::Uninsertable<T> && ...);

   template<class... T>
   concept Insertable = !Uninsertable<T...>;

   namespace Inner
   {
      template<class T>
      concept POD = ::std::is_trivial_v<Decay<T>> || Decay<T>::CTTI_POD;
   }

   /// A POD (Plain Old Data) type is any type with a static member           
   /// T::CTTI_POD set to true. If no such member exists, the type is         
   /// assumed NOT POD by default, unless ::std::is_trivial.                  
   /// POD types improve construction, destruction, copying, and cloning      
   /// by using some batching runtime optimizations                           
   /// All POD types are also directly serializable to binary                 
   /// Use LANGULUS(POD) macro as member to tag POD types                     
   template<class... T>
   concept POD = (Inner::POD<T> && ...);

   namespace Inner
   {
      template<class T>
      concept Nullifiable = Decay<T>::CTTI_Nullifiable;
   }

   /// A nullifiable type is any type with a static member                    
   /// T::CTTI_Nullifiable set to true. If no such member exists, the type    
   /// is assumed NOT nullifiable by default                                  
   /// Nullifiable types improve construction by using some batching          
   /// runtime optimizations                                                  
   /// Use LANGULUS(NULLIFIABLE) macro as member to tag nullifiable types     
   template<class... T>
   concept Nullifiable = (Inner::Nullifiable<T> && ...);

   namespace Inner
   {
      template<class T>
      concept Concretizable = requires {
         typename Decay<T>::CTTI_Concrete;
      };
   }

   /// A concretizable type is any type with a member type CTTI_Concrete      
   /// If no such member exists, the type is assumed NOT concretizable by     
   /// default. Concretizable types provide a default concretization for      
   /// when	allocating abstract types                                         
   /// Use LANGULUS(CONCRETIZABLE) macro as member to tag such types          
   template<class... T>
   concept Concretizable = (Inner::Concretizable<T> && ...);

   /// Get the reflected concrete type                                        
   template<class T>
   using ConcreteOf = typename Decay<T>::CTTI_Concrete;

   namespace Inner
   {
      template<class T>
      concept Producible = requires {
         typename Decay<T>::CTTI_Producer;
      };
   }

   /// A producible type is any type with a member type CTTI_Producer         
   /// If no such member exists, the type is assumed NOT producible by        
   /// default. Producible types can not be created at compile-time, and need 
   /// to be produced by executing Verbs::Create in the producer's context    
   /// Use LANGULUS(PRODUCER) macro as member to tag such types               
   template<class... T>
   concept Producible = (Inner::Producible<T> && ...);

   /// Get the reflected producer type                                        
   template<class T>
   using ProducerOf = typename Decay<T>::CTTI_Producer;

   /// Check if T is abstract (has at least one pure virtual function)        
   template<class... T>
   concept Abstract = ((!Sparse<T> && (::std::is_abstract_v<Decay<T>> || Decay<T>::CTTI_Abstract)) && ...);

   namespace Inner
   {
      template<class T>
      concept DispatcherMutable = requires (Decay<T>& a, ::Langulus::Flow::Verb& b) {
         {a.Do(b)};
      };
      template<class T>
      concept DispatcherConstant = requires (const Decay<T>& a, ::Langulus::Flow::Verb& b) {
         {a.Do(b)};
      };
   }

   /// Check if all T have a mutable dispatcher (has Do() method for verbs)   
   template<class... T>
   concept DispatcherMutable = (Inner::DispatcherMutable<T> && ...);

   /// Check if all T have a constant dispatcher (has Do() method for verbs)  
   template<class... T>
   concept DispatcherConstant = (Inner::DispatcherConstant<T> && ...);

   /// Check if all T has a dispatcher, compatible with the cv-quality of T   
   template<class... T>
   concept Dispatcher = ((CT::DispatcherMutable<T> || (CT::Constant<T> && CT::DispatcherConstant<T>)) && ...);

   namespace Inner
   {
      template<class T>
      concept Typed = requires {
         typename Decay<T>::MemberType;
      };

      /// A convenience function that wraps std::underlying_type_t for enums, 
      /// as well as anything with MemberType defined                         
      template<Dense T>
      constexpr auto GetUnderlyingType() noexcept {
         using DT = Decay<T>;
         if constexpr (Typed<DT>)
            return (typename DT::MemberType*) nullptr;
         else if constexpr (::std::is_enum_v<DT>)
            return (::std::underlying_type_t<DT>*) nullptr;
         else
            return (DT*) nullptr;
      };

   } //namespace Langulus::CT::Inner

} // namespace Langulus::CT


namespace Langulus
{

   /// Get internal type of an enum, custom number, statically optimized      
   /// container or vector                                                    
   template<CT::Dense T>
   using TypeOf = Deptr<decltype(CT::Inner::GetUnderlyingType<T>())>;

} // namespace Langulus


namespace Langulus::CT
{

   /// Check if a type has an underlying type defined                         
   template<class... T>
   concept Typed = ((Inner::Typed<T> && Data<TypeOf<T>>) && ...);

   /// Custom boolean concept (wrapped in another type)                       
   template<class... T>
   concept CustomBool = ((Typed<T> &&
      BuiltinBool<TypeOf<T>> && sizeof(T) == sizeof(TypeOf<T>)
   ) && ...);

   /// Custom character concept (wrapped in another type)                     
   template<class... T>
   concept CustomCharacter = ((Typed<T> &&
      BuiltinCharacter<TypeOf<T>> && sizeof(T) == sizeof(TypeOf<T>)
   ) && ...);

   /// Custom integer concept (wrapped in another type)                       
   /// Unlike CT::Integer, this includes character and boolean types          
   template<class... T>
   concept CustomInteger = ((Typed<T> &&
      ::std::is_integral_v<TypeOf<T>> && sizeof(T) == sizeof(TypeOf<T>)
   ) && ...);

   /// Custom real concept (wrapped in another type)                          
   template<class... T>
   concept CustomReal = ((Typed<T> &&
      ::std::is_floating_point_v<TypeOf<T>> && sizeof(T) == sizeof(TypeOf<T>)
   ) && ...);
   
   /// Custom number concept (either sparse or dense)                         
   /// Any T that has underlying arithmetic type and is binary compatible     
   /// Unlike CT::BuiltinNumber, this one includes booleans and characters    
   template<class... T>
   concept CustomNumber = ((
      CustomBool<T> || CustomCharacter<T> || CustomInteger<T> || CustomReal<T>
   ) && ...);

   /// Any real concept, custom or not (either sparse or dense)               
   template<class... T>
   concept Real = ((BuiltinReal<T> || CustomReal<T>) && ...);

   /// Dense real concept                                                     
   template<class... T>
   concept DenseReal = ((Real<T> && Dense<T>) && ...);

   /// Sparse real concept                                                    
   template<class... T>
   concept SparseReal = ((Real<T> && Sparse<T>) && ...);

   /// Any integer concept, custom or not (either sparse or dense)            
   template<class... T>
   concept Integer = ((BuiltinInteger<T> || CustomInteger<T>) && ...);

   /// Dense integer concept                                                  
   template<class... T>
   concept DenseInteger = ((Integer<T> && Dense<T>) && ...);

   /// Sparse integer concept                                                 
   template<class... T>
   concept SparseInteger = ((Integer<T> && Sparse<T>) && ...);
   
   /// Any signed integer concept, custom or not (either sparse or dense)     
   template<class... T>
   concept SignedInteger = ((Signed<T> && Integer<T>) && ...);

   /// Any dense signed integer concept, custom or not                        
   template<class... T>
   concept DenseSignedInteger = ((SignedInteger<T> && Dense<T>) && ...);

   /// Any sparse signed integer concept, custom or not                       
   template<class... T>
   concept SparseSignedInteger = ((SignedInteger<T> && Sparse<T>) && ...);

   /// Any unsigned integer concept, custom or not (either sparse or dense)   
   template<class... T>
   concept UnsignedInteger = ((Unsigned<T> && Integer<T>) && ...);

   /// Any dense unsigned integer concept, custom or not                      
   template<class... T>
   concept DenseUnsignedInteger = ((UnsignedInteger<T> && Dense<T>) && ...);

   /// Any sparse unsigned integer concept, custom or not                     
   template<class... T>
   concept SparseUnsignedInteger = ((UnsignedInteger<T> && Sparse<T>) && ...);

   /// Any bool concept, custom or not (either sparse or dense)               
   template<class... T>
   concept Bool = ((BuiltinBool<T> || CustomBool<T>) && ...);

   /// Dense bool concept                                                     
   template<class... T>
   concept DenseBool = ((Bool<T> && Dense<T>) && ...);

   /// Sparse bool concept                                                    
   template<class... T>
   concept SparseBool = ((Bool<T> && Sparse<T>) && ...);

   /// Any character concept, custom or not (either sparse or dense)          
   template<class... T>
   concept Character = ((BuiltinCharacter<T> || CustomCharacter<T>) && ...);

   /// Dense character concept                                                
   template<class... T>
   concept DenseCharacter = ((Character<T> && Dense<T>) && ...);

   /// Sparse character concept                                               
   template<class... T>
   concept SparseCharacter = ((Character<T> && Sparse<T>) && ...);

   /// Any number concept, custom or not (either sparse or dense)             
   /// Excludes boolean and character types, unless wrapped in another type   
   template<class... T>
   concept Number = ((BuiltinNumber<T> || CustomNumber<T>) && ...);
   
   /// Dense number concept                                                   
   /// Excludes boolean and character types, unless wrapped in another type   
   template<class... T>
   concept DenseNumber = ((Number<T> && Dense<T>) && ...);

   /// Sparse number concept                                                  
   /// Excludes boolean types and character types, unless wrapped in another  
   template<class... T>
   concept SparseNumber = ((Number<T> && Sparse<T>) && ...);

} // namespace Langulus::CT


namespace Langulus
{

   /// Casts a number to its underlying type                                  
   template<CT::DenseNumber T>
   NOD() LANGULUS(ALWAYSINLINE) constexpr decltype(auto) BuiltinCast(const T& a) noexcept {
      if constexpr (CT::BuiltinNumber<T>) {
         // Already built-in, just forward it                           
         return a;
      }
      else {
         // Explicitly cast to a reference of the contained type        
         return static_cast<const TypeOf<T>&>(a);
      }
   }

   /// When given two arithmetic types, choose the one that is most lossless  
   /// after an arithmetic operation of any kind is performed between both    
   /// Works with both custom and builtin arithmetic types                    
   template<class T1, class T2>
   using Lossless = Conditional<
      // Always pick real numbers over integers if available            
      (CT::Real<T1>&& CT::Integer<T2>)
      // Always pick signed type if available                           
      || (CT::Signed<T1> && CT::Unsigned<T2>)
      // Always pick the larger type as a last resort                   
      || (sizeof(Decay<T1>) > sizeof(Decay<T2>)
   ), Decay<T1>, Decay<T2>>;
   
   /// A type naming convention for standard number types                     
   ///   @return the suffix depending on the template argument                
   template<class T>
   constexpr Token TypeSuffix() {
      if constexpr (CT::Same<T, signed int>)
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
         LANGULUS_ERROR("Unsupported atomic size");
   }

} // namespace Langulus

namespace Langulus::RTTI
{

   ///                                                                        
   ///   Meta                                                                 
   ///                                                                        
   /// Base for meta definitions                                              
   ///                                                                        
   struct Meta {
   public:
      enum MetaType {
         Data, Trait, Verb, Constant
      };

      virtual MetaType GetMetaType() const noexcept = 0;

      // Each reflection primitive has a unique token, but that         
      // uniqueness is checked only if MANAGED_REFLECTION feature is    
      // enabled                                                        
      Token mToken;
      // Each reflection may or may not have some info                  
      Token mInfo {"<no info provided>"};
      // Original name of the type                                      
      Token mCppName;
      // Each reflected type has an unique hash                         
      Hash mHash {};
      // Major version                                                  
      Count mVersionMajor {1};
      // Minor version                                                  
      Count mVersionMinor {0};
      // References, increased each time a definition is merged,        
      // and decreases each time a definition is unregistered           
      Count mReferences {1};

      NOD() const Hash& GetHash() const noexcept;

      template<CT::Data T>
      NOD() static constexpr Hash GenerateHash(const Token&) noexcept;
      template<CT::Data T>
      NOD() static constexpr Token GetCppName() noexcept;

      virtual ~Meta() = default;
   };

   template<class T>
   struct CMetaTriplet {
      Token mToken;
      T mValue;
      Token mInfo {};
   };

} // namespace Langulus::RTTI


namespace Langulus::CT
{
   /// Concept for meta definitions                                           
   template<class... T>
   concept Meta = (DerivedFrom<T, ::Langulus::RTTI::Meta> && ...);
}


namespace std
{
   using ::Langulus::RTTI::DMeta;
   using ::Langulus::RTTI::TMeta;
   using ::Langulus::RTTI::VMeta;

   template<>
   struct hash<DMeta> {
      LANGULUS(ALWAYSINLINE) size_t operator()(DMeta k) const noexcept;
   };
   template<>
   struct hash<vector<DMeta>> {
      LANGULUS(ALWAYSINLINE) size_t operator()(const vector<DMeta>& k) const noexcept;
   };
   template<>
   struct hash<TMeta> {
      LANGULUS(ALWAYSINLINE) size_t operator()(TMeta k) const noexcept;
   };
   template<>
   struct hash<VMeta> {
      LANGULUS(ALWAYSINLINE) size_t operator()(VMeta k) const noexcept;
   };
}

#include "Byte.hpp"
#include "MetaData.hpp"
#include "MetaTrait.hpp"
#include "MetaVerb.hpp"
#include "Fundamental.hpp"

#include "MetaData.inl"
#include "MetaTrait.inl"
#include "MetaVerb.inl"
#include "Reflection.inl"
#include "Hashing.hpp"

namespace std
{
   size_t hash<DMeta>::operator()(DMeta k) const noexcept {
      return k->mHash.mHash;
   }
   size_t hash<vector<DMeta>>::operator()(const vector<DMeta>& k) const noexcept {
      using ::Langulus::Hash;
      vector<Hash> coalesced;
      for (auto& i : k)
         coalesced.emplace_back(i->mHash);
      return ::Langulus::HashBytes<::Langulus::DefaultHashSeed, false>(
         coalesced.data(), 
         static_cast<int>(coalesced.size() * sizeof(Hash))
      ).mHash;
   }
   size_t hash<TMeta>::operator()(TMeta k) const noexcept {
      return k->mHash.mHash;
   }
   size_t hash<VMeta>::operator()(VMeta k) const noexcept {
      return k->mHash.mHash;
   }
}

