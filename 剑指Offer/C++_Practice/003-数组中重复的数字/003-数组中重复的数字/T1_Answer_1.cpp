//在答案代码基础上修改
//使得输出的是重复的值
//时间复杂度也为O(n)
//空间复杂度也为O(1)

#include<iostream>
using namespace std;

int find_duplicate(int arr[],int length)
{
	if (arr == nullptr || length <= 0)
	{
		return - 1;
	}
	for (int i = 0; i < length; ++i)
	{
		if (arr[i] < 0 || arr[i] > length - 1)
			return -1;
	}
	for (int i = 0; i < length; i++)
	{
		while (i != arr[i])
		{
			if (arr[i] == arr[arr[i]])
			{
				return arr[i];
			}
			int tempt = arr[i];
			arr[i] = arr[tempt];
			arr[tempt] = tempt;
		}

	}
	return -1;
}

// ====================测试代码====================
bool contains(int array[], int length, int number)
{
	for (int i = 0; i < length; ++i)
	{
		if (array[i] == number)
			return true;
	}

	return false;
}

void test(char* testName, int numbers[], int lengthNumbers, int expected[], int expectedExpected)
{
	printf("%s begins: ", testName);

	int validInput = find_duplicate(numbers, lengthNumbers);
	if (expected != nullptr)
	{
		if (contains(expected, expectedExpected, validInput))
			printf("Passed.\n");
		else
			printf("FAILED.\n");
	}
	else
	{
		if (validInput == -1) 
		{
			printf("Passed.\n");
		}
		else
			printf("FAILED.\n");
		    
	}

}

// 重复的数字是数组中最小的数字
void test1()
{
	int numbers[] = { 2, 1, 3, 1, 4 };
	int duplications[] = { 1 };
	test("Test1", numbers, sizeof(numbers) / sizeof(int), duplications, sizeof(duplications) / sizeof(int));
}

// 重复的数字是数组中最大的数字
void test2()
{
	int numbers[] = { 2, 4, 3, 1, 4 };
	int duplications[] = { 4 };
	test("Test2", numbers, sizeof(numbers) / sizeof(int), duplications, sizeof(duplications) / sizeof(int));
}

// 数组中存在多个重复的数字
void test3()
{
	int numbers[] = { 2, 4, 2, 1, 4 };
	int duplications[] = { 2, 4 };
	test("Test3", numbers, sizeof(numbers) / sizeof(int), duplications, sizeof(duplications) / sizeof(int));
}

// 没有重复的数字
void test4()
{
	int numbers[] = { 2, 1, 3, 0, 4 };
	int duplications[] = { -1 }; // not in use in the test function
	test("Test4", numbers, sizeof(numbers) / sizeof(int), duplications, sizeof(duplications) / sizeof(int));
}

// 没有重复的数字
void test5()
{
	int numbers[] = { 2, 1, 3, 5, 4 };
	int duplications[] = { -1 }; // not in use in the test function
	test("Test5", numbers, sizeof(numbers) / sizeof(int), duplications, sizeof(duplications) / sizeof(int));
}

// 无效的输入
void test6()
{
	int* numbers = nullptr;
	int duplications[] = { -1 }; // not in use in the test function
	test("Test6", numbers, 0, duplications, sizeof(duplications) / sizeof(int));
}


void main()
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	system("pause");
}
