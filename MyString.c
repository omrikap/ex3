#include <string.h>
#include "MyString.h"

// todo update doc
/********************************************************************************
 * @file MyString.h
 * @author  slabc <slbac@cs.huji.ac.il>
 * @version 1.0
 * @date 9 Aug 2015
 *
 * @brief The SLabC Standard Strings library.
 *
 * @section LICENSE
 * This program is not a free software;
 *
 * @section DESCRIPTION
 * The LabC Standard Strings library.
 *
 * This library provides a string type with extra capabilities.
 *
 * The library provides the following features:
 *  - basic string operations
 *  - conversion to other types (ints, C strings)
 *
 * Error handling
 * ~~~~~~~~~~~~~~
 * Most functions may fail due to failure to allocate dynamic memory. When
 * this happens the functions will return an appropriate failure value. If this
 * happens, then the state of the other outputs of the function is undefined.
 ********************************************************************************/

// -------------------------- const definitions -------------------------

struct _MyString
{
	char *_string;
	size_t _length; // todo verify if long needed
};

// --------------------------- My functions -----------------------------

/**
 * @brief A function that allocates memory for an array of a given size.
 * @param The number of Bytes to allocate in the area in memory. 0 will allocate a NULL pointer.
 * @return A pointer to the new area in memory.
 */
static char *charArrayAlloc(long arraySize)
{
	if (arraySize == 0)
	{
		return NULL;
	}
	return (char *)malloc(arraySize * sizeof(char));
}

// ------------------------------ functions -----------------------------

MyString *myStringAlloc() // toTest
{
	MyString *pNewString = (MyString *)malloc(sizeof(MyString));
	pNewString->_string = charArrayAlloc(0);
	pNewString->_length = 0;
	return pNewString;
}

void myStringFree(MyString *str) // toTest
{
	if (str != NULL)
	{
		free(str->_string);
		free(str);
	}
}

MyString *myStringClone(const MyString *str) // toTest
{
	MyString *pClonedString = myStringAlloc();
	if (pClonedString != NULL)
	{
		memcpy(pClonedString->_string, str->_string, str->_length);
//		pClonedString->_string = str->_string; // todo needed?
		pClonedString->_length = str->_length;
	}
	return pClonedString;
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

// ------------------------- unit-testing ------------------------------

#ifndef NDEBUG

int main()
{
	/* MyString Alloc Test */
	// create a new empty myString.
	MyString *newMyString = myStringAlloc(); // how to test this?

	/* MyString Free Test */
	// free an already created myString.
	myStringFree(newMyString); // todo how to test if it works?

	/* MyString Clone */
	// clone myList. verify different array pointers.

	/* Set From MyString */
	// set from identical size.
	// set from bigger list.
	// set from smaller size.


	/* mem games */
	char *pStr = "hi all";
//	char *pNewStr = NULL;
	char *pNewStr = (char *)malloc(6*sizeof(char));
	memcpy(pNewStr, pStr, 6);
	puts(pNewStr);

	return 0;
}

#endif // NDEBUG