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
		return NULL;
	}
	return (char *)malloc(arraySize * sizeof(char));
}

/**
 * @brief Uses to reallocate the memory size of a MyString struct.
 * @param str the string to change it's length.
 * @param size the new size.
 * @return void.
 */
static void reallocMyString(MyString *str, size_t size)
{
	if (str->_length >= size)
	{
		return;
	}
	else if (str->_length < size)
	{
		realloc(str->_array, size);
	}
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
		if (!filt(&str->_array[i]))  // todo might not be the right argument.
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
	if (str == NULL || cString == NULL)
	{
		return MYSTRING_ERROR;
	}
	const char *head = cString;
	size_t i = 0;
	while (*cString++)
	{
		i++;
	}
	str->_length = i;
	free(str->_array);
	str->_array = charArrayAlloc(i);
	memcpy(str->_array, head, i);
	return MYSTRING_SUCCESS;
}

/**
 * @brief return the given int without the last digit.
 * @param integer to remove it's last gidit.
 */
static int reduceModulo(int n)
{
	return (n - (n % 10)) / 10;

}

/**
 * @brief find the number of digits in a given int.
 */
static size_t findIntLentgh(int n)
{
	size_t length = 0;
	if (n != 0)
	{
		while (n != 0)
		{
			n = reduceModulo(n);
			length++;
		}
	}
	return length;
}

MyStringRetVal myStringSetFromInt(MyString *str, int n) // toTest
{
	if (str == NULL || &n == NULL)
	{
		return MYSTRING_ERROR;
	}
	bool negative = false;
	size_t intLength;
	if (n < 0)
	{
		negative = true;
		n = abs(n);
		intLength = findIntLentgh(n) + 1;
	}
	else
	{
		intLength = findIntLentgh(n);
	}
	reallocMyString(str, intLength); // todo error if this fails
	while (((intLength > 0) && (!negative)) || ((intLength > 1) && (negative)))
	{
		str->_array[intLength - 1] = (char) ((n % 10) + 48);
		n = reduceModulo(n);
		intLength--;
	}
	if (negative)
		str->_array[0] = '-';
	return MYSTRING_SUCCESS;
}

int myStringToInt(const MyString *str) // toTest fixme
{
	// todo verify the string containing int content.
	int result = 0;
	bool negative = false;
	int i = 0;
	if (str->_array == NULL)
	{
		return result;
	}
	if (str->_array[0] == '-') // fixme verify not null
	{
		negative = true;
		i = 1;
	}
	for (; i < str->_length; ++i)
	{
		result *= 10;
		result += (str->_array[i] - 48);
	}
	if (negative)
	{
		result *= (-1);
	}
	return result;
}

char *myStringToCString(const MyString *str) // toTest fixme
{
	MyString *newString = myStringClone(str);
	reallocMyString(newString, str->_length + 1);
	str->_array[str->_length-1] = '\0'; // fixme. crashes myStringFromInt. SEGMENTATION.
	return str->_array;
}

MyStringRetVal myStringCat(MyString *dest, const MyString *src)
{
	if (dest == NULL || src == NULL)
	{
		return MYSTRING_ERROR;
	}
	size_t copy_index = dest->_length;
	reallocMyString(dest, dest->_length + src->_length);
	memcpy(&dest->_array[copy_index+1], src, src->_length);
	return MYSTRING_SUCCESS;
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

unsigned long myStringMemUsage(const MyString *str1)
{
	return 0;
}

unsigned long myStringLen(const MyString *str1)
{
	return 0;
}

MyStringRetVal myStringWrite(const MyString *str, FILE *stream)
{
	return MYSTRING_ERROR;
}

// ------------------------- unit-testing ------------------------------

#ifndef NDEBUG

// -------------------------- test-utils -------------------------------

/**
 * @brief A simple string filter to test the MyStringFilter function.
 * @param the character to filter.
 * @return true if the character is filtered, false otherwise
 */
static bool filter(const char *c)
{
	if (*c == 'a')
	{
		return true;
	}
	return false;
}

// ----------------------- tester-functions ----------------------------

void myStringAllocTester() // fixme
{
	// create a new empty myString.
	MyString *newMyString = myStringAlloc(); // how to test this?

	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringFreeTester() // fixme
{
	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringCloneTester()
{
	printf("Runing %s:\n", __func__);

//	test #1: clone a not empty MyString
	char * cString = "hi again";
	MyString *str = myStringAlloc();
	myStringSetFromCString(str, cString);
	MyString *dest = myStringClone(str);
	int got = 1;
	if (!memcmp(str->_array, dest->_array, str->_length))
	{
		got = MYSTRING_SUCCESS;
	}
	int expected = MYSTRING_SUCCESS;

	printf("expected %d, got %d.\n", expected, got);
}

void myStringSetFromMyStringTester()
{
	MyString *aString = myStringAlloc();
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
	myStringSetFromCString(other, "hey you");
	aString = myStringAlloc();
	myStringSetFromCString(aString, "hey joe");
	retVal = myStringSetFromMyString(aString, other);
	printf("%d\n", retVal);

	// set from bigger/smaller list. should return SUCCESS
	myStringFree(aString);
	aString = myStringAlloc();
	retVal = myStringSetFromMyString(aString, other);
	printf("%d\n", retVal);

	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringFilterTester()
{
	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringSetFromCStringTester()
{
	printf("Runing %s:\n", __func__);
	puts("--------------------------------------\n");

	MyString *str = myStringAlloc();
	char *cString = "hi all";

//	test #1: from a string fo length 6.
	int expected1 = 0;
	int got1 = myStringSetFromCString(str, cString);
	puts("test #1");
	printf("nexpected %d, got %d.\n\n",  expected1, got1);
	myStringFree(str);
	str = NULL;

//	test #2: to a NULL MyString.
	int expected2 = MYSTRING_ERROR;
	int got2 = myStringSetFromCString(str, cString);
	puts("test #2");
	printf("nexpected %d, got %d.\n\n",  expected2, got2);

//	test #3: from a NULL string.
	str = myStringAlloc();
	cString = NULL;
	int expected3 = MYSTRING_ERROR;
	int got3 = myStringSetFromCString(str, cString);
	puts("test #3");
	printf("nexpected %d, got %d.\n\n",  expected3, got3);

	// todo add from empty string.
}

void myStringSetFromIntTester() // todo
{
	MyString *str = myStringAlloc();
	int n = 0;
	MyStringRetVal res = myStringSetFromInt(str, n);

	int expected = 0;
	int got = res;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
	myStringFree(str);
}

void myStringToIntTester() // todo
{
	MyString *str = myStringAlloc();

	int expected = 0;
	int got = myStringToInt(str);
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
	myStringFree(str);
}

void myStringToCStringTester() // todo
{
	MyString *str = myStringAlloc();
	char *res = myStringToCString(str);
	int got = 0;
	if (res == "hi")
		got = 1;
	int expected = 0;

	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
	myStringFree(str);
}

void myStringCatTester() // todo
{
	MyString *str = myStringAlloc();
	MyString *dest = myStringAlloc();
	myStringCat(dest, str);
	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringCatToTester() // todo
{
	MyString *str1 = myStringAlloc();
	MyString *str2 = myStringAlloc();
	MyString *res = myStringAlloc();
	myStringCatTo(str1, str2, res);
	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringCompareTester() // todo
{
	MyString *str1 = myStringAlloc();
	MyString *str2 = myStringAlloc();
	myStringCompare(str1, str2);
	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringEqualTester() // todo
{
	MyString *str1 = myStringAlloc();
	MyString *str2 = myStringAlloc();
	myStringEqual(str1, str2);
	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringMemUsageTester() // todo
{
	MyString *str = myStringAlloc();
	myStringMemUsage(str);
	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringStringLenTester() // todo
{
	MyString *str = myStringAlloc();
	myStringLen(str);
	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

void myStringWriteTester() // todo
{
	MyString *str = myStringAlloc();
	FILE *testFile = fopen("","r"); // todo put file
	myStringWrite(str, testFile);
	int expected = 0;
	int got = 0;
	printf("Runing %s:\nexpected %d, got %d.\n", __func__, expected, got);
}

int main()
{
	/* myStringAlloc Test */
	myStringAllocTester();
	// create a new empty myString.
	MyString *newMyString = myStringAlloc(); // how to test this?

	/* MyString Free Test */
	myStringFreeTester();
	// free an already created myString.
	myStringFree(newMyString); // todo how to test if it works?
	newMyString = NULL;

	/* MyString Clone */
	myStringCloneTester();
	// clone myList. verify different array pointers.
	MyString *aString = myStringAlloc();
	MyString *bString = myStringClone(aString);

	/* Set From MyString */
	myStringSetFromMyStringTester();
//	int retVal = 0;
//	// other == NULL should return ERROR
//	MyString *other = NULL;
//	retVal = myStringSetFromMyString(aString, other);
//	printf("%d\n", retVal);
//
//	// str == NULL should return ERROR. Can't change the pointer itself.
//	other = myStringAlloc();
//	myStringFree(aString);
//	aString = NULL; // todo note: have to null this after free, because it can't be done from
//	// inside a function. pass-by-value.
//	retVal = myStringSetFromMyString(aString, other);
//	printf("%d\n", retVal);
//
//	// set from identical size. should return SUCCESS
//	other = testArrayAlloc('b', 8);
//	aString = testArrayAlloc('t', 8);
//	retVal = myStringSetFromMyString(aString, other);
//	printf("%d\n", retVal);
//
//	// set from bigger/smaller list. should return SUCCESS
//	myStringFree(aString);
//	aString = myStringAlloc();
//	retVal = myStringSetFromMyString(aString, other);
//	printf("%d\n", retVal);

	/* Filter */ // todo make a good tester
	myStringFilterTester();
	myStringFilter(aString, filter);

	/* SetFromCString */ // todo make a good tester
	myStringSetFromCStringTester();
	myStringSetFromCString(aString, "hi all");

	/* setFromInt */
	myStringSetFromIntTester();

	/* ToInt */
	myStringToIntTester();

	/* ToCString */
	myStringToCStringTester();

	/* CatTester */
	myStringCatTester();

	/* CatToTester */
	myStringCatToTester();

	/* Compare */
	myStringCompareTester();

	/* Equal */
	myStringEqualTester();

	/* MemUsage */
	myStringMemUsageTester();

	/* myStringLen */
	myStringStringLenTester();

	/* WriteTester */
	myStringWriteTester();

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
	printf("b: %li", b);

	/* modulo */
	printf("%d", 18%10);

	return 0;
}

#endif // NDEBUG