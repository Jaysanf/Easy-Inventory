#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //tests if the user put an argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    
    FILE *file = fopen(argv[1], "r");
    
    //Checks if file is existing
    if (file == NULL)
    {
        printf("Usage: ./recover image\n");
        return 1; 
    }
    
    //Buffers is an array where I store my data of 512 bytes
    unsigned char buffer[512];
    int jpeg_counter = 0;
    FILE *img = NULL;
    
    
    // loops though the memory card for each 512 bytes and write them
    while (!feof(file))
    {
        int size = fread(&buffer, 1, 512, file);
        //looks if the the first 4 bytes of buffer(512 bytes) is the header of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (img != NULL) 
            {
                fclose(img);
            }
            
            //creates jpeg and start writing
            char filename[10];
            sprintf(filename, "%03i.jpg", jpeg_counter);
            img = fopen(filename, "w");
                
            jpeg_counter ++;
        }
        
        //if not a header: keeps writing the bytes to last jpeg found
        if (img != NULL)
        {
            fwrite(&buffer, 1, size, img);
        }
            
    }
    fclose(img);
    
}
