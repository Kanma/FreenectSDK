#### fakenect/libfreenect

# List the source files
set(SRCS fakenect.c)

# List the include paths
xmake_import_search_paths(LIBFREENECT)

# Declaration of the library
xmake_create_dynamic_library(LIBFAKENECT fakenect "0.1.2" "0.1.2" ${SRCS})

file(MAKE_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/fakenect")

set_target_properties(fakenect PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/fakenect"
                                          OUTPUT_NAME "freenect")

#### fakenect-record

# List the source files
set(SRCS record.c)

# List the include paths
xmake_import_search_paths(LIBFREENECT)

# Declaration of the library
xmake_create_executable(TOOL_RECORD fakenect-record ${SRCS})

xmake_project_link(TOOL_RECORD LIBFREENECT)


#### fakenect.sh

configure_file(fakenect.sh "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/fakenect.sh" COPYONLY)
