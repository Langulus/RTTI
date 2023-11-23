add_executable(LangulusRTTITest
	Main.cpp
	TestNameOf.cpp
	TestCTTI.cpp
	TestSimilarity.cpp
	TestSemantics.cpp
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