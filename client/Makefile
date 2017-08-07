c:../include/chat.o client.o
	gcc -o c ../include/chat.o client.o
chat.o:../include/chat.c ../include/chat.h
	gcc -c -o ../include/chat.o ../include/chat.c ../include/chat.h -g
client.o:client.c ../include/chat.h
	gcc -c -o client.o client.c  -I../include

clean:
	rm -f c client.o ../include/chat.o
