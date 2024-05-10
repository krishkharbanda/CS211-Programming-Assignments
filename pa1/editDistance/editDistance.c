#include <stdlib.h>
#include <stdio.h>
#include <string.h>

size_t min(size_t x, size_t y) {
    return x < y ? x : y;
}

size_t levenshtein_recursive(char *source, char *target) {
    size_t source_len = strlen(source);
    size_t target_len = strlen(target);

    size_t dp[source_len + 1][target_len + 1];

    for (size_t i = 0; i <= source_len; i++) {
        dp[i][0] = i;
    }
    for (size_t j = 0; j <= target_len; j++) {
        dp[0][j] = j;
    }

    for (size_t i = 1; i <= source_len; i++) {
        for (size_t j = 1; j <= target_len; j++) {
            if (source[i - 1] == target[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                size_t insert_cost = dp[i][j - 1] + 1;
                size_t delete_cost = dp[i - 1][j] + 1;
                size_t substitute_cost = dp[i - 1][j - 1] + 1;
                dp[i][j] = min(min(insert_cost, delete_cost), substitute_cost);
            }
        }
    }

    return dp[source_len][target_len];
}


int main(int argc, char* argv[])
{

    FILE* inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    char source[32];
    char target[32];

    fscanf (inputFile, "%s\n%s", source, target);

    printf("%ld\n", levenshtein_recursive ( source, target ));

    return EXIT_SUCCESS;

}