debug:
	gcc -Wall -g simple_ls.c -o main.out
	
valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./main.out -i /lab/bin

compile:
	gcc -Wall simple_ls.c -o main.out

run1:
	./main.out
	ls

run2:
	./main.out -i
	ls -i
	
run3:
	./main.out -n
	ls -n
	
run4:
	./main.out /lab/bin
	ls /lab/bin
	
run5:
	./main.out -i /lab/bin
	ls -i /lab/bin
	
run6:
	./main.out -n /lab/bin
	ls -n /lab/bin
	
clean:
	rm *.out
	echo 'Cleaning done'
