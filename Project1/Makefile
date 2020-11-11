debug:
	gcc -Wall -g token_ring.c -o token_ring.out

valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./token_ring.out
		 
compile:
	gcc -Wall token_ring.c -o token_ring.out

run:
	./token_ring.out
	
clean:
	rm *.out
	echo 'Cleaning done'

