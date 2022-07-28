project(Langulus_Test_RTTI)
add_definitions(-DLANGULUS_ENABLE_SAFE_MODE)

# Configure Catch2 library
message(STATUS "Configuring external project: Catch2...")
download_project(
	PROJ                Catch2
	GIT_REPOSITORY      https://github.com/catchorg/Catch2.git
	GIT_TAG             v2.x
	UPDATE_DISCONNECTED 1
)
add_subdirectory(${Catch2_SOURCE_DIR} ${Catch2_BINARY_DIR})

if(LANGULUS_FEATURE_MANAGED_REFLECTION)
    add_definitions(-DLANGULUS_ENABLE_FEATURE_MANAGED_REFLECTION)
	add_executable(Test.RTTI Main.cpp TestCTTI.cpp TestRTTI.cpp)
else()
	add_executable(Test.RTTI Main.cpp TestCTTI.cpp)
endif()

target_link_libraries(Test.RTTI PRIVATE Langulus.RTTI Catch2)

add_test(NAME Test.RTTI COMMAND Test.RTTI)
