///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/Testing/CommonTypes.hpp>
//#include <Langulus/Testing/PackedPointers.hpp>
#include <Langulus/MetaOf.hpp>


///                                                                           
/// MARK: Empty meta data                                                     
TEST_CASE("Testing empty meta data") {
   const DMeta meta = {};
   REQUIRE_FALSE(meta);
   REQUIRE(meta.GetHash() == Hash {});
   REQUIRE(meta.GetCppName() == "");
   REQUIRE(meta.GetName() == RTTI::DefinitionData::InvalidName);
   REQUIRE(meta.GetInfo() == "");
   REQUIRE(meta.GetFiles() == "");
   REQUIRE(meta.GetSuffix() == "");
   REQUIRE(meta.GetVersionMajor() == 0);
   REQUIRE(meta.GetVersionMinor() == 0);

   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      REQUIRE(meta.GetID() == 0);
      REQUIRE(meta.GetBoundaries().empty());
   #endif

   REQUIRE(meta.GetOrigin() == nullptr);
   REQUIRE(meta.GetDeptr() == nullptr);
   REQUIRE(meta.GetDecvqAll() == nullptr);
   REQUIRE(meta.GetDecvq() == nullptr);
   REQUIRE(meta.GetIndirections() == 0);
   bool customptr;
   REQUIRE(meta.GetIndirections(customptr) == 0);
   REQUIRE_FALSE(customptr);
   REQUIRE(not meta.GetPointerSpecification().IsPacked());
   REQUIRE(meta.AddPtr() == nullptr);
   REQUIRE(meta.AddConst() == nullptr);
   
   REQUIRE(meta.GetSize() == 0);
   REQUIRE(meta.GetAlignment() == Alignment);
   REQUIRE(meta.IsConstant() == false);
   REQUIRE(meta.IsMutable());
   REQUIRE(not meta.IsExecutable());
   REQUIRE(meta.IsDeep() == false);
   REQUIRE(meta.IsPOD() == false);
   REQUIRE(meta.IsNullable() == false);
   REQUIRE(meta.IsAbstract() == false);
   REQUIRE(meta.IsSparse() == false);
   REQUIRE(meta.IsDense());
   REQUIRE(meta.HasGetHashMethod() == false);

   REQUIRE(meta.Is(meta));
   REQUIRE(meta.IsSame(meta));
   REQUIRE(meta.IsExact(meta));

   #if LANGULUS_FEATURE(MANAGED_MEMORY)
      REQUIRE(meta.GetMinPoolsize() == MinimalPoolSize);
      REQUIRE(meta.GetPoolTactic() == PoolTactic::Default);
   #endif
   
   REQUIRE(meta.GetDefaultConstructor() == nullptr);
   REQUIRE(meta.GetDescribeConstructor()== nullptr);
   REQUIRE(meta.GetReferConstructor()   == nullptr);
   REQUIRE(meta.GetCopyConstructor()    == nullptr);
   REQUIRE(meta.GetDisownConstructor()  == nullptr);
   REQUIRE(meta.GetCloneConstructor()   == nullptr);
   REQUIRE(meta.GetMoveConstructor()    == nullptr);
   REQUIRE(meta.GetAbandonConstructor() == nullptr);
   
   REQUIRE(meta.GetDereffer()        == nullptr);
   REQUIRE(meta.GetDestructor()      == nullptr);
   REQUIRE(meta.GetComparer()        == nullptr);
   REQUIRE(meta.GetComparerEqual()   == nullptr);

   REQUIRE(meta.GetReferAssigner()   == nullptr);
   REQUIRE(meta.GetCopyAssigner()    == nullptr);
   REQUIRE(meta.GetDisownAssigner()  == nullptr);
   REQUIRE(meta.GetCloneAssigner()   == nullptr);
   REQUIRE(meta.GetMoveAssigner()    == nullptr);
   REQUIRE(meta.GetAbandonAssigner() == nullptr);

   REQUIRE(meta.GetResolver()   == nullptr);
   REQUIRE(meta.GetHasher()     == nullptr);
   REQUIRE(meta.GetReferencer() == nullptr);
   //REQUIRE(meta.GetDispatcher() == nullptr);
   REQUIRE(meta.GetConcrete()   == nullptr);
   REQUIRE(meta.GetProducer()   == nullptr);

   REQUIRE(meta.GetMembers().size() == 0);
   REQUIRE(meta.GetVerbs().size() == 0);
   REQUIRE(meta.GetBases().size() == 0);
   REQUIRE(meta.GetMorphismsTo().size() == 0);
   //REQUIRE(meta.GetMorphismsFrom().size() == 0);
   REQUIRE(meta.GetMorphism(meta).convert == nullptr);
   REQUIRE(meta.GetMorphism(meta).serialize == nullptr);
   REQUIRE(meta.GetNamedValues().size() == 0);
}

///                                                                           
/// MARK: Incomplete types                                                    
TEST_CASE_TEMPLATE("Testing reflection of incomplete types", T
   //, void           // shouldn't compile
   //, nullptr_t      // shouldn't compile
   //, IncompleteType // shouldn't compile
   //, NotReflectable // shouldn't compile
   , IncompleteType*
   , IncompleteType**
   , const IncompleteType**
   , const IncompleteType* const*&
   , const IncompleteType* const* const&
   , const IncompleteType* const* const&&
) {
   const DMeta meta = MetaDataOf<T>();
   REQUIRE(meta);
   REQUIRE(meta != nullptr);
   REQUIRE(meta.GetHash() != Hash {});
   REQUIRE(meta.GetCppName() == CppNameOf<Deref<T>>());
   REQUIRE(meta.GetName() == NameOf<Deref<T>>());
   REQUIRE(meta.GetInfo() == InfoOf<Deref<T>>());
   REQUIRE(meta.GetFiles() == FilesOf<Deref<T>>());
   REQUIRE(meta.GetSuffix() == SuffixOf<Deref<T>>());
   REQUIRE(meta.GetVersionMajor() == VersionOf<Deref<T>>().Major);
   REQUIRE(meta.GetVersionMinor() == VersionOf<Deref<T>>().Minor);

   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      REQUIRE(meta.GetID() != 0);
      REQUIRE(meta.GetBoundaries().size() == 1);
   #endif

   if constexpr (CT::Complete<Decay<T>>)
      REQUIRE(meta.GetOrigin() == MetaDataOf<Decay<T>>());
   else
      REQUIRE(meta.GetOrigin() == nullptr);
   
   if constexpr (CT::Complete<Deptr<Deref<T>>>)
      REQUIRE(meta.GetDeptr() == MetaDataOf<Deptr<Deref<T>>>());
   else
      REQUIRE(meta.GetDeptr() == nullptr);
   
   REQUIRE(meta.GetDecvqAll() == MetaDataOf<DecvqAll<Deref<T>>>());
   REQUIRE(meta.GetDecvq() == MetaDataOf<Decvq<Deref<T>>>());
   REQUIRE(meta.GetIndirections() == IndirectsOf<T>);
   bool customptr;
   REQUIRE(meta.GetIndirections(customptr) == IndirectsOf<T>);
   REQUIRE_FALSE(customptr);
   REQUIRE(not meta.GetPointerSpecification().IsPacked());
   REQUIRE_THROWS(meta.AddPtr() == nullptr);
   REQUIRE_THROWS(meta.AddConst() == nullptr);
   
   REQUIRE(meta.GetSize() == sizeof(Deref<T>));
   REQUIRE(meta.GetAlignment() == alignof(Deref<T>));
   REQUIRE(meta.IsConstant() == CT::Constant<T>);
   REQUIRE(meta.IsMutable() == CT::Mutable<T>);
   REQUIRE(not meta.IsExecutable());
   if constexpr (CT::Complete<Decay<T>>)
      REQUIRE(meta.IsDeep() == CT::Deep<T>);
   else
      REQUIRE(meta.IsDeep() == false);
   REQUIRE(meta.IsPOD() == CT::POD<Deref<T>>);
   REQUIRE(meta.IsNullable() == CT::Nullable<Deref<T>>);
   REQUIRE(meta.IsAbstract() == CT::Abstract<Deref<T>>);
   REQUIRE(meta.HasGetHashMethod() == false);

   if constexpr (not Same<T, Decay<T>*>)
      REQUIRE_FALSE(meta.Is(MetaDataOf<Decay<T>*>()));
   REQUIRE(meta.Is(meta));
   REQUIRE(meta.IsSame(MetaDataOf<DecvqAll<T>>()));
   REQUIRE(meta.IsSame(meta));
   REQUIRE(meta.IsExact(meta));

   #if LANGULUS_FEATURE(MANAGED_MEMORY)
      REQUIRE(meta.GetMinPoolsize() == MinPoolOf<Deref<T>>());
      REQUIRE(meta.GetPoolTactic() == PoolTacticOf<Deref<T>>());
   #endif
   
   REQUIRE(meta.GetDefaultConstructor() != nullptr);
   REQUIRE(meta.GetDescribeConstructor()== nullptr);
   REQUIRE(meta.GetReferConstructor()   != nullptr);
   REQUIRE(meta.GetCopyConstructor()    != nullptr);
   REQUIRE(meta.GetDisownConstructor()  != nullptr);
   REQUIRE(meta.GetCloneConstructor()   == nullptr);
   REQUIRE(meta.GetMoveConstructor()    != nullptr);
   REQUIRE(meta.GetAbandonConstructor() != nullptr);
   
   if constexpr (IndirectsOf<T> == 1)
      REQUIRE(meta.GetDereffer()   == nullptr);
   else
      REQUIRE(meta.GetDereffer()   != nullptr);

   REQUIRE(meta.GetDestructor()    == nullptr);
   REQUIRE(meta.GetComparer()      != nullptr);
   REQUIRE(meta.GetComparerEqual() != nullptr);

   if constexpr (CT::Constant<T>) {
      REQUIRE(meta.GetReferAssigner()   == nullptr);
      REQUIRE(meta.GetCopyAssigner()    == nullptr);
      REQUIRE(meta.GetDisownAssigner()  == nullptr);
      REQUIRE(meta.GetCloneAssigner()   == nullptr);
      REQUIRE(meta.GetMoveAssigner()    == nullptr);
      REQUIRE(meta.GetAbandonAssigner() == nullptr);
   }
   else {
      REQUIRE(meta.GetReferAssigner()   != nullptr);
      REQUIRE(meta.GetCopyAssigner()    != nullptr);
      REQUIRE(meta.GetDisownAssigner()  != nullptr);
      REQUIRE(meta.GetCloneAssigner()   == nullptr);
      REQUIRE(meta.GetMoveAssigner()    != nullptr);
      REQUIRE(meta.GetAbandonAssigner() != nullptr);
   }

   REQUIRE(meta.GetResolver()   == nullptr);
   REQUIRE(meta.GetHasher()     != nullptr);
   REQUIRE(meta.GetReferencer() == nullptr);
   //REQUIRE(meta.GetDispatcher() == nullptr);
   REQUIRE(meta.GetConcrete()   == nullptr);
   REQUIRE(meta.GetProducer()   == nullptr);

   REQUIRE(meta.GetMembers().size() == 0);
   REQUIRE(meta.GetVerbs().size() == 0);
   REQUIRE(meta.GetBases().size() == 0);
   REQUIRE(meta.GetMorphismsTo().size() == 0); //== 2 Pointers are always convertible to Text and Bytes //TODO huh, really? well, not anymore!
   REQUIRE(meta.GetMorphism(meta).convert == nullptr);
   REQUIRE(meta.GetMorphism(meta).serialize == nullptr);
   REQUIRE(meta.GetNamedValues().size() == 0);
}

namespace {
   struct InUnnamedNamespace {
   };
}

///                                                                           
/// MARK: Names                                                               
SCENARIO("Testing reflection of names") {
   /*#if LANGULUS_FEATURE(MANAGED_MEMORY)
      {
         const DMeta meta = MetaDataOf<pptr8>();
         REQUIRE(meta);
         REQUIRE(meta.GetCppName() == "Langulus::Fractalloc::PackedPointer<char, 2, 6, 0>");
         REQUIRE(meta.GetName() == "Langulus::Fractalloc::PackedPointer<char, 2, 6, 0>");
      }
      {
         const DMeta meta = MetaDataOf<pptr16>();
         REQUIRE(meta);
         REQUIRE(meta.GetCppName() == "Langulus::Fractalloc::PackedPointer<char, 4, 4, 8>");
         REQUIRE(meta.GetName() == "Langulus::Fractalloc::PackedPointer<char, 4, 4, 8>");
      }
      {
         const DMeta meta = MetaDataOf<pptr32>();
         REQUIRE(meta);
         REQUIRE(meta.GetCppName() == "Langulus::Fractalloc::PackedPointer<char>");
         REQUIRE(meta.GetName() == "Langulus::Fractalloc::PackedPointer<char>");
      }
   #endif*/ //TODO test these in fractalloc instead

   {
      const DMeta meta = MetaDataOf<int>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "int");
      REQUIRE(meta.GetName() == "Int32");
   }
   {
      const DMeta meta = MetaDataOf<const int>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "int const");
      REQUIRE(meta.GetName() == "Int32 const");
   }
   {
      const DMeta meta = MetaDataOf<const int*>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "int const*");
      REQUIRE(meta.GetName() == "Int32 const*");
   }
   {
      const DMeta meta = MetaDataOf<int const>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "int const");
      REQUIRE(meta.GetName() == "Int32 const");
   }
   {
      const DMeta meta = MetaDataOf<int const*>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "int const*");
      REQUIRE(meta.GetName() == "Int32 const*");
   }
   {
      const DMeta meta = MetaDataOf<IncompleteType*>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "IncompleteType*");
      REQUIRE(meta.GetName() == "IncompleteType*");
   }
   {
      const DMeta meta = MetaDataOf<const IncompleteType**>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "IncompleteType const**");
      REQUIRE(meta.GetName() == "IncompleteType const**");
   }
   {
      const DMeta meta = MetaDataOf<ImplicitlyReflectedDataWithTraits>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "ImplicitlyReflectedDataWithTraits");
      REQUIRE(meta.GetName() == "MyType");
   }
   {
      const DMeta meta = MetaDataOf<ImplicitlyReflectedDataWithTraits*>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "ImplicitlyReflectedDataWithTraits*");
      REQUIRE(meta.GetName() == "MyType*");
   }
   {
      const DMeta meta = MetaDataOf<ImplicitlyReflectedDataWithTraits const*>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "ImplicitlyReflectedDataWithTraits const*");
      REQUIRE(meta.GetName() == "MyType const*");
   }
   {
      const DMeta meta = MetaDataOf<ImplicitlyReflectedDataWithTraits* const*>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "ImplicitlyReflectedDataWithTraits* const*");
      REQUIRE(meta.GetName() == "MyType* const*");
   }
   {
      const VMeta meta = MetaVerbOf<Verbs::Create>();
      REQUIRE(meta);
      REQUIRE(meta == MetaVerbOf<Verbs::Create*>());
      REQUIRE(meta == MetaVerbOf<Verbs::Create const>());
      REQUIRE(meta == MetaVerbOf<Verbs::Create const&>());
      REQUIRE(meta.GetCppName() == "Langulus::Verbs::Create");
      REQUIRE(meta.GetPositiveName() == "create");
      REQUIRE(meta.GetNegativeName() == "destroy");
      REQUIRE(meta.GetPositiveOperator() == " + ");
      REQUIRE(meta.GetNegativeOperator() == " - ");
   }
   {
      const TMeta meta = MetaTagOf<Tags::Name>();
      REQUIRE(meta);
      REQUIRE(meta == MetaTagOf<Tags::Name*>());
      REQUIRE(meta == MetaTagOf<Tags::Name const>());
      REQUIRE(meta == MetaTagOf<Tags::Name const&>());
      REQUIRE(meta.GetCppName() == "Langulus::Tags::Name");
      REQUIRE(meta.GetName() == "name");
   }
   {
      const CMeta meta = MetaConstOf<Pi::Number>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "Pi::Number");
      REQUIRE(meta.GetName() == "Pi::Number");
   }

   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      REQUIRE_THROWS(MetaDataOf<ConflictingName>());
      REQUIRE_THROWS(MetaDataOf<ConflictingName*>());
      REQUIRE_THROWS(MetaDataOf<ConflictingName const*>());

      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByNegOp1>());
      REQUIRE_NOTHROW(MetaVerbOf<Verbs::ConflictingByNegOp2>()); // allowed because tokens differ in capitalization, and int hasn't been associated with a verb yet
      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByNegOp3>());

      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByPosOp1>());
      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByPosOp2>());
      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByPosOp3>());

      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByNegToken1>());
      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByNegToken2>());
      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByNegToken3>());

      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByPosToken1>());
      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByPosToken2>());
      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByPosToken3>());

      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByPosToken1*>());
      REQUIRE_THROWS(MetaVerbOf<Verbs::ConflictingByPosToken1* const>());

      REQUIRE_THROWS(MetaTagOf<Tags::ConflictingName1>());
      REQUIRE_THROWS(MetaTagOf<Tags::ConflictingName2>());
      REQUIRE_THROWS(MetaTagOf<Tags::ConflictingName3>());
      REQUIRE_NOTHROW(MetaTagOf<Tags::ConflictingName4>()); // allowed because tokens differ in capitalization, and Pi::Number hasn't been associated with a tag yet
      REQUIRE_THROWS(MetaTagOf<Tags::ConflictingName3*>());
      REQUIRE_THROWS(MetaTagOf<Tags::ConflictingName3* const>());

      REQUIRE_THROWS(MetaConstOf<Pi::ConflictingNumber>());
   #endif
   
   //REQUIRE_THROWS(MetaDataOf<InvalidName1>());  // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InvalidName2>());  // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InvalidName3>());  // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InvalidName4>());  // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InvalidName5>());  // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InvalidName6>());  // shouldn't compile

   //REQUIRE_THROWS(MetaDataOf<ReservedName1>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<ReservedName2>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<ReservedName3>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<ReservedName4>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<ReservedName5>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<ReservedName6>()); // shouldn't compile
   
   //REQUIRE_THROWS(MetaDataOf<InUnnamedNamespace>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InUnnamedNamespace*>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InUnnamedNamespace**>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InUnnamedNamespace const>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InUnnamedNamespace const*>()); // shouldn't compile
   //REQUIRE_THROWS(MetaDataOf<InUnnamedNamespace const* const* const>()); // shouldn't compile
}

///                                                                           
/// MARK: Abstracts                                                           
TEST_CASE_TEMPLATE("Reflecting abstract types", T
   , PureAbstract
   , ForcedAbstractExternally
   , ForcedAbstractInternally
   , InheritedAbstract1
   , InheritedAbstract2
   , InheritedAbstract2ButPrivate
) {
   const DMeta meta = MetaDataOf<T>();
   REQUIRE(meta != nullptr);
   REQUIRE(meta.IsDeep() == false);
   REQUIRE(meta.IsPOD() == false);        // Abstract types can't be POD      
   REQUIRE(meta.IsNullable()  == false);  // Abstract types can't be nullable 
   REQUIRE(meta.GetConcrete() == nullptr);
   REQUIRE(meta.IsAbstract() == true);
   REQUIRE(meta.GetSize() == sizeof(T));
   REQUIRE(meta.GetAlignment() == alignof(T));
   REQUIRE(meta.GetDecvqAll() == MetaDataOf<T>());
   REQUIRE(meta.GetDecvq() == MetaDataOf<T>());
}

TEST_CASE_TEMPLATE("Reflecting non-abstract types", T
   , int
   , ImpureVirtual
   , InheritedAbstract1ButPrivate
   , InheritedAbstractExternally
) {
   const DMeta meta = MetaDataOf<T>();
   REQUIRE(meta != nullptr);
   REQUIRE(meta.IsAbstract() == false);
   REQUIRE(meta.IsPOD() == CT::POD<T>);
   REQUIRE(meta.IsNullable() == CT::Nullable<T>);
   REQUIRE(meta.GetConcrete() == nullptr);
   REQUIRE(meta.GetDecvqAll() == MetaDataOf<T>());
   REQUIRE(meta.GetDecvq() == MetaDataOf<T>());
}

///                                                                           
/// MARK: Virtual bases                                                       
TEST_CASE_TEMPLATE("Reflecting virtual bases", T,
   VirtuallyDerived
) {
   const DMeta meta = MetaDataOf<T>();
   T instance {};
   auto instance_base = dynamic_cast<ImpureVirtual*>(&instance);

   REQUIRE(meta.GetBases().size() == 2);
   
   REQUIRE(DMeta(meta.GetBases()[0].type).Is(MetaDataOf<ImpureVirtual>()));
   REQUIRE(meta.GetBases()[0].binaryCompatible == false);
   REQUIRE(meta.GetBases()[0].getBase(&instance) == instance_base);
   
   REQUIRE(DMeta(meta.GetBases()[1].type).Is(MetaDataOf<int>()));
   REQUIRE(meta.GetBases()[1].binaryCompatible == false);
   REQUIRE(meta.GetBases()[1].getBase == nullptr);

   REQUIRE(meta.GetDecvqAll() == MetaDataOf<T>());
   REQUIRE(meta.GetDecvq() == MetaDataOf<T>());
}

TEST_CASE_TEMPLATE("Reflecting non-virtual bases", T,
   PrivatelyDerived
) {
   const DMeta meta = MetaDataOf<T>();

   REQUIRE(meta.GetBases().size() == 3);
   
   REQUIRE(DMeta(meta.GetBases()[0].type).Is(MetaDataOf<ImpureVirtual>()));
   REQUIRE(meta.GetBases()[0].binaryCompatible == false);
   REQUIRE(meta.GetBases()[0].getBase == nullptr);
   
   REQUIRE(DMeta(meta.GetBases()[1].type).Is(MetaDataOf<int>()));
   REQUIRE(meta.GetBases()[1].binaryCompatible == false);
   REQUIRE(meta.GetBases()[1].getBase == nullptr);
   
   REQUIRE(DMeta(meta.GetBases()[2].type).Is(MetaDataOf<float>()));
   REQUIRE(meta.GetBases()[2].binaryCompatible == false);
   REQUIRE(meta.GetBases()[2].getBase == nullptr);

   REQUIRE(meta.GetDecvqAll() == MetaDataOf<T>());
   REQUIRE(meta.GetDecvq() == MetaDataOf<T>());
}

///                                                                           
/// MARK: Complex types                                                       
SCENARIO("A type reflected with all traits") {
   ImplicitlyReflectedDataWithTraits instance;
   auto ptrtobase = static_cast<ImplicitlyReflectedData*>(&instance);
   const DMeta meta = MetaDataOf<ImplicitlyReflectedDataWithTraits>();

   REQUIRE(meta != nullptr);
   REQUIRE(meta.GetName() == "MyType");
   REQUIRE(meta.GetInfo() == "Info about MyType");
   REQUIRE(meta.GetFiles() == "txt, pdf");
   REQUIRE(meta.GetVersionMajor() == 2);
   REQUIRE(meta.GetVersionMinor() == 1);
   REQUIRE(meta.IsDeep() == false);
   REQUIRE(meta.IsPOD() == false);       // not POD due to being abstract     
   REQUIRE(meta.IsNullable() == false);  // not nullable due to being abstract
   REQUIRE(meta.IsAbstract() == true);

   REQUIRE(meta.GetMinAllocation() == 1024_pot);
   for (size_t bit = 0; bit < Bitness; ++bit) {
      REQUIRE(meta.GetAllocationTable()[bit] == (bit < 10 ? 8 : ((size_t {1} << bit) / size_t {128})));
   }

   REQUIRE(meta.GetConcrete().Is(MetaDataOf<ImplicitlyReflectedData>()));
   IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta.GetPoolTactic() == PoolTactic::Type)); // Not by size despite reflected pool tactic, because alignment requirements aren't met
   IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta.GetMinPoolsize() == MinimalPoolSize));
   REQUIRE(meta.GetSize() == sizeof(ImplicitlyReflectedDataWithTraits));
   REQUIRE(meta.GetAlignment() == alignof(ImplicitlyReflectedDataWithTraits));
   REQUIRE(meta.GetOrigin() == meta);
   REQUIRE(meta.IsConstant() == false);
   REQUIRE(meta.GetDeptr() == nullptr);
   REQUIRE(meta.GetDecvqAll() == MetaDataOf<ImplicitlyReflectedDataWithTraits>());
   REQUIRE(meta.GetDecvq() == MetaDataOf<ImplicitlyReflectedDataWithTraits>());

   REQUIRE(meta.GetBases().size() == 1);
   REQUIRE(DMeta(meta.GetBases()[0].type).Is(MetaDataOf<ImplicitlyReflectedData>()));
   REQUIRE(meta.GetBases()[0].binaryCompatible == false);
   REQUIRE(meta.GetBases()[0].getBase(&instance) == ptrtobase);

   REQUIRE(meta.GetVerbs().size() == 1);
   auto ability = meta.GetVerbs().begin();
   REQUIRE(VMeta(ability->first) == MetaVerbOf<Verbs::Create>());
   REQUIRE(ability->second.binary != nullptr);
   REQUIRE(ability->second.unary == nullptr);

   auto members = meta.GetMembers();
   REQUIRE(members.size() == 4);
   
   REQUIRE(members[0].extent == 1);
   REQUIRE(members[0].member(&instance) == &instance.member);
   REQUIRE(members[0].tags.empty());
   REQUIRE(members[0].name == "member");
   REQUIRE(DMeta(members[0].type()).Is(MetaDataOf<int>()));

   REQUIRE(members[1].extent == 1);
   REQUIRE(members[1].member(&instance) == &instance.anotherMember);
   REQUIRE(members[1].tags.size() == 1);
   REQUIRE(members[1].tags.contains(RTTI::DefinitionTag::Reflect<Tags::Name>()));
   REQUIRE(members[1].name == "anotherMember");
   REQUIRE(DMeta(members[1].type()).Is(MetaDataOf<bool>()));

   REQUIRE(members[2].extent == 12);
   REQUIRE(members[2].member(&instance) == instance.anotherMemberArray);
   REQUIRE(members[2].tags.empty());
   REQUIRE(members[2].name == "anotherMemberArray");
   REQUIRE(DMeta(members[2].type()).Is(MetaDataOf<int>()));

   REQUIRE(members[3].extent == 1);
   REQUIRE(members[3].member(&instance) == &instance.sparseMember);
   REQUIRE(members[3].tags.empty());
   REQUIRE(members[3].name == "sparseMember");
   REQUIRE(DMeta(members[3].type()).Is(MetaDataOf<int>()));

   //TODO test GetMethods

   REQUIRE(meta.GetNamedValues().size() == 0);
   REQUIRE(meta.GetMorphismsTo().size() == 1);
   //REQUIRE(meta.GetMorphismsFrom().size() == 1);

   REQUIRE(meta.Is(MetaDataOf<ImplicitlyReflectedDataWithTraits***>()));
   REQUIRE(meta.Is(meta));
   REQUIRE(meta.IsSame(MetaDataOf<ImplicitlyReflectedDataWithTraits const>()));
   REQUIRE(meta.IsSame(meta));
   REQUIRE(meta.IsExact(meta));

   const auto int_definition = RTTI::DefinitionData::Reflect<int>();
   const auto pi_definition  = RTTI::DefinitionData::Reflect<Pi>();
   const auto imp_definition = RTTI::DefinitionData::Reflect<ImplicitlyReflectedDataWithTraits>();
   const auto cvt_definition = RTTI::DefinitionData::Reflect<ConvertibleData>();

   REQUIRE(meta == imp_definition);
   REQUIRE(meta.GetMorphismsTo().at(int_definition).convert != nullptr);
   REQUIRE(DMeta(int_definition).GetMorphismsTo().size() == 3);   // int -> Text; int -> Bytes; int -> ConvertibleFromIntExternally
   REQUIRE(DMeta(pi_definition).GetMorphismsTo().size() == 3);    // Pi -> Bytes; Pi -> ImplicitlyReflectedDataWithTraits; Pi -> ConvertibleData
   REQUIRE(DMeta(pi_definition).GetMorphismsTo().at(imp_definition).convert != nullptr);
   REQUIRE(DMeta(pi_definition).GetMorphismsTo().at(cvt_definition).convert != nullptr);
   REQUIRE(DMeta(cvt_definition).GetMorphismsTo().size() == 2);   // ConvertibleData -> Bytes; ConvertibleData -> int
   REQUIRE(DMeta(cvt_definition).GetMorphismsTo().at(int_definition).convert != nullptr);

   int converted = 1;
   meta.GetMorphismsTo().at(int_definition).convert(&instance, &converted);
   REQUIRE(converted == 664);
   /*{
      Pi source;
      ImplicitlyReflectedDataWithTraits convertedFromPi1;
      //meta.GetMorphismsFrom().at(pi_definition).convert(&source, &convertedFromPi1);
      REQUIRE(convertedFromPi1.member == 314);
   }*/
   {
      Pi source;
      ImplicitlyReflectedDataWithTraits convertedFromPi1;
      DMeta(pi_definition).GetMorphismsTo().at(imp_definition).convert(&source, &convertedFromPi1);
      REQUIRE(convertedFromPi1.member == 314);
   }
   {
      Pi source;
      ImplicitlyReflectedDataWithTraits convertedFromPi1;
      DMeta(pi_definition).GetMorphism(imp_definition).convert(&source, &convertedFromPi1);
      REQUIRE(convertedFromPi1.member == 314);
   }
   {
      std::string source = "the devil";
      int convertedFromString = 0;
      auto stdmeta = MetaDataOf<std::string>();
      REQUIRE(stdmeta.GetMorphismsTo().size() == 1);
      //REQUIRE(stdmeta.GetMorphismsFrom().size() == 0);
      REQUIRE(stdmeta.GetMorphism(int_definition).convert);
      stdmeta.GetMorphism(int_definition).convert(&source, &convertedFromString);
      REQUIRE(convertedFromString == 666);
   }
}

//TODO reflecting custom pointers

namespace Langulus::Annies
{
   template<class> 
   struct TVerb {};
}

///                                                                           
/// MARK: Verbs                                                               
SCENARIO("Reflecting a verb") {
   const DMeta dmeta = MetaDataOf<int>();
   const DMeta dmeta_c = MetaDataOf<int const>();

   {
      const VMeta vmeta = {};
      REQUIRE_FALSE(vmeta);
      REQUIRE(vmeta.GetPositiveName() == RTTI::DefinitionVerb::InvalidName);
      REQUIRE(vmeta.GetNegativeName() == RTTI::DefinitionVerb::InvalidName);
      REQUIRE(vmeta.GetInfo() == "");
      REQUIRE(vmeta.GetVersionMajor() == 0);
      REQUIRE(vmeta.GetVersionMinor() == 0);
      REQUIRE(vmeta.GetPositiveOperator() == "");
      REQUIRE(vmeta.GetNegativeOperator() == "");
      REQUIRE(vmeta.GetPrecedence() == 0);
   }
   {
      const auto vdef = RTTI::DefinitionVerb::Reflect<Verbs::Create>();
      const VMeta vmeta = MetaVerbOf<Verbs::Create>();
      REQUIRE(vmeta != nullptr);
      REQUIRE(vmeta.GetPositiveName() == "create");
      REQUIRE(vmeta.GetNegativeName() == "destroy");
      REQUIRE(vmeta.GetInfo().starts_with("Used for allocating new elements."));
      REQUIRE(vmeta.GetVersionMajor() == 6);
      REQUIRE(vmeta.GetVersionMinor() == 10);
      REQUIRE(vmeta.GetPositiveOperator() == " + ");
      REQUIRE(vmeta.GetNegativeOperator() == " - ");
      REQUIRE(vmeta.GetPrecedence() == 5);

      auto able = vmeta.GetAble();
      REQUIRE(able.size() == 3); // const/mutable int and const ImplicitlyReflectedDataWithTraits
      REQUIRE(able.contains(dmeta.GetDefinition()));
      REQUIRE(able.contains(dmeta_c.GetDefinition()));

      Annies::Verb verb;
      int context = 55;

      REQUIRE(dmeta.GetVerbs().at(vdef).binary(&context, verb));
      REQUIRE(dmeta.GetVerbs().at(vdef).unary(verb));

      const DMeta dmeta_const = MetaDataOf<const DMeta>();
      REQUIRE(dmeta_const.GetVerbs().at(vdef).binary(&context, verb));
      REQUIRE(dmeta_const.GetVerbs().at(vdef).unary(verb));
   }
}

///                                                                           
/// MARK: Tags                                                                
SCENARIO("Reflecting a tag") {
   {
      const TMeta meta = {};

      REQUIRE_FALSE(meta);
      REQUIRE(meta.GetName() == RTTI::DefinitionTag::InvalidName);
      REQUIRE(meta.GetInfo() == "");
      REQUIRE(meta.GetVersionMajor() == 0);
      REQUIRE(meta.GetVersionMinor() == 0);
   }
   {
      const TMeta meta = MetaTagOf<Tags::Name>();

      REQUIRE(meta != nullptr);
      REQUIRE(meta.GetName() == "name");
      REQUIRE(meta.GetInfo() == "Used for tagging names");
      REQUIRE(meta.GetVersionMajor() == 7);
      REQUIRE(meta.GetVersionMinor() == 10);
   }
}

///                                                                           
/// MARK: Values                                                              
SCENARIO("Reflecting a value") {
   {
      const CMeta meta = {};
      REQUIRE_FALSE(meta);
      REQUIRE(meta.GetCppName() == "");
      REQUIRE(meta.GetName() == RTTI::DefinitionConst::InvalidName);
      REQUIRE(meta.GetVersionMajor() == 0);
      REQUIRE(meta.GetVersionMinor() == 0);
   }
   {
      const CMeta meta = MetaConstOf<Pi::Number>();
      REQUIRE(meta);
      REQUIRE(meta.GetCppName() == "Pi::Number");
      REQUIRE(meta.GetName() == "Pi::Number");
      REQUIRE(meta.GetVersionMajor() == 1);
      REQUIRE(meta.GetVersionMinor() == 0);
   }
}

///                                                                           
/// MARK: Functions                                                           
TEST_CASE_TEMPLATE("A reflected function signature", TestType,
   //decltype(FunctionForTesting), // shouldn't compile
   void(*)(void*)
) {
   using Signature = TestType;

   static_assert(    ::std::is_function_v<Deptr<Signature>>);
   static_assert(    CT::Sparse<Signature>);
   static_assert(not CT::Decayed<Signature>);
   static_assert(    CT::Complete<Signature>);

   const DMeta meta = MetaDataOf<Signature>();

   REQUIRE(meta != nullptr);
   REQUIRE(meta.GetName() == "<void(void*)>*");
   REQUIRE(meta.IsSparse());
}
