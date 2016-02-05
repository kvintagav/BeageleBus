
#include <linux/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include "pthread.h"

#include "jsmn.h"
#include "json.h"
#include "fifo_message.h"
#include "constant.h"
#include "webserver.h"
#include "msgtypes.h"

//#define TEST
#define DEBUG

int port_udp;
bool translation_data;

int mount_get_packet=0;
int mount_send_packet=0;

int create_udp_socket(void)
{
	int sockdesk; 
	if ((sockdesk=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
    }	
    return sockdesk;
}

void close_udp_socket(int socketdesk)
{
	close(socketdesk);
}

void * thread_func_input(void * arg)
{
	int sockdesk = *(int*) arg;
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	#ifdef DEBUG
		puts("thread input create");
	#endif

	
    struct sockaddr_in my_addr;
    memset((char *) &my_addr, 0, sizeof(my_addr));
	#ifdef DEBUG
		printf("UDP port=%d\n",port_udp );
	#endif
		
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port_udp);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     
   
    if( bind(sockdesk , (struct sockaddr*)&my_addr, sizeof(my_addr) ) == -1)
    {
        perror("bind");
    }
	
	struct sockaddr_in T2_MI_client;
	int slen =sizeof(T2_MI_client);

	while(1)
	{	
			char * input_buffer; 
			input_buffer =get_next_mes_point();
			int getdata;
			#ifdef DEBUG
				printf("ready get packet: %d\n",mount_get_packet);
			#endif
			getdata = recvfrom(sockdesk, input_buffer, BUFFER_SIZE, 0,(struct sockaddr *) &T2_MI_client, &(slen));
			mount_get_packet++;
			fifo_write(getdata);
		
	}
	#ifdef DEBUG
		puts("thread output close");
	#endif
	
}

void * thread_func_output(void * arg)
{
	struct struct_message message;
	int number_struct ;
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	#ifdef DEBUG
		puts("thread output create");
	#endif
	while(1)
	{
			
			number_struct = fifo_read(&message);
			mount_send_packet++;
			printf("mount send packet: %d\n", mount_send_packet);
			fifo_free_memory(number_struct);
	}
	#ifdef DEBUG
		puts("thread input close");
	#endif

}


int main (int argc,char ** argv){


#ifdef TEST
	if ( argc < 3)
	{
		puts("I want number port for webserver and programm");
		return -1;
	}

	port_udp = atoi(argv[2]);
	
		int id1 = 1, id2 = 2;
		fifo_init();
		pthread_t thread_input;
		int result;
		result = pthread_create(&thread_input,NULL,thread_func_input,&id1);
		if (result != 0) {
			perror("Creating the input thread");
			return EXIT_FAILURE;
		}
		thread_func_output();

		return EXIT_SUCCESS;
	
	
#else
	

	if ( argc < 3)
	{
		puts("I want number port for webserver and programm");
		return -1;
	}
	
	int msgid;
	pid_t pid= fork();
	if (pid == 0)
	{
		//dother udp client for receiving t2mi packet
		int new_pid=getppid();
		#ifdef DEBUG
			printf(" new_pid =%d\n",new_pid);
		#endif
		
		port_udp = atoi(argv[2]);
		int socketdesk = create_udp_socket();

		fifo_init();
		
		int  id2 = 2;
		pthread_t thread_input,thread_output;
		int result;
		result = pthread_create(&thread_input,NULL,thread_func_input,&socketdesk);
		if (result != 0) {
			perror("Creating the input thread");
			return EXIT_FAILURE;
		}

		result = pthread_create(&thread_output,NULL,thread_func_output,&id2);
		if (result != 0) {
			perror("Creating the output thread");
			return EXIT_FAILURE;
		}		
			
		bool work_translation=true ;
		
		msgid = msgget(KEY, 0666);  
		if (msgid == -1)  
		{
			msgid = msgget(KEY, 0666 | IPC_CREAT); 
		}

		struct msg_t message_proc;
		while (1)
		{
			msgrcv(msgid, &message_proc, sizeof(message_proc) - sizeof(message_proc.mtype), WEB_TO_UDP, 0);  // ждем сообщение
			#ifdef DEBUG
				printf("WEB Server (pid = %i) sent: %s\n", message_proc.snd_pid, message_proc.body);
			#endif
			if (strncmp("stop",message_proc.body,4)==0)
			{
				if (work_translation)
				{
					work_translation=false;
					pthread_cancel(thread_input);
					pthread_cancel(thread_output);
					sleep(1);
					close_udp_socket(socketdesk);
					fifo_clean();
					strcpy(message_proc.body, "Stop input and output data threads");
				}
				else
					strcpy(message_proc.body, "Input and output data threads already stop");

			}
			else if (strncmp("start",message_proc.body,5)==0)
			{
				if (work_translation)
					strcpy(message_proc.body, "Input and output data threads already start");
				else
				{
					work_translation=true;
					fifo_init();
					socketdesk = create_udp_socket();
					result = pthread_create(&thread_input,NULL,thread_func_input,&socketdesk);
					if (result != 0) {
						perror("Creating the input thread");
						return EXIT_FAILURE;
					}

					result = pthread_create(&thread_output,NULL,thread_func_output,&id2);
					if (result != 0) {
						perror("Creating the output thread");
						return EXIT_FAILURE;
					}
					
					strcpy(message_proc.body, "Start input and output data threads");
				}
			}
			/*
			else if (strncmp("restart",message_proc.body,7)==0)
			{
				translation_data=false;
				sleep(1);
				fifo_clean();
				sleep(1);
				fifo_init();
				translation_data=true;
				start_thread();
				strcpy(message_proc.body, "Restart input and output data threads");
			}
			else if (strncmp("test_1",message_proc.body,6)==0)
			{
				int id1 = 1;
				pthread_t thread_test;
				int result;
				result = pthread_create(&thread_test,NULL,thread_test_func_output,&id1);
				if (result != 0) {
					perror("Creating the test output thread");
					return EXIT_FAILURE;
				}
				strcpy(message_proc.body, "Start output test data thread");
			}
			else
			{

			}
			*/
			message_proc.mtype = UDP_TO_WEB;
			message_proc.snd_pid = getpid();
			
			msgsnd(msgid, &message_proc, sizeof(message_proc) - sizeof(message_proc.mtype), 0); // посылаем ответ
  		
  		}
  		
		//thread_test_func_output();
		return EXIT_SUCCESS;
	}
	else
	{
		//father user webserver
		printf("old pid =%d\n",pid);
		

		msgid = msgget(KEY, 0666);  
		if (msgid == -1)  
		{
			msgid = msgget(KEY, 0666 | IPC_CREAT); 	
		}

/*
		
		struct msg_t message_proc;
		strcpy(message_proc.body,"hello udp process");

		message_proc.mtype = 2;
		message_proc.snd_pid = getpid ();
		msgsnd(msgid, &message_proc, sizeof(message_proc) - sizeof(message_proc.mtype), 0);  // посылаем сообщение
		msgrcv(msgid, &message_proc, sizeof(message_proc) - sizeof(message_proc.mtype), 1, 0);  // ждем ответа
 		printf("Server (pid= %i) responded: %s\n", message_proc.snd_pid, message_proc.body);
  */
  	
		webserver_process(atoi(argv[1]),msgid);
		return EXIT_SUCCESS;
	    
	}
#endif

	return EXIT_SUCCESS;
	
}
