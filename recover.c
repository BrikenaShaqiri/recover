#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("Could not open file");
        return 2;
    }

    unsigned char buffer[512];
    int count_image = 0;
    FILE *output_file = NULL;
    char filename[8];

    while (fread(buffer, sizeof(char), 512, input_file))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (output_file != NULL)
            {
                fclose(output_file);
            }

            sprintf(filename, "%03i.jpg", count_image);
            output_file = fopen(filename, "wb");

            if (output_file == NULL)
            {
                fclose(input_file);
                fprintf(stderr, "Could not create %s.\n", filename);
                return 3;
            }

            count_image++;
        }

        if (output_file != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output_file);
        }
    }

    fclose(output_file);
    fclose(input_file);

    return 0;
}
