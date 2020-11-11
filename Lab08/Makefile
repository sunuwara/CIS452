debug1:
	clang -Wall -g sample_program1.c -o sp1_debug.out -lefence
	
valgrindEmp:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sp1_debug.out

compile1:
	clang -Wall sample_program1.c -o sp1.out

run1:
	./sp1.out
	
clean:
	rm *.out
	echo 'Cleaning done'
