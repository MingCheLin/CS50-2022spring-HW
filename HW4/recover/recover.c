#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check the argc
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    char *infile = argv[1];
    FILE *file = fopen(infile, "r");        // open input file and check if it could open
    if (file == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }
    // variables initialize
    int num = 0;
    char outfilename[8];
    BYTE buffer[512];
    // open output file and name it
    sprintf(outfilename, "%03i.jpg", num);
    FILE *img = fopen(outfilename, "w");
    // seeking for first JPEG
    while (fread(buffer, sizeof(BYTE), 512, file) == 512)
    {
        if ((buffer[0] == 0xff) & (buffer[1] == 0xd8) & (buffer[2] == 0xff) & ((buffer[3] & 0xf0) == 0xe0))
        {
            fwrite(buffer, sizeof(BYTE), 512, img);
            break;
        }
    }
    // write JPEG and seeking for more JPEG
    while (fread(buffer, sizeof(BYTE), 512, file) == 512)           
    {
        if ((buffer[0] == 0xff) & (buffer[1] == 0xd8) & (buffer[2] == 0xff) & ((buffer[3] & 0xf0) == 0xe0))
        {
            num++;
            fclose(img);
            sprintf(outfilename, "%03i.jpg", num);
            img = fopen(outfilename, "w");
            fwrite(buffer, sizeof(BYTE), 512, img);
        }
        else
        {
            fwrite(buffer, sizeof(BYTE), 512, img);
        }
    }
    fclose(file);
    fclose(img);
}
