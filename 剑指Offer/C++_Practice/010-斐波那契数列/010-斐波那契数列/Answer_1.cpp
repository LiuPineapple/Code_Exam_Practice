//用C++重写原答案

#include<iostream>
using namespace std;

//方法1，递归，函数简洁，但效率太差
int Fibonacci_Solution1(int n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return Fibonacci_Solution1(n - 1) + Fibonacci_Solution1(n-2);
}
//方法二，循环，时间复杂度为O(n)
int Fibonacci_Solution2(int n)
{
	int mini1 = 0;
	int mini2 = 1;
	int result = 0;
	if (n == 0)
		return mini1;
	if (n == 1)
		return mini2;
	for (int i = 2; i <= n; i++)
	{
		result = mini1 + mini2;
		mini1 = mini2;
		mini2 = result;
	}
	return result;
}
//第三种方法，暂时不写
int Fibonacci_Solution3(int n)
{
	return Fibonacci_Solution2(n);
}
// ====================测试代码====================
void Test(int n, int expected)
{
	if (Fibonacci_Solution1(n) == expected)
		printf("Test for %d in solution1 passed.\n", n);
	else
		printf("Test for %d in solution1 failed.\n", n);

	if (Fibonacci_Solution2(n) == expected)
		printf("Test for %d in solution2 passed.\n", n);
	else
		printf("Test for %d in solution2 failed.\n", n);

	if (Fibonacci_Solution3(n) == expected)
		printf("Test for %d in solution3 passed.\n", n);
	else
		printf("Test for %d in solution3 failed.\n", n);
}

int main(int argc, char* argv[])
{
	Test(0, 0);
	Test(1, 1);
	Test(2, 1);
	Test(3, 2);
	Test(4, 3);
	Test(5, 5);
	Test(6, 8);
	Test(7, 13);
	Test(8, 21);
	Test(9, 34);
	Test(10, 55);

	Test(40, 102334155);
	system("pause");
	return 0;
}