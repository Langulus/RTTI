project(Langulus::RTTI::Test)

if(LANGULUS_FEATURE_MANAGED_REFLECTION)
	add_executable(LangulusRTTITest Main.cpp TestCTTI.cpp TestRTTI.cpp)
else()
	add_executable(LangulusRTTITest Main.cpp TestCTTI.cpp)
endif()

target_link_libraries(LangulusRTTITest
	PRIVATE		LangulusRTTI
				Catch2
)

add_test(
	NAME		Langulus::RTTI::Test
	COMMAND		LangulusRTTITest
)

langulus_copy_dlls(LangulusRTTITest ON LangulusRTTITest)
