cmake_minimum_required(VERSION 3.24)
project(LangulusRTTI
    VERSION         1.0.0
    DESCRIPTION     "Langulus reflection library"
    HOMEPAGE_URL    https://langulus.com
)

# Check if this project is built as standalone, or a part of something else 
if(PROJECT_IS_TOP_LEVEL OR NOT LANGULUS)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/lib)

    include(LangulusUtilities.cmake)

    # Add Langulus::Core & Logger libraries                                 
    fetch_langulus_module(Core   GIT_TAG 0097bd180a5c625f88db0db0514bbf93aff53c7a)
    fetch_langulus_module(Logger GIT_TAG b63513abb9fb3014107335949e17e41e6e8a334b)
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