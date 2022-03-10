#include "helpers.h"
#include <math.h>

void swap(RGBTRIPLE *j, RGBTRIPLE *n);
int maximum(int n);

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
            for (int h  = -1; h < 2; h++)
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary array to save G target
    BYTE gt_red[height][width], gt_green[height][width], gt_blue[height][width];

    // Set target as image[i][j]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Float for proper rounding and to reset for new targets
            float gx_red = 0, gx_blue = 0, gx_green = 0, gy_red = 0, gy_blue = 0, gy_green = 0;

            // Internal loop to search for the pixels directly around the target
            for (int h  = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    // Set parameter to only consider pixels within the frame
                    if ((i + h) < 0 || (j + w) < 0 || (i + h) > (height - 1) || (j + w) > (width - 1))
                    {
                        continue;
                    }
                    // Considering the middle horizontal line
                    else if (h == 0)
                    {
                        gx_red += image[i + h][j + w].rgbtRed * 2 * w;
                        gx_green += image[i + h][j + w].rgbtGreen * 2 * w;
                        gx_blue += image[i + h][j + w].rgbtBlue * 2 * w;

                        gy_red += image[i + h][j + w].rgbtRed * h;
                        gy_green += image[i + h][j + w].rgbtGreen * h;
                        gy_blue += image[i + h][j + w].rgbtBlue * h;
                    }
                    // Considering the middle vertical line
                    else if (w == 0)
                    {
                        gx_red += image[i + h][j + w].rgbtRed * w;
                        gx_green += image[i + h][j + w].rgbtGreen * w;
                        gx_blue += image[i + h][j + w].rgbtBlue * w;

                        gy_red += image[i + h][j + w].rgbtRed * 2 * h;
                        gy_green += image[i + h][j + w].rgbtGreen * 2 * h;
                        gy_blue += image[i + h][j + w].rgbtBlue * 2 * h;
                    }
                    // Everything else in the 9 x 9 box
                    else
                    {
                        gx_red += image[i + h][j + w].rgbtRed * w;
                        gx_green += image[i + h][j + w].rgbtGreen * w;
                        gx_blue += image[i + h][j + w].rgbtBlue * w;

                        gy_red += image[i + h][j + w].rgbtRed * h;
                        gy_green += image[i + h][j + w].rgbtGreen * h;
                        gy_blue += image[i + h][j + w].rgbtBlue * h;
                    }
                }
            }

            // Save G target to temporary array
            // Using formula for SQRT(Gx^2 + Gy^2), subsequntly rounding and setting maximum
            gt_red[i][j] = maximum(round(sqrtf(powf(gx_red, 2) + pow(gy_red, 2))));
            gt_green[i][j] = maximum(round(sqrtf(powf(gx_green, 2) + pow(gy_green, 2))));
            gt_blue[i][j] = maximum(round(sqrtf(powf(gx_blue, 2) + pow(gy_blue, 2))));
        }
    }


    // Transfer temporary array back to original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = gt_red[i][j];
            image[i][j].rgbtGreen = gt_green[i][j];
            image[i][j].rgbtBlue = gt_blue[i][j];
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