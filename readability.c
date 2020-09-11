#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

bool letter_check(char c_b);
bool space_check(char c_b);
bool ponct_check(char c_b);

int main(void)
{
    string s = get_string("Text: ");
    int pos_in_str = 0;
    int word_counter = 0;
    int sentence_counter = 0;
    int letter_counter = 0;
    for (int i = 0, lenght = strlen(s); i < lenght; i++)
    {
        if (letter_check(s[i]))
        {
            letter_counter++;
        }
        else if (ponct_check(s[i]))
        {
            word_counter++;
            sentence_counter++;
        }
        else if (space_check(s[i]) && !(ponct_check(s[i - 1])))
        {
            word_counter++;
        }
    }
    float L = ( (float) letter_counter * 100) / (float) word_counter;
    float S = ( (float) sentence_counter * 100) / (float) word_counter;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    index = round(index);
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
        printf("Grade %i", (int) index); 
    }
    
}



bool letter_check(char c_b)
{
    if ((65 <= c_b && c_b <= 90) || (97 <= c_b && c_b <= 122) )
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
    if ((c_b == 33) || (c_b == 46) || (c_b == 63 ))
    {
        return true;
    }
    else
    {
        return false;
    }
}


