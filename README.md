[![Langulus::RTTI CI](https://github.com/Langulus/RTTI/actions/workflows/ci.yml/badge.svg)](https://github.com/Langulus/RTTI/actions/workflows/ci.yml)
# Langulus::RTTI
`Langulus::RTTI` is a cross-platform, C++23 reflection library that relies fully on modern C++ features, without using any pre/post build steps.
It is used in [all other Langulus](https://github.com/Langulus) libraries and plug-ins.

# Getting the library
If you have CMake 3.28+, the easiest way to get this library is to use `FetchContent` in your `CMakeLists.txt`, like so:
```cmake
include(FetchContent)
FetchContent_Declare(LangulusRTTI
   GIT_REPOSITORY   https://github.com/Langulus/RTTI.git
   GIT_TAG          main    # you should use a specific commit/release,
                            # so that you can control when to bump your dependencies
   EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(LangulusRTTI)
```
Then just link to your target to get all the include directories:
```cmake
target_link_libraries(YourLibrary PUBLIC LangulusRTTI)
```
This will also automagically fetch all dependencies: [Langulus::Core](https://github.com/Langulus/Core) and [Langulus::Logger](https://github.com/Langulus/Logger).

# Using the library
After linking with `LangulusRTTI`, you can include it in your files and start reflecting:
```c++
#include <RTTI/Meta.hpp>
using namespace Langulus;

struct AbstractBase {
   LANGULUS_ABSTRACT() true;                      // Types can be reflected as abstract, forbidding instantiation at runtime
};

struct ImposedBase {
   // This type isn't related in C++, but can still be added as an 'imposed' base
};

struct MyReflectedType : AbstractBase {
   int mData = 0;

   LANGULUS_NAME() "CustomName";                  // Custom name - if none is specified, the C++ name will be used
   LANGULUS_INFO() "Some info about the type";    // Reflected information about the type
   LANGULUS_FILES() "txt,rtf";                    // Associated file formats
   LANGULUS_VERSION_MAJOR() 1;                    // Each type can have its own major version
   LANGULUS_VERSION_MINOR() 0;                    // Each type can have its own minor version
   LANGULUS_POD() true;                           // Make sure our type is considered Plain Old Data
                                                  // Provides default serialization to bytes and various optimizations

   LANGULUS_NULLIFIABLE() true;                   // Make sure our type is reflected as nullifiable
                                                  // Allows for batch-zero-initialization optimizations

   LANGULUS_POOL_TACTIC() RTTI::PoolTactic::Type; // Make sure that instances of this type are grouped in dedicated pools
                                                  // Available only if LangulusRTTI was built with LANGULUS_FEATURE_MANAGED_MEMORY

   LANGULUS_NAMED_VALUES(One, Two);               // Reflect some named values
   LANGULUS_SUFFIX() "my";                        // Reflect a suffix
   LANGULUS_ALLOCATION_PAGE() 128;                // Constrol the minimum number of instances allocated
   LANGULUS_ABSTRACT() false;                     // MyReflectedType inherits from AbstractBase, and will be reflected as abstract, unless we explicitly specify it isn't
   LANGULUS_MEMBERS(&MyReflectedType::mData);     // Reflect members
   LANGULUS_BASES(AbstractBase, ImposedBase);     // Reflect bases, not necessarily inherited from it
   LANGULUS_CONVERTS_TO(std::string);             // Reflect convertion to std::string, will complain at compile-time if conversion isn't possible
   LANGULUS_CONVERTS_FROM(bool);                  // Reflect conversion from bool, will complain at compile-time if conversion isn't possible
   LANGULUS_TYPED() int;                          // Reflect the contained type
};

int main(int argc, char* argv[]) {
   // Three ways to retrieve the type info at runtime:
   auto typeInfo1 = MetaOf<MyReflectedType>();       // Retrieve from a static type, deducing what kind of meta to return
                                                     // The type will be reflected on first MetaOf call

   auto typeInfo2 = MetaDataOf<MyReflectedType>();   // Retrieve from a static type
                                                     // The type will be reflected on first MetaDataOf call

   auto typeInfo3 = RTTI::GetMetaData("CustomName"); // Retrieve via the reflected name
                                                     // Works only if LangulusRTTI was built with LANGULUS_FEATURE_MANAGED_REFLECTION
                                                     // Expects type to have been reflected using one of the above functions prior to calling this one
   
   return 0;
}
```

# Considerations
Langulus is still heavily under development, and is by nature highly experimental. There could be numerous breaking changes, renames, design overhauls, etc. coming in the future. Our design is led by practice - as more features are added to Langulus, more things will be required to be reflected and worthy of being added to Langulus::RTTI.

# Community feedback
I would love feedback and suggestions, please feel free to join the [forum](https://github.com/Langulus/RTTI/discussions).
However it should be stated clearly, that Langulus is primarily a personal project that I've been developing for over 10 years, and is aiming primarily at satisfying my requirements.
As such, ideas and suggestions may not be necessarily implemented by [me](https://github.com/Epixu), unless I find them exceedingly useful.
If you really need me to add or improve something that isn't on my daily agenda, you should consider sponsoring me.
