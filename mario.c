#include <stdio.h>
#include <cs50.h>

int height(void);
void pyramid(int h);

int main(void)
{
    int h = height();
    pyramid(h);
}



int height(void)
{
    // Function to get a positive integer from the user between 1 and 8 inclusive
    int n;
    do
    {
        n = get_int("Height: "); 
    }
    while (n < 1 || n > 8);
    return n;
}

void pyramid(int h)
{
    // Functions that prints a pyramid depending on the height
    for (int i = 1; i <= h; i++)
    {
        for (int j = 0; j < (h + 2) + i; j++)
        {
            // ensure that the pyramid is being made depending on i and j
            if ((j >= (h - i) && j < h) ||  j > (h + 1))
            {
                printf("#");
            }
            //ensure that a middle row of gap two is placed
            else if (j == h || j == (h + 1))
            {
                printf(" "); 
            }
            //prints gap on each side of the pyramid to give its shape
            else
            {
                printf(" "); 
            }
        }
        printf("\n");
    }
}