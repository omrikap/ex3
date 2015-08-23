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
	if (str->_length != size)
	{
		char *reallocArr = (char *) realloc(str->_array, size);
		if (reallocArr == NULL)
		{
			return MYSTRING_ERROR;
		}
		str->_array = reallocArr;
	}
	str->_length = size; // if length >= size, just change length
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

/**
 * @brief Signature adapter for myStringCompare and the comparator function of qsort
 * @param str1 the first MyString* as a void*
 * @param str2 the second MyString* as a void*
 * @return an int. the return value of myStringCompare.
 */
static int voidAdapterCompare(const void *str1, const void *str2)
{
	return myStringCompare((MyString *) str1, (MyString *) str2);
}

// ------------------------------ functions -----------------------------

int myStringCustomEqual(const MyString *str1, const MyString *str2, int (*cmp)(const MyString*,
																			   const MyString*))
{
	if (str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	return cmp(str1, str2);
} // toTest

int myStringCustomCompare(const MyString *str1, const MyString *str2, int (*cmp)(const MyString*,
																				 const MyString*))
{
	if (str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	return cmp(str1, str2);
} // toTest

void myStringCoustomSort(MyString *arr, size_t len, int	(*cmp)(const void *, const void *))
{
	if (arr == NULL || cmp == NULL)
	{
		return;
	}
	qsort(arr, len, sizeof(MyString), cmp);
} // toTest

void myStringSort(MyString *arr, size_t len) // toTest
{
	if (arr == NULL)
	{
		return;
	}
	qsort(arr, len, sizeof(MyString), voidAdapterCompare);
}

MyStringRetVal myStringWrite(const MyString *str, FILE *stream) // toTest
{
	if (str == NULL || stream == NULL)
	{
		return MYSTRING_ERROR;
	}
	const char *cString = myStringToCString(str);
	int success = fputs(cString, stream);
	if (success)
	{
		int flushed = fflush(stream);
		if (flushed == 0)
		{
			return MYSTRING_SUCCESS;
		}
	}
	return MYSTRING_ERROR;
}

unsigned long myStringMemUsage(const MyString *str1) // toTest
{
	return sizeof(MyString) + str1->_length * sizeof(char);
}

int myStringEqual(const MyString *str1, const MyString *str2) // toTest
{
	if (str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	if (str1->_length != str2->_length)
	{
		return 0;
	}
	int result = 0;
	for (size_t i = 0; i < str1->_length; ++i)
	{
		if (str1->_array[i] - str2->_array[i])
		{
			return 0;
		}
		result++;
	}
	return result;
}

int myStringCompare(const MyString *str1, const MyString *str2)
{
	if (str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	size_t length = 0;
	if (str1->_length < str2->_length)
	{
		length = str1->_length;
	}
	else
	{
		length = str2->_length;
	}
	for (size_t i = 0; i < length; ++i)
	{
		if (str1->_array[i] != str2->_array[i])
		{
			return str1->_array[i] == str2->_array[i];
		}
	}
	return (int) (str1->_length - str2->_length);
}

int myStringToInt(const MyString *str)
{
	// todo verify the string containing int content. how?
	if (str == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	int result = 0;
	bool negative = false;
	size_t i = 0;
	if (str->_length == 0)
	{
		return result;
	}
	if (str->_array[0] == '-')
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

MyStringRetVal myStringSetFromInt(MyString *str, int n)
{
	if (str == NULL)
	{
		return MYSTRING_ERROR;
	}
	bool negative = false;
	size_t intLength;
	if (n < 0)
	{
		negative = true;
		n *= (-1);
		intLength = findIntLentgh(n) + 1;
	}
	else
	{
		intLength = findIntLentgh(n);
	}
	reallocMyString(str, intLength);
	if (str->_length != intLength)
	{
		return MYSTRING_ERROR;
	}
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

char *myStringToCString(const MyString *str)
{
	if (str == NULL)
	{
		return NULL;
	}
	MyString *cString = myStringAlloc();
	reallocMyString(cString, str->_length + 1);
	if (cString->_array == NULL)
	{
		return NULL;
	}
	memcpy(cString->_array, str->_array, str->_length);
	*(cString->_array + str->_length) = '\0';
	return cString->_array;
}

MyStringRetVal myStringCatTo(const MyString *str1, const MyString *str2, MyString *result)//toTest
{
	if (str1 == NULL || str2 == NULL || result == NULL)
	{
		return MYSTRING_ERROR;
	}
	int reallocStatus = reallocMyString(result, str1->_length + str2->_length);
	if (reallocStatus)
	{
		return MYSTRING_ERROR;
	}
	if (result->_array == NULL)
	{
		return MYSTRING_ERROR;
	}
	memcpy(result->_array, str1->_array, str1->_length);
	memcpy(result->_array + str1->_length, str2->_array, str2->_length);
	result->_length = str1->_length + str2->_length;
	return MYSTRING_SUCCESS;
}

MyStringRetVal myStringCat(MyString *dest, const MyString *src)
{
	if (dest == NULL || src == NULL)
	{
		return MYSTRING_ERROR;
	}
	int reallocStatus = reallocMyString(dest, dest->_length + src->_length + 1);
	if (reallocStatus)
	{
		return MYSTRING_ERROR;
	}
	if (src->_array != NULL)
	{
		memcpy(dest->_array + dest->_length, src->_array, src->_length);
		dest->_length = dest->_length + src->_length;
	}
	return MYSTRING_SUCCESS;
}

unsigned long myStringLen(const MyString *str1)
{
	if (str1 == NULL)
	{
		return 0;
	}
	return str1->_length;
}

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
		for (size_t i=0; i < str->_length; ++i)
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
		if (str->_array != NULL)
		{
			free(str->_array);
			str->_array = NULL;
		}
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
	printf("   - test #4: expected %d, got %d.\n\n", PASS, got);
	myStringFree(str);
	str = NULL;
}

void myStringLenDriver()
{
	printf("++ Runing %s:\n", __func__);

//	test #1: str is NULL - PASS
	MyString *str = NULL;
	unsigned long len = myStringLen(str);
	int got = FAIL;
	if (len == 0)
	{
		got = PASS;
	}
	printf("   - test #1: expected %d, got %d.\n", PASS, got);

	//	test #1: str is allocated with legal length - PASS
	str = myStringAlloc();
	myStringSetFromCString(str, "testing testing testing");
	len = myStringLen(str);

	got = FAIL;
	if (len != 0)
	{
		got = PASS;
	}
	printf("   - test #2: expected %d, got %d.\n\n", PASS, got);
	myStringFree(str);
	str = NULL;
}

void myStringCatDriver()
{
	printf("++ Runing %s:\n", __func__);

//	test #1: dest is NULL - FAIL
	MyString *dest = NULL;
	MyString *src = myStringAlloc();
	myStringSetFromCString(src, "hi again");

	int got = myStringCat(dest, src);
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);
	myStringFree(src);
	src = NULL;

//	test #2: src is NULL - FAIL
	dest = myStringAlloc();
	myStringSetFromCString(src, "you again");

	got = myStringCat(dest, src);
	printf("   - test #2: expected %d, got %d.\n", FAIL, got);
	myStringFree(dest);
	dest = NULL;

//	test #3: dest._arr is NULL - PASS
	dest = myStringAlloc();
	assert(dest->_array == NULL);
	src = myStringAlloc();
	myStringSetFromCString(src, "testing");

	got = myStringCat(dest, src);
	printf("   - test #3: expected %d, got %d.\n", PASS, got);
	myStringFree(dest);
	dest = NULL;
	myStringFree(src);
	src = NULL;

//	test #4: src._arr is NULL - PASS
	dest = myStringAlloc();
	myStringSetFromCString(dest, "shmesting");
	src = myStringAlloc();
	assert(src->_array == NULL);

	got = myStringCat(dest, src);
	printf("   - test #4: expected %d, got %d.\n", PASS, got);
	myStringFree(dest);
	dest = NULL;
	myStringFree(src);
	src = NULL;

//	test #5: both MyStrings are initialized with an array - PASS
	dest = myStringAlloc();
	myStringSetFromCString(src, "good ");
	src = myStringAlloc();
	myStringSetFromCString(src, "times");

	got = myStringCat(dest, src);
	printf("   - test #5: expected %d, got %d.\n\n", PASS, got);
	myStringFree(dest);
	dest = NULL;
	myStringFree(src);
	src = NULL;
}

void myStringCatToDriver()
{
	printf("++ Runing %s:\n", __func__);

//	test #1: str1 is NULL - FAIL
	MyString *str1 = NULL;
	MyString *str2 = myStringAlloc();
	myStringSetFromCString(str2, "hi again");
	MyString *result = NULL;

	int got = myStringCatTo(str1, str2, result);
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);
	myStringFree(str2);
	str2 = NULL;

//	test #2: src is NULL - FAIL
	str1 = myStringAlloc();
	myStringSetFromCString(str1, "you again");

	got = myStringCatTo(str1, str2, result);
	printf("   - test #2: expected %d, got %d.\n", FAIL, got);
	myStringFree(str1);
	str1 = NULL;

//	test #3: result is NULL - PASS
	str1 = myStringAlloc();
	myStringSetFromCString(str1, "you again");
	str2 = myStringAlloc();
	myStringSetFromCString(str2, " whats up?");

	got = myStringCatTo(str1, str2, result);
	printf("   - test #3: expected %d, got %d.\n", PASS, got);
	myStringFree(str1);
	str1 = NULL;
	myStringFree(str2);
	str2 = NULL;
	myStringFree(result);
	result = NULL;

//	test #4: str1._arr is NULL
//	test #5: str2._arr is NULL
//	test #6: result._arr is NULL
//	test #6: both MyStrings are initialized with an array, not allocated.
}

void myStringToCStringDriver()
{
	printf("++ Runing %s:\n", __func__);

//	test #1: str is NULL - FAIL
	MyString *str = NULL;

	char *result = (char *) malloc(1);
	result = myStringToCString(str);
	int got = PASS;
	if (result == NULL)
	{
		got = FAIL;
	}
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);
	free(result);
	result = NULL;

//	test #2: str is empty - PASS
	str = myStringAlloc();

	result = (char *) malloc(1);
	result = myStringToCString(str);
	got = FAIL;
	if (result != NULL)
	{
		got = PASS;
	}
	printf("   - test #2: expected %d, got %d.\n", PASS, got);
	free(result);
	result = NULL;
	myStringFree(str);
	str = NULL;

//	test #3: str is contains a meaningful string - PASS
	str = myStringAlloc();
	myStringSetFromCString(str, "great fun");

	result = (char *) malloc(str->_length + 1);
	result = myStringToCString(str);
	got = FAIL;
	if (result != NULL)
	{
		got = PASS;
	}
	printf("   - test #3: expected %d, got %d.\n", PASS, got);
	free(result);
	result = NULL;
	myStringFree(str);
	str = NULL;
}

void myStringSetFromIntDriver()
{
	printf("++ Runing %s:\n", __func__);

//	test #1: str is NULL - FAIL
	MyString *str = NULL;

	MyStringRetVal got = myStringSetFromInt(str, 245);
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);

//	test #2: str is an empty string, n is a positive int - PASS
	str = myStringAlloc();

	got = myStringSetFromInt(str, 564);
	printf("   - test #2: expected %d, got %d.\n", PASS, got);
	myStringFree(str);
	str = NULL;

//	test #3: str is an empty string, n is a negative int - PASS
	str = myStringAlloc();

	got = myStringSetFromInt(str, -2564);
	printf("   - test #3: expected %d, got %d.\n", PASS, got);
	myStringFree(str);
	str = NULL;

//	test #4: str is an empty string, n is zero - PASS
	str = myStringAlloc();

	got = myStringSetFromInt(str, 0);
	printf("   - test #4: expected %d, got %d.\n", PASS, got);
	myStringFree(str);
	str = NULL;

//	test #5: str is not empty, n is a positive int - PASS
	str = myStringAlloc();
	myStringSetFromCString(str, "change me");

	got = myStringSetFromInt(str, 4567);
	printf("   - test #5: expected %d, got %d.\n\n", PASS, got);
	myStringFree(str);
	str = NULL;
}

void myStringToIntDriver()
{
	printf("++ Runing %s:\n", __func__);

//	test #1: str is NULL - FAIL
	MyString *str = NULL;

	int result = myStringToInt(str);
	int got = PASS;
	if (result == -999)
	{
		got = FAIL;
	}
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);

//	test #2: str is empty - PASS
	str = myStringAlloc();

	result = myStringToInt(str);
	got = FAIL;
	if (result == 0)
	{
		got = PASS;
	}
	printf("   - test #2: expected %d, got %d.\n", PASS, got);
	myStringFree(str);
	str = NULL;

//	test #3: str is a positive int - PASS
	str = myStringAlloc();
	myStringSetFromInt(str, 6345);

	result = myStringToInt(str);
	got = FAIL;
	if (result == 6345)
	{
		got = PASS;
	}
	printf("   - test #3: expected %d, got %d.\n", PASS, got);
	myStringFree(str);
	str = NULL;

//	test #4: str is a negative int - PASS
	str = myStringAlloc();
	myStringSetFromInt(str, -287);

	result = myStringToInt(str);
	got = FAIL;
	if (result == -287)
	{
		got = PASS;
	}
	printf("   - test #4: expected %d, got %d.\n\n", PASS, got);
	myStringFree(str);
	str = NULL;
}

void myStringCompareDriver()
{
	printf("++ Runing %s:\n", __func__);

//	test #1: str1 is NULL - FAIL
	MyString *str1 = NULL;
	MyString *str2 = myStringAlloc();

	int result = myStringCompare(str1, str2);
	int got = PASS;
	if (result == -999)
	{
		got = FAIL;
	}
	printf("   - test #1: expected %d, got %d.\n", FAIL, got);
	myStringFree(str2);
	str2 = NULL;

	//	test #2: str2 is NULL - FAIL
	str1 = myStringAlloc();

	result = myStringCompare(str1, str2);
	got = PASS;
	if (result == -999)
	{
		got = FAIL;
	}
	printf("   - test #2: expected %d, got %d.\n", FAIL, got);
	myStringFree(str1);
	str1 = NULL;

	//	test #3: str1 is greater than str2 - PASS
	str1 = myStringAlloc();
	myStringSetFromCString(str1, "greater");
	str2 = myStringAlloc();
	myStringSetFromCString(str2, "grb");

	result = myStringCompare(str1, str2);
	got = FAIL;
	if (result > 0)
	{
		got = PASS;
	}
	printf("   - test #3: expected %d, got %d.\n\n", PASS, got);
	myStringFree(str1);
	str1 = NULL;
	myStringFree(str2);
	str2 = NULL;

	// todo more tests
}

int main()
{
//	reallocMyStringDriver();
//	myStringAllocFreeDriver();
//	myStringSetFromCStringDriver();
//	myStringSetFromMyStringDriver();
//	myStringCloneDriver();
//	myStringFilterDriver();
//	myStringLenDriver();
	myStringCatDriver();
	myStringCatToDriver();
//	myStringToCStringDriver();
//	myStringSetFromIntDriver();
//	myStringToIntDriver();
//	myStringCompareDriver();

	return 0;
}

#endif // NDEBUG