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
	gcc -Wall -g named_semaphore.c -o named.out -lpthread -lrt
	
valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./named.out $$ARGS

compile:
	gcc -Wall named_semaphore.c -o named.out -lpthread -lrt

run:
	./named.out $$ARGS

debugEC:
	gcc -Wall -g unnamed_semaphore.c -o unnamed.out -lpthread -lrt
	
valgrindEC:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./unnamed.out $$ARGS

compileEC:
	gcc -Wall unnamed_semaphore.c -o unnamed.out -lpthread -lrt

runEC:
	./unnamed.out $$ARGS
	
clean:
	rm *.out
	echo 'Cleaning done'
