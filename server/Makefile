s:server.o ../include/chat.o
	gcc -o s server.o ../include/chat.o
chat.o:../include/chat.c ../include/chat.h
	gcc -c -o ../include/chat.o ../include/chat.c ../include/chat.h
server.o:server.c ../include/chat.h
	gcc -c -o server.o server.c -I../include

clean:
	rm -f s server.o ../include/chat.o
