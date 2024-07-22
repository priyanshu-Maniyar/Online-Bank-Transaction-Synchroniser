all: Server

Server: Server.o server_to_client.o admin_ops.o user_ops.o
	gcc Server.o server_to_client.o admin_ops.o user_ops.o -lpthread -lsqlite3 -o Server

Server.o: Server.c
	gcc -c Server.c

server_to_client.o: server_to_client.c
	gcc -c server_to_client.c

user_ops.o: user_ops.c
	gcc -c user_ops.c

admin_ops.o: admin_ops.c
	gcc -c admin_ops.c

clean:
	rm *.o Server

