# PThreads Concurrency Showcase

Multi-threaded implementations of Matrix Multiplication and Merge Sort using POSIX Threads in C.

## Overview
This repository serves as a practical demonstration of OS-level concurrency and parallel computing techniques. It features two classic algorithmic problems—Matrix Multiplication and Merge Sort—optimized through multithreading using the POSIX Threads (`pthreads`) library. The project illustrates the performance implications and implementation details of dividing computational workloads across multiple threads.

## Key Features
- **Parallel Matrix Multiplication**: Implements and benchmarks two distinct threading strategies:
  - **Per-Element**: Spawns a dedicated thread for calculating each individual element of the result matrix.
  - **Per-Row**: Spawns a thread for calculating an entire row of the result matrix, showcasing a different granularity of parallelism.
- **Multithreaded Merge Sort**: Utilizes a recursive, divide-and-conquer strategy where separate threads are spawned to sort the left and right halves of an array concurrently.
- **Performance Profiling**: Includes execution time measurement (`sys/time.h`) to compare the overhead and efficiency of different threading strategies.
- **Dynamic Memory Management**: Safely allocates and deallocates multi-dimensional arrays and thread arguments.

## Technical Skills
- **Multithreading & Concurrency**: POSIX Threads (`pthread_create`, `pthread_join`), race condition avoidance.
- **C Systems Programming**: Pointers, structs, manual memory management (`malloc`, `free`), standard I/O.
- **Performance Analysis**: Benchmarking execution time to understand thread creation overhead versus parallel speedup.
- **Algorithms**: Divide-and-conquer (Merge Sort), Linear Algebra operations (Matrix Multiplication).

## Tech Stack
- **Language**: C
- **Libraries**: `<pthread.h>`, `<sys/time.h>`, `<stdio.h>`, `<stdlib.h>`
- **Build System**: Make / GCC

## Project Structure
```text
.
├── Makefile                     # Build instructions for compiling the programs
├── Matrix_Multiplication.c      # Parallel matrix multiplication implementation
├── Merge_sort.c                 # Parallel merge sort implementation
└── sample_input.txt / input     # Data files containing matrix dimensions/values and array inputs
```

## Getting Started

### Prerequisites
To build and run these programs, you need a POSIX-compliant operating system (Linux, macOS, or WSL on Windows) with the following installed:
- GCC (GNU Compiler Collection)
- GNU Make

## Configuration

Both programs read their input from a file named `input` located in the same directory as the executable.

**For Matrix Multiplication (`matrix_mul`)**, the `input` file should contain two matrices formatted as:
1. `Rows Columns` for Matrix 1, followed by the matrix elements.
2. `Rows Columns` for Matrix 2, followed by the matrix elements.

**For Merge Sort (`merge_sort`)**, the `input` file should contain:
1. The total number of elements `N`.
2. A space-separated list of `N` integers.

## Running the Project

1. **Build the executables** using Make:
   ```bash
   make all
   ```
2. **Run Matrix Multiplication**:
   Make sure you have a valid `input` file configured for matrices, then run:
   ```bash
   ./matrix_mul
   ```
3. **Run Merge Sort**:
   Update your `input` file for array sorting, then run:
   ```bash
   ./merge_sort
   ```

## Usage Examples

**Example `input` for Matrix Multiplication:**
```text
2 3
1 2 3
4 5 6
3 2
7 8
9 1
2 3
```

**Example `input` for Merge Sort:**
```text
5
38 27 43 3 9
```