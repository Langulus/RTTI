cmake_minimum_required(VERSION 3.20)
include(cmake/FetchLangulusModule.cmake)
project(Langulus_RTTI)

# Check if this project is built as standalone, or a part of something else
if(PROJECT_IS_TOP_LEVEL)
    # We're building it as standalone
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    fetch_langulus_module(Core)

	enable_testing()
	add_subdirectory(test)
endif()

# Build and install RTTI library
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
add_library(Langulus.RTTI ${SOURCE_FILES})

target_include_directories(Langulus.RTTI
	PRIVATE   ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC	  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Langulus.RTTI
	PUBLIC   Langulus.Core
)

if (MSVC)
    target_compile_options(Langulus.RTTI PRIVATE /utf-8)
endif()