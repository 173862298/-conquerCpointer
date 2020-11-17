#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read_line2.h"
#define ALLOC_SIZE  (256)
#pragma warning(disable:4996)
/*该文件是read_line.cpp的修订版*/


/*读取行的缓冲，必要时进行扩展。但是区域不会被缩小。调用free_buffer释放。*/
static char *st_line_buffer = NULL;

/*在st_line_buffer前方分配的内存区域的大小*/
static int st_current_buffer_size = 0;

/*st_line_buffer中现在被保存的字符的大小*/
static int st_current_used_size = 0;

/*在st_line_buffer末尾追加一个字符，如有必要，扩展st_line_buffer前方的内存区域。*/
static ReadLineStatus add_character(int ch)
{
	/*此函数每次被调用，st_line_used都必定会增加1，
	*正常情况下，下面的断言肯定不会出错。	*/
	assert(st_current_buffer_size >= st_current_used_size);

	/*st_current_used_size达到st_current_buffer_size的时候，扩展缓冲区的内存区域。*/
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

	/*在缓冲区末尾追加一个字符*/
	st_line_buffer[st_current_used_size] = ch;
	st_current_used_size++;

	return READ_LINE_SUCCESS;
}


/*在read_line()中，一旦malloc返回null，程序马上通过goto将处理转移到FUNC_END;一般内存不足时才会发生错误，此时用free_buffer来释放缓冲区内存。*/
/*从fp读取一行字符，一旦读到文件末尾，就返回null*/
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
				goto FUNC_END;//异常处理中使用goto反而可以让程序更加整洁。
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
			/*如果最终行后面没有换行*/
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

/*释放缓冲区内存，其实即使不调用也不会有什么问题。*/
void free_buffer(void)
{
	free(st_line_buffer);
	st_line_buffer = NULL;
	st_current_buffer_size = 0;
	st_current_used_size = 0;
}