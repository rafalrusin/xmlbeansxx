
PROJECT (xmlbeansxx)

cmake_minimum_required(VERSION 2.4.7)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


if (WIN32)
	set(EXEC_SUF ".bat")
else (WIN32) 
	set(EXEC_SUF "")
endif (WIN32)

IF(MSVC80)
	SET(CMAKE_CXX_WARNING_LEVEL 1)
	IF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		STRING(REGEX REPLACE "/W[0-4]" "/W1"
            	CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   	ELSE(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1")
	ENDIF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
ENDIF(MSVC80)



find_package(Log4cxx REQUIRED)
find_package(Boost REQUIRED)
find_package(Xerces REQUIRED)
find_package(CppUnit REQUIRED)
find_package(Gmp REQUIRED)
find_package(Gmpxx REQUIRED)
include_directories (${XERCES_INCLUDE_DIR} ${LOG4CXX_INCLUDE_DIR})


SUBDIRS (generator src/xmlbeansxx)

