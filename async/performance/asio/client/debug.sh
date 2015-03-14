mkdir debug
cd debug
cp ../debug.cmake CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Debug .
make
#./test.exe
#make cover
#make check
