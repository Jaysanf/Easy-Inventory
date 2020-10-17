#include <stdio.h>
#include <ctype.h>
#include <string.h>


int hashing(const char *word);

int main(void)
{
    char *word = "ALLO'";
    char word_nocap[strlen(word)];
    for(int i = 0; word[i]; i++)
    {
        word_nocap[i] = tolower(word[i]);
    }
    

    printf("%s\n", word_nocap);
}
