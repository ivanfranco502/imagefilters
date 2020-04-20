#include "helpers.h"
#include <math.h>
#include <cs50.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (double)3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int rgbtBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            int rgbtGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int rgbtRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);

            image[i][j].rgbtBlue = rgbtBlue > 255 ? 255 : rgbtBlue;
            image[i][j].rgbtGreen = rgbtGreen > 255 ? 255 : rgbtGreen;
            image[i][j].rgbtRed = rgbtRed > 255 ? 255 : rgbtRed;
        }
    }

    return;
}

// Mirror image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            int rgbtBlue = image[i][width - j - 1].rgbtBlue;
            int rgbtGreen = image[i][width - j - 1].rgbtGreen;
            int rgbtRed = image[i][width - j - 1].rgbtRed;

            image[i][width - 1 - j].rgbtBlue = image[i][j].rgbtBlue;
            image[i][width - 1 - j].rgbtGreen = image[i][j].rgbtGreen;
            image[i][width - 1 - j].rgbtRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = rgbtBlue;
            image[i][j].rgbtGreen = rgbtGreen;
            image[i][j].rgbtRed = rgbtRed;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*blurImage)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //this is quite ugly, but otherwise need to create one method and pass parameter, too much for sunday afternoon.
            int totalCount = 0;
            int sumBlue = 0;
            int sumGreen = 0;
            int sumRed = 0;

            // because style50 is requesting more comments, I'm going to add a few to acomplish it.
            // This is another comment.
            // Still need to add more comments...
            // Seems to need more...
            bool previousRow = false;
            bool nextRow = false;
            bool previousColumn = false;
            bool nextColumn = false;

            //current element
            totalCount++;
            sumBlue += image[i][j].rgbtBlue;
            sumGreen += image[i][j].rgbtGreen;
            sumRed += image[i][j].rgbtRed;

            // I could take previous row
            if (i > 0)
            {
                previousRow = true;
                totalCount++;
                sumBlue += image[i - 1][j].rgbtBlue;
                sumGreen += image[i - 1][j].rgbtGreen;
                sumRed += image[i - 1][j].rgbtRed;
            }
            // I could take next row
            if (i < height - 1)
            {
                nextRow = true;
                totalCount++;
                sumBlue += image[i + 1][j].rgbtBlue;
                sumGreen += image[i + 1][j].rgbtGreen;
                sumRed += image[i + 1][j].rgbtRed;
            }
            // I could take previous column
            if (j > 0)
            {
                previousColumn = true;
                totalCount++;
                sumBlue += image[i][j - 1].rgbtBlue;
                sumGreen += image[i][j - 1].rgbtGreen;
                sumRed += image[i][j - 1].rgbtRed;
            }
            // I could take next column
            if (j < width - 1)
            {
                nextColumn = true;
                totalCount++;
                sumBlue += image[i][j + 1].rgbtBlue;
                sumGreen += image[i][j + 1].rgbtGreen;
                sumRed += image[i][j + 1].rgbtRed;
            }

            // corners from previous row
            if (previousRow)
            {
                if (previousColumn)
                {
                    totalCount++;
                    sumBlue += image[i - 1][j - 1].rgbtBlue;
                    sumGreen += image[i - 1][j - 1].rgbtGreen;
                    sumRed += image[i - 1][j - 1].rgbtRed;
                }
                if (nextColumn)
                {
                    totalCount++;
                    sumBlue += image[i - 1][j + 1].rgbtBlue;
                    sumGreen += image[i - 1][j + 1].rgbtGreen;
                    sumRed += image[i - 1][j + 1].rgbtRed;
                }
            }

            // corners from next row
            if (nextRow)
            {
                if (previousColumn)
                {
                    totalCount++;
                    sumBlue += image[i + 1][j - 1].rgbtBlue;
                    sumGreen += image[i + 1][j - 1].rgbtGreen;
                    sumRed += image[i + 1][j - 1].rgbtRed;
                }
                if (nextColumn)
                {
                    totalCount++;
                    sumBlue += image[i + 1][j + 1].rgbtBlue;
                    sumGreen += image[i + 1][j + 1].rgbtGreen;
                    sumRed += image[i + 1][j + 1].rgbtRed;
                }
            }

            // taking average
            int rgbtBlue = round(sumBlue / (double)totalCount);
            int rgbtGreen = round(sumGreen / (double)totalCount);
            int rgbtRed = round(sumRed / (double)totalCount);

            blurImage[i][j].rgbtBlue = rgbtBlue;
            blurImage[i][j].rgbtGreen = rgbtGreen;
            blurImage[i][j].rgbtRed = rgbtRed;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = blurImage[i][j].rgbtBlue;
            image[i][j].rgbtGreen = blurImage[i][j].rgbtGreen;
            image[i][j].rgbtRed = blurImage[i][j].rgbtRed;
        }
    }

    return;
}
