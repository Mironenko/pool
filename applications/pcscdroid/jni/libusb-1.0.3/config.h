/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */
#pragma once
/* Default visibility */
// #define API_EXPORTED __attribute__((visibility("default")))
#define LIBUSB_CALL
#define DEFAULT_VISIBILITY /**/
#define API_EXPORTED LIBUSB_CALL DEFAULT_VISIBILITY

#define POLLIN      0x0001    /* There is data to read */
#define POLLPRI     0x0002    /* There is urgent data to read */
#define POLLOUT     0x0004    /* Writing now will not block */
#define POLLERR     0x0008    /* Error condition */
#define POLLHUP     0x0010    /* Hung up */
#define POLLNVAL    0x0020    /* Invalid request: fd not open */

// access modes
enum rw_type {
	RW_NONE,
	RW_READ,
	RW_WRITE,
};

/* Debug message logging */
/* #undef ENABLE_DEBUG_LOGGING */

/* Message logging */
#define ENABLE_LOGGING 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `rt' library (-lrt). */
#define HAVE_LIBRT 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Darwin backend */
/* #undef OS_DARWIN */

/* Linux backend */
#define OS_LINUX /**/

/* Name of package */
#define PACKAGE "libusb"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "libusb"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libusb 1.0.3"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libusb"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.0.3"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Backend handles timeout */
/* #undef USBI_OS_HANDLES_TIMEOUT */

/* Version number of package */
#define VERSION "1.0.3"

/* Use GNU extensions */
#define _GNU_SOURCE /**/

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif
