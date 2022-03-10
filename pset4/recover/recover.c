#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");

    if (card == NULL)
    {
        printf("Unable to open %s.\n", argv[1]);
        return 1;
    }

    // Assign variables
    BYTE *raw = malloc(512);
    if (raw == NULL)
    {
        printf("Insufficient memory\n");
        return 1;
    }

    char filename[8];
    int fileno = 0;
    FILE *img;

    // Repeat as long as fread returns 512
    while (fread(raw, 1, 512, card) == 512)
    {
        // Check if the first 4 bytes of the file matches a JPEG signature
        if (raw[0] == 0xff && raw[1] == 0xd8 && raw[2] == 0xff && (raw[3] & 0xf0) == 0xe0)
        {
            // For every other file
            if (fileno > 0)
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", fileno);
                img = fopen(filename, "w");
                fileno++;
                if (img == NULL)
                {
                    printf("Unable to open %s.\n", filename);
                    return 1;
                }
            }
            // For first JPEG file 000.jpg
            else
            {
                sprintf(filename, "%03i.jpg", fileno);
                img = fopen(filename, "w");
                fileno++;
                if (img == NULL)
                {
                    printf("Unable to open %s\n", filename);
                    return 1;
                }
            }
        }
        // Write read data to opened file
        fwrite(raw, 1, 512, img);
    }
    // Close files and free malloc
    fclose(img);
    fclose(card);
    free(raw);
}