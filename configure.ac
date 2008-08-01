# touk/wfm/configure.ac,v 1.34 2004/06/29 08:24:11 trombka Exp
# V_1_1
AC_PREREQ(2.57)

AC_INIT(XMLBEANSXX, 0.9.7, support@TouK.pl)
#Before release, change library versions 
#Read http://www.gnu.org/software/libtool/manual.html#SEC32 before doing this
AC_SUBST(library_version_info, 5:2:0)


AC_CONFIG_AUX_DIR([config])
#AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER([src/xmlbeansxx/config_auto.h])

AM_INIT_AUTOMAKE([1.7])

AC_PROG_CC
AC_PROG_CXX
AC_LANG(C++)

AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

AC_PATH_PROG(XML2_CONFIG, xml2-config, no)

ACX_PTHREAD([
		LIBS="$PTHREAD_LIBS $LIBS"
		CPPFLAGS="$CPPFLAGS $PTHREAD_CFLAGS"
		CC="$PTHREAD_CC"
	],[
		AC_MSG_WARN([Compiling without threads support])
	])


m4_ifdef([AM_PATH_CPPUNIT],[
    AM_PATH_CPPUNIT(1.8.0)
    AM_CONDITIONAL(HAVE_CPPUNIT, test "$CPPUNIT_LIBS")
])

BB_ENABLE_DOXYGEN

AC_ARG_WITH(xmlbeansxx-gen, 	[AS_HELP_STRING([--with-xmlbeansxx-gen=DIR],[Specify xmlbeansxx-gen distribution directory (dist)])],
				[ xmlbeansxx_gen_dir="$withval"],
				[ xmlbeansxx_gen_dir="$PWD/generator" ])
AC_ARG_ENABLE(gmpxx,		[AS_HELP_STRING([--disable-gmpxx],[disable support for gmpxx (default is enabled) ])],
				[],
				[enable_gmpxx=yes])
AC_ARG_ENABLE(log4cxx,		[AS_HELP_STRING([--disable-log4cxx],[disable support for log4cxx (default is enabled) ])],
				[],
				[enable_log4cxx=yes])
AC_ARG_ENABLE(libxml2,		[AS_HELP_STRING([--disable-libxml2],[disable support for libxml2 (default is enabled) ])],
				[],
				[enable_libxml2=yes])



AS_IF([test "x$enable_log4cxx" != xno],	[AC_CHECK_LIB(log4cxx,main,,AC_MSG_ERROR([log4cxx library not found. Please install it.]))],[])
AS_IF([test "x$enable_gmpxx"   != xno],	[AC_CHECK_LIB(gmp,main,,AC_MSG_ERROR([gmp library not found. Please install it.]),[])
					 AC_CHECK_LIB(gmpxx,main,,AC_MSG_ERROR([gmpxx library not found. Please install gmp with gmpxx option enabled.]))],[])

AS_IF([test "x$enable_libxml2" != xno],[
		AS_IF([test "x$XML2_CONFIG" = "xno"], 
					[
					 AC_CHECK_LIB([xml2], [main])
    					 AC_CHECK_HEADER([libxml/xmlversion.h])],
					[
					 CPPFLAGS="$CPPFLAGS `$XML2_CONFIG --cflags`"
					 LIBS="$LIBS `$XML2_CONFIG --libs`"
					])],[])




AC_CHECK_LIB(xerces-c,main,,AC_MSG_ERROR([xerces-c library not found. Please install it.]))
AC_CHECK_LIB(boost_date_time,main,,AC_MSG_ERROR([boost_date_time library not found. Please install it.]))
AC_CHECK_LIB(boost_thread,main,,AC_MSG_ERROR([boost_thread library not found. Please install it.]))
AC_CHECK_HEADERS([boost/shared_ptr.hpp boost/shared_array.hpp boost/date_time/gregorian/gregorian.hpp],,AC_MSG_ERROR([boost library headers not found. Please install boost library.]))

AC_PATH_PROG(MVN, mvn, [not_found])
AS_IF([test "x$MVN" == xnot_found], [AC_MSG_ERROR([mvn not found. Please install it.])],[])

AC_SUBST(MVN)
AC_SUBST(ac_aux_dir)
AC_SUBST(xmlbeansxx_gen_dir)
AC_SUBST(enable_gmpxx)
AC_SUBST(enable_libxml2)
AC_SUBST(enable_log4cxx)

AC_CONFIG_FILES([Makefile generator/Makefile src/Makefile src/xmlbeansxx/Makefile tests/Makefile examples/Makefile examples/small/Makefile examples/geoip/Makefile docs/Doxyfile docs/Makefile cmake/Makefile])

AC_OUTPUT
