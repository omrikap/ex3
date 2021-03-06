CC = c99
FLAGS = -Wall -Wvla -Wextra

.PHONY: clean
.PHONY: tests
.PHONY: main

#make myStringLib
libmyString.a: MyString.c MyString.h
	$(CC) -c -D NDEBUG $(FLAGS) MyString.c -o MyString.o
	ar rcs libmyString.a MyString.o

#make tests
tests: MyString.o
	$(CC) $(FLAGS) MyString.c -o MyStringTests
	MyStringTests

#make main
MyStringMain: MyStringMain.o MyString.o libmyString.a
	$(CC) $(FLAGS) -D NDEBUG MyStringMain.c -L. -lmyString -o MyStringMain
	MyStringMain

#make clean
clean:
	rm -f *.o *.out libmyString.a MyStringMain MyStringTests
