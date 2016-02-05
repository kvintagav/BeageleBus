


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>


#include "json.h"





/****
	*	@brief  
	*	@param  
	*	@retval 
	*/
void json_add_string_parameter(char *buffer, char *add_desc , char *parameter)
{
	char buff_param[100]={0};	
	sprintf(buff_param,"\"%s\":\"%s\",",add_desc,parameter);
	strcat(buffer,buff_param);
}

/****
	*	@brief  
	*	@param  
	*	@retval 
	*/
void json_add_int_parameter(char *buffer, char *add_desc , int parameter)
{
	char buff_param[30];	
	sprintf(buff_param,"\"%s\":\"%d\",",add_desc,parameter);
	strcat(buffer,buff_param);
}


/****
	*	@brief  
	*	@param  
	*	@retval 
	*/
void json_add_int_array(char *buffer, char *add_desc, int * array ,int mount)
{
	char buff_param[30];
	int *param = array;
	int i;
	sprintf(buff_param,"\"%s\":[",add_desc);
	strcat(buffer,buff_param);

	for (i=0 ; i<mount ; i++)
	{
		sprintf(buff_param,"\"%d\",", *param);
		strcat(buffer,buff_param);
		param++;
	}
	json_finish_array(buffer);
}

/****
	*	@brief  
	*	@param  
	*	@retval 
	*/
void json_finish_array(char * buffer)
{
	buffer+=strlen(buffer)-1;
	if (*buffer==',')	*buffer = ']';
	buffer++;
	*buffer=',';
}

/****
	*	@brief  
	*	@param  
	*	@retval 
	*/
void json_add_date_create(DATE desc_bloc ,char *buffer)
{
	strcat(buffer,"\"create\":{");
	json_add_int_parameter(buffer,"day",desc_bloc.day);	
	json_add_int_parameter(buffer,"month",desc_bloc.month);	
	json_add_int_parameter(buffer,"year",desc_bloc.year);	
	json_finish_packet(buffer);
}

/****
	*	@brief  
	*	@param  
	*	@retval 
	*/
void json_start_packet(char * buffer)
{
	*buffer='{';
}

/****
	*	@brief  
	*	@param  
	*	@retval 
	*/
void json_finish_packet(char * buffer)
{
	buffer+=strlen(buffer)-1;
	if (*buffer==',')
		*buffer = '}';
	else
	{
		buffer++;
		*buffer='}';
	}
}



/****
	*	@brief  search first param in string
	*	@param  buffer_in - input buffer
	*	@param  first_parameter - output name parameter
	*	@retval None
	*/
void search_first_param (char * buffer_in, char *first_parameter)
{

	char *point=first_parameter ;
	char *buf;
	int number_parameter =0;

	for (buf =buffer_in ; (*buf !='\n') ||(*buf !='\r') ; buf++)
	{
		if (*buf==' ')
		{
			number_parameter++;
			if (number_parameter==2) break;
		}
		else 
		{
			if (number_parameter==1)
			{
				*point=*buf;
				point++;
			}
		}
	}	
}
/****
	*	@brief  create json packet from input buffer
	*	@param  buffer_in - buffer with json packet 
	*	@retval buffer_out - ready buffer for json parser
	*/
void json_get_packet(char *buffer_in,char *buffer_out)
{
	char json_buffer[MAX_JSON_STRING+10]={0};
	int i;
	char *buf_in=json_buffer;
	char *buf_out=buffer_out;

	search_first_param(buffer_in , json_buffer); //search json string in input buffer


	for (i=0; i<strlen(json_buffer);i++) //delete simvol '/' , '?' and '%22' from packet
	{
		if (*buf_in=='%')
		{
			buf_in+=3;
			*buf_out='\"';
			buf_out++;
		}
		else if ((*buf_in=='/') || (*buf_in=='?'))	buf_in++;
		else
		{
			*buf_out=*buf_in;
			buf_out++;
			buf_in++;
		}
	}
}