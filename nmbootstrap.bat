rd /S /Q build
mkdir build
cd build

@REM cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=c:\usr\  -DWITHOUT_LOG4CXX=1 -DWITHOUT_GMP=1 

cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=c:\usr\
