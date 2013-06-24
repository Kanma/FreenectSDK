cmake_minimum_required(VERSION 2.6)
project(FREENECT_SDK)


##########################################################################################
# XMake-related settings

# Set the output paths
if (NOT DEFINED XMAKE_BINARY_DIR)
    set(XMAKE_BINARY_DIR "${FREENECT_SDK_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/bin")
endif()

# Set the dependencies path
if (NOT DEFINED XMAKE_DEPENDENCIES_DIR)
    set(XMAKE_DEPENDENCIES_DIR "${FREENECT_SDK_SOURCE_DIR}")
endif()


##########################################################################################
# XMake importation

if (NOT EXISTS "${XMAKE_DEPENDENCIES_DIR}/XMake/XMake.cmake")
    message(FATAL_ERROR
"Missing dependency: XMake
The Freenect SDK requires XMake as a dependency.

It is provided as GIT submodule of this repository.

Did you forgot to execute the following commands?
   git submodule init
   git submodule update")
endif()

include("${XMAKE_DEPENDENCIES_DIR}/XMake/XMake.cmake")


##########################################################################################
# Check that OpenNI2 and NiTE2 are imported

if (NOT EXISTS "${FREENECT_SDK_SOURCE_DIR}/OpenNI2/bin/libOpenNI2.dylib")
    message(FATAL_ERROR
"Missing dependency: OpenNI2
The Freenect SDK requires OpenNI2 as a dependency.

You have to download it from www.openni.org, decompress it and import it using the
following command:
   ./tools/import_openni2.sh /path/to/OpenNI2"
)
endif()

if (NOT EXISTS "${FREENECT_SDK_SOURCE_DIR}/NiTE2/bin/libNiTE2.dylib")
    message(FATAL_ERROR
"Missing dependency: OpenNI2
The Freenect SDK requires NiTE2 as a dependency.

You have to download it from www.openni.org, decompress it and import it using the
following command:
   ./tools/import_nite2.sh /path/to/NiTE2"
)
endif()


##########################################################################################
# Process the subdirectories

add_subdirectory(libusb)
add_subdirectory(libfreenect)
add_subdirectory(OpenNI2)
add_subdirectory(NiTE2)
add_subdirectory(FreenectDriver)
