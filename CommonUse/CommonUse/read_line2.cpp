#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read_line2.h"
#define ALLOC_SIZE  (256)
#pragma warning(disable:4996)
/*���ļ���read_line.cpp���޶���*/


/*��ȡ�еĻ��壬��Ҫʱ������չ���������򲻻ᱻ��С������free_buffer�ͷš�*/
static char *st_line_buffer = NULL;

/*��st_line_bufferǰ��������ڴ�����Ĵ�С*/
static int st_current_buffer_size = 0;

/*st_line_buffer�����ڱ�������ַ��Ĵ�С*/
static int st_current_used_size = 0;

/*��st_line_bufferĩβ׷��һ���ַ������б�Ҫ����չst_line_bufferǰ�����ڴ�����*/
static ReadLineStatus add_character(int ch)
{
	/*�˺���ÿ�α����ã�st_line_used���ض�������1��
	*��������£�����Ķ��Կ϶��������	*/
	assert(st_current_buffer_size >= st_current_used_size);

	/*st_current_used_size�ﵽst_current_buffer_size��ʱ����չ���������ڴ�����*/
	if (st_current_buffer_size == st_current_used_size)
	{
		char *temp;
		temp = (char*)realloc(st_line_buffer, (st_current_buffer_size + ALLOC_SIZE)*sizeof(char));
		if (temp == NULL)
		{
			return READ_LINE_OUT_OF_MEMORY;
		}
		st_line_buffer = temp;
		st_current_buffer_size += ALLOC_SIZE;
	}

	/*�ڻ�����ĩβ׷��һ���ַ�*/
	st_line_buffer[st_current_used_size] = ch;
	st_current_used_size++;

	return READ_LINE_SUCCESS;
}


/*��read_line()�У�һ��malloc����null����������ͨ��goto������ת�Ƶ�FUNC_END;һ���ڴ治��ʱ�Żᷢ�����󣬴�ʱ��free_buffer���ͷŻ������ڴ档*/
/*��fp��ȡһ���ַ���һ�������ļ�ĩβ���ͷ���null*/
ReadLineStatus read_line(FILE *fp,char **line)
{
	int ch;
	ReadLineStatus status=READ_LINE_SUCCESS;

	st_current_used_size = 0;
	while ((ch = getc(fp)) != EOF)
	{
		if (ch == '\n')
		{
			status = add_character('\0');
			if (status != READ_LINE_SUCCESS)
			{
				goto FUNC_END;//�쳣������ʹ��goto���������ó���������ࡣ
			}
			break;			
		}
		status = add_character(ch);
		if (status != READ_LINE_SUCCESS)
		{
			goto FUNC_END;
		}
	}
	if (ch == EOF)
	{
		if (st_current_used_size > 0)
		{
			/*��������к���û�л���*/
			status = add_character('\0');
			if (status != READ_LINE_SUCCESS)
			{
				goto FUNC_END;
			}
		}
		else
		{
			status = READ_LINE_EOF;
			goto FUNC_END;			
		}
	}

	*line = (char*)malloc(sizeof(char)*st_current_used_size);
	if (*line == NULL)
	{
		status = READ_LINE_OUT_OF_MEMORY;
		goto FUNC_END;
	}
	strcpy(*line, st_line_buffer);

FUNC_END:
	if (status != READ_LINE_SUCCESS && status != READ_LINE_EOF)
	{
		free_buffer();
	}

	return status;
}

/*�ͷŻ������ڴ棬��ʵ��ʹ������Ҳ������ʲô���⡣*/
void free_buffer(void)
{
	free(st_line_buffer);
	st_line_buffer = NULL;
	st_current_buffer_size = 0;
	st_current_used_size = 0;
}