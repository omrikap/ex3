#include <assert.h>
#include "MyString.h"
/********************************************************************************
 * @file MyStringMain.c
 * @author  omri.kaplan
 *
 * @date 23 Aug 2015
 *
 * @brief A little program to compare strings.
 *
 * @section LICENSE
 * This program is not a free software;
 *
 * @section DESCRIPTION
 * This program is able to tell if two strings are different from each other, and
 * to order them lexicographically.
 ********************************************************************************/
//#ifdef NDEBUG
#define MAX_INPUT_SIZE 500

/**
 * @brief A little program to compare strings.
 */
int main()
{
	char cStrA[MAX_INPUT_SIZE];
	char cStrB[MAX_INPUT_SIZE];
	puts("this program compares tow strings.");
	puts("Please enter the first string:");
	scanf("%s", cStrA);
	puts("Please enter the second string:");
	scanf("%s", cStrB);

	MyString *strA = myStringAlloc();
	myStringSetFromCString(strA, cStrA);

	MyString *strB = myStringAlloc();
	myStringSetFromCString(strB, cStrB);

	FILE *stream = fopen("test.out", "w");

	if (myStringCompare(strA, strB) > 0)
	{
		fprintf(stream, "%s is greater than %s\n", cStrA, cStrB);
	}
	else
	{
		fprintf(stream, "%s is greater than %s\n", cStrB, cStrA);
	}

	fclose(stream);

	myStringFree(strA);
	myStringFree(strB);

	return 0;
}

#endif // NDEBUG