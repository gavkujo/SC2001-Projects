#include <stdio.h>
#include <stdlib.h>
#include <time.h> //to calculate the time complexity

#define NUM_TESTS 5 //number of tests to average

long long comparisons = 0; //to count the number to comparisons

//insertion sort function to sort small subarrays

void insertionSort(int arr[], int left , int right ) //start index - end index
{
 //traverse through the array
 for(int i = left+1; i<= right ; i++)
 {
    //take out the value
    int value = arr[i];
    int position = i;

    //go through the while loop to compare
    while(position > 0 && value < arr[position - 1])
    {
        comparisons++;
        //move the larger value to the right
        arr[position] = arr[position - 1];
        //decrement the position to move the smaller value to the left
        position --;


    }

    //spot found!

     arr[position] = value;


 }


}

//Merge sort for larger arrays

void merge(int arr[], int left, int mid, int right)
{
    //number of elements in the left
    int nleft  = (mid - left) + 1;
    //number of elements in the right
    int nright =    right - mid;

    //allocating memory for 2 arrays
    int *leftarray = malloc(nleft * sizeof(int));
    int *rightarray = malloc(nright * sizeof(int));

    //Populate the left array
    for(int i = 0 ; i< nleft ; i++ )
    {
        leftarray[i] = arr[left + i];
    }
    //populate the right array
    for(int i = 0 ; i< nright ; i++)
    {

        rightarray[i] = arr[mid + 1 + i];
    }


    //MERGING BOTH

    int i = left ; //pointer for arr
    int l = 0 ; //left array pointer
    int r = 0; //right array pointer

    while(l<nleft && r < nright)
    {
         comparisons++;
        if(leftarray[l] <= rightarray[r])
        {
             arr[i] = leftarray[l];
             l++;
        }
        else
        {
          arr[i] = rightarray[r];
             r++;
        }
        i++;

    }

    //remaining elements

    while(l<nleft)
    {
      arr[i] = leftarray[l];
      l++;
      i++;
    }

    while(r<nright)
    {
      arr[i] = rightarray[r];
      r++;
      i++;
    }

    //free
    free(leftarray);
    free(rightarray);

}

//Hybrid Merge

void hybrideMergeSort(int arr[], int left , int right, int S)
{
    //check with the array size is smaller then threshold S
    int n = (right - left)+1;
    if(n <= S)
    {
        insertionSort(arr,left,right);
    }
    else
    {
        //terminate recurrsion line
        if(left < right)
        {
            int mid = left + (right - left) / 2;
            //left tree
            hybrideMergeSort(arr,left,mid,S);
            //right tree
            hybrideMergeSort(arr,mid+1, right,S);
            //merge both
            merge(arr,left,mid,right);

        }
    }

}

//Function to generate random arrays
void generateArray(int arr[], int size , int max_value)
{
    for(int i = 0 ; i<size ; i++)
    {
        arr[i] = rand() % max_value+1; //within the max_value
    }

}

//function to perform sorting and record comparisions
long long performSortingAndRecord(int size, int S) {
    long long totalComparisons = 0;

    for (int test = 0; test < NUM_TESTS; test++) {
        int *arr = (int *)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation failed for array size %d\n", size);
            return -1;
        }

        generateArray(arr, size, 1000);  // Generate random array

        comparisons = 0;  // Reset comparisons for each test
        hybrideMergeSort(arr, 0, size - 1, S);  // Perform hybrid merge sort
        totalComparisons += comparisons;  // Accumulate comparisons

        free(arr);
    }

    return totalComparisons / NUM_TESTS;  // Return average comparisons
}


int main()
{
      srand(time(NULL));  // Initialize random number generator
      //writing to the file
       FILE *file = fopen("optimal_S.csv", "w");  // Open CSV file for writing
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fprintf(file, "InputSize,S,KeyComparisons\n");  // Write CSV header
    int inputSizes[] = {100, 1000, 5000, 10000, 50000};  // Array of input sizes
    int numSizes = sizeof(inputSizes) / sizeof(inputSizes[0]);
     for (int i = 0; i < numSizes; i++) {
        int n = inputSizes[i];  // Get the input size

        // Test with different values of S for each input size
        for (int s = 1; s <= 100; s += 5) {
            long long avgComparisons = performSortingAndRecord(n, s);
            if (avgComparisons >= 0) {
                fprintf(file, "%d,%d,%lld\n", n, s, avgComparisons);  // Write results to CSV
            }
        }
    }

    fclose(file);  // Close the CSV file
    printf("Data written to optimal_S.csv\n");


    return 0;
}
