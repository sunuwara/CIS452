compile1:
	gcc -Wall sample_program1.c -o sample1.out

run1:
	./sample1.out
	
compile1Mod:
	gcc -Wall sample_program1_modified.c -o sample1_modified.out

run1Mod:
	./sample1_modified.out
	
debugReader:
	gcc -Wall -g reader_shm.c -o reader_debug.out
	
valgrindReader:
	valgrind --leak-check=full --track-origins=yes ./reader_debug.out
	
compileReader:
	gcc -Wall reader_shm.c -o reader.out

runReader:
	./reader.out
	
debugWriter:
	gcc -Wall -g writer_shm.c -o writer_debug.out
	
valgrindWriter:
	valgrind --leak-check=full --track-origins=yes ./writer_debug.out
	
compileWriter:
	gcc -Wall writer_shm.c -o writer.out

runWriter:
	./writer.out

clean:
	rm *.out
	echo 'Cleaning done'
