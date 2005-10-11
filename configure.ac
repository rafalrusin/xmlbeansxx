# touk/wfm/configure.ac,v 1.34 2004/06/29 08:24:11 trombka Exp
# V_1_1
AC_PREREQ(2.57)

AC_INIT(XMLBEANSXX, 0.7.3, support@TouK.pl)
#Before release, change library versions 
#Read http://www.gnu.org/software/libtool/manual.html#SEC32 before doing this
AC_SUBST(library_version_info, 2:1:0)


AC_CONFIG_AUX_DIR([config])
#AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([1.7])

AC_PROG_CC
AC_PROG_CXX
AC_LANG(C++)

AC_LIB_LTDL
AC_LIBLTDL_CONVENIENCE
AC_SUBST(LTDLINCL)
AC_SUBST(LIBLTDL)
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

ACX_PTHREAD([
		LIBS="$PTHREAD_LIBS $LIBS"
		CPPFLAGS="$CPPFLAGS $PTHREAD_CFLAGS"
		CC="$PTHREAD_CC"
	],[
		AC_MSG_WARN([Compiling without threads support])
	])

AC_ARG_WITH(xmlbeansxx-gen, 
			[  --with-xmlbeansxx-gen=DIR Specify xmlbeansxx-gen distribution directory (dist) ],
			[
				xmlbeansxx_gen_dir="$withval"
			],
			[ 
             xmlbeansxx_gen_dir="$PWD/generator"
             #AC_MSG_ERROR([xmlbeansxx-gen distribution directory (dist) not specified. Please use --with-xmlbeansxx-gen.])
			])

m4_ifdef([AM_PATH_CPPUNIT],[
    AM_PATH_CPPUNIT(1.8.0)
    AM_CONDITIONAL(HAVE_CPPUNIT, test "$CPPUNIT_LIBS")
])

BB_ENABLE_DOXYGEN

AC_SUBST(ac_aux_dir)
AC_SUBST(xmlbeansxx_gen_dir)

#AC_CHECK_FILE($xmlbeansxx_gen_dir/bin/xmlbeansxx-gen, , AC_MSG_ERROR([File $xmlbeansxx_gen_dir/bin/xmlbeansxx-gen not found]))
AC_CHECK_LIB(xerces-c,main,,AC_MSG_ERROR([xerces-c library not found. Please install it.]))
AC_CHECK_LIB(log4cxx,main,,AC_MSG_ERROR([log4cxx library not found. Please install it.]))
AC_CHECK_LIB(gmp,main,,AC_MSG_ERROR([gmp library not found. Please install it.]))
AC_CHECK_LIB(gmpxx,main,,AC_MSG_ERROR([gmpxx library not found. Please install gmp with gmpxx option enabled.]))
AC_CHECK_LIB(boost_date_time,main,,AC_MSG_ERROR([boost_date_time library not found. Please install it.]))
AC_CHECK_LIB(boost_thread,main,,AC_MSG_ERROR([boost_thread library not found. Please install it.]))
AC_CHECK_HEADERS([boost/shared_ptr.hpp boost/shared_array.hpp boost/date_time/gregorian/gregorian.hpp],,AC_MSG_ERROR([boost library headers not found. Please install boost library.]))

AC_CONFIG_FILES([Makefile generator/Makefile src/Makefile src/xmlbeansxx/Makefile tests/Makefile examples/Makefile examples/small/Makefile doc/Makefile doc/Doxyfile])

AC_OUTPUT
