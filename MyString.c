#include <stddef.h>
#include "MyString.h"

// todo string size function/string size variable.

struct _MyString *myStringAlloc()
{
	// todo find a mechanism for EOL
	return NULL;
}

void myStringFree(MyString *str)
{

}


MyString *myStringClone(const MyString *str)
{
	return NULL;
}

MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
	// todo check size of both strings
	return MYSTRING_ERROR;
}

MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
	// gets a pointer to a function
	// for loop
	// pointers inside arrays
	return MYSTRING_ERROR;
}

MyStringRetVal myStringSetFromCString(MyString *str, const char *cString)
{
	// calculation allocated memory (reaching the last index of an array) to check '\0'
	return MYSTRING_ERROR;
}


MyStringRetVal myStringSetFromInt(MyString *str, int n)
{
	// modulo to extract digits -> make them chars
	// todo what if size does not match?
	return MYSTRING_ERROR;
}

int myStringToInt(const MyString *str)
{
	// verify the string containing int content.
	// how to convert character to int. ascii?
	return 0;
}

char *myStringToCString(const MyString *str)
{
	// add '\0'. what more?
	return NULL;
}

MyStringRetVal myStringCat(MyString *dest, const MyString *src)
{
	// todo implement before mystringToCString. It's doing the same with a static dest.
	// use memcpy
	return MYSTRING_ERROR;
}

MyStringRetVal myStringCatTo(const MyString *str1, const MyString *str2, MyString *result)
{
	// similar to the last function.
	// malloc the sum of sizes and copy the strings to it.
	return MYSTRING_ERROR;
}

int myStringCompare(const MyString *str1, const MyString *str2)
{
	// bonus: compare pointers.
	// if not bonus: for loop and compare ascii codes.
	return 0;
}

int myStringEqual(const MyString *str1, const MyString *str2)
{

	return 0;
}
