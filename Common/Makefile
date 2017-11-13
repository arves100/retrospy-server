CC=gcc
CXX=g++

BIN = libcommon.a
CFLAGS=-Wall -O0 -g3 -ggdb -fno-exceptions -pipe -D_DEBUG
IFLAGS=-I.

OBJS = Database.o GSBuffer.o Socket_lx.o sqlite3.o

default: $(BIN)

$(BIN) : $(OBJS)
	ar cr $(BIN) $(OBJS) $(LIBS)
	@ranlib $(BIN)
	@chmod 700 $(BIN)
	
%.o: %.cpp
	$(CXX) $(CFLAGS) $(IFLAGS) -c $< -o $@
	
%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
	
clean:
	@rm *.a
	@rm *.o