cmake_minimum_required(VERSION 3.22)
project(Langulus.RTTI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if this project is built as standalone, or a part of something else
if(PROJECT_IS_TOP_LEVEL)
    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init -- cmake 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    include(FetchLangulusModule)
    include(HandleLangulusFeatures)

    handle_langulus_features()

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
