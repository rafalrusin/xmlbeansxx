* Dependencies

 -  g++ 4.3.3 or MSVC 8.0

 -  cmake 2.6
    http://www.cmake.org/
    http://www.cmake.org/HTML/Download.html
    
    or 
    
    Autotools (GNU Build System)
    http://www.gnu.org/software/autoconf/
    http://www.gnu.org/software/automake/
  
    
 -  Boost C++ Libraries 1.34
    http://www.boost.org/
    
 -  Xerces C++ Parser 2.8
    http://xerces.apache.org/xerces-c/
 
 -  CppUnit - C++ port of JUnit - (required for testing) 1.12
    http://cppunit.sourceforge.net/
 
 -  log4cxx 1.10 (not required, but highly recommended
    http://logging.apache.org/log4cxx/index.html

 -  maven2 (for building xsd to cpp generator)

* Installing deps.

 - Ubuntu 9.04:
    #apt-get install liblog4cxx10-dev libcppunit-dev libxerces-c2-dev libboost-dev autotools-dev automake cmake maven2

 - Gentoo
    #USE="-nocxx" emerge ">=dev-util/cmake-2.6" automake ">=sys-devel/gcc-4.3.3" ">=dev-libs/xerces-c-2.8" ">=dev-libs/boost-1.34" ">=dev-util/cppunit-1.12" log4cxx maven-bin
 
* How to build and install.
  - with cmake
    run:
    ./cbootstrap
    cd build
    make 
    make test
    make install

  - with Autotools
    run:
      ./bootstrap
      ./configure
      make 
      make check
      make install

