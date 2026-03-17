#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define MAX_SIZE 8192

enum
{
    countNormal = 0,
    nullError = -1,
    readError = -2
};

void bitCountFunc16(unsigned char * amountOnes16)
{
    if (amountOnes16 == NULL)
    {
        fprintf(stderr, "Array amountOnes16 is NULL\n");
        return;
    }

    uint32_t i;

    for (i = 0; i < 65536; i++)
    {
        unsigned char count = 0;
        uint16_t tmp = (uint16_t)i;

        while (tmp)
        {
            count +=(unsigned char)(tmp & 1);
            tmp >>= 1;
        }

        amountOnes16[i] = count;
    }
}

int countOnesInFile(FILE * from, const unsigned char * amountOnes16, uint64_t * result)
{

    if (from == NULL || amountOnes16 == NULL || result == NULL)
        return nullError;

    uint64_t totalAmountOnesBits = 0;
    unsigned char buffer[MAX_SIZE];
    size_t read_info;

    while ((read_info = fread(buffer, 1, sizeof(buffer), from)) > 0)
    {
        size_t i = 0;

        while (i + 1 < read_info)
        {
            uint16_t value = ((uint16_t)buffer[i]<<8) | (uint16_t)buffer[i + 1];
            totalAmountOnesBits += amountOnes16[value];
            i += 2;
        }

        if (i < read_info)
            totalAmountOnesBits += amountOnes16[buffer[i]];
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

    unsigned char amountOnes16[65536];
    uint64_t result;
    int status;
    bitCountFunc16(amountOnes16);

    status = countOnesInFile(from, amountOnes16, &result);
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
