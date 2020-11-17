#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define ALLOC_SIZE  (256)
#pragma warning(disable:4996)

/*��ȡ�еĻ��壬��Ҫʱ������չ���������򲻻ᱻ��С������free_buffer�ͷš�*/
/*st_line_bufferָ�򻺳����ĳ�ʼλ�ã��û��������ڴ����ʱ��ȡ���ַ��������������ռ䲻��ʱ������������չALLOC_SIZE��С������*/
static char *st_line_buffer = NULL;

/*��st_line_bufferǰ����������ڴ�����Ĵ�С*/
static int st_current_buffer_size = 0;

/*st_line_buffer�����ڱ�����ַ��Ĵ�С*/
static int st_current_used_size = 0;

/*���б�Ҫ����չst_line_bufferǰ�����ڴ�����
��st_line_bufferĩβ׷��һ���ַ�ch*/
static void add_character(int ch)
{
	/*�˺���ÿ�α����ã�st_current_used_size���ض�������1��
	 *��������£�����Ķ��Կ϶��������	*/
	assert(st_current_buffer_size >= st_current_used_size);

	/*st_current_used_size�ﵽst_current_buffer_size��ʱ����չ���������ڴ�����*/
	if (st_current_buffer_size == st_current_used_size)
	{
		/*��ʵ���ַ�ʽ��Ƶ������realloc�ή�ͳ�������Ч�ʣ�ͬʱ�����ڴ���Ƭ���ķ��ա�*/
		st_line_buffer = (char*)realloc(st_line_buffer, (st_current_buffer_size + ALLOC_SIZE)*sizeof(char));
		st_current_buffer_size += ALLOC_SIZE;
	}

	/*�ڻ�����ĩβ׷��һ���ַ�*/
	st_line_buffer[st_current_used_size] = ch;
	st_current_used_size++;
}

/*��fp��ȡһ���ַ���Ϊ����ֵ���أ�ɾ���˻����ַ�����һ�������ļ�ĩβ���ͷ���null*/
char *read_line(FILE *fp)
{
	int ch;
	char *ret;
	st_current_used_size = 0;
	while ((ch = getc(fp)) != EOF)
	{
		if (ch == '\n')
		{
			add_character('\0');
			break;
		}
		add_character(ch);
	}
	if (ch == EOF)
	{
		if (st_current_used_size > 0)
		{
			/*��������к���û�л���*/
			add_character('\0');
		}
		else
		{
			return NULL;
		}
	}
	/*һ��������ĩ������ݵ�ǰ�еĴ�С���·����ڴ�����Ȼ��st_line_buffer�����ݸ��Ƶ����ڴ�������Ϊ��һ�εĵ��û���ʹ�û����������Դ�ʱ�����ͷ�st_line_buffer.*/
	ret = (char*)malloc(sizeof(char)*st_current_used_size);
	strcpy(ret, st_line_buffer);

	return ret;
}

/*�ͷŻ������ڴ棬��ʵ��ʹ������Ҳ������ʲô���⡣*/
void free_buffer(void)
{
	free(st_line_buffer);
	st_line_buffer = NULL;
	st_current_buffer_size = 0;
	st_current_used_size = 0;
}