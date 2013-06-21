project(LIBUSB)

# List the header files
set(HEADERS libusb.h
            libusbi.h
)

if (APPLE)
    set(PLATFORM_HEADERS os/darwin_usb.h
                         os/poll_posix.h
                         os/threads_posix.h
    )
elseif (WIN32)
    set(PLATFORM_HEADERS os/windows_usb.h
                         os/poll_windows.h
                         os/threads_windows.h
    )
else()
    set(PLATFORM_HEADERS os/linux_usbfs.h
                         os/poll_posix.h
                         os/threads_posix.h
    )
endif()


# List the source files
set(SRCS core.c
         descriptor.c
         io.c
         sync.c
)

if (APPLE)
    set(PLATFORM_SRCS os/darwin_usb.c
    )
elseif (WIN32)
    set(PLATFORM_SRCS os/windows_usb.c
                      os/poll_windows.c
                      os/threads_windows.c
    )
else()
    set(PLATFORM_SRCS os/linux_usbfs.c
    )
endif()


# List the include paths
# set(INCLUDE_PATHS "${LIBUSB_SOURCE_DIR}")
# 
include_directories(${LIBUSB_SOURCE_DIR})

# Declaration of the library
xmake_create_dynamic_library(LIBUSB usb "1.0.8" "1.0.8"
                             ${HEADERS} ${PLATFORM_HEADERS} ${SRCS} ${PLATFORM_SRCS})

if (APPLE)
    target_link_libraries(usb "-framework IOKit -framework CoreFoundation")
endif()

# Exportations
xmake_export_include_paths(LIBUSB "${LIBUSB_SOURCE_DIR}/.." "${LIBUSB_SOURCE_DIR}")
