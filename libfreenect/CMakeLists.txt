project(LIBFREENECT)

# Settings
set(BUILD_LIBFREENECT_EXAMPLES OFF CACHE BOOL "Build libfreenect examples")
set(BUILD_FAKENECT OFF CACHE BOOL "Build fakenect (note: it doesn't work with OpenNI)")


add_subdirectory(src)

if (BUILD_FAKENECT)
    add_subdirectory(fakenect)
endif()

if (BUILD_LIBFREENECT_EXAMPLES)
    add_subdirectory(examples)
endif()
