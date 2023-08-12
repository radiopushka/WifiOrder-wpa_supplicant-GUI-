all:
	gcc main.c -o wifiorder -lncurses
debug:
	gcc main.c -o wifiorder -lncurses -g -Wall

install:
	chmod +x ./startwifi
	chmod +x ./stopwifi
	chmod +x ./restartwifi
	cp ./restartwifi /usr/local/bin
	cp ./startwifi /usr/local/bin
	cp ./stopwifi /usr/local/bin
	cp ./wifiorder /usr/local/bin
