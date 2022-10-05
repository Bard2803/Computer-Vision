#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over height and width to get to each pixel separately
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // set blue channel to the average of all three and then just assign this value to the green and red channel.
            image[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtGreen = image[i][j].rgbtRed  = image[i][j].rgbtBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over height and width to get to each pixel separately
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // create temporary copy of struct
            RGBTRIPLE tmp;
            // get the address of each pixel in the struct from the left side of image
            RGBTRIPLE *left = &image[i][j];
            // get the address of each pixel in the struct from the right side of image
            RGBTRIPLE *right = &image[i][width - 1 - j];
            // swap the left and right pixel location in memory.
            tmp = *left;
            *left = *right;
            *right = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a copy of image to take the pixels for blurring and not to take already blurred ones
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            image_copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            image_copy[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }
    //loop over height and width to get to each pixel separately
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float counter = 0;
            int kernel_blue = 0;
            int kernel_green = 0;
            int kernel_red = 0;
            for (int k = -1; k < 2; k++)
            {
                // loop in the kernel
                for (int l = -1; l < 2; l++)
                {
                    // check if the particular pixel in the kernel is not out of frame
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        kernel_blue += image_copy[i + k][j + l].rgbtBlue;
                        kernel_green += image_copy[i + k][j + l].rgbtGreen;
                        kernel_red += image_copy[i + k][j + l].rgbtRed;
                        counter++;
                    }
                }
            }
            image[i][j].rgbtBlue = round(kernel_blue / counter);
            image[i][j].rgbtGreen = round(kernel_green / counter);
            image[i][j].rgbtRed = round(kernel_red / counter);
        }
    }
    // // out of curiousity print the addresses of image and image_copy arrays
    // RGBTRIPLE *pointer_image = &image[0][0];
    // RGBTRIPLE *pointer_image_copy = &image_copy[0][0];
    // printf("Image pointer: %p, image_copy pointer: %p\n", pointer_image, pointer_image_copy);

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create Gx and Gy kernels
    int Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    // Create a copy of image to take the pixels for blurring and not to take already blurred ones
    RGBTRIPLE image_copy[height][width];
    //Insert values to the copy of image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            image_copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            image_copy[i][j].rgbtRed = image[i][j].rgbtRed;

        }
    }
    //loop over height and width to get to each pixel separately
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int kernel_blue_x = 0;
            int kernel_green_x = 0;
            int kernel_red_x = 0;
            int kernel_blue_y = 0;
            int kernel_green_y = 0;
            int kernel_red_y = 0;
            for (int k = -1; k < 2; k++)
            {
                // loop in the kernel
                for (int l = -1; l < 2; l++)
                {
                    // check if the particular pixel in the kernel is not out of frame
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        kernel_blue_x += image_copy[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];
                        kernel_green_x += image_copy[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                        kernel_red_x += image_copy[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                        kernel_blue_y += image_copy[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                        kernel_green_y += image_copy[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                        kernel_red_y += image_copy[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                    }
                }
            }
            float b = round(sqrt(pow(kernel_blue_x, 2) + pow(kernel_blue_y, 2)));
            if (b > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = b;
            }
            float g = round(sqrt(pow(kernel_green_x, 2) + pow(kernel_green_y, 2)));
            if (g > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = g;
            }
            float r = round(sqrt(pow(kernel_red_x, 2) + pow(kernel_red_y, 2)));
            if (r > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = r;
            }

        }
    }
    return;
}
