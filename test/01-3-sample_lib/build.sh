mkdir _build
cd _build
cmake -G "Unix Makefiles" ../lib
make -f Makefile
cd sample_program
./sample_program
