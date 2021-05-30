rd /Q /S build

rm engine.log

cmake -S ./src/ -G "NMake Makefiles" -B build

cd build

nmake

cd smolSDK\bin

slauncher.exe

cd ..\..\..\
