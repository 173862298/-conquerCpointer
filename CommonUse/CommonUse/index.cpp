#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable:4996)//解决strcpy报错
void func1(int *array,int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("array[%d]..%d\n", i, array[i]);
	}
}
void read_slogan(FILE *fp, char **slogan)
{
	char buf[1024];
	int i;
	for (i = 0; i < 7; i++)
	{
		fgets(buf, 1024, fp);
		//删除换行字符
		buf[strlen(buf) - 1] = '\0';
		//分配保存一个标语的内存空间
		slogan[i] = (char*)malloc(sizeof(char)*(strlen(buf) + 1));//字符串以\0结尾
		//复制标语的内容
		strcpy(slogan[i], buf);		
	}
}
int main(void)
{
#if 0 //一、将数组作为函数参数传递
	/* 
	1.在 c语言中，数组不能作为参数进行传递。但可以通过传递指向数组初始元素的指针func1()，使得在函数内部操作数组成为可能。
	2.array[i]是*(array+i)的语法糖。
	3.func1()以参数size来接收数组array的元素个数。因为array只是一个指针，所以函数并不知道调用方数组的元素个数。
	4.可以用sizeof运算符取得数组元素的个数。
	5.sizeof(array)取得数组总和。
	*/
	int array[] = { 1,2,3,4,5 };
	func1(array, sizeof(array)/sizeof(int));
#endif	

#if 0 //二、可变长数组
	/*
	1.一般情况，c语言在编译时必须知道数组的元素个数，但是可以使用malloc()在运行时再为数组申请必要的内存区域，这种叫可变数组.	
	*/
	char buf[256];
	int size;
	int *variable_array;
	int i;
	printf("input array size>");
	fgets(buf, 256, stdin);
	sscanf_s(buf, "%d", &size);//输入需要的内存大小

	variable_array = (int*)malloc(sizeof(int)*size);//malloc得到指针，不是数组

	for (i = 0; i < size; i++)
	{
		variable_array[i] = i;
	}
	for (i = 0; i < size; i++)
	{
		printf("variable_array[%d]..%d\n", i, variable_array[i]);
	}
	printf("================使用realloc修改已分配的可变长数组的大小========================\n");
	variable_array = NULL;
	size = 0;
	char buf2[256];
	while ((fgets(buf2, 256, stdin)!=NULL) && (buf2[0]!='\n'))//fgets函数正确退出判断
	{
		size++;
		variable_array = (int*)realloc(variable_array, sizeof(int)*size);
		sscanf_s(buf2, "%d", &variable_array[size - 1]);
	}
	
	
	for (i = 0; i < size; i++)
	{
		printf("variable_array[%d]..%d\n", i, variable_array[i]);
	}
#endif

#if 1 //三、可变长数组的数组
	/**/
	char *slogan[7];
	int i;
	read_slogan(stdin, slogan);
	//输出读取的标语
	for (i = 0; i < 7; i++)
	{
		printf("%s\n", slogan[i]);
	}

#endif
	system("pause");
	return 0;
}