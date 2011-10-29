IF (WIN32)
        SET(EXEC_SUF ".bat")
        SET(LIB_SUF "")
        IF (CMAKE_BUILD_TYPE STREQUAL Debug)
                SET(LIB_SUF "d")
        ENDIF (CMAKE_BUILD_TYPE STREQUAL Debug)
ELSE (WIN32)
        SET(EXEC_SUF "")
        SET(LIB_SUF "")
ENDIF (WIN32)


FIND_PATH(XMLBEANSXX_INCLUDE_DIR xmlbeansxx/xmlbeansxx.h /usr/include /usr/local/include)
FIND_LIBRARY(XMLBEANSXX_LIBRARY NAMES xmlbeansxx${LIB_SUF} PATHS /usr/lib /usr/local/lib)
FIND_PROGRAM(XMLBEANSXX_GEN_PROG xmlbeansxx-gen${EXEC_SUF} PATHS /usr /usr/local PATH_SUFFIXES xmlbeansxx/bin share/xmlbeansxx/bin xmlbeansxx/share/xmlbeansxx/bin)

IF( NOT XMLBEANSXX_GEN_PROG )
   MESSAGE(STATUS "xmlbeansxx-gen program not found. Install it if you want to generate source files.")
ELSE( NOT XMLBEANSXX_GEN_PROG )
# xmlbeansxx  macros
	MACRO(XMLBEANSXX_GEN XSD SRC_C SRC_H)
        SET(${XSD}_SRC ${CMAKE_CURRENT_BINARY_DIR}/${SRC_H} ${CMAKE_CURRENT_BINARY_DIR}/${SRC_C})
        SET(${XSD}_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${SRC_H})
        ADD_CUSTOM_COMMAND(OUTPUT ${${XSD}_SRC}
                COMMAND "${XMLBEANSXX_GEN_PROG}" "\"${CMAKE_CURRENT_SOURCE_DIR}/${XSD}\""
                DEPENDS ${XMLBEANSXX_GEN_PROG} ${XSD}
        )
	ENDMACRO(XMLBEANSXX_GEN)

ENDIF(NOT XMLBEANSXX_GEN_PROG )


IF (XMLBEANSXX_INCLUDE_DIR AND XMLBEANSXX_LIBRARY)
   SET(XMLBEANSXX_FOUND TRUE)
ENDIF (XMLBEANSXX_INCLUDE_DIR AND XMLBEANSXX_LIBRARY)

IF (XMLBEANSXX_FOUND)
   IF (NOT XMLBEANSXX_FIND_QUIETLY)
      MESSAGE(STATUS "Found XMLBEANSXX: ${XMLBEANSXX_LIBRARY}")
	  IF(XMLBEANSXX_GEN_PROG )
		MESSAGE(STATUS "Found XMLBEANSXX-GEN: ${XMLBEANSXX_GEN_PROG}")
	ENDIF(XMLBEANSXX_GEN_PROG )
   ENDIF (NOT XMLBEANSXX_FIND_QUIETLY)
ELSE (XMLBEANSXX_FOUND)
   IF (XMLBEANSXX_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find XMLBEANSXX")
   ENDIF (XMLBEANSXX_FIND_REQUIRED)
ENDIF (XMLBEANSXX_FOUND)
