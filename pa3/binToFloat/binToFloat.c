#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP

    // first, read the binary number representation of float point number
    char buff;
    unsigned int binary = 0;

    bool sign;
    int exponents[8];
    int mantissas[23];
    int eCount = 0;
    int mCount = 0;

    for (int i=EXP_SZ+FRAC_SZ; 0<=i; i--) { // read MSB first as that is what comes first in the file
        fscanf(fp, "%c", &buff);
        binary = buff == '1' ? 1 : 0;
        if (i == EXP_SZ+FRAC_SZ) {
            sign = binary;
        }
        else if (i > FRAC_SZ - 1) {
            exponents[eCount] = binary;
            eCount++;
        }
        else {
            mantissas[mCount] = binary;
            mCount++;
        }
    }
    // float number = *(float *)&binary; // you are not allowed to do this.

    /* ... */

    // E
    /* ... */
    int e = 0;
    for (int i = 7; i >= 0; i--) {
        e += ((exponents[i] == 1) ? 1 : 0) * pow(2, (7 - i));
    }
    e -= 127;
    // M
    /* ... */
    double m = 0;
    for (int i = 0; i < 23; i++) {
        m += ((mantissas[i] == 1) ? 1 : 0) * 1/(pow(2, (i+1)));
    }
    m += 1;
    // https://www.tutorialspoint.com/c_standard_library/c_function_ldexp.htm
    double number = ldexp ( m, e );
    number = sign ? -number : number;
    printf("%e\n", number);

    return EXIT_SUCCESS;

}