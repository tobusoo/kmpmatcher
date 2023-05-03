GLIB = pkg-config --cflags --libs glib-2.0

.PHONY: all logs
all: app logs

logs:
	mkdir -p logs 

kmp: kmp.c kmp.h
	gcc -Wall -o test kmp.c

app: main.c kmp.c
	$(GLIB) | xargs gcc -Wall -o app main.c kmp.c

clean: 
	rm -rf app test logs