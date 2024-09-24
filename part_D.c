#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define S 7       // Threshold for switching to Insertion Sort in Hybrid Sort
#define NUM_TESTS 30  // Number of tests to average

// Global variables to count the number of comparisons
long long comparisonsHybrid = 0;
long long comparisonsMerge = 0;

// Insertion Sort function for small subarrays
void insertionSort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            comparisonsHybrid++;  // Increment comparisons for Hybrid Sort
            arr[j + 1] = arr[j];
            j--;
        }
        comparisonsHybrid++;  // Increment comparisons for the last failed comparison in while
        arr[j + 1] = key;
    }
}

// Merging function used in both Mergesort and Hybrid Sort
void merge(int arr[], int left, int mid, int right, long long *comparisons) {
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
        (*comparisons)++;  // Increment comparisons
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

// Standard Merge Sort function
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        // Recursively apply Merge Sort on both halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right, &comparisonsMerge);
    }
}

// Hybrid Merge Sort function
void hybridMergeSort(int arr[], int left, int right) {
    if (right - left + 1 <= S) {
        // If subarray size is smaller than threshold, use Insertion Sort
        insertionSort(arr, left, right);
    } else {
        if (left < right) {
            int mid = (left + right) / 2;

            // Recursively apply Hybrid Merge Sort on both halves
            hybridMergeSort(arr, left, mid);
            hybridMergeSort(arr, mid + 1, right);

            // Merge the sorted halves
            merge(arr, left, mid, right, &comparisonsHybrid);
        }
    }
}

// Function to generate random arrays
void generateArray(int arr[], int size, int max_value) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % max_value + 1;
    }
}

// Function to perform Hybrid Sort and record comparisons and time
long long performHybridSortAndRecord(int size, double *timeTaken) {
    long long totalComparisons = 0;
    double totalTime = 0.0;

    for (int test = 0; test < NUM_TESTS; test++) {
        int *arr = (int *)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation failed for array size %d\n", size);
            return -1;  // Indicate an error
        }

        generateArray(arr, size, 1000);  // Generating random array with elements between 1 and 1000

        // Reset the comparison count for each test
        comparisonsHybrid = 0;

        // Measure start time
        clock_t start = clock();

        // Perform Hybrid Merge Sort
        hybridMergeSort(arr, 0, size - 1);

        // Measure end time
        clock_t end = clock();

        // Accumulate the total comparisons and time
        totalComparisons += comparisonsHybrid;
        totalTime += (double)(end - start) / CLOCKS_PER_SEC;

        free(arr);
    }

    // Set the average time taken for Hybrid Sort
    *timeTaken = totalTime / NUM_TESTS;

    // Return the average number of comparisons
    return totalComparisons / NUM_TESTS;
}

// Function to perform standard Merge Sort and record comparisons and time
long long performMergeSortAndRecord(int size, double *timeTaken) {
    long long totalComparisons = 0;
    double totalTime = 0.0;

    for (int test = 0; test < NUM_TESTS; test++) {
        int *arr = (int *)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation failed for array size %d\n", size);
            return -1;  // Indicate an error
        }

        generateArray(arr, size, 1000);  // Generating random array with elements between 1 and 1000

        // Reset the comparison count for each test
        comparisonsMerge = 0;

        // Measure start time
        clock_t start = clock();

        // Perform standard Merge Sort
        mergeSort(arr, 0, size - 1);

        // Measure end time
        clock_t end = clock();

        // Accumulate the total comparisons and time
        totalComparisons += comparisonsMerge;
        totalTime += (double)(end - start) / CLOCKS_PER_SEC;

        free(arr);
    }

    // Set the average time taken for Merge Sort
    *timeTaken = totalTime / NUM_TESTS;

    // Return the average number of comparisons
    return totalComparisons / NUM_TESTS;
}

int main() {
    FILE *file = fopen("Partd_key_comparisons.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(file, "Size,HybridKeyComparisons,MergeSortKeyComparisons,HybridTime,MergeSortTime\n");  // Header for CSV file

    // Start from 1000 to 1000000 with step size 100
    int start = 1000;
    int end = 1000000;
    int step = 100;

    for (int size = start; size <= end; size += step) {
        double hybridTime = 0.0;
        double mergeTime = 0.0;

        long long avgHybridComparisons = performHybridSortAndRecord(size, &hybridTime);
        long long avgMergeComparisons = performMergeSortAndRecord(size, &mergeTime);

        if (avgHybridComparisons >= 0 && avgMergeComparisons >= 0) {  // Only write to file if no error occurred
            fprintf(file, "%d,%lld,%lld,%.6f,%.6f\n", size, avgHybridComparisons, avgMergeComparisons, hybridTime, mergeTime);
        } else {
            printf("Error occurred with array size %d\n", size);
            break;  // Exit the loop if an error occurred
        }
    }

    fclose(file);
    return 0;
}

