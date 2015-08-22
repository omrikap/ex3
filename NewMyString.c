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
static MyStringRetVal reallocMyString(MyString *str, size_t size)
{
	if (str == NULL)
	{
		return MYSTRING_ERROR;
	}
	if (str->_array == NULL)
	{
		str->_array = (char *) malloc(size);
		if (str->_array != NULL)
		{
			str->_length = size;
		}
		else
		{
			return MYSTRING_ERROR;
		}
	}
	else if (str->_length < size)
	{
		realloc(str->_array, size);
		str->_length = size;
	}
	assert(str->_length >= size);
	return MYSTRING_SUCCESS; // if length >= size, do nothing
}

// ------------------------------ functions -----------------------------

MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
	if (str == NULL || filt == NULL)
	{
		return MYSTRING_ERROR;
	}
	char *tmp = (char *) malloc(str->_length);
	if (tmp == NULL)
	{
		return MYSTRING_ERROR;
	}
	if (str->_length > 0)
	{
		int tmp_index = 0;
		for (int i=0; i < str->_length; ++i)
		{
			if (!filt(&str->_array[i]))
			{
				tmp[tmp_index] = str->_array[i];
				tmp_index++;
			}
		}
		free(str->_array);
		str->_array = tmp;
		str->_length = (size_t) --tmp_index; // reduce last loop increment
	}
	return MYSTRING_SUCCESS;
}

MyString *myStringClone(const MyString *str)
{
	MyString *pClonedString = NULL;
	if (str != NULL)
	{
		pClonedString = myStringAlloc();
		if (pClonedString == NULL)
		{
			return pClonedString;
		}
		reallocMyString(pClonedString, str->_length);
		memcpy(pClonedString->_array, str->_array, str->_length);
		pClonedString->_length = str->_length;
	}
	return pClonedString;
}

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

// -------------------------- tester-functions --------------------------

void reallocMyStringDriver()
{
	printf("++ Runing %s:\n", __func__);

	//	test #1: str is NULL - FAIL
	MyString *str = NULL;

	int got = reallocMyString(str, 5);
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);

	//	test #2: array is NULL - PASS
	str = myStringAlloc(); // newString is declared in the previous test.

	got = reallocMyString(str, 5);
	printf("   - test #2: expected %d, got %d.\n", PASS, got);
	myStringFree(str);
	str = NULL;

	//	test #3: str._length < size - PASS
	str = myStringAlloc();
	myStringSetFromCString(str, "hey you");
	got = reallocMyString(str, 10);
	printf("   - test #3: expected %d, got %d.\n", PASS, got);
	myStringFree(str);

	//	test #4: str._length == size - PASS
	str = myStringAlloc();
	myStringSetFromCString(str, "hey you");
	got = reallocMyString(str, 7);
	printf("   - test #4: expected %d, got %d.\n", PASS, got);
	myStringFree(str);

	//	test #5: str._length > size - PASS
	str = myStringAlloc();
	myStringSetFromCString(str, "hey you");
	got = reallocMyString(str, 3);
	printf("   - test #5: expected %d, got %d.\n\n", PASS, got);
	myStringFree(str);
}

void myStringAllocFreeDriver()
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

void myStringSetFromCStringDriver()
{
	printf("++ Runing %s:\n", __func__);

	//	test #1: str is NULL
	MyString *newMyString = NULL;

	int got = myStringSetFromCString(newMyString, "hi all!");
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);

	//	test #2: cString is NULL
	newMyString = myStringAlloc(); // newString is declared in the previous test.

	got = myStringSetFromCString(newMyString, NULL);
	printf("   - test #2: expected %d, got %d.\n", FAIL, got);
	myStringFree(newMyString);
	newMyString = NULL;

	//	test #3: cString is a well formatted C string
	newMyString = myStringAlloc();
	got = myStringSetFromCString(newMyString, "hi all!");
	printf("   - test #3: expected %d, got %d.\n\n", PASS, got);
	myStringFree(newMyString);
}

void myStringSetFromMyStringDriver()
{
	printf("++ Runing %s:\n", __func__);

	//	test #1: str is NULL - FAIL
	MyString *str = NULL;
	MyString *other = myStringAlloc();
	myStringSetFromCString(other, "test string");

	int got = myStringSetFromMyString(str, other);
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);

	//	test #2: other is NULL - FAIL
	myStringFree(other);
	other = NULL;
	str = myStringAlloc(); // newString is declared in the previous test.

	got = myStringSetFromMyString(str, other);
	printf("   - test #2: expected %d, got %d.\n", FAIL, got);
	myStringFree(str);
	str = NULL;

	//	test #3: str and other are well defined - PASS
	str = myStringAlloc();
	other = myStringAlloc();
	myStringSetFromCString(other, "test string");
	got = myStringSetFromMyString(str, other);
	printf("   - test #3: expected %d, got %d.\n\n", PASS, got);
	myStringFree(str);
	str = NULL;
	myStringFree(other);
	other = NULL;
}

void myStringCloneDriver()
{
	printf("++ Runing %s:\n", __func__);

//	test #1: str is NULL - PASS
	MyString *str = NULL;
	MyString *result = myStringClone(str);

	int got = FAIL;
	if (result == NULL)
	{
		got = PASS;
	}
	printf("   - test #1: expected %d, got %d.\n", PASS, got);
	myStringFree(str);
	str = NULL;
	myStringFree(result);
	result = NULL;

// 	test #2: str is a legal MyString - PASS
	str = myStringAlloc();
	myStringSetFromCString(str, "cloning");
	result = myStringAlloc();
	result = myStringClone(str);

	got = FAIL;
	if (result != NULL) // fixme. test with memcmp?
	{
		got = PASS;
	}
	printf("   - test #2: expected %d, got %d.\n\n", PASS, got);
	myStringFree(str);
	str = NULL;
	myStringFree(result);
	result = NULL;
}

void myStringFilterDriver()
{
	printf("++ Runing %s:\n", __func__);

//	test #1: str is NULL - FAIL
	MyString *str = NULL;

	int got = myStringFilter(str, filter);
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);

//	test #2: flt is NULL - FAIL
	str = myStringAlloc();
	myStringSetFromCString(str, "atesat asatrianga");

	got = myStringFilter(str, NULL);
	printf("   - test #2: expected %d, got %d.\n", FAIL, got);
	myStringFree(str);
	str = NULL;

//	test #3: str._arr is NULL - PASS
	str = myStringAlloc();

	got = myStringFilter(str, filter);
	printf("   - test #3: expected %d, got %d.\n", PASS, got);
	myStringFree(str);
	str = NULL;

//	test #4: str._arr is a string - PASS
	str = myStringAlloc();
	myStringSetFromCString(str, "atesat asatrianga");

	got = myStringFilter(str, filter);
	printf("   - test #4: expected %d, got %d.\n", PASS, got);
	myStringFree(str);
	str = NULL;
}

int main()
{
//	reallocMyStringDriver();
//	myStringAllocFreeDriver();
//	myStringSetFromCStringDriver();
//	myStringSetFromMyStringDriver();
//	myStringCloneDriver();
	myStringFilterDriver();

	return 0;
}

#endif // NDEBUG