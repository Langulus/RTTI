cmake_minimum_required(VERSION 3.28)
project(LangulusRTTI
    VERSION         1.0.0
    DESCRIPTION     "Langulus reflection library"
    HOMEPAGE_URL    https://langulus.com
)

# Check if this project is built as standalone, or a part of something else 
if (PROJECT_IS_TOP_LEVEL OR NOT LANGULUS)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
    include(LangulusUtilities.cmake)
    fetch_langulus_module(Logger GIT_TAG f9b18f9ca69cda9b5d73b2406f23c25132c385f3)
endif()

# Build and install library                                                 
add_langulus_library(LangulusRTTI
    $<TARGET_OBJECTS:LangulusLogger>
	$<$<BOOL:${LANGULUS_FEATURE_MANAGED_REFLECTION}>:source/RTTI.cpp>
)

target_include_directories(LangulusRTTI
    PUBLIC      $<TARGET_PROPERTY:LangulusLogger,INTERFACE_INCLUDE_DIRECTORIES>
				include
)

target_link_libraries(LangulusRTTI
    PUBLIC      LangulusCore
                fmt
)

target_compile_definitions(LangulusRTTI
    PRIVATE     LANGULUS_EXPORT_ALL
)

if(LANGULUS_TESTING)
	enable_testing()
	add_subdirectory(test)
endif()