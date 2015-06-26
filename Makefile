CC = g++
CFLAGS = 

TARGETS = out/server

SOURCES = \
		  src/DiagnosticsClient.cpp \
		  src/DiagnosticsServer.cpp \
		  src/Error.cpp \
		  src/Main.cpp \
		  src/PollReactor.cpp \
		  src/TcpServer.cpp

INCLUDES += -Iinc

LIBS = -lrt

all:$(TARGETS)

$(TARGETS): $(SOURCES)
	$(CC) $(INCLUDES) $(CFLAGS) $(LIBS) $^ -g -o $@

clean:
	rm -rf *.o out/server


