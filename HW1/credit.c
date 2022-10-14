#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long creddit_num = get_long("Number: ");
    long tmp = creddit_num;
    int num = 0;
    int digit = 0;
    int x = 0;
    while (tmp > 0)
    {
        digit++;
        if ((digit % 2) == 0)
        {
            x = ((tmp % 10) * 2);
        }
        else
        {
            x = tmp % 10;
        }
        if (x > 9)
        {
            x = (x % 10) + (x / 10);
        }
        num += x;
        tmp = tmp / 10;
    }
    if ((num % 10) == 0)
    {
        if ((digit == 15) && (((creddit_num / 10000000000000) == 34) || ((creddit_num / 10000000000000) == 37)))
        {
            printf("AMEX\n");
        }
        else if ((digit == 16) && (((creddit_num / 100000000000000) == 51) || ((creddit_num / 100000000000000) == 52)\
                                   || ((creddit_num / 100000000000000) == 53) || ((creddit_num / 100000000000000) == 54) || ((creddit_num / 100000000000000) == 55)))
        {
            printf("MASTERCARD\n");
        }
        else if (((digit == 16) && ((creddit_num / 1000000000000000) == 4)) || ((digit == 13) && ((creddit_num / 1000000000000) == 4)))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
