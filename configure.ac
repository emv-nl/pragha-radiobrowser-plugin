AC_PREREQ([2.64])
AC_INIT([pragha radiobrowser plugin], [0.0.1], [nospam@nospam.com], [pragha-radiobrowser-plugin], [https://github.com/emv-nl/pragha-radiobrowser-plugin])
AC_CONFIG_SRCDIR([src/pragha-sample-plugin.c])
AC_CONFIG_HEADERS([config.h])
LT_INIT

AM_INIT_AUTOMAKE([no-dist-gzip dist-bzip2])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AM_PROG_LIBTOOL

# Checks for libraries.
PKG_CHECK_MODULES(PRAGHA_PLUGINS, libpragha > 1.3.99)

AC_DEFINE([HAVE_LIBPEAS], [1], ["Libpeas-1.0 is present"])
AC_DEFINE([HAVE_LIBPEAS_GTK], [1], ["Libpeas-gtk-1.0 is present "])

# Checks for header files.
AC_CHECK_HEADERS([ctype.h stdlib.h string.h stdint.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT

echo "
  Configuration info for ${PACKAGE} ${VERSION}
    Prefix: ${prefix}
    Libbir: ${libdir}
"
