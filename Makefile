CC=/opt/cross-pi-gcc/bin/arm-linux-gnueabihf-gcc
CFLAGS=-lm -lasound -I/opt/cross-pi-gcc/arm-linux-gnueabihf/include -I/opt/cross-pi-libs/include -L/opt/cross-pi-libs/lib
EXEC=w48play

all:
	$(CC) w48play.c -o $(EXEC) $(CFLAGS)

build-rpi:
	$(CC) w48play.c -o $(EXEC) $(CFLAGS)

install:
	cp w48play /usr/sbin/
	mkdir -p /usr/share/w48play/
	cp -r sounds/* /usr/share/w48play/

clean:
	rm -f w48play *.deb
