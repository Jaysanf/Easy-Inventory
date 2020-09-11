#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

bool letter_check(char c_b);
bool space_check(char c_b);
bool ponct_check(char c_b);

int main(void)
{
    //main function
    string s = get_string("Text: ");
    
    // counts the words
    int word_counter = 0;
    
    //counts the sentences
    int sentence_counter = 0;
    
    //counts the letters
    int letter_counter = 0;
    
    //Loop that goues through the text char by char
    for (int i = 0, lenght = strlen(s); i < lenght; i++)
    {
        //if statement to check the value of char
        if (letter_check(s[i]))
        {
            letter_counter++;
        }
        else if (ponct_check(s[i]))
        {
            word_counter++;
            sentence_counter++;
        }
        //if its a space and the character before isn't a ponct, add a word to word counter
        else if (space_check(s[i]) && !(ponct_check(s[i - 1])))
        {
            word_counter++;
        }
    }
    // the average number of letters per 100 words in the text
    float L = ((float) letter_counter * 100) / (float) word_counter;
    
    // the average number of sentences per 100 words in the text
    float S = ((float) sentence_counter * 100) / (float) word_counter;
    
    // Formula of Coleman-Liau index and rounding it
    float index = 0.0588 * L - 0.296 * S - 15.8;
    index = round(index);
    
    //Grade checker
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) index); 
    }
    
}



bool letter_check(char c_b)
{
    //Checks if the Char is a letter with ASCII
    if ((65 <= c_b && c_b <= 90) || (97 <= c_b && c_b <= 122))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool space_check(char c_b)
{
    //checks if the char is a space with ASCII
    if (c_b == 32)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool ponct_check(char c_b)
{
    // Check if the Char is a ponctuation with ASCII
    if ((c_b == 33) || (c_b == 46) || (c_b == 63))
    {
        return true;
    }
    else
    {
        return false;
    }
}


