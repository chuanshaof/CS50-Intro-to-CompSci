#include <cs50.h>
#include <stdio.h>

int main(void)
{

    int n;
    int j;
    int i;
    int k;

    do // Prompt height within range
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    for (i = 0; i < n; i++) // Loop for rows
    {
        for (k = n; k > i + 1; k--) // Loop for spaces
        {
            printf(" ");
        }
        for (j = 0; j < i + 1; j++) // Loop for hashes
        {
            printf("#");
        }
        printf("\n");
    }
}
