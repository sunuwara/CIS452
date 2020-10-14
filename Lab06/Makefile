debug1:
	gcc -Wall -g sample_program1.c -o sample1.out
	
valgrind1:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sample1.out

compile1:
	gcc -Wall sample_program1.c -o sample1.out

run1:
	./sample1.out $$ARGS
	
debug:
	gcc -Wall -g named_semaphore.c -o main.out
	
valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./main.out

compile:
	gcc -Wall named_semaphore.c -o main.out

run:
	./main.out

clean:
	rm *.out
	echo 'Cleaning done'
