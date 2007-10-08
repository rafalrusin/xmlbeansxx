


ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/log4j.xml
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/log4j.xml.default ${CMAKE_CURRENT_BINARY_DIR}/log4j.xml
)

SET(TEST_LIBS
	xmlbeansxx
	${CPPUNIT_LIBRARY}
)

SET(TEST_PROGS
	BeansTest
	XmlArrayTest
	ArrayTest
#	GenTest
#	ParseTest
#	UnqualTest
#	EnumTest
#	XPathTest
#	EntitiesTest
)

# c.xsd
#SET(C_SRC ${CMAKE_CURRENT_BINARY_DIR}/c.h ${CMAKE_CURRENT_BINARY_DIR}/c.cpp)

MACRO(GEN XSD SRC_C SRC_H)
	SET(${XSD}_SRC ${CMAKE_CURRENT_BINARY_DIR}/${SRC_H} ${CMAKE_CURRENT_BINARY_DIR}/${SRC_C})
	ADD_CUSTOM_COMMAND(OUTPUT ${${XSD}_SRC}
		COMMAND ${XMLBEANSXX_GEN} ${CMAKE_CURRENT_SOURCE_DIR}/${XSD}
		DEPENDS ${XMLBEANSXX_GEN} ${XSD}
			
	)	
ENDMACRO(GEN)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/..
)


GEN(c.xsd c.cpp c.h)
#ADD_CUSTOM_COMMAND(OUTPUT ${C_SRC}
#	COMMAND ${XMLBEANSXX_GEN} ${CMAKE_CURRENT_SOURCE_DIR}/c.xsd
#	DEPENDS ${XMLBEANSXX_GEN}
#)


ADD_EXECUTABLE(BeansTest	TestApp.cpp BeansTest.cpp BeansTest.h)
ADD_EXECUTABLE(XmlArrayTest	TestApp.cpp XmlArrayTest.cpp XmlArrayTest.h)
ADD_EXECUTABLE(ArrayTest	TestApp.cpp ArrayTest.cpp ArrayTest.h 			${c.xsd_SRC})
#ADD_EXECUTABLE(GenTest		TestApp.cpp GenTest.cpp GenTest.h			c.h c.cpp pawel.h pawel.cpp)
#ADD_EXECUTABLE(ParseTest	TestApp.cpp ParseTest.cpp ParseTest.h 			c.h c.cpp)
#ADD_EXECUTABLE(UnqualTest	TestApp.cpp UnqualTest.cpp UnqualTest.h 		unqual.h unqual.cpp)
#ADD_EXECUTABLE(EnumTest		TestApp.cpp EnumTest.cpp EnumTest.h			pawel.h pawel.cpp)
#ADD_EXECUTABLE(XPathTest	TestApp.cpp XPathTest.cpp XPathTest.h			pawel.h pawel.cpp)
#ADD_EXECUTABLE(EntitiesTest	TestApp.cpp EntitiesTest.cpp EntitiesTest.h		entities.h entities.cpp)



FOREACH(P ${TEST_PROGS})
	TARGET_LINK_LIBRARIES(${P} ${TEST_LIBS})
ENDFOREACH(P)

#TARGET_LINK_LIBRARIES(BeansTest ${TEST_LIBS})


ADD_CUSTOM_TARGET(tests
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/log4j.xml BeansTest
)