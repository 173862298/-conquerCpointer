#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable:4996)//���strcpy����
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
		//ɾ�������ַ�
		buf[strlen(buf) - 1] = '\0';
		//���䱣��һ��������ڴ�ռ�
		slogan[i] = (char*)malloc(sizeof(char)*(strlen(buf) + 1));//�ַ�����\0��β
		//���Ʊ��������
		strcpy(slogan[i], buf);		
	}
}
int main(void)
{
#if 0 //һ����������Ϊ������������
	/* 
	1.�� c�����У����鲻����Ϊ�������д��ݡ�������ͨ������ָ�������ʼԪ�ص�ָ��func1()��ʹ���ں����ڲ����������Ϊ���ܡ�
	2.array[i]��*(array+i)���﷨�ǡ�
	3.func1()�Բ���size����������array��Ԫ�ظ�������Ϊarrayֻ��һ��ָ�룬���Ժ�������֪�����÷������Ԫ�ظ�����
	4.������sizeof�����ȡ������Ԫ�صĸ�����
	5.sizeof(array)ȡ�������ܺ͡�
	*/
	int array[] = { 1,2,3,4,5 };
	func1(array, sizeof(array)/sizeof(int));
#endif	

#if 0 //�����ɱ䳤����
	/*
	1.һ�������c�����ڱ���ʱ����֪�������Ԫ�ظ��������ǿ���ʹ��malloc()������ʱ��Ϊ���������Ҫ���ڴ��������ֽпɱ�����.	
	*/
	char buf[256];
	int size;
	int *variable_array;
	int i;
	printf("input array size>");
	fgets(buf, 256, stdin);
	sscanf_s(buf, "%d", &size);//������Ҫ���ڴ��С

	variable_array = (int*)malloc(sizeof(int)*size);//malloc�õ�ָ�룬��������

	for (i = 0; i < size; i++)
	{
		variable_array[i] = i;
	}
	for (i = 0; i < size; i++)
	{
		printf("variable_array[%d]..%d\n", i, variable_array[i]);
	}
	printf("================ʹ��realloc�޸��ѷ���Ŀɱ䳤����Ĵ�С========================\n");
	variable_array = NULL;
	size = 0;
	char buf2[256];
	while ((fgets(buf2, 256, stdin)!=NULL) && (buf2[0]!='\n'))//fgets������ȷ�˳��ж�
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

#if 1 //�����ɱ䳤���������
	/**/
	char *slogan[7];
	int i;
	read_slogan(stdin, slogan);
	//�����ȡ�ı���
	for (i = 0; i < 7; i++)
	{
		printf("%s\n", slogan[i]);
	}

#endif
	system("pause");
	return 0;
}