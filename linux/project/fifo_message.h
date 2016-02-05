#ifndef __FIFO_MESSAGE_H_
#define __FIFO_MESSAGE_H_


#define MAX_LENGTH_FIFO 100000  


struct struct_message
{
	char * point_memory;
	int length;
} ;

void fifo_init(void);

int fifo_read(struct struct_message * message);
int fifo_mount_message(void);
char * get_next_mes_point(void);
void fifo_free_memory(int point_struct_read);
void print_status(void);
int fifo_clean(void);
int fifo_write(int length);

#endif