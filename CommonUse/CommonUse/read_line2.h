#ifndef READ_LINE_H_INCLUDED
#define READ_LINE_H_INCLUDED
/*���ļ�Ϊread_line.h���޶���*/

#include <stdio.h>

/*����read_line()����÷����صĴ���״̬�������¼���״����*/
typedef enum 
{
	READ_LINE_SUCCESS,  /*�����Ķ�ȡһ��*/
	READ_LINE_EOF,	/*�������ļ���ĩβ*/
	READ_LINE_OUT_OF_MEMORY		/*�ڴ治�㵼�´���ʧ��*/
} ReadLineStatus;

ReadLineStatus read_line(FILE *fp, char **line);
void free_buffer(void);

#endif
