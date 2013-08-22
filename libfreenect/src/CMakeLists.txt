# List the header files
set(HEADERS ../include/libfreenect-registration.h
            ../include/libfreenect.h
            cameras.h
            flags.h
            freenect_internal.h
            registration.h
            usb_libusb10.h
)


# List the source files
set(SRCS cameras.c
         core.c
         flags.c
         registration.c
         tilt.c
         usb_libusb10.c
)


# List the include paths
set(INCLUDE_PATHS "${LIBFREENECT_SOURCE_DIR}/include"
                  "${LIBFREENECT_SOURCE_DIR}/src")

include_directories(${INCLUDE_PATHS})

xmake_import_search_paths(LIBUSB)


# Declaration of the library
xmake_create_dynamic_library(LIBFREENECT freenect "0.1.2" "0.1.2"
                             ${HEADERS} ${SRCS})

xmake_project_link(LIBFREENECT LIBUSB)


# Exportations
xmake_export_include_paths(LIBFREENECT "${LIBFREENECT_SOURCE_DIR}/include")
