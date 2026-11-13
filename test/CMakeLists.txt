add_langulus_test(LangulusRTTITest
   SOURCES     TestConverters.cpp
               TestMetaOf.cpp
               TestRTTI.cpp
               TestRTTIIntent.cpp
               TestRTTIConcepts.cpp
               TestSimilarity.cpp
               TestTag.cpp
               $<$<BOOL:${LANGULUS_OPTION_MANAGED_REFLECTION}>:TestRTTI.cpp>
   LIBRARIES   LangulusRTTI
)