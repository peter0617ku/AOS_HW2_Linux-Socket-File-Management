#--------------------------------------------
CC = gcc
CFLAG = -c
OFLAG = -o
TFLAG = -lpthread
SOURCE = server.c client*.c token.c capability_list.c file_mutex.c
OBJECT = %.o
SERVER_OBJECT = server.o token.o capability_list.o file_mutex.o
CLIENT_OBJECT1 = client1.o token.o
CLIENT_OBJECT2 = client2.o token.o
CLIENT_OBJECT3 = client3.o token.o
CLIENT_OBJECT4 = client4.o token.o
CLIENT_OBJECT5 = client5.o token.o
CLIENT_OBJECT6 = client6.o token.o
NEW_TERMINAL = gnome-terminal

#--------------------------------------------
all: $(OBJECT)
	$(CC) $(OFLAG) server/server.exe $(SERVER_OBJECT) $(TFLAG)
	$(CC) $(OFLAG) client1/client1.exe $(CLIENT_OBJECT1)
	$(CC) $(OFLAG) client2/client2.exe $(CLIENT_OBJECT2)
	$(CC) $(OFLAG) client3/client3.exe $(CLIENT_OBJECT3)
	$(CC) $(OFLAG) client4/client4.exe $(CLIENT_OBJECT4)
	$(CC) $(OFLAG) client5/client5.exe $(CLIENT_OBJECT5)
	$(CC) $(OFLAG) client6/client6.exe $(CLIENT_OBJECT6)
%.o:
	$(CC) $(CFLAG) $(SOURCE)
clean:
	rm *.o
	rm */*.exe

