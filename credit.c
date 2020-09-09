#include <stdio.h>
#include <cs50.h>

bool odd(int number);

int main(void)
{
    long card_nb = get_long("Number: ");
    long card_nb_dump = card_nb;
    int total_sum = 0;
    int last_number_1;
    int last_number_2;
    int lenght = 0;

    
    while (card_nb_dump)
    
    {
        // Calculate the length of the carn number and also Luhn's algorithm
        int nb_dump = card_nb_dump % 10;
        if (odd(lenght))
        {
            if (nb_dump * 2 < 10)
            {
                total_sum += nb_dump * 2;
            }
            else
            {
                total_sum += 1 + (nb_dump * 2) - 10;
            }
            last_number_1 = nb_dump;
        }
        else
        {
            total_sum += nb_dump;
            last_number_2 = nb_dump;
        }
        
        lenght++;
        card_nb_dump /= 10;
    }
    
    int first2_nb;
    
    // Finding the first numbers
    if (odd(lenght))
    {
        first2_nb = last_number_2 * 10 + last_number_1;
    }
    else
    {
        first2_nb = last_number_1 * 10 + last_number_2;
    }

    //Checking if card is valid
    if (total_sum % 10 == 0)
        //Luhn's algo check
    {
        if ((lenght == 15) && ((first2_nb == 34) || (first2_nb == 37)))
            //Check if AMEX
        {
            printf("AMEX\n");
        }
        else if ((lenght == 16) && ((first2_nb == 51) || (first2_nb == 52) || (first2_nb == 53) || (first2_nb == 54) || (first2_nb == 55)))
            //Check if MASTERCARD
        {
            printf("MASTERCARD\n");
        }
        else if (((lenght == 16) || (lenght == 13)) && (((first2_nb - (first2_nb % 10)) == 40)))
            //Check if VISA       
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");    
        }
    
    }
    else
    {
        printf("INVALID\n");
    }
}


bool odd(int number)
{
    // return 1 if a number is odd
    return number % 2 != 0;
}

 

