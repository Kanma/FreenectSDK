project(OPENNI2)

# Settings
set(BUILD_OPENNI2_EXAMPLES OFF CACHE BOOL "Build OpenNI2 examples")


# Copy the binaries into the build folder
configure_file(bin/libOpenNI2.dylib "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libOpenNI2.dylib" COPYONLY)
configure_file(bin/OpenNI.ini "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/OpenNI.ini" COPYONLY)


# Export the library
xmake_set(XMAKE_OPENNI2_INCLUDE_PATHS "${OPENNI2_SOURCE_DIR}/include")
xmake_set(XMAKE_OPENNI2_LINK_PATHS    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
xmake_set(XMAKE_OPENNI2_LINK_FLAGS    "-lOpenNI2")


if (BUILD_OPENNI2_EXAMPLES)

    file(MAKE_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/OpenNI2/Drivers")
    file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/OpenNI2")


    # List the include paths
    xmake_import_search_paths(OPENNI2)
    include_directories(examples/Common)


    #### examples/libMWClosestPoint

    # List the source files
    set(SRCS examples/MWClosestPoint/MWClosestPoint.cpp
             examples/MWClosestPoint/MWClosestPoint.h
    )

    # Declaration of the library
    xmake_create_dynamic_library(DEMO_OPENNI2_LIBMWCLOSESTPOINT MWClosestPoint "1.0.0" "1.0.0" ${SRCS})

    xmake_project_link(DEMO_OPENNI2_LIBMWCLOSESTPOINT OPENNI2)

    set_target_properties(MWClosestPoint PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Demos/OpenNI2"
                                                    COMPILE_DEFINITIONS "_CLOSEST_POINT"
    )

    if (APPLE)
        xmake_add_to_property(DEMO_OPENNI2_LIBMWCLOSESTPOINT LINK_FLAGS "-Wl,-rpath,@loader_path/../..")
    else()
        xmake_add_to_list_property(DEMO_OPENNI2_LIBMWCLOSESTPOINT INSTALL_RPATH "../..")
    endif()


    #### examples/ClosestPointViewer

    # List the source files
    set(SRCS examples/ClosestPointViewer/main.cpp
             examples/ClosestPointViewer/Viewer.cpp
             examples/ClosestPointViewer/Viewer.h
    )

    # List the include paths
    include_directories(examples/)

    # Declaration of the executable
    xmake_create_executable(DEMO_OPENNI2_CLOSEST_POINT_VIEWER ClosestPointViewer ${SRCS})

    xmake_project_link(DEMO_OPENNI2_CLOSEST_POINT_VIEWER OPENNI2 DEMO_OPENNI2_LIBMWCLOSESTPOINT)

    set_target_properties(ClosestPointViewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/OpenNI2")

    if (APPLE)
        target_link_libraries(ClosestPointViewer "-framework OpenGL -framework GLUT")
        xmake_add_to_property(DEMO_OPENNI2_CLOSEST_POINT_VIEWER LINK_FLAGS "-Wl,-rpath,@loader_path/../..")
    else()
        target_link_libraries(ClosestPointViewer "-lgl -lglut")
        xmake_add_to_list_property(DEMO_OPENNI2_CLOSEST_POINT_VIEWER INSTALL_RPATH "../..")
    endif()


    #### examples/EventBasedRead

    # List the source files
    set(SRCS examples/EventBasedRead/main.cpp)

    # Declaration of the executable
    xmake_create_executable(DEMO_OPENNI2_EVENTBASEDREAD EventBasedRead ${SRCS})

    xmake_project_link(DEMO_OPENNI2_EVENTBASEDREAD OPENNI2)

    set_target_properties(EventBasedRead PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/OpenNI2")

    if (APPLE)
        xmake_add_to_property(DEMO_OPENNI2_EVENTBASEDREAD LINK_FLAGS "-Wl,-rpath,@loader_path/../..")
    else()
        xmake_add_to_list_property(DEMO_OPENNI2_EVENTBASEDREAD INSTALL_RPATH "../..")
    endif()


    #### examples/SimpleRead

    # List the source files
    set(SRCS examples/SimpleRead/main.cpp)

    # Declaration of the executable
    xmake_create_executable(DEMO_OPENNI2_SIMPLE_READ SimpleRead ${SRCS})

    xmake_project_link(DEMO_OPENNI2_SIMPLE_READ OPENNI2)

    set_target_properties(SimpleRead PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/OpenNI2")

    if (APPLE)
        xmake_add_to_property(DEMO_OPENNI2_SIMPLE_READ LINK_FLAGS "-Wl,-rpath,@loader_path/../..")
    else()
        xmake_add_to_list_property(DEMO_OPENNI2_SIMPLE_READ INSTALL_RPATH "../..")
    endif()


    #### examples/SimpleViewer

    # List the source files
    set(SRCS examples/SimpleViewer/main.cpp
             examples/SimpleViewer/Viewer.cpp
             examples/SimpleViewer/Viewer.h
    )

    # Declaration of the executable
    xmake_create_executable(DEMO_OPENNI2_SIMPLE_VIEWER SimpleViewer ${SRCS})

    xmake_project_link(DEMO_OPENNI2_SIMPLE_VIEWER OPENNI2)

    set_target_properties(SimpleViewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/OpenNI2")

    if (APPLE)
        target_link_libraries(SimpleViewer "-framework OpenGL -framework GLUT")
        xmake_add_to_property(DEMO_OPENNI2_SIMPLE_VIEWER LINK_FLAGS "-Wl,-rpath,@loader_path/../..")
    else()
        target_link_libraries(SimpleViewer "-lgl -lglut")
        xmake_add_to_list_property(DEMO_OPENNI2_SIMPLE_VIEWER INSTALL_RPATH "../..")
    endif()
endif()
