add_langulus_test(LangulusRTTITest
   SOURCES     TestConverters.cpp
               TestRTTIIntent.cpp
               TestRTTIConcepts.cpp
               TestSimilarity.cpp
               $<$<BOOL:${LANGULUS_OPTION_MANAGED_REFLECTION}>:TestRTTI.cpp>
   LIBRARIES   LangulusRTTI
)