#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//Declare Globally the var n and var key , which is the len of the key and the key
int n;
string key;

string strlwr(string s);
bool alphabetic_check(string s);
bool repeat_letter_check(string s);
string encipher(string s);


int main(int argc, string argv[])
{
    if (argc != 2 )
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }
    
    //Get key and checks key
    key = argv[1];
    
    n = strlen(key);
    key = strlwr(key);

    if (alphabetic_check(key))
    {
        printf("Key must only contain alphabetic characters.\n");
        return 1;
    }
    
    else if (n != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    else if (repeat_letter_check(key))
    {
        printf("Key must not contain repeated characters.\n");
        return 1;        
    }
    
    //Get text and encipher it
    
    string text = get_string("plaintext: ");
    string text_enciphered = encipher(text);
    printf("ciphertext: %s\n", text_enciphered);
    return 0;
}











string strlwr(string s)
{
    //return a lowered version of the string
    for(int i = 0; i < n; i++)
    {
        s[i] = tolower(s[i]);
    }
    return s;
}

bool alphabetic_check(string s)
{
    //Check if the char in the string are all letters aka ASCII code of 97-122
    // returns false if all the char are letters
    for(int i = 0; i < n; i++)
    {
        if ( (97 >  s[i]) || (s[i] > 122))
        {
            return true;
        }
    }
        return false;   
}

bool repeat_letter_check(string s)
{
    //Check for each letter in the alphabet and if there is more than one, return true
    
    // Returns false if there is no letter repeated
   
    //initializing / declaring array of alphabet and pos
    int alphabet[26]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int pos;
    
    
    for(int i = 0; i < n; i++)
        {
            pos  = (int) s[i] - 97;
            if (alphabet[pos] == 0)
            {
                alphabet[pos] = 1;
            }
            else
            {
                return true;
            }
        }
    return false;
}

string encipher(string s)
{
    //Goes through a text and enciphers it using the key
    
    // declare local var
    int pos_key;
    
    for(int i = 0, l = (strlen(s)); i < l; i++ )
    {
        if ( (97 <=  s[i]) && (s[i] <= 122))
        {
            pos_key = (int) s[i] - 97;
            s[i] = key[pos_key];
        }
        
        else if ( (65 <=  s[i]) && (s[i] <= 90))
        {
            pos_key = (int) s[i] - 65;
            s[i] = ((int) key[pos_key]) - 32;
        }
        
    }
    return s;
}