debug:
	gcc -Wall -g sampleProgram.c -o sample.out
	
valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sample.out

compile:
	gcc -Wall sampleProgram.c -o sample.out

run:
	./sample.out

clean:
	rm *.out
	echo 'Cleaning done'
