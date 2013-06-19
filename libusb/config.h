#ifdef __APPLE_CC__
    #define OS_DARWIN
    #define THREADS_POSIX
    #define DEFAULT_VISIBILITY __attribute__((visibility("default")))
    #define USBI_OS_HANDLES_TIMEOUT
#elif defined(__WIN32__)
    #define OS_WINDOWS
    #define DEFAULT_VISIBILITY
#else
    #define OS_LINUX
    #define THREADS_POSIX
    #define DEFAULT_VISIBILITY __attribute__((visibility("default")))
#endif
