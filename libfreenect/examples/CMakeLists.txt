# List the include paths
xmake_import_search_paths(LIBFREENECT)


#### glpclview

# List the source files
set(SRCS glpclview.c)

# Declaration of the library
xmake_create_executable(DEMO_GLPCLVIEW glpclview ${SRCS})

xmake_project_link(DEMO_GLPCLVIEW LIBFREENECT)

if (APPLE)
    target_link_libraries(glpclview "-framework OpenGL -framework GLUT")
else()
    target_link_libraries(glpclview "-lgl -lglut")
endif()


#### glview

# List the source files
set(SRCS glview.c)

# Declaration of the library
xmake_create_executable(DEMO_GLVIEW glview ${SRCS})

xmake_project_link(DEMO_GLVIEW LIBFREENECT)

if (APPLE)
    target_link_libraries(glview "-framework OpenGL -framework GLUT")
else()
    target_link_libraries(glview "-lgl -lglut")
endif()


#### hiview

# List the source files
set(SRCS hiview.c)

# Declaration of the library
xmake_create_executable(DEMO_HIVIEW hiview ${SRCS})

xmake_project_link(DEMO_HIVIEW LIBFREENECT)

if (APPLE)
    target_link_libraries(hiview "-framework OpenGL -framework GLUT")
else()
    target_link_libraries(hiview "-lgl -lglut")
endif()


#### regview

# List the source files
set(SRCS regview.c)

# Declaration of the library
xmake_create_executable(DEMO_REGVIEW regview ${SRCS})

xmake_project_link(DEMO_REGVIEW LIBFREENECT)

if (APPLE)
    target_link_libraries(regview "-framework OpenGL -framework GLUT")
else()
    target_link_libraries(regview "-lgl -lglut")
endif()


#### tiltdemo

# List the source files
set(SRCS tiltdemo.c)

# Declaration of the library
xmake_create_executable(DEMO_TILTDEMO tiltdemo ${SRCS})

xmake_project_link(DEMO_TILTDEMO LIBFREENECT)
