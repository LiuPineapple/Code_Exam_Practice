//快速排序 
//输入为数组及其长度
#include<iostream>
using namespace std;
void Quick_Sort(int arr[], int low, int high) 
{
	if (high <= low) return;
	int i = low;
	int j = high + 1;
	int key = arr[low];
	while (true)
	{
		/*从左向右找比key大的值*/
		while (arr[++i] < key)
		{
			if (i == high) {
				break;
			}
		}
		/*从右向左找比key小的值*/
		while (arr[--j] > key)
		{
			if (j == low) {
				break;
			}
		}
		if (i >= j) break;
		/*交换i,j对应的值*/
		int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
	/*中枢值与j对应值交换*/
	int temp = arr[low];
	arr[low] = arr[j];
	arr[j] = temp;
	Quick_Sort(arr, low, j - 1);
	Quick_Sort(arr, j + 1, high);
}
//测试
int main()
{
	int arr[10] = { 1,4,6,8,0,2,5,7,9,3 };
	cout << "排序前" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
	cout << "排序后" << endl;
	Quick_Sort(arr,0,10);
	for (int i = 0; i < 10; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
	system("pause");
}
