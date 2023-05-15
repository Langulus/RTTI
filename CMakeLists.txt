cmake_minimum_required(VERSION 3.24)
project(LangulusRTTI)

# Check if this project is built as standalone, or a part of something else 
if(PROJECT_IS_TOP_LEVEL OR NOT LANGULUS)
    # Populate the cmake directory											
    include(LangulusUtilities.cmake)
    langulus_fetch_cmake()

    # Handle feature flags and global definitions/requirements				
    include(HandleLangulusFeatures)
    handle_langulus_features()

    # Add Langulus::Core & Logger library                                   
    include(FetchLangulusModule)
    fetch_langulus_module(Core)
    fetch_langulus_module(Logger)
endif()

# Build and install library                                                 
if(LANGULUS_FEATURE_MANAGED_REFLECTION)
    set(LANGULUS_LOGGER_SOURCES source/RTTI.cpp)
endif()

add_library(LangulusRTTI ${LANGULUS_LIBRARY_TYPE}
    $<TARGET_OBJECTS:LangulusLogger>
    ${LANGULUS_LOGGER_SOURCES}
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

if(LANGULUS_ENABLE_TESTING)
    enable_testing()
	add_subdirectory(test)
endif()