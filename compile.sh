gcc -c dict_gen.c bst.c combine.c case_change.c substitute.c append.c
gcc dict_gen.o bst.o combine.o case_change.o substitute.o append.o -o dict_gen.exe
rm dict_gen.o bst.o combine.o case_change.o substitute.o append.o