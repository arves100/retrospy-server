CC=gcc
CXX=g++


BIN = libcommon.a
CFLAGS=-Wall -O0 -g3 -ggdb -fno-exceptions -pipe
IFLAGS=-I.

OBJS = Database.o GSBuffer.o Socket.o sqlite3.o

default: $(BIN)

$(BIN) : $(OBJS)
	ar cru $(BIN) $(OBJS) $(LIBS)
	ranlib $(BIN)
	chmod 700 $(BIN)
	
%.o: %.cpp
	$(CXX) $(CFLAGS) $(IFLAGS) -c $< -o $@
	
%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@