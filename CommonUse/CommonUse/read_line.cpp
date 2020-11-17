#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define ALLOC_SIZE  (256)
#pragma warning(disable:4996)

/*读取行的缓冲，必要时进行扩展。但是区域不会被缩小。调用free_buffer释放。*/
/*st_line_buffer指向缓冲区的初始位置，该缓冲区用于存放临时读取的字符串，当缓冲区空间不足时，缓冲区会扩展ALLOC_SIZE大小的区域。*/
static char *st_line_buffer = NULL;

/*在st_line_buffer前方被分配的内存区域的大小*/
static int st_current_buffer_size = 0;

/*st_line_buffer中现在保存的字符的大小*/
static int st_current_used_size = 0;

/*如有必要，扩展st_line_buffer前方的内存区域。
在st_line_buffer末尾追加一个字符ch*/
static void add_character(int ch)
{
	/*此函数每次被调用，st_current_used_size都必定会增加1，
	 *正常情况下，下面的断言肯定不会出错。	*/
	assert(st_current_buffer_size >= st_current_used_size);

	/*st_current_used_size达到st_current_buffer_size的时候，扩展缓冲区的内存区域。*/
	if (st_current_buffer_size == st_current_used_size)
	{
		/*其实这种方式，频繁调用realloc会降低程序运行效率，同时带来内存碎片化的风险。*/
		st_line_buffer = (char*)realloc(st_line_buffer, (st_current_buffer_size + ALLOC_SIZE)*sizeof(char));
		st_current_buffer_size += ALLOC_SIZE;
	}

	/*在缓冲区末尾追加一个字符*/
	st_line_buffer[st_current_used_size] = ch;
	st_current_used_size++;
}

/*从fp读取一行字符作为返回值返回（删除了换行字符），一旦读到文件末尾，就返回null*/
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
			/*如果最终行后面没有换行*/
			add_character('\0');
		}
		else
		{
			return NULL;
		}
	}
	/*一旦读到行末，会根据当前行的大小重新分配内存区域，然后将st_line_buffer的内容复制到此内存区域。因为下一次的调用还会使用缓冲区。所以此时无需释放st_line_buffer.*/
	ret = (char*)malloc(sizeof(char)*st_current_used_size);
	strcpy(ret, st_line_buffer);

	return ret;
}

/*释放缓冲区内存，其实即使不调用也不会有什么问题。*/
void free_buffer(void)
{
	free(st_line_buffer);
	st_line_buffer = NULL;
	st_current_buffer_size = 0;
	st_current_used_size = 0;
}