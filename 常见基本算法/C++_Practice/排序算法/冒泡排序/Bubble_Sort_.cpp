 //冒泡排序 
//输入为数组及其长度
#include<iostream>
using namespace std;
 void Bubble_Sort(int arr[], int size_arr)
 {
	if (arr == NULL) return;
    if (size_arr <= 1) return;
	for (int i = 0; i < size_arr - 1; i++)
	{
		for (int j = 0; j < size_arr - 1 - i; j++)
		{
			if (arr[j] > arr[j + 1])//若为降序则将大于号改为小于号
			{
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				//或者直接使用swap(arr[j],arr[j+1])
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
	 Bubble_Sort(arr,10);
	 for (int i = 0; i < 10; i++)
	 {
		 cout << arr[i] << " ";
	 }
	 cout << endl;
	 system("pause");
 }
