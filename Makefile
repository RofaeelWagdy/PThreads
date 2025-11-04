all: matrix_mul merge_sort

# Compiling Matrix Multiplication
matrix_mul : matrix_mul.o
	gcc Matrix_Multiplication.o -o matrix_mul

matrix_mul.o : Matrix_Multiplication.c
	gcc -c Matrix_Multiplication.c

# Compiling Merge Sort
merge_sort: merge_sort.o
	gcc Merge_sort.o -o merge_sort

merge_sort.o: Merge_sort.c
	gcc -c Merge_sort.c

clean:
	rm *.o merge_sort matrix_mul