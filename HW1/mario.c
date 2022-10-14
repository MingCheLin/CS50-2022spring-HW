#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int stairs;
    do
    {
        stairs = get_int("Height: ");
    }
    while ((stairs < 1) || (stairs > 8));

    for (int i = 0; i < stairs; i++)
    {
        for (int j = 0; j < stairs; j++)
        {
            if (j < (stairs - i - 1))
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("  ");
        for (int j = 0; j < stairs; j++)
        {
            if (j <= i)
            {
                printf("#");
            }
        }
        printf("\n");
    }
}
