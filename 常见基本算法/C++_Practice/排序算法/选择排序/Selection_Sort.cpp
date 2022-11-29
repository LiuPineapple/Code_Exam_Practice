//选择排序 
//输入为数组及其长度
#include<iostream>
using namespace std;
void Selection_Sort(int arr[], int size_arr)
{
	if (arr == NULL) return;
	if (size_arr <= 1) return;
	int index;
	for (int i = 0; i < size_arr - 1; i++)
	{
		index = i;
		for (int j = i+1; j < size_arr; j++)
		{
			if (arr[index] > arr[j])//若为降序则将大于号改为小于号
			{
				index = j;
			}
		}
		swap(arr[index], arr[i]);
	}
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
	Selection_Sort(arr, 10);
	for (int i = 0; i < 10; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
	system("pause");
}
