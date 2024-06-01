cmake_minimum_required(VERSION 3.28)
project(LangulusRTTI
    VERSION         1.0.0
    DESCRIPTION     "Langulus reflection library"
    HOMEPAGE_URL    https://langulus.com
)

# Check if this project is built as standalone, or a part of something else 
if(PROJECT_IS_TOP_LEVEL OR NOT LANGULUS)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

    include(LangulusUtilities.cmake)

    # Add Langulus::Core & Logger libraries                                 
    fetch_langulus_module(Core   GIT_TAG 828bd2400b3f58069cdde28d8b189ed915069ad6)
    fetch_langulus_module(Logger GIT_TAG a4fb95beb9ba95ca7d4b4b69cb0de9448bce7862)
endif()

# Build and install library                                                 
add_library(LangulusRTTI ${LANGULUS_LIBRARY_TYPE}
    $<TARGET_OBJECTS:LangulusLogger>
	$<$<BOOL:${LANGULUS_FEATURE_MANAGED_REFLECTION}>:source/RTTI.cpp>
)

target_include_directories(LangulusRTTI
    PUBLIC      include
                $<TARGET_PROPERTY:LangulusLogger,INTERFACE_INCLUDE_DIRECTORIES>
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