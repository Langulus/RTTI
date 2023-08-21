add_executable(LangulusRTTITest
	Main.cpp
	TestCTTI.cpp
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

langulus_copy_dlls(LangulusRTTITest ON LangulusRTTITest)
