// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>    // added
#include <stdio.h>
#include <cs50.h>       // added
#include <stdlib.h>     // added

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 * 27 * 27;
// Hash table and initialization
node *table[N] = {NULL, };
// Dictionary size
int DIC_size = 0;
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // define which category the word in and set a template
    node *cursor = table[hash(word)];
    // searching through the linked list
    while (cursor)
    {
        if (strcasecmp(cursor -> word, word) == 0)
        {
            return true;
        }
        cursor = cursor -> next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // Use three letters to make hash table
    int A[2];
    // Hash 2nd and 3rd letter
    for (int i = 0; i < 2; i++)
    {
        if (isalpha(word[i + 1]))
        {
            A[i] = toupper(word[i + 1]) - 'A' + 1;
        }
        else
        {
            A[i] = 0;
        }
    }
    return ((toupper(word[0]) - 'A') * 27 * 27) + (A[0] * 27) + A[1];
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open file
    FILE *DIC = fopen(dictionary, "r");
    if (DIC == NULL)
    {
        printf("Couldn't open %s.\n", dictionary);
        return false;
    }
    // create buffer to store words in DIC
    char buffer[LENGTH + 1];
    while (fscanf(DIC, "%s", buffer) != EOF)
    {
        // create new node
        node *newnode = malloc(sizeof(node));
        // Check whether memory allocate sucess
        if (newnode == NULL)
        {
            printf("memory not enough.\n");
            return false;
            fclose(DIC);
            free(newnode);
            unload();
        }
        // Copy word into node & insert node into hash table
        strcpy(newnode -> word, buffer);
        newnode -> next = table[hash(buffer)];
        table[hash(buffer)] = newnode;
        // Count dictionary size
        DIC_size++;
    }
    fclose(DIC);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return DIC_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Make two node template
    node *cursor;
    node *tmp;
    // Walk through hash table with size N
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        tmp = table[i];
        // Free memory on each linked list
        while (cursor)
        {
            cursor = cursor -> next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}
