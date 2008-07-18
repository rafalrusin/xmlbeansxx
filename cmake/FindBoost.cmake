# - Try to find Boost
# Once done this will define
#
#  BOOST_FOUND - System has Boost
#  BOOST_INCLUDE_DIRS - Boost include directory
#  BOOST_LIBRARIES - Link these to use Boost
#  BOOST_LIBRARY_DIRS - The path to where the Boost library files are.
#  BOOST_DEFINITIONS - Compiler switches required for using Boost
#  BOOST_LIBRARIES_SUFFIX - Boost libraries name suffix (e.g -vc71-mt-gd-1_34, -gcc41-mt...)
#
#  BOOST_DATE_TIME_FOUND               True if Boost Date Time was found.
#  BOOST_FILESYSTEM_FOUND              True if Boost Filesystem was found.
#  BOOST_IOSTREAMS_FOUND               True if Boost Iostream was found.
#  BOOST_PRG_EXEC_MONITOR_FOUND        True if Boost Program Exec Monitor was found.
#  BOOST_PROGRAM_OPTIONS_FOUND         True if Boost Program Options was found.
#  BOOST_PYTHON_FOUND                  True if Boost Python was found.
#  BOOST_REGEX_FOUND                   True if Boost Regex was found.
#  BOOST_SERIALIZATION_FOUND           True if Boost Serialization was found.
#  BOOST_SIGNALS_FOUND                 True if Boost Signals was found.
#  BOOST_TEST_EXEC_MONITOR_FOUND       True if Boost Test Exec Monitor was found.
#  BOOST_THREAD-MT_FOUND               True if Boost Thread was found.
#  BOOST_UNIT_TEST_FRAMEWORK_FOUND     True if Boost Unit Test Framework was found.
#  BOOST_WSERIALIZATION_FOUND          True if Boost WSerialization was found.
#
#  BOOST_DATE_TIME_LIBRARY             The Boost Date Time libary.
#  BOOST_FILESYSTEM_LIBRARY            The Boost Filesystem libary.
#  BOOST_IOSTREAMS_LIBRARY             The Boost Iostream libary.
#  BOOST_PRG_EXEC_MONITOR_LIBRARY      The Boost Program libary.
#  BOOST_PROGRAM_OPTIONS_LIBRARY       The Boost Program libary.
#  BOOST_PYTHON_LIBRARY                The Boost Python libary.
#  BOOST_REGEX_LIBRARY                 The Boost Regex libary.
#  BOOST_SERIALIZATION_LIBRARY         The Boost Serialization libary.
#  BOOST_SIGNALS_LIBRARY               The Boost Signals libary.
#  BOOST_TEST_EXEC_MONITOR_LIBRARY     The Boost Test Exec Monitor libary.
#  BOOST_THREAD_LIBRARY                The Boost Thread libary.
#  BOOST_UNIT_TEST_FRAMEWORK_LIBRARY   The Boost Unit Test Framework libary.
#  BOOST_WSERIALIZATION_LIBRARY        The Boost WSerialization libary.
#
#  Copyright (c) 2006 Andreas Schneider <mail@cynapses.org>
#  Copyright (c) 2007 Wengo
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
#IF(MSVC80)
#	SET(BOOST_INCLUDE_DIR "C:/include")
#	#	SET(BOOST_LIBRARIES "boost_date_time boost_regex")
#	SET(BOOST_LIBRARIES_SUFFIX -vc80-mt-gd-1_34_1)
#	SET(BOOST_DATE_TIME_LIBRARY "C:/libboost_date_time-vc80-mt-gd-1_34_1.lib")
#	SET(BOOST_REGEX_LIBRARY "C:/libboost_regex-vc80-mt-gd-1_34_1.lib")
#ENDIF(MSVC80)

if (BOOST_LIBRARIES AND BOOST_INCLUDE_DIRS)
  # in cache already
  set(BOOST_FOUND TRUE)
else (BOOST_LIBRARIES AND BOOST_INCLUDE_DIRS)
  # Add in some path suffixes. These will have to be updated whenever
  # a new Boost version comes out.
  set(BOOST_PATH_SUFFIX
    boost-1_35_0
    boost-1_35
    boost-1_34_1
    boost-1_34_0
    boost-1_34
    boost-1_33_1
    boost-1_33_0
    boost-1_33
  )

  SET(BOOST_PREFIX "")
  if (WIN32)
    SET(BOOST_PREFIX "lib")
    # In windows, automatic linking is performed, so you do not have to specify the libraries.
    # If you are linking to a dynamic runtime, then you can choose to link to either a static or a
    # dynamic Boost library, the default is to do a static link.  You can alter this for a specific
    # library "whatever" by defining BOOST_WHATEVER_DYN_LINK to force Boost library "whatever" to
    # be linked dynamically.  Alternatively you can force all Boost libraries to dynamic link by
    # defining BOOST_ALL_DYN_LINK.

    # This feature can be disabled for Boost library "whatever" by defining BOOST_WHATEVER_NO_LIB,
    # or for all of Boost by defining BOOST_ALL_NO_LIB.

    # If you want to observe which libraries are being linked against then defining
    # BOOST_LIB_DIAGNOSTIC will cause the auto-linking code to emit a #pragma message each time
    # a library is selected for linking.
    set(BOOST_LIB_DIAGNOSTIC_DEFINITIONS "-DBOOST_LIB_DIAGNOSTIC")

    set(BOOST_INCLUDE_SEARCH_DIRS
      $ENV{BOOSTINCLUDEDIR}
      $ENV{BOOST_ROOT}/include
      C:/boost/include
      "C:/Program Files/boost/boost_1_35_0"
      "C:/Program Files/boost/boost_1_34_0"
      "C:/Program Files/boost/boost_1_33_1"
      # D: is very often the cdrom drive, if you don't have a
      # cdrom inserted it will popup a very annoying dialog
      #D:/boost/include
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
    )

    set(BOOST_LIBRARIES_SEARCH_DIRS
      $ENV{BOOSTLIBDIR}
      $ENV{BOOST_ROOT}/lib
      C:/boost/lib
      "C:/Program Files/boost/boost_1_35_0/lib"
      "C:/Program Files/boost/boost_1_34_0/lib"
      "C:/Program Files/boost/boost_1_33_1/lib"
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
    )

    if (MSVC71)
      if (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(BOOST_LIBRARIES_SUFFIXES
          -vc71-mt-gd-1_35
          -vc71-mt-gd-1_34
          -vc71-mt-gd-1_33_1
        )
      else (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(BOOST_LIBRARIES_SUFFIXES
          -vc71-mt-1_35
          -vc71-mt-1_34
          -vc71-mt-1_33_1
        )
      endif (CMAKE_BUILD_TYPE STREQUAL Debug)
    endif (MSVC71)

    if (MSVC80)
      if (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(BOOST_LIBRARIES_SUFFIXES
          -vc80-mt-gd-1_35
          -vc80-mt-gd-1_34_1
          -vc80-mt-gd-1_33_1
        )
      else (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(BOOST_LIBRARIES_SUFFIXES
          -vc80-mt-1_35
          -vc80-mt-1_34_1
          -vc80-mt-1_33_1
        )
      endif (CMAKE_BUILD_TYPE STREQUAL Debug)
    endif (MSVC80)

    if (MINGW)
      if (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(BOOST_LIBRARIES_SUFFIXES
          -mgw-mt-d
        )
      else (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(BOOST_LIBRARIES_SUFFIXES
          -mgw-mt
        )
      endif (CMAKE_BUILD_TYPE STREQUAL Debug)
    endif (MINGW)

    if (CYGWIN)
      if (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(BOOST_LIBRARIES_SUFFIXES
          -gcc-mt-d
        )
      else (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(BOOST_LIBRARIES_SUFFIXES
          -gcc-mt
        )
      endif (CMAKE_BUILD_TYPE STREQUAL Debug)
    endif (CYGWIN)

  else (WIN32)
    set(BOOST_LIBRARIES_SUFFIXES
        -gcc-mt
        -gcc41-mt
    )
  endif (WIN32)

  find_path(BOOST_INCLUDE_DIR
    NAMES
      boost/config.hpp
    PATHS
      ${BOOST_INCLUDE_SEARCH_DIRS}
    PATH_SUFFIXES
      ${BOOST_PATH_SUFFIX}
  )

#message(STATUS "trying paths ${BOOST_LIBRARIES_SEARCH_DIRS}")


  foreach (TMP_BOOST_LIBRARIES_SUFFIX "" ${BOOST_LIBRARIES_SUFFIXES})
#	message(STATUS "trying  boost_date_time${TMP_BOOST_LIBRARIES_SUFFIX}")

    if (NOT BOOST_DATE_TIME_LIBRARY)
      find_library(BOOST_DATE_TIME_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_date_time${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )

      if (BOOST_DATE_TIME_LIBRARY)
        # BOOST_DATE_TIME_LIBRARY was found
        # sets the libraries suffix, this code is ugly
        # but CMake does not allow to break a loop :/
        set(BOOST_LIBRARIES_SUFFIX
          ${TMP_BOOST_LIBRARIES_SUFFIX}
          CACHE INTERNAL "" FORCE
        )
      endif (BOOST_DATE_TIME_LIBRARY)

    endif (NOT BOOST_DATE_TIME_LIBRARY)

    if (NOT BOOST_FILESYSTEM_LIBRARY)
      find_library(BOOST_FILESYSTEM_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_filesystem${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_FILESYSTEM_LIBRARY)

    if (NOT BOOST_IOSTREAMS_LIBRARY)
      find_library(BOOST_IOSTREAMS_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_iostreams${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_IOSTREAMS_LIBRARY)

    if (NOT BOOST_PRG_EXEC_MONITOR_LIBRARY)
      find_library(BOOST_PRG_EXEC_MONITOR_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_prg_exec_monitor${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_PRG_EXEC_MONITOR_LIBRARY)

    if (NOT BOOST_PROGRAM_OPTIONS_LIBRARY)
      find_library(BOOST_PROGRAM_OPTIONS_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_program_options${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_PROGRAM_OPTIONS_LIBRARY)

    if (NOT BOOST_PYTHON_LIBRARY)
      find_library(BOOST_PYTHON_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_python${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_PYTHON_LIBRARY)

    if (NOT BOOST_REGEX_LIBRARY)
      find_library(BOOST_REGEX_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_regex${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_REGEX_LIBRARY)

    if (NOT BOOST_SERIALIZATION_LIBRARY)
      find_library(BOOST_SERIALIZATION_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_serialization${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_SERIALIZATION_LIBRARY)

    if (NOT BOOST_SIGNALS_LIBRARY)
      find_library(BOOST_SIGNALS_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_signals${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_SIGNALS_LIBRARY)

    if (NOT BOOST_TEST_EXEC_MONITOR_LIBRARY)
      find_library(BOOST_TEST_EXEC_MONITOR_LIBRARY
        NAMES
	  ${BOOST_PREFIX}boost_test_exec_monitor${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_TEST_EXEC_MONITOR_LIBRARY)

    if (NOT BOOST_THREAD_LIBRARY)
      find_library(BOOST_THREAD_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_thread${TMP_BOOST_LIBRARIES_SUFFIX}
          ${BOOST_PREFIX}boost_thread-mt
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_THREAD_LIBRARY)

    if (NOT BOOST_UNIT_TEST_FRAMEWORK_LIBRARY)
      find_library(BOOST_UNIT_TEST_FRAMEWORK_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_unit_test_framework${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_UNIT_TEST_FRAMEWORK_LIBRARY)

    if (NOT BOOST_WSERIALIZATION_LIBRARY)
      find_library(BOOST_WSERIALIZATION_LIBRARY
        NAMES
          ${BOOST_PREFIX}boost_wserialization${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    endif (NOT BOOST_WSERIALIZATION_LIBRARY)

    if (BOOST_DATE_TIME_LIBRARY)
      set(BOOST_DATE_TIME_FOUND TRUE)
    endif (BOOST_DATE_TIME_LIBRARY)
    if (BOOST_FILESYSTEM_LIBRARY)
      set(BOOST_FILESYSTEM_FOUND TRUE)
    endif (BOOST_FILESYSTEM_LIBRARY)
    if (BOOST_IOSTREAMS_LIBRARY)
      set(BOOST_IOSTREAMS_FOUND TRUE)
    endif (BOOST_IOSTREAMS_LIBRARY)
    if (BOOST_PRG_EXEC_MONITOR_LIBRARY)
      set(BOOST_PRG_EXEC_MONITOR_FOUND TRUE)
    endif (BOOST_PRG_EXEC_MONITOR_LIBRARY)
    if (BOOST_PROGRAM_OPTIONS_LIBRARY)
      set(BOOST_PROGRAM_OPTIONS_FOUND TRUE)
    endif (BOOST_PROGRAM_OPTIONS_LIBRARY)
    if (BOOST_PYTHON_LIBRARY)
      set(BOOST_PYTHON_FOUND TRUE)
    endif (BOOST_PYTHON_LIBRARY)
    if (BOOST_REGEX_LIBRARY)
      set(BOOST_REGEX_FOUND TRUE)
    endif (BOOST_REGEX_LIBRARY)
    if (BOOST_SERIALIZATION_LIBRARY)
      set(BOOST_SERIALIZATION_FOUND TRUE)
    endif (BOOST_SERIALIZATION_LIBRARY)
    if (BOOST_SIGNALS_LIBRARY)
      set(BOOST_SIGNALS_FOUND TRUE)
    endif (BOOST_SIGNALS_LIBRARY)
    if (BOOST_TEST_EXEC_MONITOR_LIBRARY)
      set(BOOST_TEST_EXEC_MONITOR_FOUND TRUE)
    endif (BOOST_TEST_EXEC_MONITOR_LIBRARY)
    if (BOOST_THREAD_LIBRARY)
      set(BOOST_THREAD-MT_FOUND TRUE)
    endif (BOOST_THREAD_LIBRARY)
    if (BOOST_UNIT_TEST_FRAMEWORK_LIBRARY)
      set(BOOST_UNIT_TEST_FRAMEWORK_FOUND TRUE)
    endif (BOOST_UNIT_TEST_FRAMEWORK_LIBRARY)
    if (BOOST_WSERIALIZATION_LIBRARY)
      set(BOOST_WSERIALIZATION_FOUND TRUE)
    endif (BOOST_WSERIALIZATION_LIBRARY)

  endforeach (TMP_BOOST_LIBRARIES_SUFFIX)

  set(BOOST_INCLUDE_DIRS
    ${BOOST_INCLUDE_DIR}
  )

  if (BOOST_DATE_TIME_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_DATE_TIME_LIBRARY}
    )
  endif (BOOST_DATE_TIME_FOUND)
  if (BOOST_FILESYSTEM_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_FILESYSTEM_LIBRARY}
    )
  endif (BOOST_FILESYSTEM_FOUND)
  if (BOOST_IOSTREAMS_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_IOSTREAMS_LIBRARY}
    )
  endif (BOOST_IOSTREAMS_FOUND)
  if (BOOST_PRG_EXEC_MONITOR_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_PRG_EXEC_MONITOR_LIBRARY}
    )
  endif (BOOST_PRG_EXEC_MONITOR_FOUND)
  if (BOOST_PROGRAM_OPTIONS_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_PROGRAM_OPTIONS_LIBRARY}
    )
  endif (BOOST_PROGRAM_OPTIONS_FOUND)
  if (BOOST_PYTHON_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_PYTHON_LIBRARY}
    )
  endif (BOOST_PYTHON_FOUND)
  if (BOOST_REGEX_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_REGEX_LIBRARY}
    )
  endif (BOOST_REGEX_FOUND)
  if (BOOST_SERIALIZATION_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_SERIALIZATION_LIBRARY}
    )
  endif (BOOST_SERIALIZATION_FOUND)
  if (BOOST_SIGNALS_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_SIGNALS_LIBRARY}
    )
  endif (BOOST_SIGNALS_FOUND)
  if (BOOST_TEST_EXEC_MONITOR_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_TEST_EXEC_MONITOR_LIBRARY}
    )
  endif (BOOST_TEST_EXEC_MONITOR_FOUND)
  if (BOOST_THREAD-MT_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_THREAD_LIBRARY}
    )
  endif (BOOST_THREAD-MT_FOUND)
  if (BOOST_UNIT_TEST_FRAMEWORK_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
  endif (BOOST_UNIT_TEST_FRAMEWORK_FOUND)
  if (BOOST_WSERIALIZATION_FOUND)
    set(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_WSERIALIZATION_LIBRARY}
    )
  endif (BOOST_WSERIALIZATION_FOUND)

  if (BOOST_INCLUDE_DIRS AND BOOST_LIBRARIES)
    set(BOOST_FOUND TRUE)
  endif (BOOST_INCLUDE_DIRS AND BOOST_LIBRARIES)

  if (BOOST_FOUND)
    if (NOT Boost_FIND_QUIETLY)
      message(STATUS "Found Boost: ${BOOST_LIBRARIES}")
    endif (NOT Boost_FIND_QUIETLY)
  else (BOOST_FOUND)
    if (Boost_FIND_REQUIRED)
      message(FATAL_ERROR "Please install the Boost libraries and development packages")
    endif (Boost_FIND_REQUIRED)
  endif (BOOST_FOUND)

  foreach (BOOST_LIBDIR ${BOOST_LIBRARIES})
    get_filename_component(BOOST_LIBRARY_DIRS ${BOOST_LIBDIR} PATH)
  endforeach (BOOST_LIBDIR ${BOOST_LIBRARIES})

  # Under Windows, automatic linking is performed, so no need to specify the libraries.
  if (WIN32)
    set(BOOST_LIBRARIES "")
  endif (WIN32)

  # show the BOOST_INCLUDE_DIRS and BOOST_LIBRARIES variables only in the advanced view
  mark_as_advanced(BOOST_INCLUDE_DIRS BOOST_LIBRARIES BOOST_LIBRARY_DIRS BOOST_DEFINITIONS BOOST_LIBRARIES_SUFFIX)

endif (BOOST_LIBRARIES AND BOOST_INCLUDE_DIRS)
