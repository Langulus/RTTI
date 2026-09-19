///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include <Langulus/Typenav.hpp>
#include <Langulus/CT/Abstract.hpp>
#include <Langulus/CT/ReflectAs.hpp>
#include <Langulus/CT/DefineTag.hpp>
#include <Langulus/CT/DefineVerb.hpp>
#include <Langulus/CT/Defaultable.hpp>
#include <Langulus/CT/Destroyable.hpp>
#include <Langulus/CT/Deep.hpp>
#include <Langulus/CT/Referenced.hpp>
#include <Langulus/CT/Resolvable.hpp>
#include <Langulus/CT/Nullable.hpp>
#include <Langulus/CT/POD.hpp>
#include <Langulus/CT/Concrete.hpp>
#include <Langulus/CT/Producer.hpp>
#include <Langulus/CT/Convertible.hpp>
#include <Langulus/CT/DefineConst.hpp>
#include <Langulus/CT/Members.hpp>
#include <Langulus/CT/MinAlloc.hpp>
#include <Langulus/CT/Files.hpp>
#include <Langulus/CT/Suffix.hpp>
#include <Langulus/CT/Serializer.hpp>
#include <Langulus/CT/Executable.hpp>
#include <Langulus/CT/Tags.hpp>
#include <Langulus/IntentOf.hpp>

#include "DefinitionData.hpp"

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   #include "Registry.hpp"
#else
   #include <optional>
#endif

#include "DefinitionVerb.hpp"
#include "DefinitionConst.hpp"
#include "DefinitionTag.hpp"

#if 0 or LANGULUS_META_VERBOSITY_MASTER_SWITCH()
   #include <Langulus/Logger/EnableVerbose.hpp>
#else
   #include <Langulus/Logger/NoVerbose.hpp>
#endif


namespace Langulus::RTTI
{
   namespace Inner
   {
      /// Function callbacks for custom pointers                              
      template<CT::CustomPointer T> LANGULUS(NOINLINE)
      void CustomSparseDefaultDeref(void* from, void* to) {
         static_assert(CT::NotConvolutedAnywhere<T>,
            "Strip qualifiers to avoid unnecessary instantiations");
         using DenserT = Deptr<T>;
         auto typed_from = static_cast<T*>(from);
         auto typed_to   = static_cast<DenserT*>(to);
         *typed_to = **typed_from;
      };

      template<::std::integral T> LANGULUS(NOINLINE)
      void CustomSparseDefaultConstructor(void* at) noexcept {
         static_assert(CT::Decayed<T>,
            "Strip qualifiers to avoid unnecessary instantiations");
         auto atT = static_cast<T*>(at);
         new (atT) T{};
      };

      template<::std::integral T> LANGULUS(NOINLINE)
      void CustomSparseCopyConstructor(void* from, void* to) noexcept {
         static_assert(CT::Decayed<T>,
            "Strip qualifiers to avoid unnecessary instantiations");
         auto fromT = static_cast<T*>(from);
         auto toT = static_cast<T*>(to);
         *toT = *fromT;
      };
      
      template<::std::integral T> LANGULUS(NOINLINE)
      auto CustomSparseCompare(const void* lhs, const void* rhs) noexcept -> Compared {
         static_assert(CT::Decayed<T>,
            "Strip qualifiers to avoid unnecessary instantiations");
         auto lhsT = static_cast<T const*>(lhs);
         auto rhsT = static_cast<T const*>(rhs);
         return *lhsT == *rhsT ? Compared::Equal : Compared::Unordered;
      };

      template<::std::integral T> LANGULUS(NOINLINE)
      bool CustomSparseCompareEqual(const void* lhs, const void* rhs) noexcept {
         static_assert(CT::Decayed<T>,
            "Strip qualifiers to avoid unnecessary instantiations");
         auto lhsT = static_cast<T const*>(lhs);
         auto rhsT = static_cast<T const*>(rhs);
         return *lhsT == *rhsT;
      };

      template<::std::integral T> LANGULUS(NOINLINE)
      auto CustomSparseHash(void* lhs) noexcept -> Hash {
         static_assert(CT::Decayed<T>,
            "Strip qualifiers to avoid unnecessary instantiations");
         auto lhsT = static_cast<T*>(lhs);
         return HashOf<true>(*lhsT);
      };
   }
   
   
   /// Reflect or return an already reflected data                            
   ///   @attention when making a shared library and reflecting your types    
   ///      at library initialization, it is recommended you mark all other   
   ///      relevant instantiations of this function as extern template, to   
   ///      save on a lot of compiler resources:                              
   ///      https://stackoverflow.com/questions/8130602                       
   ///   @tparam T the type to reflect                                        
   template<class T> requires (CT::Dense<T> and not ::std::is_const_v<T>)
   auto DefinitionData::Reflect() -> DefinitionData const* {
      static_assert(CT::Complete<T>,
         "Can't reflect incomplete type - "
         "make sure you have included the corresponding headers "
         "before the point of reflection. "
         "This could also be triggered due to an incomplete member in T");
      static_assert(not CT::Array<T>,
         "Reflecting a bounded array is forbidden to avoid bloat");
      static_assert(not CT::Volatile<T>,
         "Can't reflect volatile type, use Devq before reflection");
      static_assert(not CT::Reference<T>,
         "Can't reflect reference type, use Deref before reflection");
      static_assert(not CT::DefineTag<T>,
         "Can't reflect tag as data");
      static_assert(not CT::DefineVerb<T>,
         "Can't reflect constant as data");
      static_assert(CT::Decayed<T>,
         "Unsupported qualifiers detected");
      static_assert(CT::Reflectable<T>,
         "Can't reflect data that was explicitly marked unreflectable");
      static_assert(Exact<CT::ReflectedAs<T>, T>,
         "Data is marked to be reflected as something else, "
         "make sure this is respected before reaching this function");
      static_assert(not ::std::is_function_v<T>,
         "Can't reflect this function signature - "
         "make sure you're using a pointer to it instead");

      constexpr auto cppname = CppNameOf<T>();

      #if LANGULUS_COMPILER(MSVC)
         static_assert(not cppname.starts_with("`anonymous-namespace'"),
            "Reflecting types inside anonymous namespaces is disallowed. "
            "You would expect that the C++ standard demands, "
            "that these namespaces have unique names generated, however that's not the case. "
            "Instead, they will all end up in the same `anonymous-namespace', and result in very "
            "subtle and infuriating bugs when types with the same name are reflected from multiple translation units."
         );
      #elif LANGULUS_COMPILER(CLANG)
         static_assert(not cppname.starts_with("(anonymous namespace)"),
            "Reflecting types inside anonymous namespaces is disallowed. "
            "You would expect that the C++ standard demands, "
            "that these namespaces have unique names generated, however that's not the case. "
            "Instead, they will all end up in the same (anonymous namespace), and result in very "
            "subtle and infuriating bugs when types with the same name are reflected from multiple translation units."
         );
      #endif

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get an already existing definition - the data might  
         // have been reflected previously in another shared library    
         DefinitionData const* meta = Registry::GetMetaDataByCppName(cppname);
         if (meta and meta->IsInRelevantBoundary())
            return meta;

         const auto token = NameOf<T, false>();
         DefinitionData& definition = meta
            ? const_cast<DefinitionData&>(*meta)
            : Registry::RegisterData(cppname, token);
      #else
         // There's no centralized registry when MANAGED_REFLECTION is  
         // disabled, so all we can do is keep a definition on the stack
         // for each translation unit, and rely on runtime checks to    
         // make sure that definitions match between those.             
         static constinit std::optional<DefinitionData> s_definition;
         if (s_definition.has_value())
            return &s_definition.value();

         DefinitionData& definition = s_definition.emplace(cppname);
         definition.mNameOf = Inner::NormalizeAtRuntime(NameOf<T, false>());
         LglsAssert(not definition.mNameOf.empty(),
            "Invalid data token is not allowed - "
            "you have equipped your type (or its base) with an empty CTTI_Named. "
            "The type in question is: ", cppname
         );
         definition.mNameOf[0] = ToUppercase(definition.mNameOf[0]);
      #endif
      
      //                                                                
      // If this is reached, then data is not defined yet from the      
      // viewpoint of the current boundary                              
      definition.ReflectCommon<T>();
      definition.mSize       = sizeof(T);
      definition.mAlign      = alignof(T);
      definition.mDeep       = CT::Deep<T>;
      definition.mPOD        = CT::POD<T> and not CT::Abstract<T>;
      definition.mNullable   = CT::Nullable<T> and not CT::Abstract<T>;
      definition.mAbstract   = CT::Abstract<T>;
      definition.mExecutable = CT::Executable<T>;
      definition.mSuffixOf   = SuffixOf<T>();
      definition.mFilesOf    = FilesOf<T>();

      // Reflect the concrete type                                      
      if constexpr (CT::Concretizable<T>) {
         using C = CT::ReflectedAs<ConcreteOf<T>>;
         static_assert(not CT::Abstract<C>,
            "Concrete type can't be abstract");
         definition.mCurrentBoundary.mConcrete = Reflect<C>;
      }

      // Reflect the producer type                                      
      if constexpr (CT::Producible<T>) {
         using P = CT::ReflectedAs<ProducerOf<T>>;
         definition.mCurrentBoundary.mProducer = Reflect<P>;
      }

      //                                                                
      // Constructor reflections                                        
      if constexpr (CT::Defaultable<T>) {
         // Generate a default constructor                              
         definition.mCurrentBoundary.mDefaultConstructor =
            [](void* at) noexcept(noexcept(T {})) {
               auto atT = static_cast<T*>(at);
               new (atT) T {};
            };
      }

      if constexpr (CT::CopyConstructible<T>) {
         // Generate a copy-constructor                                 
         definition.mCurrentBoundary.mCopyConstructor =
            [](void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               IntentNew(toT, Copy(*fromT));
            };
      }
         
      if constexpr (CT::ReferConstructible<T>) {
         // Generate a refer-constructor                                
         definition.mCurrentBoundary.mReferConstructor =
            [](void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               IntentNew(toT, Refer(*fromT));
            };
      }
         
      if constexpr (CT::CloneConstructible<T>) {
         // Generate a clone-constructor                                
         definition.mCurrentBoundary.mCloneConstructor =
            [](void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               IntentNew(toT, Clone(*fromT));
            };
      }

      if constexpr (CT::DisownConstructible<T>) {
         // Generate a disown-constructor                               
         definition.mCurrentBoundary.mDisownConstructor =
            [](void* from, void* to) {
               auto fromT = static_cast<const T*>(from);
               auto toT = static_cast<T*>(to);
               IntentNew(toT, Disown(*fromT));
            };
      }

      if constexpr (CT::MoveConstructible<T>) {
         // Generate a move-constructor                                 
         definition.mCurrentBoundary.mMoveConstructor =
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               IntentNew(toT, Move(*fromT));
            };
      }

      if constexpr (CT::AbandonConstructible<T>) {
         // Generate a abandon-constructor                              
         definition.mCurrentBoundary.mAbandonConstructor =
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               IntentNew(toT, Abandon(*fromT));
            };
      }
   
      if constexpr (CT::Destroyable<T>) {
         // Generate a destructor                                       
         definition.mCurrentBoundary.mDestructor =
            [](void* at) {
               auto atT = static_cast<T*>(at);
               atT->~T();
            };
      }

      //                                                                
      // Assignment reflections                                         
      // @note allowed only if T is mutable                             
      if constexpr (CT::CopyAssignable<T>) {
         // Generate a copy-assigner                                    
         definition.mCurrentBoundary.mCopyAssigner =
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               IntentAssign(*toT, Copy(*fromT));
            };
      }
   
      if constexpr (CT::ReferAssignable<T>) {
         // Generate a refer-assigner                                   
         definition.mCurrentBoundary.mReferAssigner =
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               IntentAssign(*toT, Refer(*fromT));
            };
      }

      if constexpr (CT::DisownAssignable<T>) {
         // Generate a disown-assigner                                  
         definition.mCurrentBoundary.mDisownAssigner =
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               IntentAssign(*toT, Disown(*fromT));
            };
      }
         
      if constexpr (CT::CloneAssignable<T>) {
         // Generate a clone-assigner                                   
         definition.mCurrentBoundary.mCloneAssigner =
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               IntentAssign(*toT, Clone(*fromT));
            };
      }

      if constexpr (CT::MoveAssignable<T>) {
         // Generate a move-assigner                                    
         definition.mCurrentBoundary.mMoveAssigner =
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               IntentAssign(*toT, Move(*fromT));
            };
      }

      if constexpr (CT::AbandonAssignable<T>) {
         // Generate an abandon-assigner                                
         definition.mCurrentBoundary.mAbandonAssigner =
            [](void* from, void* to) {
               auto fromT = static_cast<T*>(from);
               auto toT = static_cast<T*>(to);
               IntentAssign(*toT, Abandon(*fromT));
            };
      }

      //                                                                
      // Other utilities                                                
      if constexpr (CT::Hashable<T>) {
         // Generate a hashing function                                 
         definition.mHasGetHashMethod = CT::HasGetHashMethod<T>;
         definition.mCurrentBoundary.mHasher = [](void* at) {
            auto self = static_cast<T*>(at);
            return HashOf<true>(*self);
         };
      }

      if constexpr (CT::Referenced<T>) {
         // Generate a referencing function                             
         definition.mCurrentBoundary.mReferencer =
            [](void* at, int modifier) -> int {
               auto atT = static_cast<T*>(at);
               return atT->Reference(modifier);
            };
      }

      if constexpr (CT::Comparable<T, T>) {
         // Generate a three-way comparison function                    
         definition.mCurrentBoundary.mComparer =
            [](void const* t1, void const* t2) -> Compared {
               auto t1T = static_cast<T const*>(t1);
               auto t2T = static_cast<T const*>(t2);
               return FromOrdering(*t1T, *t2T);
            };

         // Generate an equality comparison function                    
         definition.mCurrentBoundary.mComparerEqual =
            [](void const* t1, void const* t2) -> bool {
               auto t1T = static_cast<T const*>(t1);
               auto t2T = static_cast<T const*>(t2);
               return *t1T == *t2T;
            };
      }

      if constexpr (CT::Resolvable<T>) {
         // Generate a resolving function                               
         definition.mCurrentBoundary.mResolver =
            [](void* at) {
               auto atT = static_cast<T*>(at);
               return atT->GetResolved();
            };
      }

      // Reflect the minimal allocation in bytes                        
      definition.mMinimalAllocation = MinAllocOf<T>();

      #if LANGULUS_FEATURE(MANAGED_MEMORY)
         // Reflect pooling properties                                  
         definition.mMinimalPoolSize = MinPoolOf<T>();
         definition.mPoolTactic      = PoolTacticOf<T>();

         // Make sure that types registered from an external shared     
         // library are _always_ pooled by type, so that we're able to  
         // unregister them and free their dedicated pools when the     
         // shared library is unloaded                                  
         /// @attention this has to be inlined, always                  
         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            if (LglsBoundary() != LglsMainBoundary())
               definition.mPoolTactic = PoolTactic::Type;
         #endif
      #endif
      
      // Calculate the allocation table                                 
      auto minElements = MinAllocOf<T>() / sizeof(T);
      definition.ReflectOrigin(minElements, sizeof(T));

      using BASES = BasesOf<T>;
      if constexpr (not CT::Void<BASES>) {
         // Set reflected bases                                         
         ForEach(BASES{}, [&definition]<class B>{
            definition.mCurrentBoundary.mBases.push_back(
               Base::From<T, B>()
            );
         });
      }

      using VERBS = VerbsOf<T>;
      if constexpr (not CT::Void<VERBS>) {
         // Set reflected abilities                                     
         ForEach(VERBS{}, [&definition]<class V>{
            static_assert(CT::DefineVerb<V>,
               "Verb list must contain only verbs");
            static_assert(CT::Decayed<V>,
               "Verbs must be fully decayed when listed");
            static_assert(V::template IsAble<T>,
               "T doesn't have the required verb method/specialization");

            auto verb_definition = const_cast<DefinitionVerb*>(
               DefinitionVerb::Reflect<V>());
            definition.mCurrentBoundary.mVerbs.emplace(
               verb_definition,
               [](void* self, Flow::Verb& verb) -> bool {
                  auto in = static_cast<T*>(self);
                  return V::template In<T>::Execute(*in, verb);
               }
            );
            verb_definition->mAble.insert(&definition);
         });
      }
   
      definition.FillMorphisms<T>();

      using CONSTANTS = NamedValuesOf<T>;
      if constexpr (not CT::Void<CONSTANTS>) {
         // Reflecting named values                                     
         CONSTANTS::ForEach([&definition]<auto C>{
            definition.mNamedValues.push_back(
               DefinitionConst::Reflect<C>()
            );
         });
      }

      using MEMBERS = MembersOf<T>;
      if constexpr (not CT::Void<MEMBERS>) {
         // Reflecting members                                          
         ForEach(MEMBERS{}, [&definition]<class M>{
            definition.mCurrentBoundary.mMembers.push_back(
               Member::From<M>()
            );
         });
      }
      
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         LglsVerbose(
            Logger::Cyan, "Data ", definition.mNameOf,
            " (ID: ", definition.mID, ") ", Logger::Green,
            "registered from ", (Boundary?Boundary:"MAIN")
         );
      #else
         LglsVerbose(
            Logger::Cyan, "Data ", definition.mNameOf,
            Logger::Green, " reflected"
         );
      #endif
      
      return &definition;
   }
   
   /// Reflect or return an already reflected data                            
   ///   @attention when making a shared library and reflecting your types    
   ///      at library initialization, it is recommended you mark all other   
   ///      relevant instantiations of this function as extern template, to   
   ///      save on a lot of compiler resources:                              
   ///      https://stackoverflow.com/questions/8130602                       
   ///   @tparam T the type to reflect                                        
   template<class T> requires (CT::Dense<T> and ::std::is_const_v<T>)
   auto DefinitionData::Reflect() -> DefinitionData const* {
      static_assert(CT::Complete<T>,
         "Can't reflect incomplete type - "
         "make sure you have included the corresponding headers "
         "before the point of reflection. "
         "This could also be triggered due to an incomplete member in T");
      static_assert(not CT::Array<T>,
         "Reflecting a bounded array is forbidden to avoid bloat");
      static_assert(not CT::Volatile<T>,
         "Can't reflect volatile type, use Devq before reflection");
      static_assert(not CT::Reference<T>,
         "Can't reflect reference type, use Deref before reflection");
      static_assert(not CT::DefineTag<T>,
         "Can't reflect tag as data");
      static_assert(not CT::DefineVerb<T>,
         "Can't reflect constant as data");
      static_assert(CT::Reflectable<T>,
         "Can't reflect data that was explicitly marked unreflectable");
      static_assert(Same<CT::ReflectedAs<T>, T>,
         "Data is marked to be reflected as something else, "
         "make sure this is respected before reaching this function");
      static_assert(not ::std::is_function_v<T>,
         "Can't reflect this function signature - "
         "make sure you're using a pointer to it instead");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get an already existing definition - the data might  
         // have been reflected previously in another shared library    
         const auto cppname {CppNameOf<Decvq<T>>() + " const"};
         DefinitionData const* meta = Registry::GetMetaDataByCppName(cppname);
         if (meta and meta->IsInRelevantBoundary())
            return meta;
      
         const auto token {NameOf<Decvq<T>, false>() + " const"};
         DefinitionData& definition = meta
            ? const_cast<DefinitionData&>(*meta)
            : Registry::RegisterData(cppname, token);
      #else
         // There's no centralized registry when MANAGED_REFLECTION is  
         // disabled, so all we can do is keep a definition on the stack
         // for each translation unit, and rely on runtime checks to    
         // make sure that definitions match between those.             
         static constinit std::optional<DefinitionData> s_definition;
         if (s_definition.has_value())
            return &s_definition.value();

         const auto cppname {CppNameOf<Decvq<T>>() + " const"};
         DefinitionData& definition = s_definition.emplace(cppname);
      
         definition.mNameOf = Inner::NormalizeAtRuntime(NameOf<Decvq<T>, false>());
         definition.mNameOf += " const";
         definition.mNameOf[0] = ToUppercase(definition.mNameOf[0]);
      #endif
      
      //                                                                
      // If this is reached, then data is not defined yet from the      
      // viewpoint of the current boundary                              
      definition.ReflectCommon<T>();
      definition.mSize       = sizeof(T);
      definition.mAlign      = alignof(T);
      definition.mDeep       = CT::Deep<T>;
      definition.mPOD        = CT::POD<T> and not CT::Abstract<T>;
      definition.mNullable   = CT::Nullable<T> and not CT::Abstract<T>;
      definition.mAbstract   = CT::Abstract<T>;
      definition.mExecutable = CT::Executable<T>;
      definition.mOrigin     = Reflect<CT::ReflectedAs<Decay<T>>>();
      definition.ReflectConstOrigin();

      #if LANGULUS_FEATURE(MANAGED_MEMORY)
         // Make sure that types registered from an external shared     
         // library are always pooled by type, so that we're able to    
         // unregister them and free their dedicated pools when the     
         // shared library is unloaded                                  
         /// @attention this has to be inlined, always                  
         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            if (LglsBoundary() != LglsMainBoundary())
               definition.mPoolTactic = PoolTactic::Type;
         #endif
      #endif

      using VERBS = VerbsOf<T>;
      if constexpr (not CT::Void<VERBS>) {
         // Set reflected abilities                                     
         // These can be different for constant/mutable types           
         ForEach(VERBS{}, [&definition]<class V>{
            static_assert(CT::DefineVerb<V>,
               "Verb list must contain only verbs");
            static_assert(CT::Decayed<V>,
               "Verbs must be fully decayed when listed");
            static_assert(V::template IsAble<T>,
               "T doesn't have the required verb method/specialization");

            auto verb_definition = const_cast<DefinitionVerb*>(
               DefinitionVerb::Reflect<V>());
            definition.mCurrentBoundary.mVerbs.emplace(
               verb_definition,
               [](void* self, Flow::Verb& verb) -> bool {
                  auto in = static_cast<T*>(self);
                  return V::template In<T>::Execute(*in, verb);
               }
            );
            verb_definition->mAble.insert(&definition);
         });
      }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         LglsVerbose(
            Logger::Cyan, "Data ", definition.mNameOf,
            " (ID: ", definition.mID, ") ", Logger::Green,
            "registered from ", (Boundary?Boundary:"MAIN")
         );
      #else
         LglsVerbose(
            Logger::Cyan, "Data ", definition.mNameOf,
            Logger::Green, " reflected"
         );
      #endif
      
      return &definition;
   }
   
   /// Reflect or return an already reflected data (standard pointer)         
   ///   @attention when making a shared library and reflecting your types    
   ///      at library initialization, it is recommended you mark all other   
   ///      relevant instantiations of this function as extern template, to   
   ///      save on a lot of compiler resources:                              
   ///      https://stackoverflow.com/questions/8130602                       
   ///   @tparam T the type to reflect                                        
   template<class T> requires ::std::is_pointer_v<T>
   auto DefinitionData::Reflect() -> DefinitionData const* {
      static_assert(not CT::Volatile<T>,
         "Can't reflect volatile type, use Devq before reflection");
      static_assert(not CT::Reference<T>,
         "Can't reflect reference type, use Deref before reflection");
      static_assert(CT::Reflectable<T>,
         "Can't reflect data that was explicitly marked unreflectable");
      static_assert(Exact<CT::ReflectedAs<T>, T>,
         "Data is marked to be reflected as something else, "
         "make sure this is respected before reaching this function");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get an already existing definition - the data might  
         // have been reflected previously in another shared library    
         ::std::string cppname;
         ::std::string token;
         DefinitionData const* meta;

         // Recostruct pointer name and token at runtime to avoid a     
         // lot of compilation time                                     
         cppname = CppNameOf<Decvq<Deptr<T>>>();
         if constexpr (CT::Constant<Deptr<T>>) cppname += " const";
         if constexpr (CT::Constant<T>) cppname += "* const";
         else cppname += "*";
         meta = Registry::GetMetaDataByCppName(cppname);
         if (meta and meta->IsInRelevantBoundary())
            return meta;

         token = NameOf<Decvq<Deptr<T>>, false>();
         if constexpr (CT::Constant<Deptr<T>>) token += " const";
         if constexpr (CT::Constant<T>) token += "* const";
         else token += "*";

         DefinitionData& definition = meta
            ? const_cast<DefinitionData&>(*meta)
            : Registry::RegisterData(cppname, token);
      #else
         // There's no centralized registry when MANAGED_REFLECTION is  
         // disabled, so all we can do is keep a definition on the stack
         // for each translation unit, and rely on runtime checks to    
         // make sure that definitions match between those.             
         static constinit std::optional<DefinitionData> s_definition;
         if (s_definition.has_value())
            return &s_definition.value();

         ::std::string cppname;
         cppname = CppNameOf<Decvq<Deptr<T>>>();
         if constexpr (CT::Constant<Deptr<T>>) cppname += " const";
         if constexpr (CT::Constant<T>) cppname += "* const";
         else cppname += "*";

         DefinitionData& definition = s_definition.emplace(cppname);
         definition.mNameOf = Inner::NormalizeAtRuntime(NameOf<Decvq<Deptr<T>>, false>());
         if constexpr (CT::Constant<Deptr<T>>)
            definition.mNameOf += " const";
         if constexpr (CT::Constant<T>)
            definition.mNameOf += "* const";
         else
            definition.mNameOf += "*";
         definition.mNameOf[0] = ToUppercase(definition.mNameOf[0]);
      #endif
      
      //                                                                
      // If this is reached, then data is not defined yet from the      
      // viewpoint of the current boundary/library                      
      definition.ReflectCommon<T>();
      definition.mSize      = sizeof(T);
      definition.mAlign     = alignof(T);
      definition.mConst     = CT::Constant<T>;

      if constexpr (CT::Complete<Decay<T>>) {
         definition.mDeep = CT::Deep<T>;
         definition.mExecutable = CT::Executable<T>;
      }
      else {
         definition.mDeep = false;
         definition.mExecutable = false;
      }

      // Reflect the origin type                                        
      if constexpr (CT::Complete<Decay<T>>)
         definition.mOrigin = Reflect<CT::ReflectedAs<Decay<T>>>();

      // Reflect the dequalified types and generate/propagate IDs       
      using DTOnce = Decvq<T>;
      using DTAll  = DecvqAll<T>;

      if constexpr (not ::std::same_as<T, DTAll>) {
         // T has qualifiers                                            
         definition.mDecvqOnce = Reflect<DTOnce>();
         definition.mDecvqAll  = Reflect<DTAll>();
         
         if constexpr (CT::Constant<T>) {
            auto decvq = const_cast<DefinitionData*>(definition.mDecvqOnce);
            decvq->mAddConst = &definition;
         }
      }
      else {
         // T has no qualifiers                                         
         definition.mDecvqOnce = &definition;
         definition.mDecvqAll  = &definition;
      }

      using DenserT = Deref<Deptr<T>>;
      if constexpr (CT::Complete<DenserT>) {
         // Reflect the denser type                                     
         definition.mDeptr = Reflect<CT::ReflectedAs<DenserT>>();
         auto deptr = const_cast<DefinitionData*>(definition.mDeptr);
         LglsAssumeDev(deptr->mConst == CT::Constant<DenserT>,
            "Deptr didn't preserve mutability, reflecting ", NameOf<DenserT>(),
            " (reflected as ", NameOf<CT::ReflectedAs<DenserT>>(),
            ") as ", deptr->mNameOf
         );

         if constexpr (Exact<DenserT*, DTOnce>) {
            IF_SAFE(if (deptr->mAddPtr and deptr->mAddPtr != definition.mDecvqOnce) {
               Logger::Error(
                  deptr->mNameOf, "'s mAddPtr (", deptr->mAddPtr->mNameOf,
                  ") was erroneously replaced with ", definition.mDecvqOnce->mNameOf,
                  " while reflecting ", definition.mNameOf
               );
            })
            LglsAssumeDev(
               not deptr->mAddPtr or deptr->mAddPtr == definition.mDecvqOnce,
               "mAddPtr was set with different value"
            );
            deptr->mAddPtr = definition.mDecvqOnce;
         }

         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            if (not CT::Sparse<DenserT> and deptr->mDedicatedID) {
               // We can define ID relatively to deptr to save up on    
               // IDs.                                                  
               //LglsAssumeDev(not deptr->mConst,
               //   "base ID must be for a mutable type");
               definition.mID = deptr->mID;
            }
            else {
               // Deptr already reuses an ID and we can't define this   
               // pointer relative to it. Time to generate a new ID.    
               //LglsAssumeDev(not mConst,
               //   "can't make unique ID for a constant type");
               if constexpr (CT::Mutable<T>) {
                  definition.mID = Registry::ReserveDataID(&definition);
                  definition.mDedicatedID = true;
               }
               else {
                  // The ID has already been generated by reflecting    
                  // mDecvq                                             
                  definition.mID = definition.mDecvqOnce->mID;
               }

               definition.mPtrIncludedInID = true;
            }
         #endif
      }
      else {
         // An incomplete sparse type always has mDeptr of 1.           
         // We need a unique ID to represent this pointer.              
         definition.mDeptr = reinterpret_cast<DefinitionData*>(intptr_t {1});

         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            if constexpr (CT::Mutable<T>) {
               definition.mID = Registry::ReserveDataID(&definition);
               definition.mDedicatedID = true;
            }
            else {
               // The ID has already been generated by reflecting          
               // mDecvq                                                   
               definition.mID = definition.mDecvqOnce->mID;
            }

            definition.mPtrIncludedInID = true;
         #endif
      }


      // Reflect the minimal allocation in bytes                        
      definition.mMinimalAllocation  = MinAllocOf<T>();

      #if LANGULUS_FEATURE(MANAGED_MEMORY)
         // Reflect pooling properties                                  
         definition.mPoolTactic      = PoolTacticOf<T>();
         definition.mMinimalPoolSize = MinPoolOf<T>();

         // Make sure that types registered from an external shared     
         // library are always pooled by type, so that we're able to    
         // unregister them and free their dedicated pools when the     
         // shared library is unloaded                                  
         /// @attention this has to be inlined, always                  
         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            if (LglsBoundary() != LglsMainBoundary())
               definition.mPoolTactic = PoolTactic::Type;
         #endif
      #endif
      
      // Calculate the allocation table                                 
      auto minElements = MinAllocOf<T>() / sizeof(T);
      definition.ReflectStandardSparse(CT::Mutable<T>, CT::Complete<Decay<T>>, CT::Complete<DenserT>, minElements);
      definition.FillMorphisms<DecvqAll<T>>();

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         LglsVerbose(
            Logger::Cyan, "Data ", definition.mNameOf,
            " (ID: ", definition.mID, ") ", Logger::Green,
            "registered from ", (Boundary?Boundary:"MAIN")
         );
      #else
         LglsVerbose(
            Logger::Cyan, "Data ", definition.mNameOf,
            Logger::Green, " reflected"
         );
      #endif
      
      return &definition;
   }
   /// Reflect or return an already reflected data (custom pointer)           
   ///   @attention when making a shared library and reflecting your types    
   ///      at library initialization, it is recommended you mark all other   
   ///      relevant instantiations of this function as extern template, to   
   ///      save on a lot of compiler resources:                              
   ///      https://stackoverflow.com/questions/8130602                       
   ///   @tparam T the type to reflect                                        
   template<class T> requires CT::CustomPointer<T>
   auto DefinitionData::Reflect() -> DefinitionData const* {
      static_assert(not CT::Array<T>,
         "Reflecting a bounded array is forbidden to avoid bloat");
      static_assert(not CT::Volatile<T>,
         "Can't reflect volatile type, use Devq before reflection");
      static_assert(not CT::Reference<T>,
         "Can't reflect reference type, use Deref before reflection");
      static_assert(CT::Reflectable<T>,
         "Can't reflect data that was explicitly marked unreflectable");
      static_assert(Exact<CT::ReflectedAs<T>, T>,
         "Data is marked to be reflected as something else, "
         "make sure this is respected before reaching this function");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         // Try to get an already existing definition - the data might  
         // have been reflected previously in another shared library    
         ::std::string cppname;
         ::std::string token;
         DefinitionData const* meta;
         cppname = CppNameOf<Decvq<T>>();
         if constexpr (CT::Constant<T>) cppname += " const";
         meta = Registry::GetMetaDataByCppName(cppname);
         if (meta and meta->IsInRelevantBoundary())
            return meta;

         token = NameOf<Decvq<T>, false>();
         if constexpr (CT::Constant<T>) token += " const";

         DefinitionData& definition = meta
            ? const_cast<DefinitionData&>(*meta)
            : Registry::RegisterData(cppname, token);
      #else
         // There's no centralized registry when MANAGED_REFLECTION is  
         // disabled, so all we can do is keep a definition on the stack
         // for each translation unit, and rely on runtime checks to    
         // make sure that definitions match between those.             
         static constinit std::optional<DefinitionData> s_definition;
         if (s_definition.has_value())
            return &s_definition.value();

         ::std::string cppname;
         cppname = CppNameOf<Decvq<T>>();

         DefinitionData& definition = s_definition.emplace(cppname);
         definition.mNameOf = Inner::NormalizeAtRuntime(NameOf<Decvq<T>, false>());
         definition.mNameOf[0] = ToUppercase(definition.mNameOf[0]);
      #endif
      
      //                                                                
      // If this is reached, then data is not defined yet from the      
      // viewpoint of the current boundary/library                      
      definition.ReflectCommon<T>();
      definition.mSize      = sizeof(T);
      definition.mAlign     = alignof(T);
      definition.mConst     = CT::Constant<T>;

      if constexpr (CT::Complete<Decay<T>>) {
         definition.mDeep = CT::Deep<T>;
         definition.mExecutable = CT::Executable<T>;
      }
      else {
         definition.mDeep = false;
         definition.mExecutable = false;
      }

      // Reflect the origin type                                        
      if constexpr (CT::Complete<Decay<T>>)
         definition.mOrigin = Reflect<CT::ReflectedAs<Decay<T>>>();

      // Reflect the dequalified types and generate/propagate IDs       
      using DTOnce = Decvq<T>;
      using DTAll  = DecvqAll<T>;
      definition.mPointerSpecification = DTAll::Specification;

      if constexpr (not ::std::same_as<T, DTAll>) {
         // T has qualifiers                                            
         definition.mDecvqOnce = Reflect<DTOnce>();
         definition.mDecvqAll  = Reflect<DTAll>();
         
         if constexpr (CT::Constant<T>) {
            auto decvq = const_cast<DefinitionData*>(definition.mDecvqOnce);
            decvq->mAddConst = &definition;
         }
      }
      else {
         // T has no qualifiers                                         
         definition.mDecvqOnce = &definition;
         definition.mDecvqAll  = &definition;
      }

      using LambdaT = DecvqAll<T>;
      using DenserT = Deref<Deptr<T>>;
      if constexpr (CT::Complete<DenserT>) {
         // Reflect the denser type                                     
         definition.mDeptr = Reflect<CT::ReflectedAs<DenserT>>();
         auto deptr = const_cast<DefinitionData*>(definition.mDeptr);
         LglsAssumeDev(deptr->mConst == CT::Constant<DenserT>,
            "Deptr didn't preserve mutability, reflecting ", NameOf<DenserT>(),
            " (reflected as ", NameOf<CT::ReflectedAs<DenserT>>(),
            ") as ", deptr->mNameOf
         );

         if constexpr (Exact<DenserT*, DTOnce>) {
            IF_SAFE(if (deptr->mAddPtr and deptr->mAddPtr != definition.mDecvqOnce) {
               Logger::Error(
                  deptr->mNameOf, "'s mAddPtr (", deptr->mAddPtr->mNameOf,
                  ") was erroneously replaced with ", definition.mDecvqOnce->mNameOf,
                  " while reflecting ", definition.mNameOf
               );
            })
            LglsAssumeDev(
               not deptr->mAddPtr or deptr->mAddPtr == definition.mDecvqOnce,
               "mAddPtr was set with different value"
            );
            deptr->mAddPtr = definition.mDecvqOnce;
         }

         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            // Deptr already reuses an ID and we can't define this      
            // pointer relative to it. Time to generate a new ID.       
            if constexpr (CT::Mutable<T>) {
               definition.mID = Registry::ReserveDataID(&definition);
               definition.mDedicatedID = true;
            }
            else {
               // The ID has already been generated by reflecting       
               // mDecvq                                                
               definition.mID = definition.mDecvqOnce->mID;
            }

            definition.mPtrIncludedInID = true;
         #endif

         definition.mCurrentBoundary.mDereference = Inner::CustomSparseDefaultDeref<LambdaT>;
      }
      else {
         // An incomplete sparse type always has mDeptr of 1.           
         // We need a unique ID to represent this pointer.              
         definition.mDeptr = reinterpret_cast<DefinitionData*>(intptr_t {1});

         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            if constexpr (CT::Mutable<T>) {
               definition.mID = Registry::ReserveDataID(&definition);
               definition.mDedicatedID = true;
            }
            else {
               // The ID has already been generated by reflecting       
               // mDecvq                                                
               definition.mID = definition.mDecvqOnce->mID;
            }

            definition.mPtrIncludedInID = true;
         #endif
      }

      using PointerIntEquivalent = typename Decvq<T>::Inner;

      //                                                                
      // Constructor reflections                                        
      // @note these are allowed even if T is constant                  
      definition.mCurrentBoundary.mDefaultConstructor
         = Inner::CustomSparseDefaultConstructor<PointerIntEquivalent>;
      definition.mCurrentBoundary.mCopyConstructor
         = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
      definition.mCurrentBoundary.mReferConstructor
         = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
      definition.mCurrentBoundary.mDisownConstructor
         = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
      definition.mCurrentBoundary.mMoveConstructor
         = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
      definition.mCurrentBoundary.mAbandonConstructor
         = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
     
      //                                                                
      // Assignment reflections                                         
      // @note allowed only if T is mutable                             
      if constexpr (CT::Mutable<T>) {
         definition.mCurrentBoundary.mCopyAssigner
            = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
         definition.mCurrentBoundary.mReferAssigner
            = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
         definition.mCurrentBoundary.mDisownAssigner
            = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
         definition.mCurrentBoundary.mMoveAssigner
            = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
         definition.mCurrentBoundary.mAbandonAssigner
            = Inner::CustomSparseCopyConstructor<PointerIntEquivalent>;
      }

      //                                                                
      // Other utilities                                                
      definition.mCurrentBoundary.mHasher
         = Inner::CustomSparseHash<PointerIntEquivalent>;   
      definition.mCurrentBoundary.mComparer
         = Inner::CustomSparseCompare<PointerIntEquivalent>;         
      definition.mCurrentBoundary.mComparerEqual
         = Inner::CustomSparseCompareEqual<PointerIntEquivalent>;         

      // Reflect the minimal allocation in bytes                        
      definition.mMinimalAllocation  = MinAllocOf<T>();

      #if LANGULUS_FEATURE(MANAGED_MEMORY)
         // Reflect pooling properties                                  
         definition.mPoolTactic      = PoolTacticOf<T>();
         definition.mMinimalPoolSize = MinPoolOf<T>();

         // Make sure that types registered from an external shared     
         // library are always pooled by type, so that we're able to    
         // unregister them and free their dedicated pools when the     
         // shared library is unloaded                                  
         /// @attention this has to be inlined, always                  
         #if LANGULUS_FEATURE(MANAGED_REFLECTION)
            if (LglsBoundary() != LglsMainBoundary())
               definition.mPoolTactic = PoolTactic::Type;
         #endif
      #endif
      
      // Calculate the allocation table                                 
      auto minElements = MinAllocOf<T>() / sizeof(T);
      definition.ReflectCustomSparse(CT::Mutable<T>, CT::Complete<Decay<T>>, minElements, sizeof(T));
      definition.FillMorphisms<DecvqAll<T>>();

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         LglsVerbose(
            Logger::Cyan, "Data ", definition.mNameOf,
            " (ID: ", definition.mID, ") ", Logger::Green,
            "registered from ", (Boundary?Boundary:"MAIN")
         );
      #else
         LglsVerbose(
            Logger::Cyan, "Data ", definition.mNameOf,
            Logger::Green, " reflected"
         );
      #endif
      
      return &definition;
   }
   
   inline DefinitionData::~DefinitionData() {
      LglsVerbose(Logger::Red, "Destroying data definition: ", Logger::Cyan, mNameOf);
   }
   
   /// Set reflected morphisms                                                
   /// @attention morphisms assume that source is initialized,                
   ///    but destination is only allocated and not yet constructed           
   /// @attention serialization assumes both sides are valid and constructed  
   ///    pointers. Context is optional.                                      
   template<class T>
   void DefinitionData::FillMorphisms() {
      ForEach(GatherMorphismsFrom<T>{}, [this]<class TO_RAW>{
         using TO = CT::ReflectedAs<TO_RAW>;

         auto destination_type = const_cast<DefinitionData*>(Reflect<TO>());
         Morphism morphism;

         if constexpr (CT::Inner::FindMorphism<T, TO>() >= 0) {
            morphism.convert = [](void* from, void* to) {
               auto fromT = static_cast<ConstAll<T*>>(from);
               auto toT   = static_cast<TO*>(to);
               new (toT) TO {Langulus::Convert<TO>(*fromT)};
            };
         }
         
         if constexpr (CT::Serializer<TO>) {
            // Destination type can act as a serializer, too         
            using S = CTTI::Serializer<TO>;
            morphism.serialize = [](void const* from, void* to, void* context) -> size_t {
               auto fromT = static_cast<ConstAll<T*>>(from);
               auto toT   = static_cast<TO*>(to);
               auto conT  = static_cast<typename S::Context*>(context);
               return Langulus::Serialize(*fromT, *toT, conT);
            };
         }

         mCurrentBoundary.mMorphismsTo.emplace(destination_type, morphism);
      });
   }

   /// Generate a member definition                                           
   ///   @return the generated member descriptor                              
   template<class HANDLE>
   auto DefinitionData::Member::From() -> Member {
      using THIS = typename HANDLE::Owner;
      static_assert(CT::NotConvoluted<THIS>, "Can't have qualifiers here");
      using DATA = typename HANDLE::Type;

      Member m;
      m.extent = ExtentOf<DATA>;
      m.member = [](void* owner) -> void* {
         auto context = static_cast<THIS*>(owner);
         return &(context->*HANDLE::Handle);
      };
      m.name = HANDLE::Name;
      m.name = m.name.substr(m.name.find_last_of(':') + 1);

      using AS = CT::ReflectedAs<Deext<DATA>>;
      using TAGS = TagsOf<DATA>;
      if constexpr (not CT::Void<TAGS>) {
         // Reflect the trait tag                                       
         m.type = Reflect<AS>;
         ForEach(TAGS{}, [&m]<class T>{
            static_assert(CT::DefineTag<T>, "T is not a tag definition");
            m.tags.insert(DefinitionTag::Reflect<T>());
         });
      }
      else m.type = Reflect<AS>;
      return m;
   }
   
   /// Create a base descriptor for the derived type T                        
   ///   @attention private bases will end up as imposed                      
   ///   @return the generated base descriptor                                
   template<CT::Dense T, CT::Dense B>
   auto DefinitionData::Base::From() assumptious -> Base {
      using BASE = CT::ReflectedAs<B>;
      static_assert(not CT::Void<BASE>,
         "Can't have void as base");
      static_assert(CT::NotConvoluted<T, BASE>,
         "Can't have qualifiers here");
      static_assert(not Akin<T, BASE>,
         "Can't have base of the same type as the derived");
      static_assert(NameOf<T, false>() != NameOf<BASE, false>(),
         "T and BASE have the same NameOf, possibly due to inheritance. "
         "Specify a different CTTI::Named<T> or T::CTTI_Named for each!");

      Base result;
      result.type = Reflect<BASE>();

      if constexpr (CT::DerivedFrom<T, BASE>) {
         if constexpr (CT::VirtuallyDerivedFrom<T, BASE>) {
            // Needs to use slower dynamic_cast when base is virtual    
            result.getBase = [](void* from) noexcept -> void* {
               return dynamic_cast<BASE*>(static_cast<T*>(from));
            };
         }
         else {
            result.getBase = [](void* from) noexcept -> void* {
               return static_cast<BASE*>(static_cast<T*>(from));
            };
            
            // If sizes match and there's no byte offset, then the      
            // base and the derived type are binary compatible          
            result.binaryCompatible = (sizeof(BASE) == sizeof(T));
         }
      }

      return result;
   }
}

#include <Langulus/Logger/DisableVerbose.hpp>
