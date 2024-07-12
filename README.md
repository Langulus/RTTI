[![Langulus::RTTI CI](https://github.com/Langulus/RTTI/actions/workflows/ci.yml/badge.svg)](https://github.com/Langulus/RTTI/actions/workflows/ci.yml)
# Langulus::RTTI
`Langulus::RTTI` is a cross-platform, C++23 reflection library that relies fully on modern C++ features, without using any pre/post build steps.
It is used in [Langulus libraries and plug-ins](https://github.com/Langulus).
By itself, the library is nothing more than a consistent protocol for reflecting types.
It becomes slightly more powerful, when built with the `LANGULUS_FEATURE_MANAGED_REFLECTION` option (enabled by default). 
This allows for a centralized location where type definitions are stored, and allows for faster type comparisons and runtime modification of meta data, like registering/unregistering new stuff after a shared library is loaded in.
The real power of `Langulus::RTTI` comes from the use of [`Langulus::Anyness`](https://github.com/Langulus/Anyness) containers, that are specially tailored to take full use of the reflection, allowing for highly optimized type-erasure.

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
   // Types can be reflected as abstract, forbidding instantiation at runtime
   // C++ types that have pure virtual functions will be autodetected as abstract
   LANGULUS_ABSTRACT() true;
};

struct ImposedBase {
   // This type isn't inherited from in MyReflectedType
   // but can still be added as an 'imposed' base - useful to add runtime tags
   // Imposed bases should never be serialized
};

struct MyReflectedType : AbstractBase {
   using T = int;

   // Contained data
   T mData = 0;

   enum Named : T {
      One = 1, Two = 2
   };

   // Custom name - if none is specified, the C++ name will be used, see NameOf
   LANGULUS_NAME() "CustomName";

   // Reflected information about the type
   LANGULUS_INFO() "Some info about the type";

   // Associated file formats, useful to deduce data type from a filename
   LANGULUS_FILES() "txt,rtf";

   // Each type can have its own major version.
   // This can be used to detect version mismatches at runtime, when linking different builds of modules/libraries
   LANGULUS_VERSION_MAJOR() 1;
   // Each type can have its own minor version
   // This can be used to warn users of minor changes, whenl linking different builds of modules/libraries
   LANGULUS_VERSION_MINOR() 0;

   // Make sure our type is considered Plain Old Data
   // This allows for default serialization to bytes and various batching optimizations
   // Beware of padding bytes when comparing POD types
   LANGULUS_POD() true;
                                                  
   // Make sure our type is reflected as nullifiable
   // This allows for batch zero-initialization optimizations
   LANGULUS_NULLIFIABLE() true;                                                  

   // Make sure that instances of this type are grouped in dedicated pools
   // Available only if LangulusRTTI was built with LANGULUS_FEATURE_MANAGED_MEMORY
   LANGULUS_POOL_TACTIC() RTTI::PoolTactic::Type;                                                  

   // Reflect some named values, that can be used for substituting values when serializing to text
   LANGULUS_NAMED_VALUES(One, Two);

   // Reflect a suffix, that can be used when parsing literals
   LANGULUS_SUFFIX() "my";

   // Constrol the minimum number of instances allocated
   LANGULUS_ALLOCATION_PAGE() 128;

   // MyReflectedType inherits from AbstractBase, and will be reflected as abstract
   // unless we explicitly specify it isn't
   LANGULUS_ABSTRACT() false;

   // Reflect members
   LANGULUS_MEMBERS(&MyReflectedType::mData);

   // Reflect bases, not necessarily inherited from them
   // Imposed bases will be auto-detected
   LANGULUS_BASES(AbstractBase, ImposedBase);

   // Reflect convertion to std::string
   // Will complain at compile-time if conversion isn't possible
   LANGULUS_CONVERTS_TO(std::string);

   // Reflect conversion from bool
   // Will complain at compile-time if conversion isn't possible
   LANGULUS_CONVERTS_FROM(bool);

   // Reflect the contained type
   LANGULUS_TYPED() decltype(mData);
};

int main(int argc, char* argv[]) {
   // Four ways to retrieve the type info at runtime:
   // 1. Retrieve from a static type, deducing what kind of type info to return
   // The type will be reflected on first MetaOf call
   auto typeInfo1 = MetaOf<MyReflectedType>();                                                      

   // 2. Retrieve from a static type
   // The type will be reflected on first MetaDataOf call
   auto typeInfo2 = MetaDataOf<MyReflectedType>();   

   // 3. Retrieve via the reflected name
   // Works only if LangulusRTTI was built with LANGULUS_FEATURE_MANAGED_REFLECTION enabled
   // Expects type to have been reflected using one of the above functions prior to calling this one
   auto typeInfo3 = RTTI::GetMetaData("CustomName"); 

   // 4. Retrieve via the reflected type extension
   // Works only if LangulusRTTI was built with LANGULUS_FEATURE_MANAGED_REFLECTION enabled
   // Expects type to have been reflected using one of the above functions prior to calling this one
   auto typeInfo4 = RTTI::ResolveFileExtension("txt");

   if (typeInfo1 == typeInfo2
   and typeInfo1 == typeInfo3
   and typeInfo1 == typeInfo4)
      return 0;
   else
      return -1;
}
```
For a full list of reflection options, see the [wiki](https://github.com/Langulus/RTTI/wiki/Reflection).

# Considerations
Langulus is still heavily under development and is by nature highly experimental.
There could be numerous breaking changes, renames, design overhauls, etc. coming in the future.
Our design is led by practice - as more features are added to Langulus, more things will be required to be reflected and worthy of being added to Langulus::RTTI.

# Community feedback
I would love feedback and suggestions, please feel free to join the [forum](https://github.com/Langulus/RTTI/discussions).
However it should be stated clearly, that Langulus is primarily a personal project that I've been developing for over 10 years, and is aiming primarily at satisfying my requirements.
As such, ideas and suggestions may not be necessarily implemented by [me](https://github.com/Epixu), unless I find them exceedingly useful.
If you really need me to add or improve something that isn't on my daily agenda, you should consider sponsoring me.
