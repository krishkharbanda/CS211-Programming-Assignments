#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

size_t anyToInteger(char* source, int base, int digitCount) {
    size_t repr = 0;

    for (int i = 0; i < digitCount; i++) {
        char digit = source[i];
        int value = 0;

        if (digit >= '0' && digit <= '9') {
            value = digit - '0';
        } else if (digit >= 'A' && digit <= 'Z') {
            value = digit - 'A' + 10;
        } else {
            exit(EXIT_FAILURE);
        }

        repr *= base; 
        repr += value;
    }

    return repr;
}

void integerToAny(char* result, size_t repr, int base) {
    int i = 0;
    while (repr > 0) {
        int digit = repr % base;
        result[i++] = (digit < 10) ? '0' + digit : 'A' + digit - 10;
        repr /= base;
    }
    for (int j = 0; j < i / 2; j++) {
        char temp = result[j];
        result[j] = result[i - j - 1];
        result[i - j - 1] = temp;
    }
    result[i] = '\0';
}

int main(int argc, char* argv[]) {
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    int digitCount;
    int sourceBase;
    int destBase;

    if (!fscanf(fp, "%d\n", &digitCount)) {
      perror("reading the input digit count failed");
      exit(EXIT_FAILURE);
    }

    if (!fscanf(fp, "%d\n", &sourceBase)) {
      perror("reading the source base failed");
      exit(EXIT_FAILURE);
    }

    if (!fscanf(fp, "%d\n", &destBase)) {
      perror("reading the destination base failed");
      exit(EXIT_FAILURE);
    }

    char* sourceNum = calloc(sizeof(char), digitCount+1);

    if (!fscanf(fp, "%s\n", sourceNum)) {
      perror("reading the source number");
      exit(EXIT_FAILURE);
    }

    size_t repr;

    repr = anyToInteger(sourceNum, sourceBase, digitCount);

    char* result = (char*)calloc(sizeof(char), 65);

    integerToAny(result, repr, destBase);

    printf("%s", result);

    free(sourceNum);
    free(result);

    return 0;
}
