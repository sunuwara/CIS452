debug1:
	gcc -Wall -g sample_program1.c -o sample1.out
	
valgrind1:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sample1.out $$ARGS

compile1:
	gcc -Wall sample_program1.c -o sample1.out

run1:
	./sample1.out $$ARGS
	
debug:
	gcc -Wall -g named_semaphore.c -o main.out -lpthread -lrt
	
valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./main.out $$ARGS

compile:
	gcc -Wall named_semaphore.c -o main.out -lpthread -lrt

run:
	./main.out $$ARGS

clean:
	rm *.out
	echo 'Cleaning done'
