//二分查找适用于排序数组，时间复杂度为O(log(n))
//要考虑四种特殊情况:
//-数组长度为0
//- 数组长度为1
//- 数组没有旋转
//- 二分查找的两边和中间相等

#include<exception>
#include<iostream>
#include<cstdio>
using namespace std;

//顺序查找
int MinInorder(int * number, int index1, int index2)
{
	int result = number[index1];
	for (int i = index1+1; i <= index2; i++)
	{
		if (result > number[i])
			result = number[i];
	}
	return result;
}


//二分查找
int Min(int * number, int length)
{
	if (number == nullptr || length <= 0)
		throw new exception("Invalide Input");
	int index1 = 0;
	int index2 = length - 1;
	int indexM = index1;
	while (number[index1] >= number[index2])
	{
		if (index2 - index1 == 1)
		{
			indexM = index2;
			break;
		}
		indexM = (index1 + index2) / 2;
		if (number[index2] == number[index1] && number[index1] == number[indexM])
			return MinInorder(number, index1, index2);
			return 1;
		if (number[indexM] >= number[index1])
			index1 = indexM;
		if (number[indexM] <= number[index2])
			index2 = indexM;
	}
	return number[indexM];
}

	// ====================测试代码====================
	void Test(int* numbers, int length, int expected)
	{
		int result = 0;
		try
		{
			result = Min(numbers, length);

			for (int i = 0; i < length; ++i)
				printf("%d ", numbers[i]);

			if (result == expected)
				printf("\tpassed\n");
			else
				printf("\tfailed\n");
		}
		catch (...)
		{
			if (numbers == nullptr)
				printf("Test passed.\n");
			else
				printf("Test failed.\n");
		}
	}

	int main(int argc, char* argv[])
	{
		// 典型输入，单调升序的数组的一个旋转
		int array1[] = { 3, 4, 5, 1, 2 };
		Test(array1, sizeof(array1) / sizeof(int), 1);

		// 有重复数字，并且重复的数字刚好的最小的数字
		int array2[] = { 3, 4, 5, 1, 1, 2 };
		Test(array2, sizeof(array2) / sizeof(int), 1);

		// 有重复数字，但重复的数字不是第一个数字和最后一个数字
		int array3[] = { 3, 4, 5, 1, 2, 2 };
		Test(array3, sizeof(array3) / sizeof(int), 1);

		// 有重复的数字，并且重复的数字刚好是第一个数字和最后一个数字
		int array4[] = { 1, 0, 1, 1, 1 };
		Test(array4, sizeof(array4) / sizeof(int), 0);

		// 单调升序数组，旋转0个元素，也就是单调升序数组本身
		int array5[] = { 1, 2, 3, 4, 5 };
		Test(array5, sizeof(array5) / sizeof(int), 1);

		// 数组中只有一个数字
		int array6[] = { 2 };
		Test(array6, sizeof(array6) / sizeof(int), 2);

		// 输入nullptr
		Test(nullptr, 0, 0);
		system("pause");
		return 0;
	}

