//在答案代码基础上修改了动态规划的方法
//时间复杂度也为O(n……2)
//空间复杂度也为O(n)
//贪婪算法时间和空间复杂度均为O(1)

#include<iostream>
using namespace std;

//int maxProductAfterCutting_solution1(int length)
//{
//	if (length <= 1)
//		return 0;
//	if (length == 2)
//		return 1;
//	if (length == 3)
//		return 2;
//	int *products = new int[length + 1];
//	products[0] = 0;
//	products[1] = 1;
//	products[2] = 2;
//	products[3] = 3;//当n>=4的时候，max(n,f(n))=f(n)
//	for (int i = 4; i <= length; i++)
//	{
//		int max = 0;
//		for (int j = 1; j <= i / 2; j++)
//		{
//			int product = products[j] * products[i - j];
//			if (product > max)
//				max = product;
//		}
//		products[i] = max;
//	}
//	return products[length];
//}
int max(int a, int b)
{
	if (a >= b)
		return a;
	return b;
}
int maxProductAfterCutting_solution1(int length)
{
	if (length <= 1)
		return 0;
	int *products = new int[length + 1];
	products[0] = 0;
	products[1] = 0;
	for (int i = 2; i <= length; i++)
	{
		int maxx = 0;
		for (int j = 1; j <= i / 2; j++)
		{
			int product = max(j, products[j])*max(i - j, products[i - j]);
			if (product > maxx)
				maxx = product;
		}
		products[i] = maxx;
	}
	return products[length];
}

int maxProductAfterCutting_solution2(int length)
{
	if (length <= 1)
		return 0;
	if (length == 2)
		return 1;
	if (length == 3)
		return 2;
	int number_of_3 = length / 3;
	if (length - number_of_3 * 3 == 1)
		number_of_3--;
	int number_of_2 = (length - number_of_3 * 3) / 2;
	return (int)(pow(3, number_of_3)) * (int)(pow(2, number_of_2));

}

// ====================测试代码====================
void test(const char* testName, int length, int expected)
{
	int result1 = maxProductAfterCutting_solution1(length);
	if (result1 == expected)
		std::cout << "Solution1 for " << testName << " passed." << std::endl;
	else
		std::cout << "Solution1 for " << testName << " FAILED." << std::endl;

	int result2 = maxProductAfterCutting_solution2(length);
	if (result2 == expected)
		std::cout << "Solution2 for " << testName << " passed." << std::endl;
	else
		std::cout << "Solution2 for " << testName << " FAILED." << std::endl;
}

void test1()
{
	int length = 1;
	int expected = 0;
	test("test1", length, expected);
}

void test2()
{
	int length = 2;
	int expected = 1;
	test("test2", length, expected);
}

void test3()
{
	int length = 3;
	int expected = 2;
	test("test3", length, expected);
}

void test4()
{
	int length = 4;
	int expected = 4;
	test("test4", length, expected);
}

void test5()
{
	int length = 5;
	int expected = 6;
	test("test5", length, expected);
}

void test6()
{
	int length = 6;
	int expected = 9;
	test("test6", length, expected);
}

void test7()
{
	int length = 7;
	int expected = 12;
	test("test7", length, expected);
}

void test8()
{
	int length = 8;
	int expected = 18;
	test("test8", length, expected);
}

void test9()
{
	int length = 9;
	int expected = 27;
	test("test9", length, expected);
}

void test10()
{
	int length = 10;
	int expected = 36;
	test("test10", length, expected);
}

void test11()
{
	int length = 50;
	int expected = 86093442;
	test("test11", length, expected);
}

int main(int agrc, char* argv[])
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
	test10();
	test11();
	system("pause");
	return 0;
}

