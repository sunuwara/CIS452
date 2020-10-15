debug:
	gcc -Wall -g empirical.c -o empirical.out -lpthread -lrt
	
valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./empirical.out

compile:
	gcc -Wall empirical.c -o empirical.out -lpthread -lrt

run:
	./empirical.out
	
clean:
	rm *.out
	echo 'Cleaning done'
