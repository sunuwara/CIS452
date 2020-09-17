clean:
	rm *.out
	echo 'Cleaning done'

debug:
	gcc -Wall -g  simple_shell.c -o simple_shell.out
	echo 'Debug Compile done'

compile:
	gcc -Wall simple_shell.c -o simple_shell.out
	echo 'Compile done'

run:
	./simple_shell.out

valgrind:
	valgrind --leak-check=full --track-origins=yes ./simple_shell.out
