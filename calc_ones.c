#include <stdio.h>
#include <stdint.h>

#define MAX_SIZE 65536

enum
{
    nullError = -1,
    readError = -2
};

void bitCountFunc(unsigned char * amountOnes)
{
    if (amountOnes == NULL)
    {
        fprintf(stderr, "Array amountOnes is NULL\n");
        return;
    }

    int i;

    for (i = 0; i < 256; i++)
    {
        unsigned char count = 0;
        unsigned char tmp = (unsigned char)i;

        while (tmp)
        {
            count +=(unsigned char)(tmp & 1);
            tmp >>= 1;
        }
        amountOnes[i] = count;
    }
}

int64_t countOnesInFile(FILE * from, unsigned char * amountOnes)
{
    if (from == NULL || amountOnes == NULL)
        return nullError;

    int64_t totalAmountOnesBits = 0;
    unsigned char buffer[MAX_SIZE];
    size_t read_info;

    while ((read_info = fread(buffer, 1, sizeof(buffer), from)) > 0)
    {
        size_t i = 0;

        while (i < read_info)
        {
            totalAmountOnesBits += amountOnes[buffer[i]];
            i++;
        }
    }

    if (ferror(from))
        return readError;

    return totalAmountOnesBits;

}


int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Amount of arguments should be 2\n");
        return 1;
    }

    FILE * from = fopen(argv[1], "rb");
    if (!from)
    {
        perror(argv[1]);
        return 2;
    }

    unsigned char amountOnes[256];
    bitCountFunc(amountOnes);

    int64_t result = countOnesInFile(from, amountOnes);
    fclose(from);
    if (result == nullError)
    {
        fprintf(stderr, "NULL error\n");
        return 3;
    }

    if (result == readError)
    {
        fprintf(stderr, "Read from file error\n");
        return 4;
    }

    printf("File %s contains %lld ones\n", argv[1], result);
    
    return 0;
}
