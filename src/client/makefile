all: gui client

gui: gui.o
	gcc -o $@ $^ `pkg-config --cflags --libs gtk+-2.0` -pthread

client: client.o
	gcc -o $@ $^ -pthread

gui.o: gui.c
	gcc -o $@ -c $^ `pkg-config --cflags --libs gtk+-2.0` -pthread

%.o: %.c
	gcc -o $@ -c $^

clean:
	rm client gui *.o