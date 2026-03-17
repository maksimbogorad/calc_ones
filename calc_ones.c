#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define MAX_SIZE 65536

enum
{
    countNormal = 0,
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

int countOnesInFile(FILE * from, unsigned char * amountOnes, uint64_t * result)
{
    if (from == NULL || amountOnes == NULL || result == NULL)
        return nullError;

    uint64_t totalAmountOnesBits = 0;
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

    *result = totalAmountOnesBits;
    return countNormal;
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
    uint64_t result;
    int status;
    bitCountFunc(amountOnes);

    status = countOnesInFile(from, amountOnes, &result);
    fclose(from);

    if (status == nullError)
    {
        fprintf(stderr, "NULL error\n");
        return 3;
    }

    if (status == readError)
    {
        fprintf(stderr, "Read from file error\n");
        return 4;
    }

    printf("File %s contains %" PRIu64 " ones\n", argv[1], result);
    
    return 0;
}
