#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "read_line.h"

#define ALLOC_SIZE  (256)

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
	/*限制最大长度为1024，如果在主函数中声明多维数组最大限制1024：char slogan[7][1024];此时内存消耗1024×7个字符。可是在此方法中读取1024个字符使用一个临时缓冲区就能解决问题，因为这个数组是临时变量，方法执行结束就会释放。*/
	/*尽管如此在某些情况下，可以使用malloc动态分配读取字符用的缓冲区，空间不足时，用realloc进行扩展。详见read_line.cpp和read_line.h文件*/
	char buf[1024];
	
	for (int i = 0; i < 7; i++)
	{
		fgets(buf, 1024, fp);
		//删除换行字符
		buf[strlen(buf) - 1] = '\0';//strlen计算\n在内的长度，不计算\0
		//分配保存一个标语的内存空间
		slogan[i] = (char*)malloc(sizeof(char)*(strlen(buf) + 1));//字符串以\0结尾
		//复制标语的内容
		strcpy(slogan[i], buf);		
	}
}

char **add_line(char **text_data, char *line, int *line_alloc_num, int *line_num)
{
	assert(*line_alloc_num >= *line_num);
	if (*line_alloc_num == *line_num)
	{
		*text_data = (char*)realloc(text_data, (*line_alloc_num + ALLOC_SIZE)*sizeof(char*));
		*line_alloc_num += ALLOC_SIZE;
	}
	text_data[*line_num] = line;
	(*line_num)++;

	return text_data;
}
char ** read_file(FILE *fp, int *line_num_p)
{
	char **text_data = NULL;
	int line_num = 0;
	int line_alloc_num = 0;
	char *line=NULL;

	/*read_line()中，为共享一些变量，使用文件内的static变量。但这里使用了通过参数传递指针的方式。通过文件内的static变量和全局变量共享数据时，因为无法知道“值在什么地方被改写”，所以在很多情况下通过参数传递指针的方式显得很有效。*/
	while ((line = read_line(fp)) != NULL)
	{
		text_data = add_line(text_data, line, &line_alloc_num, &line_num);
	}
	/*因为不读到文件的最后就无法知道总共的行数，所以在read_file（）中对于指针数组也使用realloc顺序的将其占用的空间加长。*/
	/*将text_data缩小到实际需要的大小*/
	*text_data = (char*)realloc(text_data, line_num*sizeof(char*));
	*line_num_p = line_num;

	return text_data;
}

void func(int(*hope)[3])
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 3; j++)
		{
			printf("%d,", hope[i][j]);
		}
		putchar('\n');
	}
}
int main(void)//第一种main函数
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
	//输入需要的内存大小
	fgets(buf, 256, stdin);//fgets从 stdin 流中读取 256 个字符存储到字符指针变量 buf 所指向的内存空间。它的返回值是一个指针，指向字符串中第一个字符的地址。
	sscanf_s(buf, "%d", &size);//sscanf将参数buf的字符串根据参数format字符串来转换并格式化数据。格式转换形式请参考scanf()。转换后的结果存于对应的参数size内。

	variable_array = (int*)malloc(sizeof(int)*size);//malloc得到指针，不是数组

	for (i = 0; i < size; i++)
	{
		variable_array[i] = i;
	}
	for (i = 0; i < size; i++)
	{
		printf("variable_array[%d]..%d\n", i, variable_array[i]);
	}
	printf("==每当用户输入int值，就使用realloc修改已分配的可变长数组的大小=加1=======================\n");
	variable_array = NULL;
	size = 0;
	char buf2[256];
	while ((fgets(buf2, 256, stdin)!=NULL) && (buf2[0]!='\n'))//fgets函数正确退出判断,//按回车结束循环
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

#if 0 //三、可变长数组的数组 slogan
	
	char *slogan[7];//包含7个指针的数组
	
	read_slogan(stdin, slogan);
	//输出读取的标语
	for (int i = 0; i < 7; i++)
	{
		printf("%s\n", slogan[i]);
	}

#endif

#if 0 //四、可变长数组的可变长数组   待定
	char **text_data;
	int line_num;
	int i;

	text_data = read_file(stdin, &line_num);

	for (i = 0; i < line_num; i++)
	{
		printf("%s\n", text_data[i]);
	}
#endif
#if 0 //六、通过参数返回指针  待定
	/*在read_line::read_line()中，将读取的行作为返回值返回，如果到达文件的终点则返回null。可是，作为返回值的形式，read_line()返回的时通过malloc分配的内存区域。代码中并没有做返回值检查。如果真的想要让read_line()成为通用的函数，就必须好好做返回值检查，并且能向调用方返回函数的处理状态。
	对以上，进行代码修订，详见read_line2.h   read_line2.cpp*/
#endif
#if 0 //七、将多维数组作为函数的参数传递
	/*在C中，其实不存在多维数组，其实时数组的数组。
	将类型T的数组作为参数进行传递时，可以传递“指向T的指针”。因此将“数组的数组”作为参数进行传递，可以考虑传递“指向数组的指针”。*/
	int hope[][3] = { {1,2,3},{4,5,6 },{7,8,9},{10,11,12} };
	func(hope);
#endif
	system("pause");
	return 0;
}
void type_one_file(FILE *fp)
{
	int ch;
	while ((ch = getc(fp)) != EOF)
	{
		putchar(ch);
	}
}
#if 0
/*五、取得命令行参数*/
/*argv在内存中的结构
二级指针    一级指针      
argv-------->ptr1------->字符串
			 ptr2------->字符串
			 ptr3------->字符串
			  ...
*/
/*argv[0]中保存了命令名自身，在程序输出错误提示信息时，或需要通过命令名称改变程序的行为时，会经常使用arg[0]。
	argc中保存了参数的个数（包含了arg[0]）。实际上，从ANSI C之后，会保证argv[argc]肯定为null，所以完全可以没有argc，但是目前仍然还有很多人习惯性的引用argc。*/
int main(int argc, char **argv)//第二种main函数
{
	if (argc == 1)
	{
		type_one_file(stdin);
	}
	else
	{
		int i;
		FILE *fp;
		for (i = 1; i < argc; i++)
		{
			fp = fopen(argv[i], "rb");
			if (fp == NULL)
			{
				fprintf(stderr, "%s:%s can not open.\n", argv[0], argv[i]);
				exit(1);
			}
			type_one_file(fp);
			fclose(fp);
		}
	}
	system("pause");
	return 0;
}

#endif