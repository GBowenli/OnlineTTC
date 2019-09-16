SRCS=ttt_client.c ttt_server.c
CFLAGS=-g
ifneq ($(PORT),)
	P=$(PORT)
else
	P=8888
endif

all: client server

client: ttt_client.c
	gcc ${CFLAGS} -o client ttt_client.c

server: ttt_server.c
	gcc ${CFLAGS} -o server ttt_server.c

test: client server
	./server ${P} &
	sleep 1
	
	echo LOGIN,bowen,1234 | ./client 127.0.0.1 ${P}
	echo CREATE,bowen,1234,mygame | ./client 127.0.0.1 ${P}

	echo LOGIN,bowen2,4321  | ./client 127.0.0.1 ${P}
	echo JOIN,bowen2,4321,mygame,b2  | ./client 127.0.0.1 ${P}

	echo MOVE,bowen,1234,mygame,a1  | ./client 127.0.0.1 ${P}	
	echo MOVE,bowen2,4321,mygame,b1  | ./client 127.0.0.1 ${P}

	echo MOVE,bowen,1234,mygame,a2  | ./client 127.0.0.1 ${P}
	echo MOVE,bowen2,4321,mygame,b3  | ./client 127.0.0.1 ${P}

harder_test: client server
	./server ${P} &
	sleep 1
	
	echo LOGIN,bowen,1234 | ./client 127.0.0.1 ${P}
	echo LOGIN,bowen2,4321  | ./client 127.0.0.1 ${P}

	echo CREATE,bowen,1234,mygame | ./client 127.0.0.1 ${P}
	echo JOIN,bowen2,4321,mygame,b2  | ./client 127.0.0.1 ${P}

	echo MOVE,bowen,1234,mygame,a1  | ./client 127.0.0.1 ${P}	
	echo JOIN,bowen2,4321,mygame,b2  | ./client 127.0.0.1 ${P}	
	echo SHOW,bowen,1234,mygame     | ./client 127.0.0.1 ${P}	

	echo MOVE,bowen,1234,mygame,a1  | ./client 127.0.0.1 ${P}	
	echo MOVE,bowen2,4321,mygame,b1  | ./client 127.0.0.1 ${P}

	echo MOVE,bowen,1234,mygame,a3  | ./client 127.0.0.1 ${P}	
	echo JOIN,bowen,1234,mygame,c1  | ./client 127.0.0.1 ${P}	
	echo SHOW,bowen,1234,mygame     | ./client 127.0.0.1 ${P}	

	echo JOIN,bowen,1234,mygame,b3  | ./client 127.0.0.1 ${P}	

	echo MOVE,bowen,1234,mygame,a2  | ./client 127.0.0.1 ${P}
	echo MOVE,bowen2,4321,mygame,b3  | ./client 127.0.0.1 ${P}

clean:
	rm client server	

