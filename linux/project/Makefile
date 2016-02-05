
all:t2mi_client

t2mi_client: main.o  jsmn.o json.o fifo_message.o webserver.o
	gcc -o t2mi_client main.o jsmn.o json.o fifo_message.o webserver.o -lpthread 

main.o: main.c 
	gcc -c  main.c -lpthread 

jsmn.o: jsmn.c
	gcc -c jsmn.c -lpthread 
	
fifo_message.o: fifo_message.c
	gcc -c fifo_message.c -lpthread 
	
webserver.o: webserver.c
	gcc -c webserver.c -lpthread

json.o: json.c	
	gcc -c json.c -lpthread 

clean:
	rm -f main *.o


