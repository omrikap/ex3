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
	char *_array;
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
	if (arraySize == 0) // todo what should be here?
	{
		char *str = (char *)malloc(arraySize * sizeof(1));
		str[0] = NULL;
		return str;
	}
	return (char *)malloc(arraySize * sizeof(char));
}

// ------------------------------ functions -----------------------------

MyString *myStringAlloc() // toTest
{
	MyString *pNewString = (MyString *)malloc(sizeof(MyString));
	pNewString->_array = charArrayAlloc(0);
	pNewString->_length = 0;
	return pNewString;
}

void myStringFree(MyString *str) // toTest
{
	if (str != NULL)
	{
		free(str->_array);
		str->_array = NULL;
		free(str); // toCheck why gives weired values? (debug)
//		str = NULL;
	}
}

MyString *myStringClone(const MyString *str) // toTest
{
	MyString *pClonedString = myStringAlloc();
	if (pClonedString != NULL)
	{
		pClonedString->_array = charArrayAlloc(str->_length);
		memcpy(pClonedString->_array, str->_array, str->_length);
		pClonedString->_length = str->_length;
	}
	return pClonedString;
}

MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other) // toTest fixme
{
	if (other == NULL || str == NULL)
	{
		return MYSTRING_ERROR;
	}
	else if (str->_length == other->_length)
	{
		memcpy(str->_array, other->_array, other->_length);
		return MYSTRING_SUCCESS;
	}
	else if (str->_length > other->_length || str->_length < other->_length)
	{
		free(str->_array);
		str->_array = charArrayAlloc(other->_length);
		memcpy(str->_array, other->_array, other->_length);
	}
	return MYSTRING_ERROR;
}

MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *)) // toTest
{
	char *tmp = (char *)malloc(str->_length);
	if (tmp == NULL)
	{
		return MYSTRING_ERROR;
	}
	int tmp_index = 0;
	for (int i=0; i < str->_length; ++i)
	{
		if (!filt((char *) str->_array[i]))
		{
			tmp[tmp_index] = str->_array[i];
			tmp_index++;
		}
	}
	free(str->_array);
	str->_array = tmp;
	return MYSTRING_SUCCESS;
}

MyStringRetVal myStringSetFromCString(MyString *str, const char *cString) // toTest
{
	free(str->_array);
	str->_array = charArrayAlloc(0);
	if (str->_array == NULL)
	{
		return MYSTRING_ERROR;
	}
	int i = 0;
	while (*cString)
	{
		str->_array[i] = *cString++;
		i++;
	}
	return MYSTRING_SUCCESS;
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

/**
 * @brief A procedure to help creating meaningful arrays with ease for testing purposes.
 * @param a pointer to MyString type.
 * @param a character to fill the array with.
 * @param new size for the array.
 * @return MYSTRING_SUCCESS if the array was initialized, MYSTRING_ERROR otherwise.
 */
static MyString* testArrayAlloc(MyString *str, int c, size_t size) // fixme
{
	if (str == NULL)
	{
		str = myStringAlloc();
	}
	free(str->_array);
	str->_array = NULL;
	str->_array = charArrayAlloc(size);
	memset(str->_array, c, size);
	str->_length = size;
	return str;
}

int main()
{
	/* MyString Alloc Test */
	// create a new empty myString.
	MyString *newMyString = myStringAlloc(); // how to test this?

	/* MyString Free Test */
	// free an already created myString.
	myStringFree(newMyString); // todo how to test if it works?
	newMyString = NULL;

	/* MyString Clone */
	// clone myList. verify different array pointers.
	MyString *aString = myStringAlloc();
	MyString *bString = myStringClone(aString);

	/* Set From MyString */
	int retVal = 0;
	// other == NULL should return ERROR
	MyString *other = NULL;
	retVal = myStringSetFromMyString(aString, other);
	printf("%d\n", retVal);

	// str == NULL should return ERROR. Can't change the pointer itself.
	other = myStringAlloc();
	myStringFree(aString);
	aString = NULL; // todo note: have to null this after free, because it can't be done from
	// inside a function. pass-by-value.
	retVal = myStringSetFromMyString(aString, other);
	printf("%d\n", retVal);

	// set from identical size. should return SUCCESS
	testArrayAlloc(other, 'b', 8);
	testArrayAlloc(aString, 't', 8);
	retVal = myStringSetFromMyString(aString, other);
	printf("%d\n", retVal);

	// set from bigger/smaller list. should return SUCCESS
	myStringFree(aString);
	aString = myStringAlloc();
	retVal = myStringSetFromMyString(aString, other);
	printf("%d\n", retVal);

	/* mem games */
	char *test = charArrayAlloc(4);
	memset(test, 't', 5);
	char *pStr = "hi all";
//	char *pNewStr = NULL;
	char *pNewStr = (char *)malloc(6*sizeof(char));
	memcpy(pNewStr, pStr, 6);
	puts(pNewStr);
	size_t a = 5;
	size_t b = a;
	return 0;
}

#endif // NDEBUG