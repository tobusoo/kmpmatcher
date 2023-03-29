all: app 

kmp: kmp.c kmp.h
	gcc -Wall -o test kmp.c

app: main.c kmp.c list.c
	gcc -Wall -o app main.c list.c kmp.c

clean: 
	rm -f app test log.log