debugEmp:
	gcc -Wall -g empirical.c -o empirical.out -lpthread -lrt
	
valgrindEmp:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./empirical.out

compileEmp:
	gcc -Wall empirical.c -o empirical.out -lpthread -lrt

runEmp:
	./empirical.out
	
debugDyn:
	gcc -Wall -g dynamic.c -o dynamic.out
	
valgrindDyn:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./dynamic.out

compileDyn:
	gcc -Wall dynamic.c -o dynamic.out

runDyn:
	./dynamic.out

clean:
	rm *.out
	echo 'Cleaning done'
