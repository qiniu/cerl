mkdir release
cd release
cp ../release.cmake CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release .
make
#./test.exe
#make cover
#make check
