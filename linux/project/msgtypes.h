#ifndef __MSGTYPES_
#define __MSGTYPES_

#define KEY 1174
#define MAXLEN 100

#define WEB_TO_UDP 2
#define UDP_TO_WEB 1

struct msg_t
{
	long mtype;
	int snd_pid;
	char body[MAXLEN];	
	int parameter;

};




#endif
