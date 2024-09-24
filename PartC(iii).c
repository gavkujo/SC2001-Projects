#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TESTS 10
#define MAX_S 128 // Maximum S value to test

long long comparisons = 0; // to count the number of comparisons


//INSERTION SORT//
void insertionSort(int arr[], int left, int right) {
    //always start with 1 not 0 because the first element is always sorted
    for (int i = left + 1; i <= right; i++) {
        int value = arr[i];
        int position = i;

        while (position > left && value < arr[position - 1]) {
            comparisons++;
            //BIGGER elements move to the right
            arr[position] = arr[position - 1];
            //smaller elements move to the left
            position--;
        }

        //FIND THE SPOT

        arr[position] = value;
    }
}

void merge(int arr[], int left, int mid, int right) {
    int nleft = (mid - left) + 1;
    int nright = right - mid;
    int *leftarray = malloc(nleft * sizeof(int));
    int *rightarray = malloc(nright * sizeof(int));

    for (int i = 0; i < nleft; i++) {
        leftarray[i] = arr[left + i];
    }
    for (int i = 0; i < nright; i++) {
        rightarray[i] = arr[mid + 1 + i];
    }

    int i = left, l = 0, r = 0;

    while (l < nleft && r < nright) {
        comparisons++;
        if (leftarray[l] <= rightarray[r]) {
            arr[i] = leftarray[l];
            l++;
        } else {
            arr[i] = rightarray[r];
            r++;
        }
        i++;
    }

    while (l < nleft) {
        arr[i] = leftarray[l];
        l++;
        i++;
    }

    while (r < nright) {
        arr[i] = rightarray[r];
        r++;
        i++;
    }

    free(leftarray);
    free(rightarray);
}

void hybrideMergeSort(int arr[], int left, int right, int S) {
    int n = (right - left) + 1;
    //if the array size is smaller than the THRESHOLD, do Insertion sort
    if (n <= S) {
        insertionSort(arr, left, right);
    } else {
        if (left < right) {
            int mid = left + (right - left) / 2;
            hybrideMergeSort(arr, left, mid, S);
            hybrideMergeSort(arr, mid + 1, right, S);
            merge(arr, left, mid, right);
        }
    }
}

void generateArray(int arr[], int size, int max_value) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % max_value + 1;
    }
}

//RECORDING AVERAGE TIME AND THE TOTAL NUMBER OF COMPARISIONS
void performSortingAndRecord(int size, FILE *file) {
    long long totalComparisons[MAX_S + 1] = {0}; //to store the number of comparisons
    double totalTime[MAX_S + 1] = {0.0}; // total time taken for each value of S

    for (int s = 1; s <= MAX_S; s++) {
            //EACH VALUE OF S, THERE ARE 5 tests
        for (int test = 0; test < NUM_TESTS; test++) {
            int *arr = (int *)malloc(size * sizeof(int));
            if (arr == NULL) {
                printf("Memory allocation failed for array size %d\n", size);
                return;
            }

            generateArray(arr, size, 1000); //random numbers in the array
            comparisons = 0; //INITIALIZE THE COMPARISONS
            clock_t start = clock(); //START THE TIMER
            hybrideMergeSort(arr, 0, size - 1, s);//DO THE HYBRID SORT
            clock_t end = clock(); //STOP THE TIMER

            totalComparisons[s] += comparisons;
            totalTime[s] += (double)(end - start) / CLOCKS_PER_SEC;

            free(arr);
        }
        totalComparisons[s] /= NUM_TESTS;
        totalTime[s] /= NUM_TESTS;
    }

    // Write results to CSV
    for (int s = 10; s <= MAX_S; s++) {
        fprintf(file, "%d,%d,%lld,%f\n", size, s, totalComparisons[s], totalTime[s]);
    }
}

int main() {
    srand(time(NULL));

    // Open CSV file for writing
    FILE *file = fopen("optimal_S_results.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Write CSV header
    fprintf(file, "InputSize,S,AverageComparisons,AverageTime\n");

    // Test with various input sizes
    int inputSizes[] = {100, 1000, 10000, 100000, 200000, 300000, 400000, 500000, 750000, 1000000,10000000};
    int numSizes = sizeof(inputSizes) / sizeof(inputSizes[0]);

    for (int i = 0; i < numSizes; i++) {
        performSortingAndRecord(inputSizes[i], file);
    }

    fclose(file); // Close the CSV file
    printf("Data written to optimal_S_results.csv\n");

    return 0;
}

