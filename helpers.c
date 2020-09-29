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
            
            for(int x = i - 1; x <= i + 1; x++)
            {
                for(int y = j - 1; y <= j + 1; y++)
                {
                    average.rgbtRed += image[x][y].rgbtRed;
                    average.rgbtGreen += image[x][y].rgbtGreen;
                    average.rgbtBlue += image[x][y].rgbtBlue;
                }
            }
            image_dump[i][j].rgbtRed = (int) round(average.rgbtRed / 9);
            image_dump[i][j].rgbtGreen = (int) round(average.rgbtGreen / 9);
            image_dump[i][j].rgbtBlue = (int) round(average.rgbtBlue / 9);
        }
    }
    for(int i = 1; i < height - 1; i++)
    {
        //LEFT COLUMN
        average.rgbtRed = 0;
        average.rgbtGreen = 0;
        average.rgbtBlue = 0;
        for(int x = i - 1; x <= i + 1; x++)
        {
            for(int y = 0; y <= 1; y++)
            {
                average.rgbtRed += image[x][y].rgbtRed;
                average.rgbtGreen += image[x][y].rgbtGreen;
                average.rgbtBlue += image[x][y].rgbtBlue;
            }
        }
        image_dump[i][0].rgbtRed = (int) round(average.rgbtRed / 6);
        image_dump[i][0].rgbtGreen = (int) round(average.rgbtGreen / 6);
        image_dump[i][0].rgbtBlue = (int) round(average.rgbtBlue / 6);
        
        //RIGHT COLUMN
        average.rgbtRed = 0;
        average.rgbtGreen = 0;
        average.rgbtBlue = 0;
        for(int x = i - 1; x <= i + 1; x++)
        {
            for(int y = width - 2; y <= width - 1; y++)
            {
                average.rgbtRed += image[x][y].rgbtRed;
                average.rgbtGreen += image[x][y].rgbtGreen;
                average.rgbtBlue += image[x][y].rgbtBlue;
            }
        }
        image_dump[i][width - 1].rgbtRed = (int) round(average.rgbtRed / 6);
        image_dump[i][width - 1].rgbtGreen = (int) round(average.rgbtGreen / 6);
        image_dump[i][width - 1].rgbtBlue = (int) round(average.rgbtBlue / 6);

    }
    
    for(int j = 1; j < width - 1; j++)
    {
        //top row
        average.rgbtRed = 0;
        average.rgbtGreen = 0;
        average.rgbtBlue = 0;
        for(int x = 0; x <= 1 ; x++)
        {
            for(int y = j - 1; y <= j + 1; y++)
            {
                average.rgbtRed += image[x][y].rgbtRed;
                average.rgbtGreen += image[x][y].rgbtGreen;
                average.rgbtBlue += image[x][y].rgbtBlue;
            }
        }
        image_dump[0][j].rgbtRed = (int) round(average.rgbtRed / 6);
        image_dump[0][j].rgbtGreen = (int) round(average.rgbtGreen / 6);
        image_dump[0][j].rgbtBlue = (int) round(average.rgbtBlue / 6);
        
        //bottom row
        average.rgbtRed = 0;
        average.rgbtGreen = 0;
        average.rgbtBlue = 0;
        for(int x = height - 2; x <= height - 1 ; x++)
        {
            for(int y = j - 1; y <= j + 1; y++)
            {
                average.rgbtRed += image[x][y].rgbtRed;
                average.rgbtGreen += image[x][y].rgbtGreen;
                average.rgbtBlue += image[x][y].rgbtBlue;
            }
        }
        image_dump[height - 1][j].rgbtRed = (int) round(average.rgbtRed / 6);
        image_dump[height - 1][j].rgbtGreen = (int) round(average.rgbtGreen / 6);
        image_dump[height - 1][j].rgbtBlue = (int) round(average.rgbtBlue / 6);

    }
    image_dump[0][0].rgbtRed = (int) round((image_dump[0][0].rgbtRed + image_dump[0][1].rgbtRed + image_dump[1][0].rgbtRed + image_dump[1][1].rgbtRed) / 4);
    image_dump[0][0].rgbtGreen = (int) round((image_dump[0][0].rgbtGreen + image_dump[0][1].rgbtGreen + image_dump[1][0].rgbtGreen + image_dump[1][1].rgbtGreen) / 4);
    image_dump[0][0].rgbtBlue = (int) round((image_dump[0][0].rgbtBlue + image_dump[0][1].rgbtBlue + image_dump[1][0].rgbtBlue + image_dump[1][1].rgbtBlue) / 4);
    
    image_dump[0][width - 1].rgbtRed = (int) round((image_dump[0][width - 1].rgbtRed + image_dump[0][width - 2].rgbtRed + image_dump[1][width - 2].rgbtRed + image_dump[1][width - 1].rgbtRed) / 4);
    image_dump[0][width - 1].rgbtGreen = (int) round((image_dump[0][width - 1].rgbtGreen + image_dump[0][width - 2].rgbtGreen + image_dump[1][width - 2].rgbtGreen + image_dump[1][width - 1].rgbtGreen) / 4);
    image_dump[0][width - 1].rgbtBlue = (int) round((image_dump[0][width - 1].rgbtBlue + image_dump[0][width - 2].rgbtBlue + image_dump[1][width - 2].rgbtBlue + image_dump[1][width - 1].rgbtBlue) / 4);
    
    image_dump[height - 1][0].rgbtRed = (int) round((image_dump[height - 1][0].rgbtRed + image_dump[height - 1][1].rgbtRed + image_dump[height - 2][0].rgbtRed + image_dump[height - 2][1].rgbtRed) / 4);
    image_dump[height - 1][0].rgbtGreen = (int) round((image_dump[height - 1][0].rgbtGreen + image_dump[height - 1][1].rgbtGreen + image_dump[height - 2][0].rgbtGreen + image_dump[height - 2][1].rgbtGreen) / 4);
    image_dump[height - 1][0].rgbtBlue = (int) round((image_dump[height - 1][0].rgbtBlue + image_dump[height - 1][1].rgbtBlue + image_dump[height - 2][0].rgbtBlue + image_dump[height - 2][1].rgbtBlue) / 4);
    
    image_dump[height - 1][width - 1].rgbtRed = (int) round((image_dump[height - 1][width - 1].rgbtRed + image_dump[height - 1][width - 2].rgbtRed + image_dump[height - 2][width - 1].rgbtRed + image_dump[height - 2][width - 2].rgbtRed) / 4);
    image_dump[height - 1][width - 1].rgbtGreen = (int) round((image_dump[height - 1][width - 1].rgbtGreen + image_dump[height - 1][width - 2].rgbtGreen + image_dump[height - 2][width - 1].rgbtGreen + image_dump[height - 2][width - 2].rgbtGreen) / 4);
    image_dump[height - 1][width - 1].rgbtBlue = (int) round((image_dump[height - 1][width - 1].rgbtBlue + image_dump[height - 1][width - 2].rgbtBlue + image_dump[height - 2][width - 1].rgbtBlue + image_dump[height - 2][width - 2].rgbtBlue) / 4);
    
    
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
