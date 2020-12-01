debug1:
	gcc -Wall -g sample_program1.c -o sp1.out
	
valgrind1:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sp1.out $$ARGS

compile1:
	gcc -Wall sample_program1.c -o sp1.out

run1:
	./sp1.out $$ARGS
	
debug2:
	gcc -Wall -g sample_program2.c -o sp2.out
	
valgrind2:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sp2.out

compile2:
	gcc -Wall sample_program2.c -o sp2.out

run2:
	./sp2.out
	
clean:
	rm *.out
	echo 'Cleaning done'
