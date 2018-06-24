.PHONY:all
all:server client server_thread server_fork

server_fork:./server_fork.c
	gcc $^ -o $@ 

server_thread:./server_thread.c
	gcc $^ -o $@ -lpthread

server:./server.c
	gcc $^ -o $@ 

client:./client.c
	gcc $^ -o $@

.PHONY:clean
clean:
	rm server client server_thread server_fork
