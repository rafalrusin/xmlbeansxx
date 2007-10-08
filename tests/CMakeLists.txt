


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
	ParseTest
#	UnqualTest
	EnumTest
	XPathTest
	EntitiesTest
)



include_directories(
	${CMAKE_CURRENT_BINARY_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/src/xmlbeansxx
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_BINARY_DIR}/src/xmlbeansxx
	${PROJECT_BINARY_DIR}/src
)


XMLBEANSXX_GEN(c.xsd 		c.cpp 		c.h)
XMLBEANSXX_GEN(pawel.xsd 	pawel.cpp 	pawel.h)
XMLBEANSXX_GEN(unqual.xsd 	unqual.cpp 	unqual.h)
XMLBEANSXX_GEN(entities.xsd 	entities.cpp 	entities.h)

ADD_EXECUTABLE(BeansTest	TestApp.cpp BeansTest.cpp	BeansTest.h)
ADD_EXECUTABLE(XmlArrayTest	TestApp.cpp XmlArrayTest.cpp	XmlArrayTest.h)
ADD_EXECUTABLE(ArrayTest	TestApp.cpp ArrayTest.cpp	ArrayTest.h		${c.xsd_SRC})
#ADD_EXECUTABLE(GenTest		TestApp.cpp GenTest.cpp		GenTest.h		${c.xsd_SRC} ${pawel.xsd_SRC})
ADD_EXECUTABLE(ParseTest	TestApp.cpp ParseTest.cpp 	ParseTest.h 		${c.xsd_SRC})
#ADD_EXECUTABLE(UnqualTest	TestApp.cpp UnqualTest.cpp 	UnqualTest.h 		${unqual.xsd_SRC})
ADD_EXECUTABLE(EnumTest		TestApp.cpp EnumTest.cpp 	EnumTest.h		${pawel.xsd_SRC})
ADD_EXECUTABLE(XPathTest	TestApp.cpp XPathTest.cpp 	XPathTest.h		${pawel.xsd_SRC})
ADD_EXECUTABLE(EntitiesTest	TestApp.cpp EntitiesTest.cpp 	EntitiesTest.h		${entities.xsd_SRC})



FOREACH(P ${TEST_PROGS})
	TARGET_LINK_LIBRARIES(${P} ${TEST_LIBS})
ENDFOREACH(P)


ADD_CUSTOM_TARGET(tests
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/log4j.xml BeansTest
)