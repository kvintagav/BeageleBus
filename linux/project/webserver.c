

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
#include <errno.h>
#include <sys/sem.h>

#include "webserver.h"
#include "jsmn.h"
#include "json.h"
#include "constant.h"

#include "msgtypes.h"
#define DEBUG

enum http_methods_cmd  {
	GET,
	POST,
	PUT
};

char * http_methods[] = {
	"GET",
	"POST",
	"PUT",
	NULL
};
int port_udp;

	


const char * http_header = "HTTP/1.0 200 OK\r\n"
"Content-type: text/html; charset=utf-8\r\n"
"Connection: close;\r\n"
"\r\n";


//const char * NAME_FOLDER 		="webpage";

	
/****
	*	@brief  create name abd Find file for open
	*	@param  buffer - buffer with Http packet 
	*	@retval None
	*/
int find_and_open_file(char *buffer)
{ 
	char  return_file[30]={0};
	char name_file[40]={0};
	search_first_param(buffer,return_file);  //find in request need file 
	if (return_file[1]==0) strcat(return_file,"index.html"); //add index.html if request / 
	strcat(name_file,"webpage");
	strcat(name_file,return_file);	
	return open(name_file,O_RDONLY); //open file and return file descriptor
}

/****
	*	@brief  close file
	*	@param  file_desc - input file descriptor
	*	@retval None
	*/
void file_close(int file_desc)
{
	close(file_desc);	
}

/****
	*	@brief  find json name parameter in list parameters
	*	@param  json - input buffer, tok - list parameters, s - search parameter
	*	@retval None
	*/
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}


/****
	*	@brief  clear buffer
	*	@param  buffer - need_buffer, buffer_size - need size
	*	@retval none
	*/
void clear_buffer(char *buffer, int buffer_size)
{
	memset(buffer,0,buffer_size);
}

/****
	*	@brief  parse input json packet and create output buffer
	*	@param  buffer - buffer with json packet 
	*	@retval return_buffer with answer
	*/
int parse_json_execute(char * buffer, int msgid)
{
	
	char json_string[MAX_JSON_STRING]={0};
	int i=1;
	char first_param[10]={0};
	char second_param[10]={0};
	json_get_packet(buffer,json_string); 	//find json string 
	clear_buffer(buffer,BUFFER_SIZE); 		//clear buffer for write answer with jsin packet
	jsmn_parser json_packet;
	jsmntok_t json_parameters[10];
	jsmn_init(&json_packet);

	jsmn_parse(&json_packet, json_string, strlen(json_string), json_parameters, sizeof(json_parameters)/sizeof(json_parameters[0]));

	if (jsoneq(json_string, &json_parameters[i], "command") == 0) {
		sprintf(first_param,"%.*s\n", json_parameters[i+1].end-json_parameters[i+1].start,
				json_string + json_parameters[i+1].start);
		
		struct msg_t message_proc;
		strcpy(message_proc.body,first_param);
		printf("Send message %s\n", message_proc.body);
		message_proc.mtype = WEB_TO_UDP;
		message_proc.snd_pid = getpid ();
		msgsnd(msgid, &message_proc, sizeof(message_proc) - sizeof(message_proc.mtype), 0);  // посылаем сообщение
		msgrcv(msgid, &message_proc, sizeof(message_proc) - sizeof(message_proc.mtype), UDP_TO_WEB, 0); 

		json_start_packet	(buffer);	
		json_add_string_parameter(buffer,"rezult",message_proc.body);
		json_finish_packet	(buffer);
		return 1;
	}

	
	return -1;
}


/****
	*	@brief  webserver process 
	*	@param  port - number port for socket, msgid - number message query
	*	@retval return_buffer with answer
	*/
int webserver_process (int port,int msgid)
{
	int listener = socket(AF_INET, SOCK_STREAM,0); //tcp socket
	if (listener <= 0)
	{
		perror("socket");
		return -1;
	}
	
	struct sockaddr_in serv_addr;
	memset (&serv_addr, 0, sizeof (serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
   
	if ( bind(listener,(struct sockaddr*)&serv_addr, sizeof(serv_addr))<0)
	{
	    perror("bind");
		return -1;
	}
	
	if (listen(listener, 5)<0)
	{
		perror("listen");
		return -1;
	}

	printf("Open web browser and open site http://localhost:%d\n",port);

	
    socklen_t client_len;
    
    struct sockaddr_in client_addr; //create client address struct
    int client_socket;    			//create descriptor client socket
    
    char buffer[BUFFER_SIZE]; 		//create buffer for tcp-ip socket
    
    while( -1 != ( client_socket = accept( listener, (struct sockaddr *) &client_addr, 
                                                    &client_len) ) )
    {
    	#ifdef DEBUG
			printf( "Transmission request: %s\n", inet_ntoa( client_addr.sin_addr ) );
        #endif
        int rezult = recv(client_socket,buffer,BUFFER_SIZE,0); //take request from web-browser
      	if (buffer[5]=='?') // get JSON packet 
		{
			if ((parse_json_execute(buffer, msgid))>=0)
			{
				#ifdef DEBUG
					printf("Send JSON packet:%s\n",buffer );
				#endif
				write(client_socket, buffer,strlen(buffer));	
			}
			else 
			{
				#ifdef DEBUG
					printf("Error JSON packet or JSON packet not found\n");
				#endif
			}
		}
		else
		{
	        char **method;
	        for (method = http_methods ; *method != NULL; method++)
	        {
	        	if (strncmp(*method,buffer,strlen(*method))==0) break; //find mhttp method
	        }
	    	if(*method == NULL) {
	    		#ifdef DEBUG
					puts("Error http method\n");
				#endif
				break;
			}
			switch (method - http_methods){
				case GET:
					write( client_socket, http_header, strlen( http_header ) ); //send http header
					int file_html = find_and_open_file(buffer); //find file and create file descriptor
					if (file_html < 0)
					{
						#ifdef DEBUG
							puts("Can't open html file");
						#endif
					}
					else
					{
						#ifdef DEBUG
							puts("file_open");
						#endif

						clear_buffer(buffer,BUFFER_SIZE);
			 			ssize_t read_bytes;
			 			char read_buffer[BUFFER_FILE_SIZE+1];
						while (read_bytes =read(file_html,read_buffer,BUFFER_FILE_SIZE)>0)
						{

							strncat(buffer,read_buffer,strlen(read_buffer));
							if (strlen(buffer)>=(BUFFER_SIZE-BUFFER_FILE_SIZE))
							{
								write(client_socket, buffer,strlen(buffer));
								memset(&buffer,0,BUFFER_SIZE);
							} 
							memset(&read_buffer,0,BUFFER_FILE_SIZE+1);

						}
						write(client_socket, buffer,strlen(buffer));
						memset(&buffer,0,strlen(buffer));
						file_close(file_html);
						close(file_html);	
					}
					break;

					default:
					#ifdef DEBUG
						puts("Error http method");
					#endif	
					break;
			}
		}
      
       close( client_socket );
    }
    return EXIT_FAILURE;
}