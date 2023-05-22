CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -coverage
EXEC = qsort

	$(CC) $(CFLAGS) qsort.c -o $(EXEC)

	$(CC) $(CFLAGS) -g qsort.c -o $(EXEC)

	$(CC) $(CFLAGS) qsort.c -o $(EXEC)
	./$(EXEC) < tests/test1.txt
	./$(EXEC) < tests/test2.txt
	./$(EXEC) < tests/test3.txt
	./$(EXEC) < tests/test4.txt
	./$(EXEC) < tests/test5.txt
	gcov qsort.c

	rm -f $(EXEC) *.gc* coverage.txt

