//插入排序 
//输入为数组及其长度
#include<iostream>
using namespace std;
void Insert_Sort(int arr[], int size_arr)
{
	if (arr == NULL) return;
	if (size_arr <= 1) return;
	for (int i = 1; i < size_arr; i++)
	{
		for (int j = i; j >0; j--)
		{
			if (arr[j - 1]>arr[j])//若为降序则将大于号改为小于号
			{
				swap(arr[j], arr[j-1]);
			}
		}
		
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
	Insert_Sort(arr, 10);
	for (int i = 0; i < 10; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
	system("pause");
}
