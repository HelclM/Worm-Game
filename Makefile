CC=g++
CFLAGS=-Wall -pedantic `sdl2-config --cflags` `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -g -lm -std=c++11
TARGETS=main.o MySDL.o Worm.o Control.o Texture.o Rocket.o Timer.o Input.o queue.o BSBat.o Granade.o Dynamite.o Shotgun.o

compile: $(TARGETS)
	$(CC) $(TARGETS) -o worms $(CFLAGS) -lpthread
run: compile
	./worms &

main.o: main.cpp Classes.h
	$(CC) $(CFLAGS) -c main.cpp -lpthread

MySDL.o: MySDL.cpp Classes.h
	$(CC) $(CFLAGS) -c MySDL.cpp -lpthread

Worm.o: Worm.cpp Classes.h
	$(CC) $(CFLAGS) -c Worm.cpp -lpthread

Control.o: Control.cpp Classes.h
	$(CC) $(CFLAGS) -c Control.cpp -lpthread

Texture.o: Texture.cpp Classes.h
	$(CC) $(CFLAGS) -c Texture.cpp -lpthread

Rocket.o: Rocket.cpp Classes.h
	$(CC) $(CFLAGS) -c Rocket.cpp -lpthread

Timer.o: Timer.cpp Classes.h
	$(CC) $(CFLAGS) -c Timer.cpp -lpthread

Input.o: Input.cpp Classes.h
	$(CC) $(CFLAGS) -c Input.cpp -lpthread

queue.o: queue.cpp Classes.h
	$(CC) $(CFLAGS) -c queue.cpp -lpthread

BSBat.o: BSBat.cpp Classes.h
	$(CC) $(CFLAGS) -c BSBat.cpp -lpthread

Granade.o: Granade.cpp Classes.h
	$(CC) $(CFLAGS) -c Granade.cpp -lpthread

Dynamite.o: Dynamite.cpp Classes.h
	$(CC) $(CFLAGS) -c Dynamite.cpp -lpthread

Shotgun.o: Shotgun.cpp Classes.h
	$(CC) $(CFLAGS) -c Shotgun.cpp -lpthread

clean:
	rm -f *.o worms


