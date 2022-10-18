#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
void check_alp(char word, string valid);

int main(int argc, string argv[])
{

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        char valid[26] = "abcdefghijklmnopqrstuvwxyz";
        char key[26];
        string words = argv[1];
        for (int i = 0; i < 26; i++)
        {
            char word = words[i];
            key[i] = (char) tolower(word);
            check_alp(key[i], valid);
        }

        for (int i = 0; i < 26; i++)
        {
            if (valid[i] != '0')
            {
                printf("Key must contain 26 characters.\n");
                return 1;
            }
        }

        string plaintext = get_string("enter plaintext: ");
        int len = strlen(plaintext);
        char ciphertext[len];

        for (int i = 0; i < len; i++)
        {
            char word = plaintext[i];
            if isupper(word)
            {
                ciphertext[i] = (char) toupper(key[word - 65]);
            }
            else if islower(word)
            {
                ciphertext[i] = key[word - 97];
            }
            else
            {
                ciphertext[i] = word;
            }
        }
        ciphertext[len] = '\0';
        printf("plaintext:  %s\n", plaintext);
        printf("ciphertext: %s\n", ciphertext);
        return 0;
    }
}


void check_alp(char word, char valid[26])
{
    for (int i = 0; i < 26; i++)
    {
        if (word == valid[i])
        {
            valid[i] = '0';
        }
    }
}
