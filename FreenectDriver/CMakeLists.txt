project(FREENECTDRIVER)


# List the header files
set(HEADERS ColorStream.hpp
            D2S.h
            DepthStream.hpp
            S2D.h
            VideoStream.hpp
            libfreenect.hpp
)


# List the source files
set(SRCS ColorStream.cpp
         DepthStream.cpp
         DeviceDriver.cpp
)


# List the include paths
xmake_import_search_paths(LIBFREENECT)
xmake_import_search_paths(OPENNI2)


# Declaration of the library (Note: Can't use xmake_create_dynamic_library for that,
# because it creates both the library and a symbolic link to it, which OpenNI considers
# as two different drivers to two different devices...)
xmake_set(XMAKE_FREENECTDRIVER_TARGET                        "FreenectDriver")
xmake_set(XMAKE_FREENECTDRIVER_INCLUDE_PATHS                 "")
xmake_set(XMAKE_FREENECTDRIVER_LINK_PATHS                    "")
xmake_set(XMAKE_FREENECTDRIVER_COMPILE_DEFINITIONS           "")
xmake_set(XMAKE_FREENECTDRIVER_LINK_FLAGS                    "")
xmake_set(XMAKE_FREENECTDRIVER_LINK_TARGETS                  "FreenectDriver")
xmake_set(XMAKE_FREENECTDRIVER_DISABLED_VS_WARNINGS          "")
xmake_set(XMAKE_FREENECTDRIVER_EXECUTABLE                    NO)
xmake_set(XMAKE_FREENECTDRIVER_STATIC_LIBRARY                NO)
xmake_set(XMAKE_FREENECTDRIVER_DYNAMIC_LIBRARY               YES)
xmake_set(XMAKE_FREENECTDRIVER_FRAMEWORK                     NO)

add_library(FreenectDriver SHARED ${HEADERS} ${SRCS})

# Set the INSTALL_PATH so that the dynamic library can be local
if (NOT WIN32)
    set_target_properties(FreenectDriver PROPERTIES BUILD_WITH_INSTALL_RPATH ON
                                                    INSTALL_NAME_DIR "@rpath"
    )
endif()


xmake_project_link(FREENECTDRIVER LIBFREENECT OPENNI2)

set_target_properties(FreenectDriver PROPERTIES CXX_COMPILER "/usr/bin/clang++"
                                                LIBRARY_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/OpenNI2/Drivers"
)
