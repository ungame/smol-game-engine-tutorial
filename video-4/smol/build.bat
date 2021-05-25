rd /Q /S build

cmake -S ./src/ -G "NMake Makefiles" -B build

cd build

nmake

cd ..
