all: klient server

clean:
	rm -f klient server

klient: main2.c klient.c klient.h CMemLeak.c
	gcc -std=c99 -o klient main2.c klient.c CMemLeak.c -D_DEBUG

server: main.c server.c server.h CMemLeak.c
	gcc -std=c99 -o server main.c server.c CMemLeak.c -D_DEBUG
