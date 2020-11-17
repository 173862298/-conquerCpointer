#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "read_line.h"

#define ALLOC_SIZE  (256)

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
	/*������󳤶�Ϊ1024���������������������ά�����������1024��char slogan[7][1024];��ʱ�ڴ�����1024��7���ַ��������ڴ˷����ж�ȡ1024���ַ�ʹ��һ����ʱ���������ܽ�����⣬��Ϊ�����������ʱ����������ִ�н����ͻ��ͷš�*/
	/*���������ĳЩ����£�����ʹ��malloc��̬�����ȡ�ַ��õĻ��������ռ䲻��ʱ����realloc������չ�����read_line.cpp��read_line.h�ļ�*/
	char buf[1024];
	
	for (int i = 0; i < 7; i++)
	{
		fgets(buf, 1024, fp);
		//ɾ�������ַ�
		buf[strlen(buf) - 1] = '\0';//strlen����\n���ڵĳ��ȣ�������\0
		//���䱣��һ��������ڴ�ռ�
		slogan[i] = (char*)malloc(sizeof(char)*(strlen(buf) + 1));//�ַ�����\0��β
		//���Ʊ��������
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

	/*read_line()�У�Ϊ����һЩ������ʹ���ļ��ڵ�static������������ʹ����ͨ����������ָ��ķ�ʽ��ͨ���ļ��ڵ�static������ȫ�ֱ�����������ʱ����Ϊ�޷�֪����ֵ��ʲô�ط�����д���������ںܶ������ͨ����������ָ��ķ�ʽ�Եú���Ч��*/
	while ((line = read_line(fp)) != NULL)
	{
		text_data = add_line(text_data, line, &line_alloc_num, &line_num);
	}
	/*��Ϊ�������ļ��������޷�֪���ܹ���������������read_file�����ж���ָ������Ҳʹ��realloc˳��Ľ���ռ�õĿռ�ӳ���*/
	/*��text_data��С��ʵ����Ҫ�Ĵ�С*/
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
int main(void)//��һ��main����
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
	//������Ҫ���ڴ��С
	fgets(buf, 256, stdin);//fgets�� stdin ���ж�ȡ 256 ���ַ��洢���ַ�ָ����� buf ��ָ����ڴ�ռ䡣���ķ���ֵ��һ��ָ�룬ָ���ַ����е�һ���ַ��ĵ�ַ��
	sscanf_s(buf, "%d", &size);//sscanf������buf���ַ������ݲ���format�ַ�����ת������ʽ�����ݡ���ʽת����ʽ��ο�scanf()��ת����Ľ�����ڶ�Ӧ�Ĳ���size�ڡ�

	variable_array = (int*)malloc(sizeof(int)*size);//malloc�õ�ָ�룬��������

	for (i = 0; i < size; i++)
	{
		variable_array[i] = i;
	}
	for (i = 0; i < size; i++)
	{
		printf("variable_array[%d]..%d\n", i, variable_array[i]);
	}
	printf("==ÿ���û�����intֵ����ʹ��realloc�޸��ѷ���Ŀɱ䳤����Ĵ�С=��1=======================\n");
	variable_array = NULL;
	size = 0;
	char buf2[256];
	while ((fgets(buf2, 256, stdin)!=NULL) && (buf2[0]!='\n'))//fgets������ȷ�˳��ж�,//���س�����ѭ��
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

#if 0 //�����ɱ䳤��������� slogan
	
	char *slogan[7];//����7��ָ�������
	
	read_slogan(stdin, slogan);
	//�����ȡ�ı���
	for (int i = 0; i < 7; i++)
	{
		printf("%s\n", slogan[i]);
	}

#endif

#if 0 //�ġ��ɱ䳤����Ŀɱ䳤����   ����
	char **text_data;
	int line_num;
	int i;

	text_data = read_file(stdin, &line_num);

	for (i = 0; i < line_num; i++)
	{
		printf("%s\n", text_data[i]);
	}
#endif
#if 0 //����ͨ����������ָ��  ����
	/*��read_line::read_line()�У�����ȡ������Ϊ����ֵ���أ���������ļ����յ��򷵻�null�����ǣ���Ϊ����ֵ����ʽ��read_line()���ص�ʱͨ��malloc������ڴ����򡣴����в�û��������ֵ��顣��������Ҫ��read_line()��Ϊͨ�õĺ������ͱ���ú�������ֵ��飬����������÷����غ����Ĵ���״̬��
	�����ϣ����д����޶������read_line2.h   read_line2.cpp*/
#endif
#if 0 //�ߡ�����ά������Ϊ�����Ĳ�������
	/*��C�У���ʵ�����ڶ�ά���飬��ʵʱ��������顣
	������T��������Ϊ�������д���ʱ�����Դ��ݡ�ָ��T��ָ�롱����˽�����������顱��Ϊ�������д��ݣ����Կ��Ǵ��ݡ�ָ�������ָ�롱��*/
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
/*�塢ȡ�������в���*/
/*argv���ڴ��еĽṹ
����ָ��    һ��ָ��      
argv-------->ptr1------->�ַ���
			 ptr2------->�ַ���
			 ptr3------->�ַ���
			  ...
*/
/*argv[0]�б����������������ڳ������������ʾ��Ϣʱ������Ҫͨ���������Ƹı�������Ϊʱ���ᾭ��ʹ��arg[0]��
	argc�б����˲����ĸ�����������arg[0]����ʵ���ϣ���ANSI C֮�󣬻ᱣ֤argv[argc]�϶�Ϊnull��������ȫ����û��argc������Ŀǰ��Ȼ���кܶ���ϰ���Ե�����argc��*/
int main(int argc, char **argv)//�ڶ���main����
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