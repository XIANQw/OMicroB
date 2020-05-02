all: gui

gui: gui.o ../.build/shared.o
	gcc -o $@ $^ `pkg-config --cflags --libs gtk+-2.0`
	@mv gui /tmp/

gui.o: gui.c
	gcc -c $^ `pkg-config --cflags --libs gtk+-2.0`

client: client.o
	gcc -o $@ $^ -pthread


%.o: %.c
	gcc -o $@ -c $^

clean:
	rm *.o gui client server