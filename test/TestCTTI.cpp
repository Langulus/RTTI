///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Common.hpp"


SCENARIO("An incomplete type reflected (as long as its a pointer)", "[metadata]") {
   GIVEN("IncompleteType*") {
      WHEN("Reflected") {
         //auto metafail = MetaData::Of<IncompleteType>(); // should cause compile error
         auto meta = MetaData::Of<IncompleteType*>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "IncompleteType*");
         REQUIRE(meta->mVersionMajor == 1);
         REQUIRE(meta->mVersionMinor == 0);
         REQUIRE(meta->mIsDeep == false);
         REQUIRE(meta->mIsPOD == false);
         REQUIRE(meta->mIsNullifiable == false);
         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            #if LANGULUS(DEBUG)
               REQUIRE(meta->mPoolTactic == PoolTactic::Type);
            #else
               REQUIRE(meta->mPoolTactic == PoolTactic::Default);
            #endif
         #endif
         REQUIRE(meta->mConcreteRetriever == nullptr);
         REQUIRE(meta->mIsUninsertable == false);
         REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
         REQUIRE(meta->mIsAbstract == false);
         REQUIRE(meta->mSize == sizeof(void*));
         REQUIRE(meta->mAlignment == alignof(void*));
         REQUIRE(meta->mDeptr == nullptr);
         REQUIRE(meta->mOrigin == nullptr);
         REQUIRE(meta->mIsConstant == false);
         REQUIRE(meta->mDecvq == MetaData::Of<IncompleteType*>());

         REQUIRE(meta->mBases.size() == 0);
         REQUIRE(meta->mAbilities.size() == 0);
         REQUIRE(meta->mMembers.size() == 0);
         REQUIRE(meta->mNamedValues.size() == 0);
         REQUIRE(meta->mConvertersTo.size() == 0);
         REQUIRE(meta->mConvertersFrom.size() == 0);
      }
   }

   GIVEN("IncompleteType**") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<IncompleteType**>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "IncompleteType**");
         REQUIRE(meta->mVersionMajor == 1);
         REQUIRE(meta->mVersionMinor == 0);
         REQUIRE(meta->mIsDeep == false);
         REQUIRE(meta->mIsPOD == false);
         REQUIRE(meta->mIsNullifiable == false);
         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            #if LANGULUS(DEBUG)
               REQUIRE(meta->mPoolTactic == PoolTactic::Type);
            #else
               REQUIRE(meta->mPoolTactic == PoolTactic::Default);
            #endif
         #endif
         REQUIRE(meta->mConcreteRetriever == nullptr);
         REQUIRE(meta->mIsUninsertable == false);
         REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
         REQUIRE(meta->mIsAbstract == false);
         REQUIRE(meta->mSize == sizeof(void*));
         REQUIRE(meta->mAlignment == alignof(void*));
         REQUIRE(meta->mDeptr == MetaData::Of<IncompleteType*>());
         REQUIRE(meta->mOrigin == nullptr);
         REQUIRE(meta->mIsConstant == false);
         REQUIRE(meta->mDeptr->mIsConstant == false);
         REQUIRE(meta->mDeptr->mDeptr == nullptr);
         REQUIRE(meta->mDecvq == MetaData::Of<IncompleteType**>());

         REQUIRE(meta->mBases.size() == 0);
         REQUIRE(meta->mAbilities.size() == 0);
         REQUIRE(meta->mMembers.size() == 0);
         REQUIRE(meta->mNamedValues.size() == 0);
         REQUIRE(meta->mConvertersTo.size() == 0);
         REQUIRE(meta->mConvertersFrom.size() == 0);
      }
   }

   GIVEN("const IncompleteType**") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<const IncompleteType**>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "const IncompleteType**");
         REQUIRE(meta->mVersionMajor == 1);
         REQUIRE(meta->mVersionMinor == 0);
         REQUIRE(meta->mIsDeep == false);
         REQUIRE(meta->mIsPOD == false);
         REQUIRE(meta->mIsNullifiable == false);
         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            #if LANGULUS(DEBUG)
               REQUIRE(meta->mPoolTactic == PoolTactic::Type);
            #else
               REQUIRE(meta->mPoolTactic == PoolTactic::Default);
            #endif
         #endif
         REQUIRE(meta->mConcreteRetriever == nullptr);
         REQUIRE(meta->mIsUninsertable == false);
         REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
         REQUIRE(meta->mIsAbstract == false);
         REQUIRE(meta->mSize == sizeof(void*));
         REQUIRE(meta->mAlignment == alignof(void*));
         REQUIRE(meta->mDeptr == MetaData::Of<const IncompleteType*>());
         REQUIRE(meta->mOrigin == nullptr);
         REQUIRE(meta->mIsConstant == false);
         REQUIRE(meta->mDeptr->mIsConstant == false);
         REQUIRE(meta->mDeptr->mDeptr == nullptr);
         REQUIRE(meta->mDecvq == MetaData::Of<IncompleteType**>());

         REQUIRE(meta->mBases.size() == 0);
         REQUIRE(meta->mAbilities.size() == 0);
         REQUIRE(meta->mMembers.size() == 0);
         REQUIRE(meta->mNamedValues.size() == 0);
         REQUIRE(meta->mConvertersTo.size() == 0);
         REQUIRE(meta->mConvertersFrom.size() == 0);
      }
   }

   GIVEN("const IncompleteType* const*&") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<const IncompleteType* const*&>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "const IncompleteType* const*");
         REQUIRE(meta->mVersionMajor == 1);
         REQUIRE(meta->mVersionMinor == 0);
         REQUIRE(meta->mIsDeep == false);
         REQUIRE(meta->mIsPOD == false);
         REQUIRE(meta->mIsNullifiable == false);
         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            #if LANGULUS(DEBUG)
               REQUIRE(meta->mPoolTactic == PoolTactic::Type);
            #else
               REQUIRE(meta->mPoolTactic == PoolTactic::Default);
            #endif
         #endif
         REQUIRE(meta->mConcreteRetriever == nullptr);
         REQUIRE(meta->mIsUninsertable == false);
         REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
         REQUIRE(meta->mIsAbstract == false);
         REQUIRE(meta->mSize == sizeof(void*));
         REQUIRE(meta->mAlignment == alignof(void*));
         REQUIRE(meta->mDeptr == MetaData::Of<const IncompleteType* const>());
         REQUIRE(meta->mOrigin == nullptr);
         REQUIRE(meta->mIsConstant == false);
         REQUIRE(meta->mDeptr->mIsConstant == true);
         REQUIRE(meta->mDeptr->mDeptr == nullptr);
         REQUIRE(meta->mDecvq == MetaData::Of<IncompleteType**>());
         REQUIRE(meta->mDeptr->mDecvq == MetaData::Of<IncompleteType*>());

         REQUIRE(meta->mBases.size() == 0);
         REQUIRE(meta->mAbilities.size() == 0);
         REQUIRE(meta->mMembers.size() == 0);
         REQUIRE(meta->mNamedValues.size() == 0);
         REQUIRE(meta->mConvertersTo.size() == 0);
         REQUIRE(meta->mConvertersFrom.size() == 0);
      }
   }

   GIVEN("const IncompleteType* const* const&") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<const IncompleteType* const* const&>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "const IncompleteType* const* const");
         REQUIRE(meta->mVersionMajor == 1);
         REQUIRE(meta->mVersionMinor == 0);
         REQUIRE(meta->mIsDeep == false);
         REQUIRE(meta->mIsPOD == false);
         REQUIRE(meta->mIsNullifiable == false);
         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            #if LANGULUS(DEBUG)
               REQUIRE(meta->mPoolTactic == PoolTactic::Type);
            #else
               REQUIRE(meta->mPoolTactic == PoolTactic::Default);
            #endif
         #endif
         REQUIRE(meta->mConcreteRetriever == nullptr);
         REQUIRE(meta->mIsUninsertable == false);
         REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
         REQUIRE(meta->mIsAbstract == false);
         REQUIRE(meta->mSize == sizeof(void*));
         REQUIRE(meta->mAlignment == alignof(void*));
         REQUIRE(meta->mDeptr == MetaData::Of<const IncompleteType* const>());
         REQUIRE(meta->mOrigin == nullptr);
         REQUIRE(meta->mIsConstant == true);
         REQUIRE(meta->mDeptr->mIsConstant == true);
         REQUIRE(meta->mDecvq == MetaData::Of<IncompleteType**>());
         REQUIRE(meta->mDeptr->mDecvq == MetaData::Of<IncompleteType*>());

         REQUIRE(meta->mBases.size() == 0);
         REQUIRE(meta->mAbilities.size() == 0);
         REQUIRE(meta->mMembers.size() == 0);
         REQUIRE(meta->mNamedValues.size() == 0);
         REQUIRE(meta->mConvertersTo.size() == 0);
         REQUIRE(meta->mConvertersFrom.size() == 0);
      }
   }

   GIVEN("const IncompleteType* const* const&&") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<const IncompleteType* const* const&&>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "const IncompleteType* const* const");
         REQUIRE(meta->mVersionMajor == 1);
         REQUIRE(meta->mVersionMinor == 0);
         REQUIRE(meta->mIsDeep == false);
         REQUIRE(meta->mIsPOD == false);
         REQUIRE(meta->mIsNullifiable == false);
         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            #if LANGULUS(DEBUG)
               REQUIRE(meta->mPoolTactic == PoolTactic::Type);
            #else
               REQUIRE(meta->mPoolTactic == PoolTactic::Default);
            #endif
         #endif
         REQUIRE(meta->mConcreteRetriever == nullptr);
         REQUIRE(meta->mIsUninsertable == false);
         REQUIRE(meta->mAllocationPage == ::std::max(Alignment, Roof2(sizeof(IncompleteType*))));
         REQUIRE(meta->mIsAbstract == false);
         REQUIRE(meta->mSize == sizeof(void*));
         REQUIRE(meta->mAlignment == alignof(void*));
         REQUIRE(meta->mDeptr == MetaData::Of<const IncompleteType* const>());
         REQUIRE(meta->mOrigin == nullptr);
         REQUIRE(meta->mIsConstant == true);
         REQUIRE(meta->mDeptr->mIsConstant == true);
         REQUIRE(meta->mDecvq == MetaData::Of<IncompleteType**>());
         REQUIRE(meta->mDeptr->mDecvq == MetaData::Of<IncompleteType*>());

         REQUIRE(meta->mBases.size() == 0);
         REQUIRE(meta->mAbilities.size() == 0);
         REQUIRE(meta->mMembers.size() == 0);
         REQUIRE(meta->mNamedValues.size() == 0);
         REQUIRE(meta->mConvertersTo.size() == 0);
         REQUIRE(meta->mConvertersFrom.size() == 0);
      }
   }
}

SCENARIO("A complex type reflected with CTTI traits", "[metadata]") {
   GIVEN("ImplicitlyReflectedDataWithTraits") {
      WHEN("Reflected") {
         ImplicitlyReflectedDataWithTraits instance;
         ImplicitlyReflectedData* ptrtobase = &static_cast<ImplicitlyReflectedData&>(instance);
         const auto baseoffset = reinterpret_cast<char*>(ptrtobase) - reinterpret_cast<char*>(&instance);
         auto meta = MetaData::Of<ImplicitlyReflectedDataWithTraits>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "MyType");
         REQUIRE(meta->mInfo == "Info about MyType");
         REQUIRE(meta->mFileExtensions == "txt, pdf");
         REQUIRE(meta->mVersionMajor == 2);
         REQUIRE(meta->mVersionMinor == 1);
         REQUIRE(meta->mIsDeep == true);
         REQUIRE(meta->mIsPOD == false); // not POD due to being abstract
         REQUIRE(meta->mIsNullifiable == false); // not nullifiable due to being abstract
         IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Size));
         REQUIRE(meta->mConcreteRetriever()->Is<ImplicitlyReflectedData>());
         REQUIRE(meta->mIsUninsertable == true);
         REQUIRE(meta->mAllocationPage == Roof2(250 * sizeof(ImplicitlyReflectedDataWithTraits)));
         REQUIRE(meta->mIsAbstract == true);
         REQUIRE(meta->mSize == sizeof(ImplicitlyReflectedDataWithTraits));
         REQUIRE(meta->mAlignment == alignof(ImplicitlyReflectedDataWithTraits));
         REQUIRE(meta->mOrigin == meta);
         REQUIRE(meta->mIsConstant == false);
         REQUIRE(meta->mDeptr == nullptr);
         REQUIRE(meta->mDecvq == MetaData::Of<ImplicitlyReflectedDataWithTraits>());

         REQUIRE(meta->mBases.size() == 1);
         REQUIRE(meta->mBases[0].mType->Is<ImplicitlyReflectedData>());
         REQUIRE(meta->mBases[0].mImposed == false);
         REQUIRE(meta->mBases[0].mBinaryCompatible == false);
         REQUIRE(meta->mBases[0].mCount == 1);
         REQUIRE(baseoffset >= 0);
         REQUIRE(meta->mBases[0].mOffset == static_cast<Offset>(baseoffset));

         REQUIRE(meta->mAbilities.size() == 1);
         REQUIRE(meta->mAbilities.begin()->first->Is<Verbs::Create>());
         REQUIRE(meta->mAbilities.begin()->second.mVerb->Is<Verbs::Create>());
         REQUIRE(meta->mAbilities.begin()->second.mOverloadsConstant.size() == 1);
         REQUIRE(meta->mAbilities.begin()->second.mOverloadsConstant.contains(Ability::Signature {}));
         REQUIRE(meta->mAbilities.begin()->second.mOverloadsMutable.size() == 1);
         REQUIRE(meta->mAbilities.begin()->second.mOverloadsMutable.contains(Ability::Signature {}));

         REQUIRE(meta->mMembers.size() == 4);
         REQUIRE(meta->mMembers[0].mCount == 1);
         REQUIRE(meta->mMembers[0].mOffset >= sizeof(ImplicitlyReflectedData));
         REQUIRE(meta->mMembers[0].GetTrait() == nullptr);
         REQUIRE(meta->mMembers[0].GetType()->Is<int>());

         REQUIRE(meta->mMembers[1].mCount == 1);
         REQUIRE(meta->mMembers[1].mOffset > meta->mMembers[0].mOffset);
         REQUIRE(meta->mMembers[1].GetTrait()->Is<Traits::Tag>());
         REQUIRE(meta->mMembers[1].GetType()->Is<bool>());

         REQUIRE(meta->mMembers[2].mCount == 12);
         REQUIRE(meta->mMembers[2].mOffset > meta->mMembers[1].mOffset);
         REQUIRE(meta->mMembers[2].GetTrait() == nullptr);
         REQUIRE(meta->mMembers[2].GetType()->Is<int>());

         REQUIRE(meta->mMembers[3].mCount == 1);
         REQUIRE(meta->mMembers[3].mOffset > meta->mMembers[2].mOffset);
         REQUIRE(meta->mMembers[3].GetTrait() == nullptr);
         REQUIRE(meta->mMembers[3].GetType()->Is<int>());

         REQUIRE(meta->mNamedValues.size() == 0);

         const auto intmeta = MetaData::Of<int>();
         REQUIRE(meta->mConvertersTo.size() == 1);
         REQUIRE(meta->mConvertersTo.at(intmeta).mType->Is<int>());
         REQUIRE(meta->mConvertersTo.at(intmeta).mFunction);
         REQUIRE(meta->GetConverter(intmeta) == meta->mConvertersTo.at(intmeta).mFunction);

         const auto pimeta = MetaData::Of<Pi>();
         REQUIRE(meta->mConvertersFrom.size() == 1);
         REQUIRE(meta->mConvertersFrom.at(pimeta).mType->Is<Pi>());
         REQUIRE(meta->mConvertersFrom.at(pimeta).mFunction);
         REQUIRE(meta->GetConverter(pimeta) == nullptr);

         REQUIRE(pimeta->GetConverter(meta) == meta->mConvertersFrom.at(pimeta).mFunction);

         int converted = 1;
         meta->GetConverter(intmeta)(&instance, &converted);
         REQUIRE(converted == 664);

         Pi source;
         ImplicitlyReflectedDataWithTraits convertedFromPi1;
         pimeta->GetConverter(meta)(&source, &convertedFromPi1);
         REQUIRE(convertedFromPi1.member == 314);
      }
   }

   GIVEN("CheckingWhatGetsInherited") {
      WHEN("Reflected") {
         CheckingWhatGetsInherited instance;
         ImplicitlyReflectedData* ptrtobase = &static_cast<ImplicitlyReflectedData&>(instance);
         const auto baseoffset = reinterpret_cast<char*>(ptrtobase) - reinterpret_cast<char*>(&instance);
         auto meta = MetaData::Of<CheckingWhatGetsInherited>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mInfo == "Info about MyType");
         REQUIRE(meta->mFileExtensions == "txt, pdf");
         REQUIRE(meta->mVersionMajor == 2);
         REQUIRE(meta->mVersionMinor == 1);
         REQUIRE(meta->mIsDeep == true);
         REQUIRE(meta->mIsPOD == false);           // not POD due to being abstract
         REQUIRE(meta->mIsNullifiable == false);   // not nullifiable due to being abstract
         IF_LANGULUS_MANAGED_MEMORY(REQUIRE(meta->mPoolTactic == PoolTactic::Size));
         REQUIRE(meta->mConcreteRetriever()->Is<ImplicitlyReflectedData>());
         REQUIRE(meta->mIsUninsertable == true);
         REQUIRE(meta->mAllocationPage == Roof2(250 * sizeof(ImplicitlyReflectedDataWithTraits)));
         REQUIRE(meta->mIsAbstract == true);
         REQUIRE(meta->mSize == sizeof(ImplicitlyReflectedDataWithTraits));
         REQUIRE(meta->mAlignment == alignof(ImplicitlyReflectedDataWithTraits));
         REQUIRE(meta->mOrigin == meta);
         REQUIRE(meta->mIsConstant == false);
         REQUIRE(meta->mDeptr == nullptr);
         REQUIRE(meta->mDecvq == MetaData::Of<CheckingWhatGetsInherited>());

         REQUIRE(meta->mBases.size() == 1);
         REQUIRE(meta->mBases[0].mType->Is<ImplicitlyReflectedData>());
         REQUIRE(meta->mBases[0].mImposed == false);
         REQUIRE(meta->mBases[0].mBinaryCompatible == false);
         REQUIRE(meta->mBases[0].mCount == 1);
         REQUIRE(baseoffset >= 0);
         REQUIRE(meta->mBases[0].mOffset == static_cast<Offset>(baseoffset));

         REQUIRE(meta->mAbilities.size() == 1);
         REQUIRE(meta->mAbilities.begin()->first->Is<Verbs::Create>());
         REQUIRE(meta->mAbilities.begin()->second.mVerb->Is<Verbs::Create>());
         REQUIRE(meta->mAbilities.begin()->second.mOverloadsConstant.size() == 1);
         REQUIRE(meta->mAbilities.begin()->second.mOverloadsConstant.contains(Ability::Signature {}));
         REQUIRE(meta->mAbilities.begin()->second.mOverloadsMutable.size() == 1);
         REQUIRE(meta->mAbilities.begin()->second.mOverloadsMutable.contains(Ability::Signature {}));

         REQUIRE(meta->mMembers.size() == 0);
         REQUIRE(meta->mNamedValues.size() == 0);

         const auto intmeta = MetaData::Of<int>();
         REQUIRE(meta->mConvertersTo.size() == 1);
         REQUIRE(meta->mConvertersTo.at(intmeta).mType->Is<int>());
         REQUIRE(meta->mConvertersTo.at(intmeta).mFunction);
         REQUIRE(meta->GetConverter(intmeta) == meta->mConvertersTo.at(intmeta).mFunction);

         const auto pimeta = MetaData::Of<Pi>();
         REQUIRE(meta->mConvertersFrom.size() == 1);
         REQUIRE(meta->mConvertersFrom.at(pimeta).mType->Is<Pi>());
         REQUIRE(meta->mConvertersFrom.at(pimeta).mFunction);
         REQUIRE(meta->GetConverter(pimeta) == nullptr);

         REQUIRE(pimeta->GetConverter(meta) == meta->mConvertersFrom.at(pimeta).mFunction);

         int converted = 1;
         meta->GetConverter(intmeta)(&instance, &converted);
         REQUIRE(converted == 664);

         Pi source;
         CheckingWhatGetsInherited convertedFromPi1;
         pimeta->GetConverter(meta)(&source, &convertedFromPi1);
         REQUIRE(convertedFromPi1.member == 314);
      }
   }
}

SCENARIO("A simple type reflected with CTTI traits", "[metadata]") {
   GIVEN("ImplicitlyReflectedData") {
      WHEN("Reflected") {
         auto meta = MetaData::Of<ImplicitlyReflectedData>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "ImplicitlyReflectedData");
         REQUIRE(meta->mCppName == "ImplicitlyReflectedData");
         REQUIRE(meta->mInfo == "<no info provided>");
         REQUIRE(meta->mFileExtensions == "ASE");
         REQUIRE(meta->mVersionMajor == 1);
         REQUIRE(meta->mVersionMinor == 0);
         REQUIRE(meta->mIsDeep == false);
         REQUIRE(meta->mIsPOD == true);
         REQUIRE(meta->mIsNullifiable == false);
         #if LANGULUS_FEATURE(MANAGED_MEMORY)
            #if LANGULUS(DEBUG)
               REQUIRE(meta->mPoolTactic == PoolTactic::Type);
            #else
               REQUIRE(meta->mPoolTactic == PoolTactic::Default);
            #endif
         #endif
         REQUIRE(meta->mConcreteRetriever == nullptr);
         REQUIRE(meta->mIsUninsertable == false);
         REQUIRE(meta->mAllocationPage >= Alignment);
         REQUIRE(meta->mIsAbstract == false);
         REQUIRE(meta->mSize == sizeof(ImplicitlyReflectedData));
         REQUIRE(meta->mAlignment == alignof(ImplicitlyReflectedData));

         REQUIRE(meta->mNamedValues.size() == 3);
         REQUIRE(meta->mConvertersTo.size() == 0);
         REQUIRE(meta->mConvertersFrom.size() == 0);
      }
   }
}

SCENARIO("A reflected verb with CTTI traits", "[metaverb]") {
   GIVEN("Create verb with positive/negative tokens, with stateless and contextual default functions") {
      WHEN("Reflected") {
         Anyness::Block someBlock;
         Flow::Verb someVerb;
         auto meta = MetaVerb::Of<Verbs::Create>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "Create");
         REQUIRE(meta->mTokenReverse == "Destroy");
         REQUIRE(meta->mInfo.starts_with("Used for allocating new elements. "));
         REQUIRE(meta->mVersionMajor == 1);
         REQUIRE(meta->mVersionMinor == 0);
         REQUIRE(meta->mOperator == " + ");
         REQUIRE(meta->mPrecedence == 5);
         REQUIRE(meta->mOperatorReverse == " - ");
         REQUIRE(meta->mDefaultInvocationMutable);
         REQUIRE(meta->mDefaultInvocationMutable(someBlock, someVerb) == false);
         REQUIRE(meta->mDefaultInvocationConstant);
         REQUIRE(meta->mDefaultInvocationConstant(someBlock, someVerb) == true);
         REQUIRE(meta->mStatelessInvocation);
         REQUIRE(meta->mStatelessInvocation(someVerb) == false);
      }
   }
}

void FunctionForTesting(void*) {
   Logger::Verbose("Executed FunctionForTesting");
}

SCENARIO("A reflected function signature", "[function]") {
   GIVEN("A reflected function pointer") {
      using Signature = void(*)(void*);

      WHEN("Reflected") {
         static_assert(    ::std::is_function_v<Deptr<Signature>>);
         static_assert(    CT::Sparse<Signature>);
         static_assert(not CT::Decayed<Signature>);
         static_assert(    CT::Complete<Signature>);

         auto meta = MetaData::Of<Signature>();

         REQUIRE(meta != nullptr);
         REQUIRE(meta->mToken == "Function<void(void*)>*");
         REQUIRE(meta->mIsSparse);
         REQUIRE(meta->mVersionMajor == 1);
         REQUIRE(meta->mVersionMinor == 0);
      }
   }

   /*GIVEN("A reflected function reference (shouldn't compile)") {
      auto FuncRef = FunctionForTesting;
      using Signature = decltype(*FuncRef);

      WHEN("Reflected") {
         static_assert(    ::std::is_function_v<Deref<Signature>>);
         static_assert(    CT::Dense<Signature>);
         static_assert(not CT::Decayed<Signature>);
         static_assert(not CT::Complete<Signature>);

         auto meta = MetaData::Of<Signature>();
         REQUIRE(meta != nullptr);

         THEN("Requirements should be met") {
            REQUIRE(meta->mToken == "void(*)(void*)");
            REQUIRE(meta->mIsSparse);
            REQUIRE(meta->mVersionMajor == 1);
            REQUIRE(meta->mVersionMinor == 0);
         }
      }
   }*/
}
