CC   = gcc

LIBS = -lm -lrt

OBJS = myDaemon.o updateSite.o reportUtil.o userBackup.o

HEADERS = updateSite.h reportUtil.h

myprog : $(OBJS)
	$(CC) -o siteDaemon $(OBJS) -lm

myDaemon.o : myDaemon.c $(HEADERS)
	$(CC) -c myDaemon.c

reportUtil.o : reportUtil.c
	$(CC) -c reportUtil.c

timeUtil.o : timeUtil.c
	$(CC) -c timeUtil.c

updateSite.o : updateSite.c
	$(CC) -c updateSite.c

userBackup.o : userBackup.c
	$(CC) -c userBackup.c

clean:
	rm siteDaemon $(OBJS)
