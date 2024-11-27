CC = gcc
CFLAGS = -I/usr/include/mysql 
LIBS = -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -ldl -lssl -lcrypto -lresolv -lm -lrt

TARGET =  sqlinjection

LOGIN: CFLAGS += -DLOGIN
LOGIN: sqlinjection.o
	$(CC) -o $(TARGET) sqlinjection.o $(LIBS) 

DESIGNER: CFLAGS += -DDESIGNER
DESIGNER: sqlinjection.o
	$(CC) -o $(TARGET) sqlinjection.o $(LIBS) 


sqlinjection.o: sqlinjection.c
	$(CC) -c sqlinjection.c $(CFLAGS)

clean:
	rm -rf *.o
	rm -rf sqlinjection