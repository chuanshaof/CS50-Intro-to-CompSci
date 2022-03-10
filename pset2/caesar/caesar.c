#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if (argc != 2) // Validate 2 command-line variants
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    for (int i = 0, n = strlen(argv[1]); i < n; i++) // Loop through argv[1]
    {
        if (isdigit(argv[1][i])) // Validate for non-digits
        {
            continue;
        }
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    int key = atoi(argv[1]); // Convert argv[1] to an int
    string text = get_string("plaintext: "); // Prompt for text
    printf("ciphertext: ");

    for (int i = 0, n = strlen(text); i < n; i++) // Loop through every character of text
    {
        int upper, lower, c, d, e, f;
        if (isupper(text[i])) // Take only UPPERCASE into consideration
        {
            upper = text[i] - 65; // Minus ASCII code for A
            c = (upper + key) % 26; // Fomulate and wrap Z back to A
            d = c + 65; // Return back to original ASCII code
            printf("%c", d);
        }
        else if (islower(text[i])) // Take only LOWERCASE into consideration
        {
            lower = text[i] - 97; // Minus ASCII code for a
            e = (lower + key) % 26; // Fomulate and wrap z back to a
            f = e + 97; // Return back to orginal ASCII code
            printf("%c", f);
        }
        else // Take EVERYTHING ELSE into consideration
        {
            printf("%c", text[i]);
        }
    }
    printf("\n");
}