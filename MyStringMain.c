#include <assert.h>
#include "MyString.h"
#define MAX_INPUT_SIZE 500

#ifdef NDEBUG

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

	// todo assert input?
	FILE *stream = fopen("test.out", "w");

	if (myStringCompare(strA, strB) > 0)
	{
		fprintf(stream, "%s is greater than %s", cStrA, cStrB);
	}
	else
	{
		fprintf(stream, "%s is greater than %s", cStrB, cStrA);
	}

	fclose(stream);

	myStringFree(strA);
	myStringFree(strB);

	return 0;
}

#endif // NDEBUG