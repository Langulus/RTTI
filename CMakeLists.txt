cmake_minimum_required(VERSION 3.22)
include(cmake/FetchLangulusModule.cmake)
project(Langulus.RTTI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure the features you require, all these are disabled by default
option(LANGULUS_ENABLE_SAFE_MODE 
    "Overrides additional error checking and sanity checks, \
    incurs a serious runtime overhead")

option(LANGULUS_ENABLE_PARANOIA 
    "Make sure that any unused memory is nullified, \
    incurs a serious runtime overhead")

option(LANGULUS_ENABLE_DEBUGGING 
    "Explicitly define debug mode, in the case that default flags are \
    not detected by some odd reason")

option(LANGULUS_FEATURE_MANAGED_REFLECTION
    "Reflections will be kept in a centralized location, when reflected, which speeds up \
    type comparisons, and allows you to dynamically modify the reflection at runtime")

if(LANGULUS_ENABLE_SAFE_MODE)
    add_compile_definitions(LANGULUS_ENABLE_SAFE_MODE)
endif()
if(LANGULUS_ENABLE_PARANOIA)
    add_compile_definitions(LANGULUS_ENABLE_PARANOIA)
endif()
if(LANGULUS_ENABLE_DEBUGGING)
    add_compile_definitions(LANGULUS_ENABLE_DEBUGGING)
endif()
if(LANGULUS_FEATURE_MANAGED_REFLECTION)
    add_compile_definitions(LANGULUS_ENABLE_FEATURE_MANAGED_REFLECTION)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message(STATUS "Debug mode, all assertions are enabled")
	add_compile_definitions(LANGULUS_ENABLE_ASSERTION_LEVEL=1000)
    add_compile_definitions(LANGULUS_ENABLE_DEBUGGING)
endif()

# Check if this project is built as standalone, or a part of something else
if(PROJECT_IS_TOP_LEVEL)
    fetch_langulus_module(Core)
    fetch_langulus_module(Logger)

    add_compile_definitions(LANGULUS_ENABLE_TESTING)
	enable_testing()
	add_subdirectory(test)
endif()

# Build and install RTTI library
if(LANGULUS_FEATURE_MANAGED_REFLECTION)
    add_library(Langulus.RTTI source/RTTI.cpp)
    target_include_directories(Langulus.RTTI PUBLIC include)
    target_link_libraries(Langulus.RTTI PUBLIC Langulus.Logger)

    if (MSVC)
        target_compile_options(Langulus.RTTI PRIVATE /utf-8)
    endif()
else()
    add_library(Langulus.RTTI INTERFACE)
    target_include_directories(Langulus.RTTI INTERFACE include)
    target_link_libraries(Langulus.RTTI INTERFACE Langulus.Logger)
endif()
