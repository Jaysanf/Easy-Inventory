#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <cs50.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average;
    //cycles trough the pixels 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            //makes an average of the rgb
            average = (int) round((float)(image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);
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
        for (int j = 0; j < half_width; j++)
        {
            //reverses the image horizontally by going trough the half right of the pixels
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
    int averagered;
    int averagegreen;
    int averageblue;
    int counter;
    RGBTRIPLE image_dump[height][width];
    
    //makes a dump 2darray of the pixels i can safely change
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_dump[i][j] = image[i][j];
        }
    }


    //goes trough each pixel
    for (int i = 0; i < height ; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            //reset average
            averagered = 0;
            averagegreen = 0;
            averageblue = 0;
            counter = 0;
            
            //goes trough the 3x3 box around a pixel
            for (int x = i - 1; x <= i + 1; x++)
            {
                for (int y = j - 1; y <= j + 1; y++)
                {
                    //checks if out of bound
                    if (x >= 0 && y >= 0 && x < height && y < width)
                    {
                        //add to the average
                        averagered += image_dump[x][y].rgbtRed;
                        averagegreen += image_dump[x][y].rgbtGreen;
                        averageblue += image_dump[x][y].rgbtBlue;
                        counter ++;
                    }

                }
            }
            //rendered the blur pixel to the image
            image[i][j].rgbtRed = (int) round((float)((float) averagered / counter));
            image[i][j].rgbtGreen = (int) round((float)((float) averagegreen / counter));
            image[i][j].rgbtBlue = (int) round((float)((float) averageblue / counter));
        }
    }

    return;
    

}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE image_dump[height][width];
    //makes a dump
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_dump[i][j] = image[i][j];
        }
    }
    //initialize
    int finalred;
    int finalgreen;
    int finalblue;
    
    int redsumgx;
    int greensumgx;
    int bluesumgx;
    
    int redsumgy;
    int greensumgy;
    int bluesumgy;
    
    int counterx_box;
    int countery_box;
    //goes through each pixel
    for (int i = 0; i < height ; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            redsumgx = 0;
            greensumgx = 0;
            bluesumgx = 0;
            
            redsumgy = 0;
            greensumgy = 0;
            bluesumgy = 0;
            
            counterx_box = 0;

            //goes trough the 3x3 box around a pixel
            for (int x = i - 1; x <= i + 1; x++)
            {
                
                countery_box = 0;
                for (int y = j - 1; y <= j + 1; y++)
                {
                    
                    //checks if out of bound
                    if (x >= 0 && y >= 0 && x < height && y < width)
                    {
                        //makes the sobel operator
                        redsumgx += (image_dump[x][y].rgbtRed * gx[counterx_box][countery_box]);
                        greensumgx += (image_dump[x][y].rgbtGreen * gx[counterx_box][countery_box]);
                        bluesumgx += (image_dump[x][y].rgbtBlue * gx[counterx_box][countery_box]);
                        
                        redsumgy += (image_dump[x][y].rgbtRed * gy[counterx_box][countery_box]);
                        greensumgy += (image_dump[x][y].rgbtGreen * gy[counterx_box][countery_box]);
                        bluesumgy += (image_dump[x][y].rgbtBlue * gy[counterx_box][countery_box]);
                    }
                    countery_box++;
                }
                
                
                counterx_box++;
            }
            finalred = (int) round((float) sqrt((redsumgx * redsumgx) + (redsumgy * redsumgy)));
            finalgreen = (int) round((float) sqrt((greensumgx * greensumgx) + (greensumgy * greensumgy)));
            finalblue = (int) round((float) sqrt((bluesumgx * bluesumgx) + (bluesumgy * bluesumgy)));
            
            if (finalred > 255)
            {
                finalred = 255;
            }
            
            if (finalgreen > 255)
            {
                finalgreen = 255;
            }
            
            if (finalblue > 255)
            {
                finalblue = 255;
            }
            
            
            image[i][j].rgbtRed = finalred;
            image[i][j].rgbtGreen = finalgreen;
            image[i][j].rgbtBlue = finalblue;
            

        }
    }

    
    
    return;
}
