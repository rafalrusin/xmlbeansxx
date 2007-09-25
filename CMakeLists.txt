
PROJECT (xmlbeansxx)

cmake_minimum_required(VERSION 2.4.7)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Log4cxx REQUIRED)
find_package(Boost REQUIRED)
find_package(Xerces REQUIRED)
find_package(CppUnit REQUIRED)
include_directories (${XERCES_INCLUDE_DIR} ${LOG4CXX_INCLUDE_DIR})


SUBDIRS (src/xmlbeansxx)

