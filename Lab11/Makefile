compile1a:
	gcc -Wall -g sample_program1a.c -o sp1a.out

valgrind1a:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sp1a.out sample_program1a.c

run1a:
	./sp1a.out sample_program1a.c

compile1b:
	gcc -Wall -g sample_program1b.c -o sp1b.out

valgrind1b:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sp1b.out sample_program1a.c

run1b:
	./sp1b.out sample_program1a.c
	
clean:
	rm *.out
	echo 'Cleaning done'
