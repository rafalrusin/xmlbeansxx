# touk/wfm/configure.ac,v 1.34 2004/06/29 08:24:11 trombka Exp
# V_1_1
AC_PREREQ(2.57)

AC_INIT(XMLBEANSXX, 0.6.5, support@TouK.pl)
#Before release, change library versions 
#Read http://www.gnu.org/software/libtool/manual.html#SEC32 before doing this
AC_SUBST(library_version_info, 1:3:0)


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
if test "$XML2_CONFIG" = "no" ; then
    AC_CHECK_LIB([xml2], [main])
    AC_CHECK_HEADER([libxml/xmlversion.h])
else
    CPPFLAGS="$CPPFLAGS `$XML2_CONFIG --cflags`"
    LIBS="$LIBS `$XML2_CONFIG --libs`"
fi

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

#BB_ENABLE_DOXYGEN

AC_CHECK_LIB(log4cxx,main,,AC_MSG_WARN([log4cxx library not found.]))
AC_CHECK_LIB(gmp,main,,AC_MSG_ERROR([gmp library not found. Please install it.]))
AC_CHECK_LIB(gmpxx,main,,AC_MSG_ERROR([gmpxx library not found. Please install gmp with gmpxx option enabled.]))
AC_CHECK_LIB(boost_date_time,main,,AC_MSG_ERROR([boost_date_time library not found. Please install it.]))
AC_CHECK_LIB(boost_thread,main,,AC_MSG_ERROR([boost_thread library not found. Please install it.]))
AC_CHECK_LIB(xml2,main,,AC_MSG_ERROR([xml2 library not found. Please install it.]))
AC_CHECK_HEADERS([boost/shared_ptr.hpp boost/shared_array.hpp boost/date_time/gregorian/gregorian.hpp],,AC_MSG_ERROR([boost library headers not found. Please install boost library.]))

AC_SUBST(ac_aux_dir)
AC_SUBST(xmlbeansxx_gen_dir)

AC_CONFIG_FILES([Makefile generator/Makefile src/Makefile src/xmlbeansxx/Makefile tests/Makefile examples/Makefile examples/small/Makefile])

AC_OUTPUT
