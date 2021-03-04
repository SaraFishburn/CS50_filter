#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    // Ensure user has provided correct number of inputs
    if (argc != 2)
    {
        printf("Usage: ./recover <forensic image>\n");
        return 1;
    }

    // assign variable to file provided by user
    FILE *file = fopen(argv[1], "r");

    // Ensure file is readable
    if (file == NULL)
    {
        printf("Invalid file name, file could not be read.");
        return 1;
    }

    BYTE pointer[512];
    FILE *image = NULL;
    int num_jpg = 0;
    char file_name[8];


    size_t file_size = fread(pointer, sizeof(BYTE), 512, file);

    while (file_size == 512)
    {
        // Check if first 4 elements of block denote the beggining of a jpeg
        if (
            pointer[0] == 0xff &&
            pointer[1] == 0xd8 &&
            pointer[2] == 0xff &&
            pointer[3] >= 0xe0 &&
            pointer[3] <= 0xef
        )
        {
            // Check if image file is empty
            if (image != NULL)
            {
                fclose(image);
            }

            // Create a file name denoting current image number
            sprintf(file_name, "%03i.jpg", num_jpg);
            num_jpg++;
            image = fopen(file_name, "w");
        }

        if (image != NULL)
        {
            fwrite(pointer, sizeof(BYTE), file_size, image);
        }

        file_size = fread(pointer, sizeof(BYTE), 512, file);
    }

    fclose(image);
    fclose(file);
}


/////////////////////////////////////////////////////////////

LENGTH = 10

// Reading from position 0

abcd\n
efghi\n
jklmopq\n
rstuvwxyz\n

#include <stdint.h>

typedef uint8_t  BYTE;

BYTE buffer[LENGTH + 1]
size_t file_size;

do
{
    trie *table = my_trie;

    // Loading 10 characters
    file_size = fread(buffer, sizeof(BYTE), LENGTH + 1, file);

    // Reading from position 10

    // Looping through each character
    for(int i = 0; i < LENGTH + 1; i++)
    {
        int index;
        // If character in question is the end of the word
        if(buffer[i] == '\'')
        {
            index = 26;
        }
        else
        {
            index = buffer[i] - 'a';
        }

        if (table[index].next == NULL)
        {
            if (buffer[i + 1] == '\n')
            {
                table[index].is_word == true;

                // Moves reading to position 5 (start of the next word)
                fseek(file, (i + 1) - LENGTH, SEEK_CUR);
                break;
            }
            else
            {
                trie *next = malloc(sizeof(trie) * 27);
                table[index].next = next;
                table = table[index].next;
            }
        }
        else
        {
            if (buffer[i + 1] == '\n')
            {
                table[index].is_word == true;

                // Moves reading to position 5 (start of the next word)
                fseek(file, (i + 1) - LENGTH, SEEK_CUR);
                break;
            }

            table = table[index].next;
        }
    }
}
while(file_size == LENGTH + 1)
