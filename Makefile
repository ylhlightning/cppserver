CC = g++
CFLAGS = 

TARGETS = out/server

SOURCES = \
		  src/client.cpp \
		  src/server.cpp \
		  src/error.cpp \
		  src/main.cpp \
		  src/reactor.cpp \
		  src/tcpserver_api.cpp

INCLUDES += -Iinc

LIBS = -lrt

all:$(TARGETS)

$(TARGETS): $(SOURCES)
	$(CC) $(INCLUDES) $(CFLAGS) $(LIBS) $^ -g -o $@

clean:
	rm -rf *.o out/server


