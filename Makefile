.PHONY: all logs
all: app logs

logs:
	mkdir -p logs 

kmp: kmp.c kmp.h
	gcc -Wall -o test kmp.c

app: main.c kmp.c list.c
	gcc -Wall -o app main.c kmp.c list.c

clean: 
	rm -rf app test logs