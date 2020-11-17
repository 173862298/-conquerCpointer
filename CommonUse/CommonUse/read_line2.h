#ifndef READ_LINE_H_INCLUDED
#define READ_LINE_H_INCLUDED
/*该文件为read_line.h的修订版*/

#include <stdio.h>

/*对于read_line()向调用方返回的处理状态，有以下几种状况：*/
typedef enum 
{
	READ_LINE_SUCCESS,  /*正常的读取一行*/
	READ_LINE_EOF,	/*读到了文件的末尾*/
	READ_LINE_OUT_OF_MEMORY		/*内存不足导致处理失败*/
} ReadLineStatus;

ReadLineStatus read_line(FILE *fp, char **line);
void free_buffer(void);

#endif
