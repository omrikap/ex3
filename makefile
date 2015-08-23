// fixme
#make myString
libmyString.a: MyString.c MyString.h

#make tests
tests: MyString.o
	c99 -Wall -Wvla -Wextra MyString.c

#make main
MyStringMain: MyStringMain.o MyString.o
	c99 -Wall -Wvla -Wextra MyString.o MyStringMain.o

#make clean
clean // fixme
	rm MyString.o MyStringMain.o libmyString.a tests MyStringMain
