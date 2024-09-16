#include <stdio.h>
#include <stdlib.h>

#define S 10  // Threshold for switching to Insertion Sort
#define NUM_TESTS 5  // Number of tests to average

// Global variable to count the number of comparisons
long long comparisons = 0;

// Insertion Sort function for small subarrays
void insertionSort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            comparisons++;  // Increment comparisons when comparing arr[j] and key
            arr[j + 1] = arr[j];
            j--;
        }
        comparisons++;  // Increment comparisons for the last failed comparison in while
        arr[j + 1] = key;
    }
}

// Merging function used in Mergesort
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary arrays
    int L[n1], R[n2];

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    // Merge the temporary arrays back into arr[]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        comparisons++;  // Increment comparisons for comparing L[i] and R[j]
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Hybrid Mergesort function
void hybridMergeSort(int arr[], int left, int right) {
    if (right - left + 1 <= S) {
        // If subarray size is smaller than threshold, use Insertion Sort
        insertionSort(arr, left, right);
    } else {
        if (left < right) {
            int mid = (left + right) / 2;

            // Recursively apply Mergesort on both halves
            hybridMergeSort(arr, left, mid);
            hybridMergeSort(arr, mid + 1, right);

            // Merge the sorted halves
            merge(arr, left, mid, right);
        }
    }
}

// Function to generate random arrays
void generateArray(int arr[], int size, int max_value) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % max_value + 1;
    }
}

// Function to perform sorting and record comparisons
long long performSortingAndRecord(int size) {
    long long totalComparisons = 0;

    for (int test = 0; test < NUM_TESTS; test++) {
        int *arr = (int *)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation failed for array size %d\n", size);
            return -1;  // Indicate an error
        }

        generateArray(arr, size, 1000);  // Generating random array with elements between 1 and 1000

        // Reset the comparison count for each test
        comparisons = 0;

        // Perform hybrid MergeSort
        hybridMergeSort(arr, 0, size - 1);

        // Accumulate the total comparisons
        totalComparisons += comparisons;

        free(arr);
    }

    // Return the average number of comparisons
    return totalComparisons / NUM_TESTS;
}

int main() {
    FILE *file = fopen("key_comparisons3.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(file, "Size,KeyComparisons\n");  // Header for CSV file

    // Start from 1000 to 1 million
    int start = 148800;
    int end = 1000000;  // Reduced maximum size
    int step = 100;    // Increased step size

    for (int size = start; size <= end; size += step) {
        long long avgComparisons = performSortingAndRecord(size);
        if (avgComparisons >= 0) {  // Only write to file if no error occurred
            fprintf(file, "%d,%lld\n", size, avgComparisons);
        } else {
            printf("Error occurred with array size %d\n", size);
            break;  // Exit the loop if an error occurred
        }
    }

    fclose(file);
    return 0;
}

