cmake_minimum_required(VERSION 3.22)
include(cmake/FetchLangulusModule.cmake)
project(Langulus_RTTI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LANGULUS_FEATURE_MANAGED_REFLECTION
    "Reflections will be kept in a centralized location, when reflected, which speeds up \
    type comparisons, and allows you to dynamically modify the reflection at runtime" ON)

# Check if this project is built as standalone, or a part of something else
if(PROJECT_IS_TOP_LEVEL)
    fetch_langulus_module(Core)
    fetch_langulus_module(Logger)

	enable_testing()
	add_subdirectory(test)
endif()

# Build and install RTTI library
if(LANGULUS_FEATURE_MANAGED_REFLECTION)
    add_definitions(-DLANGULUS_ENABLE_MANAGED_REFLECTION)
    add_library(Langulus.RTTI RTTI.cpp)
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
