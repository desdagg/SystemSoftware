CC   = gcc

LIBS = -lm -lrt -pthread -lsqlite3
 
CLIENT_OBJS = clientNew.o
SERVER_OBJS = serverNew.o DBWrapper.o 

ALL_OBJS = $(SERVER_OBJS) $(CLIENT_OBJS) 

HEADERS = DBWrapper.h

all: server client

server : $(SERVER_OBJS)
	$(CC) -g -o servPro $(SERVER_OBJS) $(LIBS)

client : $(CLIENT_OBJS)
	$(CC) -g -o clientPro $(CLIENT_OBJS) $(LIBS)

serverNew.o : serverNew.c $(HEADERS)
	$(CC) -c serverNew.c

clientNew.o : clientNew.c
	$(CC) -c clientNew.c

DBWrapper.o : DBWrapper.c
	$(CC) -c DBWrapper.c

clean:
	rm servPro clientPro $(ALL_OBJS)
