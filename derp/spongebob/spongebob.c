#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string spongebob = get_string ("Text: ");

    for (int i = 0, j = 0, n = strlen(spongebob); i < n; i++)
    {
        if (isspace(spongebob[i]))
        {
            printf("");
        }
        else if (!isalpha(spongebob[i]))
        {
            printf("%c ", spongebob[i]);
        }
        else if (j % 2 ==  1)
        {
            printf("%c ", toupper(spongebob[i]));
            j++;
        }
        else if (j % 2 == 0)
        {
            printf("%c ", tolower(spongebob[i]));
            j++;
        }
    }
    printf("\n");
}