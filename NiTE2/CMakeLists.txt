project(NITE2)

# Settings
set(BUILD_NITE2_EXAMPLES OFF CACHE BOOL "Build NiTE2 examples")


# Copy the binaries into the build folder
configure_file(bin/libNiTE2.dylib "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libNiTE2.dylib" COPYONLY)
configure_file(bin/NiTE.ini "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE.ini" COPYONLY)

configure_file(bin/NiTE2/FeatureExtraction.ini "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE2/FeatureExtraction.ini" COPYONLY)
configure_file(bin/NiTE2/HandAlgorithms.ini "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE2/HandAlgorithms.ini" COPYONLY)
configure_file(bin/NiTE2/h.dat "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE2/h.dat" COPYONLY)
configure_file(bin/NiTE2/s.dat "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE2/s.dat" COPYONLY)

configure_file(bin/NiTE2/Data/lbsdata.idx "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE2/Data/lbsdata.idx" COPYONLY)
configure_file(bin/NiTE2/Data/lbsdata.lbd "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE2/Data/lbsdata.lbd" COPYONLY)
configure_file(bin/NiTE2/Data/lbsparam1.lbd "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE2/Data/lbsparam1.lbd" COPYONLY)
configure_file(bin/NiTE2/Data/lbsparam2.lbd "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE2/Data/lbsparam2.lbd" COPYONLY)


# Export the library
xmake_set(XMAKE_NITE2_INCLUDE_PATHS "${NITE2_SOURCE_DIR}/include")
xmake_set(XMAKE_NITE2_LINK_PATHS    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
xmake_set(XMAKE_NITE2_LINK_FLAGS    "-lNITE2")


if (BUILD_NITE2_EXAMPLES)

    file(MAKE_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/NiTE2/Data")
    file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/NiTE2")


    # List the include paths
    xmake_import_search_paths(NITE2)
    xmake_import_search_paths(OPENNI2)
    include_directories(examples/Common)


    #### examples/HandViewer

    # List the source files
    set(SRCS examples/HandViewer/HistoryBuffer.h
             examples/HandViewer/main.cpp
             examples/HandViewer/Viewer.cpp
             examples/HandViewer/Viewer.h
    )

    # Declaration of the executable
    xmake_create_executable(DEMO_NITE2_HANDVIEWER HandViewer ${SRCS})

    xmake_project_link(DEMO_NITE2_HANDVIEWER NITE2 OPENNI2)

    set_target_properties(HandViewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/NiTE2")

    if (APPLE)
        target_link_libraries(HandViewer "-framework OpenGL -framework GLUT")
        xmake_add_to_property(DEMO_NITE2_HANDVIEWER LINK_FLAGS "-Wl,-rpath,@loader_path/../..")
    else()
        target_link_libraries(HandViewer "-lgl -lglut")
        xmake_add_to_list_property(DEMO_NITE2_HANDVIEWER INSTALL_RPATH "../..")
    endif()


    #### examples/SimpleHandTracker

    # List the source files
    set(SRCS examples/SimpleHandTracker/main.cpp)

    # Declaration of the executable
    xmake_create_executable(DEMO_OPENNI2_SIMPLEHANDTRACKER SimpleHandTracker ${SRCS})

    xmake_project_link(DEMO_OPENNI2_SIMPLEHANDTRACKER NITE2 OPENNI2)

    set_target_properties(SimpleHandTracker PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/NiTE2")

    if (APPLE)
        xmake_add_to_property(DEMO_OPENNI2_SIMPLEHANDTRACKER LINK_FLAGS "-Wl,-rpath,@loader_path/../..")
    else()
        xmake_add_to_list_property(DEMO_OPENNI2_SIMPLEHANDTRACKER INSTALL_RPATH "../..")
    endif()


    #### examples/SimpleUserTracker

    # List the source files
    set(SRCS examples/SimpleUserTracker/main.cpp)

    # Declaration of the executable
    xmake_create_executable(DEMO_OPENNI2_SIMPLEUSERTRACKER SimpleUserTracker ${SRCS})

    xmake_project_link(DEMO_OPENNI2_SIMPLEUSERTRACKER NITE2 OPENNI2)

    set_target_properties(SimpleUserTracker PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/NiTE2")

    if (APPLE)
        xmake_add_to_property(DEMO_OPENNI2_SIMPLEUSERTRACKER LINK_FLAGS "-Wl,-rpath,@loader_path/../..")
    else()
        xmake_add_to_list_property(DEMO_OPENNI2_SIMPLEUSERTRACKER INSTALL_RPATH "../..")
    endif()


    #### examples/UserViewer

    # List the source files
    set(SRCS examples/UserViewer/main.cpp
             examples/UserViewer/Viewer.cpp
             examples/UserViewer/Viewer.h
    )

    # Declaration of the executable
    xmake_create_executable(DEMO_NITE2_USERVIEWER UserViewer ${SRCS})

    xmake_project_link(DEMO_NITE2_USERVIEWER NITE2 OPENNI2)

    set_target_properties(UserViewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/NiTE2")

    if (APPLE)
        target_link_libraries(UserViewer "-framework OpenGL -framework GLUT")
        xmake_add_to_property(DEMO_NITE2_USERVIEWER LINK_FLAGS "-Wl,-rpath,@loader_path/../..")
    else()
        target_link_libraries(UserViewer "-lgl -lglut")
        xmake_add_to_list_property(DEMO_NITE2_USERVIEWER INSTALL_RPATH "../..")
    endif()
endif()
