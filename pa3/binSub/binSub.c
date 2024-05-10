#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[]) {

  FILE* fp = fopen(argv[1], "r");
  if (!fp) {
    perror("fopen failed");
    return EXIT_FAILURE;
  }

  // SETUP

  // first, read the minuend (number to be subtracted from)
  char buff;
  bool minuend[CHAR_BIT]; // suggested that you store bits as array of bools; minuend[0] is the LSB
  for (int i = CHAR_BIT - 1; 0 <= i; i--) { // read MSB first as that is what comes first in the file
    fscanf(fp, "%c", &buff);
    minuend[i] = (buff == '1');
  }

  // notice that you are reading two different lines; caution with reading
  // second, read the subtrahend (number to subtract)
  bool subtrahend[CHAR_BIT]; // suggested that you store bits as array of bools; subtrahend[0] is the LSB
  fscanf(fp, "%c", &buff);
  for (int i = CHAR_BIT - 1; 0 <= i; i--) { // read MSB first as that is what comes first in the file
    fscanf(fp, "%c", &buff);
    subtrahend[i] = (buff == '1');
  }

  // WE WILL DO SUBTRACTION BY NEGATING THE SUBTRAHEND AND ADD THAT TO THE MINUEND

  // Negate the subtrahend
  // flip all bits
  for (int i = 0; i < CHAR_BIT; i++) {
    subtrahend[i] = !subtrahend[i];
  }

  bool difference[CHAR_BIT];
  // add one
  bool carry = true; // to implement the 'add one' logic, we do binary addition logic with carry set to true at the beginning
  for (int i=0; i<CHAR_BIT; i++) { // iterate from LSB to MSB
    difference[i] = minuend[i] ^ subtrahend[i] ^ carry;
    carry = (minuend[i] & subtrahend[i]) | (minuend[i] & carry) | (subtrahend[i] & carry);
  }
  // Add the minuend and the negated subtrahend (stored in minuend)

  // difference is already stored in minuend after adding negated subtrahend

  // print the difference bit string
  for (int i=CHAR_BIT-1; 0<=i; i--)
    printf("%d",difference[i]);

  return EXIT_SUCCESS;
}

