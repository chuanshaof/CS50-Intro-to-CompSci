#include "helpers.h"
#include <math.h>

int maximum(int n);
void swap(RGBTRIPLE *j, RGBTRIPLE *n);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Float and the integer to properly utilize round() function
            float total;
            int avg;

            total = image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen;
            avg = round(total / 3);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepia_green, sepia_red, sepia_blue;

            sepia_red = round((0.393 * image[i][j].rgbtRed) + (0.769 * image[i][j].rgbtGreen) + (0.189 * image[i][j].rgbtBlue));
            sepia_green = round((0.349 * image[i][j].rgbtRed) + (0.686 * image[i][j].rgbtGreen) + (0.168 * image[i][j].rgbtBlue));
            sepia_blue = round((0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue));

            image[i][j].rgbtRed = maximum(sepia_red);
            image[i][j].rgbtBlue = maximum(sepia_blue);;
            image[i][j].rgbtGreen = maximum(sepia_green);
        }
    }
}

// Set maximum to 255
int maximum(int n)
{
    if (n > 255)
    {
        return 255;
    }
    else
    {
        return n;
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            {
                swap(&image[i][j], &image[i][width-1-j]);
            }
        }
    }
}

// Swap two values around
void swap(RGBTRIPLE *j, RGBTRIPLE *n)
{
    RGBTRIPLE temp;

    temp = *j;
    *j = *n;
    *n = temp;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary array to save the blurred pixels
    BYTE blurred_red[height][width], blurred_green[height][width], blurred_blue[height][width];

    // Set target as image[i][j]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Float for proper rounding and to reset for new targets
            float total_red = 0, total_green = 0, total_blue = 0, count = 0;

            // Internal loop to search for the pixels directly around the target
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    // Set parameter to only consider pixels within the frame
                    if ((i + h) < 0 || (j + w) < 0 || (i + h) > (height - 1) || (j + w) > (width - 1))
                    {
                        continue;
                    }
                    else
                    {
                        total_red += image[i + h][j + w].rgbtRed;
                        total_green += image[i + h][j + w].rgbtGreen;
                        total_blue += image[i + h][j + w].rgbtBlue;
                        count++;
                    }
                }
            }

            // Save blurred pixel to temporary array
            blurred_red[i][j] = round(total_red / count);
            blurred_green[i][j] = round(total_green / count);
            blurred_blue[i][j] = round(total_blue / count);
        }
    }

    // Transfer blurred pixel back to original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = blurred_red[i][j];
            image[i][j].rgbtGreen = blurred_green[i][j];
            image[i][j].rgbtBlue = blurred_blue[i][j];
        }
    }
}