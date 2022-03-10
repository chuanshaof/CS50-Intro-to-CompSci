#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2) // Validate 2 command-line variants
    {
        printf("Usage: ./substituition key\n");
        return 1;
    }

    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    for (int i = 0; i < 26; i++) // Loop through argv[1]
    {
        if (isalpha(argv[1][i])) // Validate for non-digits
        {
            continue;
        }
        else
        {
            printf("Key must only be alphabetic characters.\n");
            return 1;
        }
    }

    for (int i = 0; i < 26; i++) // Loop through argv[1] by i
    {
        for (int j = i + 1; j < 26; j++) // Loop through argv[1] by j
        {
            if (argv[1][i] == argv[1][j]) // Return 1 when repeated characters
            {
                printf("Key must not contain repeated characters\n");
                return 1;
            }
        }
    }

    string text = get_string("plaintext: "); // Prompt for text
    printf("ciphertext: ");

    for (int i = 0, n = strlen(text); i < n; i++) // Loop through every character of text
    {
        int upper, lower;
        if (isupper(text[i])) // Take only UPPERCASE into consideration
        {
            upper = text[i] - 65; // Minus ASCII code for A, now A = 0, B = 1...
            printf("%c", toupper(argv[1][upper])); // Print UPPERCASE letter per argv[1]
        }
        else if (islower(text[i])) // Take only LOWERCASE into consideration
        {
            lower = text[i] - 97; // Minus ASCII code for a, now a = 0 , b = 1...
            printf("%c", tolower(argv[1][lower])); // Print LOWERCASE letter per argv[1]
        }
        else // Take EVERYTHING ELSE into consideration
        {
            printf("%c", text[i]);
        }
    }
    printf("\n");
}