 // Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    //declare string variable for a word uncapitalized
    char word_nocap[strlen(word)];
    
    //copy word uncapitalized
    for(int i = 0; word[i]; i++)
    {
        word_nocap[i] = tolower(word[i]);
    }
    
    //hash the word
    int pos_of_word = hash(word_nocap);
    
    //declare current_node to go through all nodes of a pos in the array
    node *current_node = table[pos_of_word];
    
    while(1)
    {
        //checks if the word in the node is equal to our word
        if(strcmp(current_node->word, word_nocap) == 0)
        {
            return true;
        }
        //check if it has reached the end of the linked list
        else if(current_node->next == NULL)
        {
            return false;
        }
        //goes throught next node
        else
        {
            current_node = current_node->next;
        }
    }
    
    
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // algorithm found on StackOverflow: https://stackoverflow.com/questions/20462826/hash-function-for-strings-in-c
    int hashed = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++) {
        // alphabet case
        if (isalpha(word[i]))
            n = word[i] - 'a' + 1;
        else  // comma case
            n = 27;

        hashed = ((hashed << 3) + n) % (N - 1);
    }
    return hashed;
}


//GLOBAL VARIABLE OF NUMBER OF WORDS
int size_of_dict = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open dictionary file
    FILE *dict = fopen(dictionary, "r");
    
    //adds a word to size of dict
    size_of_dict++;
    
    // Checks if dictionary is NULL
    if (dict == NULL)
    {
        return false;
    }
    
    
    //variable for dump word
    char dump_word[LENGTH + 1];
    int pos;
    
  
    
    //goes through the dictionnary word by word
    while (fscanf(dict, "%s", dump_word) != EOF)// Not equal to EOF
    {
        //allocates mem for the node
        node *n = malloc(sizeof(node));
        
        //check if malloc failed
        if(n == NULL)
        {
            return false;
        }
        
        //copy dump word to node
        strcpy(n->word, dump_word);
        n->next = NULL;
        
        //hashes the word
        pos = hash(dump_word);
        
        //replace the head of the linked list at the pos with the new  node and switch its pointer to last node
        n->next = table[pos];
        table[pos] = n;

        
    }
    
    
    
    
    
    
    
    //dictionary has succesfully loaded
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return size_of_dict;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //Declare cursor and temp
    node *cursor;
    node *temp;
    
    for(int i = 0; i < N; i++)
    {
       cursor = table[i];
       while (cursor != NULL)
       {
           temp = cursor;
           cursor = cursor->next;
           free(temp);
       }
    }
    
    return true;
}
