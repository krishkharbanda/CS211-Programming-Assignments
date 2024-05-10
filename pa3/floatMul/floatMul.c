#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

bool *binarySum(bool *array1, bool *array2)
{
    bool carry = false;
    bool *sum = calloc(8, sizeof(bool));
    for (int i = 7; i >= 0; i--)
    {
        if ((array1[i] || array2[i]) && !(array1[i] && array2[i]))
        {
            sum[i] = !carry;
        }
        else
        {
            sum[i] = carry;
            carry = array1[i] && array2[i];
        }
    }
    return sum;
}

bool *roundBinary(bool *array, int len)
{
    bool carry = true;
    for (int i = len; i >= 0; i--)
    {
        if (carry)
        {
            array[i] = !array[i];
            carry = !array[i];
        }
    }
    return array;
}

int main(int argc, char *argv[])
{
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // read the binary number representation of multiplier
    char buff;
    bool multiplier[32];
    for (int i = 0; i < 32; i++) {
        fscanf(fp, "%c", &buff);
        multiplier[i] = buff - 48;
    }

    fscanf(fp, "%c", &buff);

    // read the binary number representation of multiplcand
    bool multiplicand[32];
    for (int i = 0; i < 32; i++) {
        fscanf(fp, "%c", &buff);
        multiplicand[i] = buff - 48;
    }
    // float product = *(float *) &multiplier * *(float *) &multiplicand; // you are not allowed to print from this.
    // unsigned int ref_bits = *(unsigned int *) &product; // you are not allowed to print from this. But you can use it to validate your solution.

    // SIGN
    bool sign = multiplier[0] != multiplicand[0];
    printf("%d_", sign);
    // assert (sign == (1&ref_bits>>(EXP_SZ+FRAC_SZ)));

    // EXP
    // get the exp field of the multiplier and multiplicand
    bool multiplierExpField[8];
    bool multpilicandExpField[8];
    for (int i = 0; i < 8; i++)
    {
        multiplierExpField[i] = multiplier[i + 1];
        multpilicandExpField[i] = multiplicand[i + 1];
    }

    // add the two exp together
    bool *expSum = binarySum(multiplierExpField, multpilicandExpField);

    // subtract bias
    bool *bias = calloc(8, sizeof(bool));
    bias[0] = true;
    bias = roundBinary(bias, 7);
    bool *exp = binarySum(expSum, bias);
    free(expSum);
    free(bias);

    // FRAC
    // get the frac field of the multiplier and multiplicand
    long int mantissaMultiplier = 0;
    long int mantissaMultiplicand = 0;
    int count = 0;
    for (int i = 31; i > 8; i--)
    {
        mantissaMultiplier += multiplier[i] * ldexp(1, count);
        mantissaMultiplicand += multiplicand[i] * ldexp(1, count);
        count++;
    }
    // assuming that the input numbers are normalized floating point numbers, add back the implied leading 1 in the mantissa
    mantissaMultiplier += ldexp(1, count);
    mantissaMultiplicand += ldexp(1, count);
    // multiply the mantissas
    long int mantissaProduct = mantissaMultiplier * mantissaMultiplicand;

    // overflow and normalize
    bool mantissaProductBinary[64];
    for (int i = 63; i >= 0; i--)
    {
        mantissaProductBinary[i] = mantissaProduct & 1;
        mantissaProduct = mantissaProduct >> 1;
    }
    
    bool mantissa[24];
    count = 0;
    while (!mantissaProductBinary[count]) {
        count++;
    }
    if (count < 17) {
        exp = roundBinary(exp, 7);
    }
    // print exp
    for (int i = 0; i < 8; i++) {
        printf("%d", exp[i]);
    }
    printf("_");
    free(exp);
    int index = count;
    count = 0;
    for (int i = 0; i < 24; i++)
    {
        mantissa[count] = mantissaProductBinary[index + i];
        count++;
    }

    // rounding
    bool *frac = mantissa;
    if (mantissaProductBinary[index + 24])
    {
        frac = roundBinary(mantissa, 23);
    }
    // PRINTING
    // print frac
    for (int i = 1; i < 24; i++) {
        printf("%d", frac[i]);
    }
    return (EXIT_SUCCESS);
}