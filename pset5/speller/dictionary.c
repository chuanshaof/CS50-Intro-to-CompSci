// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Keep track of the number of words
unsigned int words = 0;

// Number of buckets in hash table
// Number chosen based on 1.3x the keys and prime number
const unsigned int N = 186019;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int bucket = hash(word);

    // Set temporary hash table
    node *cursor;
    cursor = table[bucket];

    // Run through the linked list
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
            return true;

        else
            cursor = cursor->next;
    }
    return false;
}

// Adapted from Reddit user u/delipity
// https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
unsigned int hash(const char *word)
{
    unsigned int hash = 0;

    // Using bitwise operator to reduce collisions
    for (int i = 0, n = strlen(word); i < n; i++)
        hash = (hash << 2) ^ tolower(word[i]);

    // Ensure that returned value does not exceed number of "buckets"
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");

    if (dict == NULL)
        return false;

    char raw[LENGTH + 1];

    // Read words from dictionary into raw[array]
    while (fscanf(dict, "%s", raw) !=  EOF)
    {
        node *n = malloc(sizeof(node));

        if (n == NULL)
            return false;

        // Copy into word node
        strcpy(n->word, raw);

        // Get bucket
        int bucket = hash(raw);

        // If it is currently NULL, set as a new linked list
        if (table[bucket] == NULL)
        {
            table[bucket] = n;
            n->next = NULL;
        }
        // If already filled, add to the linked list
        else
        {
            n->next = table[bucket];
            table[bucket] = n;
        }
        // Add to global variable "Words" to keep track of dictionary size
        words++;
    }

    // End of load
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp, *cursor;

    // Loop through the entire hash table
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];

        // Free the linked list from the front
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }

        // If reached the end of the table
        if (i == (N - 1))
            return true;
    }
    return false;
}