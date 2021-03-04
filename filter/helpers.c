#include "helpers.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // Assign variables to image RGB values for readability
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            
            // Average RGB values
            int average = round((red + green + blue) / 3.0);
            
            // Assign average RGB values to each element of the RGBTRIPLE
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through half the pixels in image and swap them with mirrored pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_pixel;
    
    // Create a copy of image array for comparison
    RGBTRIPLE copy[height][width];
    
    //copy each element of image into copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    // Check for cases where pixel is on the edge of image
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {

            // Assign variables for the RGB value counts
            int red_count = 0;
            int green_count = 0;
            int blue_count = 0;

            // create variable to keep track of how many times RGB value counts was added to
            int count = 0;

            int top = x - 1;
            int bottom = x + 1;
            int left = y - 1;
            int right = y + 1;

            // if pixel is at top edge
            if (x == 0)
            {
                top = x;
            }
            // if pixel is at bottom edge
            else if (x == height - 1)
            {
                bottom = x;
            }

            // if pixel is at left edge
            if (y == 0)
            {
                left = y;
            }
            // if pixel is at right edge
            else if (y == width - 1)
            {
                right = y;
            }

            // add all adjacent RGB values to pixel
            for (int i = top; i <= bottom; i++)
            {
                for (int j = left; j <= right; j++)
                {
                    count++;
                    red_count += copy[i][j].rgbtRed;
                    green_count += copy[i][j].rgbtGreen;
                    blue_count += copy[i][j].rgbtBlue;
                }
            }

            // divide by number of RGB values added to find average values
            new_pixel.rgbtRed = round((float)red_count / count);
            new_pixel.rgbtGreen = round((float)green_count / count);
            new_pixel.rgbtBlue = round((float)blue_count / count);

            image[x][y] = new_pixel;

        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create Gx and Gy arrays in order to be able to apply the Sobel operator to pixels
    int Gx_kernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy_kernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    // Create a copy of image array for comparison
    RGBTRIPLE copy[height][width];

    //copy each element of image into copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }


    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            int top_pixel = x - 1;
            int top_Gxy = 0;

            int bottom = x + 1;

            int left_pixel = y - 1;
            int left_Gxy = 0;

            int right = y + 1;

            // if pixel is at top edge
            if (x == 0)
            {
                top_pixel = x;
                top_Gxy = 1;
            }
            // if pixel is at bottom edge
            else if (x == height - 1)
            {
                bottom = x;
            }

            // if pixel is at left edge
            if (y == 0)
            {
                left_pixel = y;
                left_Gxy = 1;
            }
            // if pixel is at right edge
            else if (y == width - 1)
            {
                right = y;
            }

            int Gx_red = 0;
            int Gx_green = 0;
            int Gx_blue = 0;

            int Gy_red = 0;
            int Gy_green = 0;
            int Gy_blue = 0;
            
            // Loops through the 3x3 block of pixels surrounding pixel in question and multiplies them by the corresponding Gx and Gy blocks
            for (int r = top_pixel, i = top_Gxy; r <= bottom; r++, i++)
            {
                for (int c = left_pixel, j = left_Gxy; c <= right; c++, j++)
                {
                    Gx_red += copy[r][c].rgbtRed * Gx_kernel[i][j];
                    Gx_green += copy[r][c].rgbtGreen * Gx_kernel[i][j];
                    Gx_blue += copy[r][c].rgbtBlue * Gx_kernel[i][j];

                    Gy_red += copy[r][c].rgbtRed * Gy_kernel[i][j];
                    Gy_green += copy[r][c].rgbtGreen * Gy_kernel[i][j];
                    Gy_blue += copy[r][c].rgbtBlue * Gy_kernel[i][j];

                }
            }
            
            // Square values found from loop
            int Gx_red_sqr = pow(Gx_red, 2);
            int Gx_green_sqr = pow(Gx_green, 2);
            int Gx_blue_sqr = pow(Gx_blue, 2);


            int Gy_red_sqr = pow(Gy_red, 2);
            int Gy_green_sqr = pow(Gy_green, 2);
            int Gy_blue_sqr = pow(Gy_blue, 2);
            
            // square root the sum of squared values and round to nearest whole number
            int new_red = round(sqrt(Gx_red_sqr + Gy_red_sqr));
            int new_green = round(sqrt(Gx_green_sqr + Gy_green_sqr));
            int new_blue = round(sqrt(Gx_blue_sqr + Gy_blue_sqr));

            // Apply conditions to ensure RGB values stay within range
            if (new_red > 255)
            {
                new_red = 255;
            }

            if (new_green > 255)
            {
                new_green = 255;
            }

            if (new_blue > 255)
            {
                new_blue = 255;
            }
            
            // Replace each pixel in image with corresponding new calculated value
            image[x][y].rgbtRed = new_red;
            image[x][y].rgbtGreen = new_green;
            image[x][y].rgbtBlue = new_blue;
        }
    }
}
