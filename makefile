scheduler:
	gcc -Wall -Wextra -Wpedantic -std=c11 -g main.c -o scheduler

run:
	./scheduler

clean:
	rm -f scheduler

.PHONY: scheduler run clean