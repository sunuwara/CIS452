debug1:
	clang -Wall -g sample_program1.c -o sp1.out -lefence
	
valgrind1:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./sp1.out

compile1:
	clang -Wall sample_program1.c -o sp1.out

run1:
	./sp1.out

sharedLib:
	gcc -c -Wall -Werror -fPIC shared.c
	gcc -shared -o libshared.so shared.o
	
debug:
	clang -L/home/sunuwara/repos/CIS452/Lab08 -Wall -g memoryMap.c -o main.out -lefence -lshared
	
valgrind:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./main.out

compile:
	clang -L/home/sunuwara/repos/CIS452/Lab08 -Wall memoryMap.c -o main.out -lshared

run:
	./main.out	
	
clean:
	rm *.out
	echo 'Cleaning done'
