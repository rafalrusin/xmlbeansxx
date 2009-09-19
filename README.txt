* Dependencies

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

* Installing deps.

 - Ubuntu 9.04:
    #apt-get install liblog4cxx10-dev libcppunit-dev libxerces-c2-dev libboost-dev autotools-dev automake cmake
 
III. How to build and install.

  1. with Autotools
  run:
    ./bootstrap
    ./configure
    make 
    make check
    make install
    
  2. with cmake
    run:
    ./cbootstrap
    cd build
    make 
    make test
    make install
    
  
  
IV. precision of xsd:decimal numbers.

To increase the precision of decimal numbers use:
 xmlbeansxx::TextUtils::setPrecision(int precision);
if you use gmpxx the precision can by very large.

