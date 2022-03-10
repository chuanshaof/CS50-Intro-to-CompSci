#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text); //Function to count number of LETTERS
int count_words(string text); //Function to count number of WORDS
int count_sentences(string text); //Function to count number of SENTENCES


int main(void)
{
    string text = get_string("Text: ");
    float letters = count_letters(text);
    float words = count_words(text);
    float sentences = count_sentences(text);
    float L = (100 * letters / words);
    float S = (100 * sentences / words);
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    if (index <= 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
    int s = 0;
    for (int i = 0, n = strlen(text); i < n; i++) // For loop to repeat the length of the sentence
    {
        if (isalpha(text[i])) // Rule to only count letters
        {
            s += 1; // Add for every letter
        }
    }
    return s;
}

int count_words(string text)
{
    int s = 1; //Start with 1 because the first and last sentence will not contain any spaces
    for (int i = 0, n = strlen(text); i < n; i++) // For loop to repeat the length of the sentence
    {
        if (isspace(text[i])) // Rule to only count spaces
        {
            s += 1; // Add for every space
        }
    }
    return s;
}

int count_sentences(string text)
{
    int s = 0;
    for (int i = 0, n = strlen(text); i < n; i++) // For loop to repeat the length of the sentence
    {
        if ((text[i] == '!') || (text[i] == '.') || (text[i] == '?'))// Rule to only count ! or . or ?
        {
            s += 1; // Add for every punctuation
        }
    }
    return s;
}