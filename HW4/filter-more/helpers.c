#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE S = 0;           // variable sum
    BYTE intensity = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            intensity = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);

            image[i][j].rgbtBlue = intensity;
            image[i][j].rgbtGreen = intensity;
            image[i][j].rgbtRed = intensity;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half = round((float)(width) / 2);            // half is the half size of width
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            RGBTRIPLE tmp = image[i][j];        // array means the content of the pointer point to, so it include * itself, don't need to add here
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            image[i][j].rgbtBlue = round((float)(tmp[i-1][j-1].rgbtBlue + tmp[i-1][j].rgbtBlue + tmp[i-1][j+1].rgbtBlue +\
                                    tmp[i][j-1].rgbtBlue + tmp[i][j].rgbtBlue + tmp[i][j+1].rgbtBlue + tmp[i\
                                    +1][j-1].rgbtBlue + tmp[i+1][j].rgbtBlue + tmp[i+1][j+1].rgbtBlue) / 9);
            image[i][j].rgbtGreen = round((float)(tmp[i-1][j-1].rgbtGreen + tmp[i-1][j].rgbtGreen + tmp[i-1][j+1].rgbtGreen +\
                                    tmp[i][j-1].rgbtGreen + tmp[i][j].rgbtGreen + tmp[i][j+1].rgbtGreen + tmp[i\
                                    +1][j-1].rgbtGreen + tmp[i+1][j].rgbtGreen + tmp[i+1][j+1].rgbtGreen) / 9);
            image[i][j].rgbtRed = round((float)(tmp[i-1][j-1].rgbtRed + tmp[i-1][j].rgbtRed + tmp[i-1][j+1].rgbtRed +\
                                    tmp[i][j-1].rgbtRed + tmp[i][j].rgbtRed + tmp[i][j+1].rgbtRed + tmp[i\
                                    +1][j-1].rgbtRed + tmp[i+1][j].rgbtRed + tmp[i+1][j+1].rgbtRed) / 9);
        }
    }
    int i = 0;
    for (int j = 1; j < width - 1; j++)
    {
        image[i][j].rgbtBlue = round((float)(tmp[i][j-1].rgbtBlue + tmp[i][j].rgbtBlue + tmp[i][j+1].rgbtBlue \
                                    + tmp[i+1][j-1].rgbtBlue + tmp[i+1][j].rgbtBlue + tmp[i+1][j+1].rgbtBlue) / 6);
        image[i][j].rgbtGreen = round((float)(tmp[i][j-1].rgbtGreen + tmp[i][j].rgbtGreen + tmp[i][j+1].rgbtGreen \
                                    + tmp[i+1][j-1].rgbtGreen + tmp[i+1][j].rgbtGreen + tmp[i+1][j+1].rgbtGreen) / 6);
        image[i][j].rgbtRed = round((float)(tmp[i][j-1].rgbtRed + tmp[i][j].rgbtRed + tmp[i][j+1].rgbtRed \
                                    + tmp[i+1][j-1].rgbtRed + tmp[i+1][j].rgbtRed + tmp[i+1][j+1].rgbtRed) / 6);
    }
    i = height - 1;
    for (int j = 1; j < width - 1; j++)
    {
        image[i][j].rgbtBlue = round((float)(tmp[i][j-1].rgbtBlue + tmp[i][j].rgbtBlue + tmp[i][j+1].rgbtBlue \
                                    + tmp[i-1][j-1].rgbtBlue + tmp[i-1][j].rgbtBlue + tmp[i-1][j+1].rgbtBlue) / 6);
        image[i][j].rgbtGreen = round((float)(tmp[i][j-1].rgbtGreen + tmp[i][j].rgbtGreen + tmp[i][j+1].rgbtGreen \
                                    + tmp[i-1][j-1].rgbtGreen + tmp[i-1][j].rgbtGreen + tmp[i-1][j+1].rgbtGreen) / 6);
        image[i][j].rgbtRed = round((float)(tmp[i][j-1].rgbtRed + tmp[i][j].rgbtRed + tmp[i][j+1].rgbtRed \
                                    + tmp[i-1][j-1].rgbtRed + tmp[i-1][j].rgbtRed + tmp[i-1][j+1].rgbtRed) / 6);
    }
    int j = 0;
    for (i = 1; i < height - 1; i++)
    {
        image[i][j].rgbtBlue = round((float)(tmp[i-1][j].rgbtBlue + tmp[i-1][j+1].rgbtBlue \
                                    + tmp[i][j].rgbtBlue + tmp[i][j+1].rgbtBlue \
                                    + tmp[i+1][j].rgbtBlue + tmp[i+1][j+1].rgbtBlue) / 6);
        image[i][j].rgbtGreen = round((float)(tmp[i-1][j].rgbtGreen + tmp[i-1][j+1].rgbtGreen \
                                    + tmp[i][j].rgbtGreen + tmp[i][j+1].rgbtGreen \
                                    + tmp[i+1][j].rgbtGreen + tmp[i+1][j+1].rgbtGreen) / 6);
        image[i][j].rgbtRed = round((float)(tmp[i-1][j].rgbtRed + tmp[i-1][j+1].rgbtRed \
                                    + tmp[i][j].rgbtRed + tmp[i][j+1].rgbtRed \
                                    + tmp[i+1][j].rgbtRed + tmp[i+1][j+1].rgbtRed) / 6);
    }
    j = width - 1;
    for (i = 1; i < height - 1; i++)
    {
        image[i][j].rgbtBlue = round((float)(tmp[i-1][j].rgbtBlue + tmp[i-1][j-1].rgbtBlue \
                                    + tmp[i][j].rgbtBlue + tmp[i][j-1].rgbtBlue \
                                    + tmp[i+1][j].rgbtBlue + tmp[i+1][j-1].rgbtBlue) / 6);
        image[i][j].rgbtGreen = round((float)(tmp[i-1][j].rgbtGreen + tmp[i-1][j-1].rgbtGreen \
                                    + tmp[i][j].rgbtGreen + tmp[i][j-1].rgbtGreen \
                                    + tmp[i+1][j].rgbtGreen + tmp[i+1][j-1].rgbtGreen) / 6);
        image[i][j].rgbtRed = round((float)(tmp[i-1][j].rgbtRed + tmp[i-1][j-1].rgbtRed \
                                    + tmp[i][j].rgbtRed + tmp[i][j-1].rgbtRed \
                                    + tmp[i+1][j].rgbtRed + tmp[i+1][j-1].rgbtRed) / 6);
    }
    height--;
    width--;
    image[0][0].rgbtBlue = round((float)(tmp[0][0].rgbtBlue + tmp[0][1].rgbtBlue + tmp[1][0].rgbtBlue + tmp[1][1].rgbtBlue) / 4);
    image[height][0].rgbtBlue = round((float)(tmp[height][0].rgbtBlue + tmp[height][1].rgbtBlue + tmp[height - 1][0].rgbtBlue + tmp[height - 1][1].rgbtBlue) / 4);
    image[0][width].rgbtBlue = round((float)(tmp[0][width].rgbtBlue + tmp[0][width - 1].rgbtBlue + tmp[1][width].rgbtBlue + tmp[1][width - 1].rgbtBlue) / 4);
    image[height][width].rgbtBlue = round((float)(tmp[height][width].rgbtBlue + tmp[height][width - 1].rgbtBlue + tmp[height - 1][width].rgbtBlue + tmp[height - 1][width - 1].rgbtBlue) / 4);
    image[0][0].rgbtGreen = round((float)(tmp[0][0].rgbtGreen + tmp[0][1].rgbtGreen + tmp[1][0].rgbtGreen + tmp[1][1].rgbtGreen) / 4);
    image[height][0].rgbtGreen = round((float)(tmp[height][0].rgbtGreen + tmp[height][1].rgbtGreen + tmp[height - 1][0].rgbtGreen + tmp[height - 1][1].rgbtGreen) / 4);
    image[0][width].rgbtGreen = round((float)(tmp[0][width].rgbtGreen + tmp[0][width - 1].rgbtGreen + tmp[1][width].rgbtGreen + tmp[1][width - 1].rgbtGreen) / 4);
    image[height][width].rgbtGreen = round((float)(tmp[height][width].rgbtGreen + tmp[height][width - 1].rgbtGreen + tmp[height - 1][width].rgbtGreen + tmp[height - 1][width - 1].rgbtGreen) / 4);
    image[0][0].rgbtRed = round((float)(tmp[0][0].rgbtRed + tmp[0][1].rgbtRed + tmp[1][0].rgbtRed + tmp[1][1].rgbtRed) / 4);
    image[height][0].rgbtRed = round((float)(tmp[height][0].rgbtRed + tmp[height][1].rgbtRed + tmp[height - 1][0].rgbtRed + tmp[height - 1][1].rgbtRed) / 4);
    image[0][width].rgbtRed = round((float)(tmp[0][width].rgbtRed + tmp[0][width - 1].rgbtRed + tmp[1][width].rgbtRed + tmp[1][width - 1].rgbtRed) / 4);
    image[height][width].rgbtRed = round((float)(tmp[height][width].rgbtRed + tmp[height][width - 1].rgbtRed + tmp[height - 1][width].rgbtRed + tmp[height - 1][width - 1].rgbtRed) / 4);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height + 2][width + 2];
    int Gx = 0;
    int Gy = 0;
    int intensity = 0;
    for (int i = 0; i < height + 2; i++)                            // padding 0 around image as template
    {
        for (int j = 0; j < width + 2; j++)
        {
            if ((i == 0) | (i == height + 1) | (j == 0) | (j == width + 1))
            {
                tmp[i][j].rgbtBlue = 0;
                tmp[i][j].rgbtGreen = 0;
                tmp[i][j].rgbtRed = 0;
            }
            else
            {
                tmp[i][j] = image[i-1][j-1];
            }
        }
    }
    // do Sobel edging for respectly blue green and red
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // blue
            Gx = tmp[i][j + 2].rgbtBlue + 2 * tmp[i + 1][j + 2].rgbtBlue + tmp[i + 2][j + 2].rgbtBlue \
                - tmp[i][j].rgbtBlue - 2 * tmp[i + 1][j].rgbtBlue - tmp[i + 2][j].rgbtBlue;
            Gy = tmp[i + 2][j].rgbtBlue + 2 * tmp[i + 2][j + 1].rgbtBlue + tmp[i + 2][j + 2].rgbtBlue \
                - tmp[i][j].rgbtBlue - 2 * tmp[i][j + 1].rgbtBlue - tmp[i][j + 2].rgbtBlue;
            intensity = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));
            if (intensity > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = intensity;
            }
            // green
            Gx = tmp[i][j + 2].rgbtGreen + 2 * tmp[i + 1][j + 2].rgbtGreen + tmp[i + 2][j + 2].rgbtGreen \
                - tmp[i][j].rgbtGreen - 2 * tmp[i + 1][j].rgbtGreen - tmp[i + 2][j].rgbtGreen;
            Gy = tmp[i + 2][j].rgbtGreen + 2 * tmp[i + 2][j + 1].rgbtGreen + tmp[i + 2][j + 2].rgbtGreen \
                - tmp[i][j].rgbtGreen - 2 * tmp[i][j + 1].rgbtGreen - tmp[i][j + 2].rgbtGreen;
            intensity = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));
            if (intensity > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = intensity;
            }
            // red
            Gx = tmp[i][j + 2].rgbtRed + 2 * tmp[i + 1][j + 2].rgbtRed + tmp[i + 2][j + 2].rgbtRed \
                - tmp[i][j].rgbtRed - 2 * tmp[i + 1][j].rgbtRed - tmp[i + 2][j].rgbtRed;
            Gy = tmp[i + 2][j].rgbtRed + 2 * tmp[i + 2][j + 1].rgbtRed + tmp[i + 2][j + 2].rgbtRed \
                - tmp[i][j].rgbtRed - 2 * tmp[i][j + 1].rgbtRed - tmp[i][j + 2].rgbtRed;
            intensity = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));
            if (intensity > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = intensity;
            }
        }
    }
    return;
}
