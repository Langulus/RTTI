project(Langulus.RTTI.Test)

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
	add_executable(Test.RTTI Main.cpp TestCTTI.cpp TestRTTI.cpp)
else()
	add_executable(Test.RTTI Main.cpp TestCTTI.cpp)
endif()
target_link_libraries(Test.RTTI PRIVATE fmt Langulus.RTTI Catch2)

if (MSVC)
    target_compile_options(Test.RTTI PRIVATE /bigobj)
endif()

add_test(NAME Test.RTTI COMMAND Test.RTTI)
