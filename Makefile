all: main

main: main.c
	gcc recusistemes.c -o recusistemes

run: main
	./main

clean:
	rm -f main
