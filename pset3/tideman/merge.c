#include <stdio.h>

void mergeSort(int original_array[], int l, int r);
void merge(int original_array[], int l, int m, int r);

int main (void)
{
    int array[10] = {3, 1, 2, 4, 5, 2, 3, 4, 6, 2};

    mergeSort(array, 0, 9);

    for (int  i = 0; i < 10; i++)
    {
        printf("%i", array[i]);
    }
    printf("\n");
}

void mergeSort(int original_array[], int l, int r)
{
    if (l < r)
    {
        int m = (l+r)/2;

        // Sort first and second halves
        mergeSort(original_array, l, m);
        mergeSort(original_array, m + 1, r);

        merge(original_array, l, m, r);
    }
}

void merge(int original_array[], int l, int m, int r)
{
    int L = m - l + 1;
    int R = r - m;

    // Create a temporary arrays
    int left_array[L], right_array[R];

    for (int i = 0; i < R; i++)
    {
        left_array[i] = original_array[l + i];
        right_array[i] = original_array[m + 1 + i];
    }

    // If total array is an odd number
    left_array[L - 1] = original_array[m];

    int i = 0, j = 0;
    for (int n = l; n < r + 1; n++)
    {
        if (i == L) // i = 0
        {
            original_array[n] = right_array[j];
            j++;
        }
        else if (j == R) // j = 0
        {
            original_array[n] = left_array[i];
            i++;
        }
        else if (left_array[i] < right_array[j])
        {
            original_array[n] = left_array[i];
            i++;
        }
        else
        {
            original_array[n] = right_array[j];
            j++;
        }
    }
}