#include <assert.h>
#include <string.h>
#include "MyString.h"

// -------------------------- const definitions -------------------------

struct _MyString
{
	char *_array;
	size_t _length;
};

enum expected
{
	PASS,
	FAIL = -1,
};

// --------------------------- My functions -----------------------------

/**
 * @brief A function that allocates memory for an array of a given size.
 * @param arraySize The number of Bytes to allocate in the area in memory.
 * @return A pointer to the new area in memory. If arraySize == 0, NULL is returned.
 */
static char *charArrayAlloc(long arraySize)
{
	if (arraySize == 0)
	{
		return NULL;
	}
	return (char *) malloc(arraySize * sizeof(char));
}

/**
 * @brief Uses to reallocate the memory size of a MyString struct.
 * @param str the string to change it's length.
 * @param size the new size.
 */
static void reallocMyString(MyString *str, size_t size) // toTest
{
	if (str->_array == NULL)
	{
		str->_array = (char *) malloc(size);
	}
	else if (str->_length >= size)
	{
		return;
	}
	else if (str->_length < size)
	{
		realloc(str->_array, size);
	}
}

// ------------------------------ functions -----------------------------

MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
	if (other == NULL || str == NULL)
	{
		return MYSTRING_ERROR;
	}
	assert(&other->_length != NULL);
	reallocMyString(str, other->_length);
	memcpy(str->_array, other->_array, other->_length);
	return MYSTRING_SUCCESS;
}

MyStringRetVal myStringSetFromCString(MyString *str, const char *cString)
{
	if (str == NULL || cString == NULL)
	{
		return MYSTRING_ERROR;
	}

	const char *head = cString;
	size_t i = 0;
	while (*cString++)
		i++;
	assert(i != 0);
	reallocMyString(str, i);
	memcpy(str->_array, head, i);
	str->_length = i;
	return MYSTRING_SUCCESS;
}

MyString *myStringAlloc()
{
	MyString *pNewString = (MyString *) malloc(sizeof(MyString));
	if (pNewString != NULL)
	{
		pNewString->_array = charArrayAlloc(0);
		pNewString->_length = 0;
	}
	return pNewString;
}

void myStringFree(MyString *str)
{
	if (str != NULL)
	{
		free(str->_array);
		str->_array = NULL;
		free(str);
	}
}

// ---------------------------- unit-testing ----------------------------

#ifndef NDEBUG

// -------------------------- tester-functions --------------------------

void myStringAllocFreeTester() // todo change tester printing mechanism (expected, got).
{
	printf("++ Runing %s:\n", __func__);

	//	test #1: myStringAlloc
	MyString *newMyString = NULL;
	newMyString = myStringAlloc();

	int got = PASS;
	if (newMyString == NULL)
	{
		got = FAIL;
	}
	myStringFree(newMyString);
	newMyString = NULL;

	printf("   - test #1: expected %d, got %d.\n\n", PASS, got);
}

void myStringSetFromCStringTester()
{
	printf("++ Runing %s:\n", __func__);

	//	test #1: str is NULL
	MyString *newMyString = NULL;

	int got = myStringSetFromCString(newMyString, "hi all!");
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);

	//	test #2: cString is NULL
	newMyString = myStringAlloc(); // newString is declared in the previous test.

	int got2 = myStringSetFromCString(newMyString, NULL);
	printf("   - test #2: expected %d, got %d.\n", FAIL, got2);
	myStringFree(newMyString);
	newMyString = NULL;

	//	test #3: cString is a well formatted C string
	newMyString = myStringAlloc();
	int got3 = myStringSetFromCString(newMyString, "hi all!");
	printf("   - test #3: expected %d, got %d.\n\n", PASS, got3);
	myStringFree(newMyString);
}

void myStringSetFromMyStringTester()
{
	printf("++ Runing %s:\n", __func__);

	//	test #1: str is NULL - FAIL // todo
	MyString *str = NULL;
	MyString *other = myStringAlloc();
	myStringSetFromCString(other, "test string");

	int got = myStringSetFromMyString(str, other);
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);

	//	test #2: other is NULL - FAIL // todo
	str = myStringAlloc(); // newString is declared in the previous test.

	int got2 = myStringSetFromMyString(str, other);
	printf("   - test #2: expected %d, got %d.\n", FAIL, got2);
	myStringFree(str);
	str = NULL;

	//	test #3: str and other are well defined - PASS // todo
	str = myStringAlloc();
	int got3 = myStringSetFromMyString(str, other);
	printf("   - test #3: expected %d, got %d.\n\n", PASS, got3);
	myStringFree(str);
}

int main()
{
	myStringAllocFreeTester();
	myStringSetFromCStringTester();

	return 0;
}

#endif // NDEBUG