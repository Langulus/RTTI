add_langulus_test(LangulusRTTITest
	SOURCES		Main.cpp
				TestConverters.cpp
				TestCoreConcepts.cpp
				TestRTTIConcepts.cpp
				TestCTTI.cpp
				TestHashing.cpp
				TestNameOf.cpp
				TestIntents.cpp
				TestSimilarity.cpp
				$<$<BOOL:${LANGULUS_FEATURE_MANAGED_REFLECTION}>:TestRTTI.cpp>
	LIBRARIES	LangulusRTTI
)