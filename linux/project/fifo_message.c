
#include "fifo_message.h"

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>

#include "constant.h"

//#define DEBUG_FIFO

int point_struct_write;
int point_struct_read;
int mount_messages_fifo;

sem_t *sem_fifo;

#define SEMAPHORE_NAME "/semphore_free_mount_mess"

struct struct_message fifo_struct_message[MAX_LENGTH_FIFO];

void fifo_init(void)
{
	sem_fifo = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 0);
	int rezult;
	rezult = sem_init(sem_fifo, 0, 0);
	mount_messages_fifo=0;
	point_struct_write=0;
	point_struct_read=0;
	#ifdef DEBUG_FIFO
		puts("fifo init");
	#endif	
} 



void print_status(void)
{
	int i,j;
	j= point_struct_read;
	printf("PRINT STATUS mount= %d\n",mount_messages_fifo);
	for (i=0 ; i<mount_messages_fifo ; i++ )
	{
		printf("%d \n",fifo_struct_message[j].length);
		j++;
		if (j>=MAX_LENGTH_FIFO) 
			j=0;
	}	
}

int fifo_write(int length)
{
	#ifdef DEBUG_FIFO
		printf("Get message length: %d\n",length);
	#endif
	fifo_struct_message[point_struct_write].length=length;
	mount_messages_fifo++;
	point_struct_write++;
	if (point_struct_write>=MAX_LENGTH_FIFO)
		point_struct_write=0;
	
	sem_post(sem_fifo);

	return mount_messages_fifo;
}

int fifo_mount_message(void)
{
	return mount_messages_fifo;
}

int fifo_read(struct struct_message * message)
{
	sem_wait(sem_fifo) ;

	mount_messages_fifo--;	
	message->point_memory=fifo_struct_message[point_struct_read].point_memory;
	message->length=fifo_struct_message[point_struct_read].length;
	return point_struct_read;
}

void fifo_free_memory(int point_struct)
{
	free(fifo_struct_message[point_struct].point_memory);

	fifo_struct_message[point_struct].point_memory = NULL;
	point_struct_read++;
	if (point_struct_read>=MAX_LENGTH_FIFO)
	{

		point_struct_read=0;
		
	}
}


int fifo_clean(void)
{
	int i;
	for (i =0 ; i<MAX_LENGTH_FIFO ; i++)
	{
		if (fifo_struct_message[i].point_memory != NULL)
		{
			free(fifo_struct_message[i].point_memory);	
			fifo_struct_message[i].point_memory = NULL;	
		}
	}
	point_struct_read = 0;	
	point_struct_write = 0;
	mount_messages_fifo=0;
	sem_close(sem_fifo);
}

// Get next point for write message in buffer
char * get_next_mes_point(void)
{
	fifo_struct_message[point_struct_write].point_memory = (char *) malloc(BUFFER_SIZE);
/*	while (fifo_struct_message[point_struct_write].point_memory== NULL)
	{
		fifo_struct_message[point_struct_write].point_memory = (char *) malloc(MAX_LENGTH_MESSAGE);
	}
*/	return fifo_struct_message[point_struct_write].point_memory ;
}
