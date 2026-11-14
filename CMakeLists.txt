cmake_minimum_required(VERSION 3.30)
project(LangulusRTTI
   VERSION         1.0.0
   DESCRIPTION     "Langulus reflection library"
   HOMEPAGE_URL    https://langulus.com
)

# Check if this project is built as standalone, or a part of something else 
if (PROJECT_IS_TOP_LEVEL OR NOT LANGULUS)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
   include(LangulusUtilities.cmake)
   fetch_langulus_module(CT     GIT_TAG wip)
   fetch_langulus_module(Logger GIT_TAG wip)
endif()

# Build and install library                                                 
add_langulus_library(LangulusRTTI
   SOURCES  $<TARGET_OBJECTS:LangulusLogger>
            $<$<BOOL:${LANGULUS_OPTION_MANAGED_REFLECTION}>:source/Registry.cpp source/Boundary.cpp>
            source/DefinitionData.cpp
)

target_include_directories(LangulusRTTI
   PUBLIC   include
)

target_link_libraries(LangulusRTTI
   PUBLIC   LangulusCT
            LangulusLogger
)

target_compile_definitions(LangulusRTTI
   PRIVATE  LANGULUS_EXPORT_ALL
)

if(LANGULUS_OPTION_TESTING)
   enable_testing()
   add_subdirectory(test)
endif()