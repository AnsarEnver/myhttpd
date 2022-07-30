CC = g++
CFLAGS = -c -O3 -I header/
OBJ = build/myhttpd.o build/acceptor.o build/connection.o \
build/receiver.o build/request.o

build/myhttpd: $(OBJ)
	$(CC) $^ -lpthread -lboost_thread -lws2_32 -lmswsock -lbcrypt -o $@

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

clean:
	@rm build/*