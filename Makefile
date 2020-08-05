build:
	gcc tema3.c format.c add.c deleteRecursively.c override_and_append.c -Wall -Wextra -o tema3

run:
	./tema3

clean:
	rm -f bmp
