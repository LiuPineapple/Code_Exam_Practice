// 更改了答案中的第一种解法，使用书中否定的会引起死循环的解法，进行修改，得到正确解法
#include<iostream>
using namespace std;

//int NumberOf1_Solution1(int n)
//{
//	int a = 1;
//	int count = 0;
//	while (a)
//	{
//		if (a&n)
//			count++;
//		a = a << 1;
//	}
//	return count;
//}
int NumberOf1_Solution1(int n)
{
	int count = 0;
	for (int i = 0; i < 32; i++)
	{
		if ((n >> i) & 1)
			count++;
	}
	return count;
}

int NumberOf1_Solution2(int n)
{
	int count = 0;
	while (n)
	{
		count++;
		n = n&(n - 1);
	}
	return count;
}

// ====================测试代码====================
void Test(int number, unsigned int expected)
{
	int actual = NumberOf1_Solution1(number);
	if (actual == expected)
		printf("Solution1: Test for %p passed.\n", number);
	else
		printf("Solution1: Test for %p failed.\n", number);

	actual = NumberOf1_Solution2(number);
	if (actual == expected)
		printf("Solution2: Test for %p passed.\n", number);
	else
		printf("Solution2: Test for %p failed.\n", number);

	printf("\n");
}

int main(int argc, char* argv[])
{
	// 输入0，期待的输出是0
	Test(0, 0);

	// 输入1，期待的输出是1
	Test(1, 1);

	// 输入10，期待的输出是2
	Test(10, 2);

	// 输入0x7FFFFFFF，期待的输出是31
	Test(0x7FFFFFFF, 31);

	// 输入0xFFFFFFFF（负数），期待的输出是32
	Test(0xFFFFFFFF, 32);

	// 输入0x80000000（负数），期待的输出是1
	Test(0x80000000, 1);
	system("pause");
	return 0;
}

