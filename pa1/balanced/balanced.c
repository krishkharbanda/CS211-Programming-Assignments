#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

struct element {
    char close;
    struct element* next;
};

struct element* push(struct element* stack, char close) {
    struct element* new_element = malloc(sizeof(struct element));
    new_element->close = close;
    new_element->next = stack;
    return new_element;
}

char pop(struct element** stack) {
    if (*stack != NULL) {
        struct element* top = *stack;
        char close = top->close;
        *stack = top->next;
        free(top);
        return close;
    } else {
        return '\0';
    }
}

char get_matching_close(char open) {
    switch (open) {
        case '<': return '>';
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        default: return '\0';
    }
}

int main(int argc, char* argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    struct element* stack = NULL;
    bool balanced = true;

    char buff;
    while (fscanf(fp, "%c", &buff) == 1) {
        switch (buff) {
            case '<':
            case '(':
            case '[':
            case '{':
                stack = push(stack, get_matching_close(buff));
                break;
            case '>':
            case ')':
            case ']':
            case '}':
                if (pop(&stack) != buff) {
                    balanced = false;
                    break;
                }
                break;
            default:
                printf("Invalid character\n");
        }
    }
    balanced = balanced && stack == NULL;
    printf(balanced ? "yes" : "no");
    fclose(fp);
    return 0;
}
