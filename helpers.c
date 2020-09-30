#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <cs50.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            average = (int) round((float) (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    
    int half_width = (width - (width % 2)) / 2 ;
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half_width; j++ )
        {
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
         
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE average;
    int counter;
    RGBTRIPLE image_dump[height][width];
    for(int i = 1; i < height - 1; i++)
    {
        for(int j = 1; j < width - 1; j++)
        {
            average.rgbtRed = 0;
            average.rgbtGreen = 0;
            average.rgbtBlue = 0;
            counter = 0;
            
            for(int x = i - 1; x <= i + 1; x++)
            {
                for(int y = j - 1; y <= j + 1; y++)
                {
                    //checks if out of bound
                    if( x >= 0 && y >= 0 && x < height && y < width )
                    {
                        average.rgbtRed += image[x][y].rgbtRed;
                        average.rgbtGreen += image[x][y].rgbtGreen;
                        average.rgbtBlue += image[x][y].rgbtBlue;
                        counter ++;
                    }

                }
            }
            image_dump[i][j].rgbtRed = (int) round(average.rgbtRed / counter);
            image_dump[i][j].rgbtGreen = (int) round(average.rgbtGreen / counter);
            image_dump[i][j].rgbtBlue = (int) round(average.rgbtBlue / counter);
        }
    }

    
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j] = image_dump[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
