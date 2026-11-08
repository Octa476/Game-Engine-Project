CPPC = g++
CFLAGS = -g

run: main
	./main

main: main.cpp
	$(CPPC) -fdiagnostics-color=always $(CFLAGS) *.cpp glad/*.c -o main -lGL -lglfw

clean:
	rm -f main