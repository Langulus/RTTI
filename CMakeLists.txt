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
    fetch_langulus_module(Logger GIT_TAG 2fa3c59f2ef5f9888954a5c3154201d134d6fc07)
    if(LANGULUS_FEATURE_PROFILING)
        fetch_langulus_module(Profiler GIT_TAG b53ab5b93628ca487a20216786c49ccf632225ab)
    endif()
endif()

# Build and install library                                                 
add_langulus_library(LangulusRTTI
    $<TARGET_OBJECTS:LangulusLogger>
	$<$<BOOL:${LANGULUS_FEATURE_MANAGED_REFLECTION}>:source/RTTI.cpp>
	$<$<BOOL:${LANGULUS_FEATURE_PROFILING}>:$<TARGET_OBJECTS:LangulusProfiler>>
)

target_include_directories(LangulusRTTI
    PUBLIC      $<TARGET_PROPERTY:LangulusLogger,INTERFACE_INCLUDE_DIRECTORIES>
				$<$<BOOL:${LANGULUS_FEATURE_PROFILING}>:$<TARGET_PROPERTY:LangulusProfiler,INTERFACE_INCLUDE_DIRECTORIES>>
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