#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string input);
int count_words(string input);
int count_sentences(string input);

int main(void)
{
    string input = get_string("Text: ");
    float words = count_words(input);
    float letters = count_letters(input);
    float sentences = count_sentences(input);
    int grade = round((letters / words * 100 * 0.0588) - (sentences / words * 100 * 0.296) - 15.8);
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}
int count_letters(string input)
{
    int num = 0;
    for (int i = 0, len = strlen(input); i < len; i++)
    {
        if ((tolower(input[i]) >= 97) & (tolower(input[i]) <= 122))
        {
            num++;
        }
    }
    return num;
}
int count_words(string input)
{
    int num = 1;
    for (int i = 0, len = strlen(input); i < len; i++)
    {
        if (input[i] == 32)
        {
            num++;
        }
    }
    return num;
}
int count_sentences(string input)
{
    int num = 0;
    for (int i = 0, len = strlen(input); i < len; i++)
    {
        if ((input[i] == 33) | (input[i] == 46) | (input[i] == 63))
        {
            num++;
        }
    }
    return num;
}
