#ifndef __JSON_H_
#define __JSON_H_


#define MAX_JSON_STRING		200 

typedef struct {
	int day ;
	int month ;
	int year ;
}DATE;




typedef struct {
	int can_id_A ;
	int can_id_B ;
	int serial_number 	 ;
	int number_PO_FPGA  ;
	int number_PO_MK  ;
	int time_work  ; 
}INFO;


void json_add_int_array(char *buffer, char *add_desc, int * array ,int mount);
void json_finish_array(char * buffer);
void json_add_string_parameter(char *buffer, char *add_desc , char *parameter);
void json_add_int_parameter(char *buffer, char *add_desc , int parameter);
void json_add_info(INFO desc_bloc ,char *buffer);
void json_add_date_create(DATE desc_bloc ,char *buffer);
void json_start_packet(char * buffer);
void json_finish_packet(char * buffer);
void json_get_packet(char *buffer_in,char *buffer_out);
void search_first_param (char * buffer_in, char *first_parameter);

#endif
