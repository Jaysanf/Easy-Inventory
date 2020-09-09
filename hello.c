#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Ask the user for his/her name
    string name = get_string("What is your name?\n") ;
    
    // Write in console hello + name provided
    printf("hello, %s\n", name);
}