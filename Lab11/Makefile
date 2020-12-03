compile:
	gcc -Wall -g sampleProgram.c -o sp1.out

valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sp1.out

run:
	./sp1.out

clean:
	rm *.out
	echo 'Cleaning done'
