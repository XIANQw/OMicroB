all: client

client: gui.o gui.h ../.build/shared.o client.o client.h
	gcc -o $@ $^ `pkg-config --cflags --libs gtk+-3.0`
	@mv client /tmp/

gui.o: gui.c gui.h
	gcc -c $^ `pkg-config --cflags --libs gtk+-3.0`

client.o: client.c client.h
	gcc -c $^ `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm *.o client