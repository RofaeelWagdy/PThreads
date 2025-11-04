all: merge_sort 

merge_sort: merge_sort.o
	gcc Merge_sort.o -o merge_sort

merge_sort.o: Merge_sort.c
	gcc -c Merge_sort.c

clean:
	rm *.o merge_sort