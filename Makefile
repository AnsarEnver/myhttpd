CC = g++
CFLAGS = -c -g3 -I"./header" -I"D:\utils\boost\include"
OBJ = build/myhttpd.o build/acceptor.o build/connection.o \
build/receiver.o build/request.o build/resource.o build/sender.o \
build/handler.o

LIBBOOST_THREAD = -lboost_thread-mgw12-mt-s-x64-1_79
LIBBOOST_FILESYSTEM = -lboost_filesystem-mgw12-mt-x64-1_79

LIB = -L"D:\utils\boost\lib" -lpthread -lws2_32 -lmswsock -lbcrypt \
$(LIBBOOST_THREAD) $(LIBBOOST_FILESYSTEM)
 

build/myhttpd: $(OBJ)
	$(CC) $^ $(LIB) -o $@

build/myhttpd.o: src/myhttpd.cpp header/myhttpd.h
	$(CC) $(CFLAGS) $< -o $@

build/acceptor.o: src/acceptor.cpp header/acceptor.h
	$(CC) $(CFLAGS) $< -o $@

build/connection.o: src/connection.cpp header/connection.h
	$(CC) $(CFLAGS) $< -o $@

build/receiver.o: src/receiver.cpp header/receiver.h
	$(CC) $(CFLAGS) $< -o $@

build/request.o: src/request.cpp header/request.h
	$(CC) $(CFLAGS) $< -o $@

build/resource.o: src/resource.cpp header/resource.h
	$(CC) $(CFLAGS) $< -o $@

build/sender.o: src/sender.cpp header/sender.h
	$(CC) $(CFLAGS) $< -o $@

build/handler.o: src/handler.cpp header/handler.h
	$(CC) $(CFLAGS) $< -o $@

build/static_content.o: src/static_content.cpp header/static_content.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm build/*
