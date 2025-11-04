#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10000

// Struct for thread arguments
typedef struct
{
    int left;
    int right;
    int *arr;
} ThreadArgs;

// Merge function (same as your original)
void merge(int arr[], int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void *threaded_merge_sort(void *arg)
{
    ThreadArgs *thread_args = (ThreadArgs *)arg;
    if (thread_args->left < thread_args->right)
    {
        int mid = (thread_args->left + thread_args->right) / 2;

        pthread_t left_arr_thread;
        pthread_t right_arr_thread;
        
        // ThreadArgs* left_arr_arg = malloc(sizeof(ThreadArgs));
        ThreadArgs left_arr_args;
        left_arr_args.arr = thread_args->arr;
        left_arr_args.left = thread_args->left;
        left_arr_args.right = mid;

        // ThreadArgs* right_arr_arg = malloc(sizeof(ThreadArgs));
        ThreadArgs right_arr_args;
        right_arr_args.arr = thread_args->arr;
        right_arr_args.left = mid+1;
        right_arr_args.right = thread_args->right;

        pthread_create(&left_arr_thread, NULL, threaded_merge_sort, &left_arr_args);
        pthread_create(&right_arr_thread, NULL, threaded_merge_sort, &right_arr_args);

        pthread_join(left_arr_thread, NULL);
        pthread_join(right_arr_thread, NULL);

        merge(thread_args->arr, thread_args->left, mid, thread_args->right);
    }
}

int main()
{
    FILE *file = fopen("input", "r");
    if (!file)
    {
        printf("Error opening file.\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);
    int arr[MAX];
    for (int i = 0; i < n; i++)
        fscanf(file, "%d", &arr[i]);
    fclose(file);

    printf("Unsorted array:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    pthread_t main_thread;
    ThreadArgs threads_main_args;
    threads_main_args.arr = arr;
    threads_main_args.left = 0;
    threads_main_args.right = n-1;

    pthread_create(&main_thread, NULL, threaded_merge_sort, &threads_main_args);

    pthread_join(main_thread, NULL);

    printf("Sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
