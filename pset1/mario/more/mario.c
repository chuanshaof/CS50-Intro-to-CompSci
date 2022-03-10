#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    int i;
    int j;
    int k;
    int l;

    do // Get the height within acceptable ranges
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    for (i = 0; i < n; i++)// Determine rows according to n
    {
        for (j = n; j > i + 1; j--)// Print out spaces according to row number
        {
            printf(" ");
        }
        for (k = 0; k < i + 1; k++)// Print out hashes according to row number
        {
            printf("#");
        }
        printf("  ");// Print out 2 spaces for split
        for (k = 0; k < i + 1; k++)// Print out hashes again
        {
            printf("#");
        }
        printf("\n");//Split to next row
    }
}