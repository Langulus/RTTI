add_executable(LangulusRTTITest
	Main.cpp
	TestConverters.cpp
	TestCTTI.cpp
	TestHashing.cpp
	TestNameOf.cpp
	TestSemantics.cpp
	TestSimilarity.cpp
	$<$<BOOL:${LANGULUS_FEATURE_MANAGED_REFLECTION}>:TestRTTI.cpp>
)

target_link_libraries(LangulusRTTITest
	PRIVATE		LangulusRTTI
				Catch2
)

add_test(
	NAME		LangulusRTTITest
	COMMAND		LangulusRTTITest
)