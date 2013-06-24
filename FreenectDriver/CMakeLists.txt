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


# Declaration of the library
xmake_create_dynamic_library(FREENECTDRIVER FreenectDriver "1.0.0" "1.0.0" ${HEADERS}  ${SRCS})

xmake_project_link(FREENECTDRIVER LIBFREENECT OPENNI2)

set_target_properties(FreenectDriver PROPERTIES CXX_COMPILER "/usr/bin/clang++"
                                                LIBRARY_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/OpenNI2/Drivers"
)
