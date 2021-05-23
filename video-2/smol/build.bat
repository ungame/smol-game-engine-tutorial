rd /Q /S build

cmake -S . -G "NMake Makefiles" -B build

cd build

nmake
