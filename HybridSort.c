#include <stdio.h>
#include <stdlib.h>

#define S 10  // Threshold for switching to Insertion Sort

// Insertion Sort function for small subarrays
void insertionSort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
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

// Utility function to print an array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Main function to test the hybrid MergeSort algorithm
int main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];
    generateArray(arr, n, 1000);  // Generating random array with elements between 1 and 1000

    printf("Original array: \n");
    printArray(arr, n);

    hybridMergeSort(arr, 0, n - 1);

    printf("Sorted array: \n");
    printArray(arr, n);

    return 0;
}
