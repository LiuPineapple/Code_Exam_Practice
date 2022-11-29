//在答案代码基础上修改
//while里面有些不同
//时间复杂度也应该为O(n)
//空间复杂度也应该为O(1)

#include<iostream>
using namespace std;

bool find_number(int *matrix, int rows, int columns, int number)
{
	bool bound = false;
	int row = 0;
	int column = columns - 1;
	if (matrix != nullptr && rows > 0 && columns > 0)
	{
		while (row < rows&&column >= 0)
		{

			if (matrix[row*columns + column] == number)
			{
				return true;

			}
			else if (matrix[row*columns + column] > number)
				column--;
			else
				row++;
		}
	
	}
	return bound;
}

// ====================测试代码====================
void Test(char* testName, int* matrix, int rows, int columns, int number, bool expected)
{
	if (testName != nullptr)
		printf("%s begins: ", testName);

	bool result = find_number(matrix, rows, columns, number);
	if (result == expected)
		printf("Passed.\n");
	else
		printf("Failed.\n");
}

//  1   2   8   9
//  2   4   9   12
//  4   7   10  13
//  6   8   11  15
// 要查找的数在数组中
void Test1()
{
	int matrix[][4] = { { 1, 2, 8, 9 },{ 2, 4, 9, 12 },{ 4, 7, 10, 13 },{ 6, 8, 11, 15 } };
	Test("Test1", (int*)matrix, 4, 4, 7, true);
}

//  1   2   8   9
//  2   4   9   12
//  4   7   10  13
//  6   8   11  15
// 要查找的数不在数组中
void Test2()
{
	int matrix[][4] = { { 1, 2, 8, 9 },{ 2, 4, 9, 12 },{ 4, 7, 10, 13 },{ 6, 8, 11, 15 } };
	Test("Test2", (int*)matrix, 4, 4, 5, false);
}

//  1   2   8   9
//  2   4   9   12
//  4   7   10  13
//  6   8   11  15
// 要查找的数是数组中最小的数字
void Test3()
{
	int matrix[][4] = { { 1, 2, 8, 9 },{ 2, 4, 9, 12 },{ 4, 7, 10, 13 },{ 6, 8, 11, 15 } };
	Test("Test3", (int*)matrix, 4, 4, 1, true);
}

//  1   2   8   9
//  2   4   9   12
//  4   7   10  13
//  6   8   11  15
// 要查找的数是数组中最大的数字
void Test4()
{
	int matrix[][4] = { { 1, 2, 8, 9 },{ 2, 4, 9, 12 },{ 4, 7, 10, 13 },{ 6, 8, 11, 15 } };
	Test("Test4", (int*)matrix, 4, 4, 15, true);
}

//  1   2   8   9
//  2   4   9   12
//  4   7   10  13
//  6   8   11  15
// 要查找的数比数组中最小的数字还小
void Test5()
{
	int matrix[][4] = { { 1, 2, 8, 9 },{ 2, 4, 9, 12 },{ 4, 7, 10, 13 },{ 6, 8, 11, 15 } };
	Test("Test5", (int*)matrix, 4, 4, 0, false);
}

//  1   2   8   9
//  2   4   9   12
//  4   7   10  13
//  6   8   11  15
// 要查找的数比数组中最大的数字还大
void Test6()
{
	int matrix[][4] = { { 1, 2, 8, 9 },{ 2, 4, 9, 12 },{ 4, 7, 10, 13 },{ 6, 8, 11, 15 } };
	Test("Test6", (int*)matrix, 4, 4, 16, false);
}

// 鲁棒性测试，输入空指针
void Test7()
{
	Test("Test7", nullptr, 0, 0, 16, false);
}

int main(int argc, char* argv[])
{
	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	Test6();
	Test7();
	system("pause");
	return 0;
}

