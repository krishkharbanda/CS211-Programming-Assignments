#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define RESTOCK "RESTOCK"
#define SHOW_STOCK "SHOW_STOCK"
#define SALE "SALE"

// Author: @nate-blum

typedef struct HashNode {
    int count;
    char* album_name;
    struct HashNode* next;
} HashNode;

static HashNode** table = NULL;
static int table_fullness = 0, table_size = 4;

int hash(char* str) {
    unsigned long long hash = 0;
    int len = (int) strlen(str);
    for (int i = 0; i < len; i++)
        hash += str[i] * pow(31, len - (i + 1));
    return hash % table_size;
}

void pretty_print_table() {
    printf("-------- FINAL COUNTS --------\n");
    for (int i = 0; i < table_size; i++) {
        HashNode* temp = table[i];
        while (temp) {
            printf("%s: %d\n", temp->album_name, temp->count);
            temp = temp->next;
        }
    }
    printf("------------------------------\n");
}

void resize() {
    table_size *= 2;
    HashNode** newTable = calloc(table_size, sizeof(HashNode*));
    for (int i = 0; i < table_size/2; i++) {
        HashNode* temp = table[i];
        while (temp) {
            int newHash = hash(temp->album_name);
            HashNode* newNode = temp;
            temp = temp->next;
            newNode->next = newTable[newHash];
            newTable[newHash] = newNode;
        }
    }
    free(table);
    table = newTable;
    printf("Resizing the table from %d to %d\n", table_size/2, table_size);
}

void update(char* album, int k) {
    int albumHash = hash(album);
    HashNode* temp = table[albumHash];
    while (temp) {
        if (strcmp(temp->album_name, album) == 0) {
            temp->count += k;
            return;
        }
        temp = temp->next;
    }
    HashNode* newNode = malloc(sizeof(HashNode));
    if (k > 0) {
        newNode->count = k;
        newNode->album_name = strdup(album);
        newNode->next = table[albumHash];
        table[albumHash] = newNode;
        table_fullness++;
        if (table_fullness >= (table_size / 2)) {
            resize();
        }
    }
}

int retrieve(char* album_name) {
    int albumHash = hash(album_name);
    HashNode* temp = table[albumHash];
    while (temp) {
        if (strcmp(temp->album_name, album_name) == 0) {
            return temp->count;
        }
        temp = temp->next;
    }
    return -1;
}

void free_table() {
    for (int i = 0; i < table_size; i++) {
        HashNode* temp = table[i];
        while (temp) {
            HashNode* next = temp->next;
            free(temp->album_name);
            free(temp);
            temp = next;
        }
    }
    free(table);
}

int main(int argc, char* argv[]) {
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

	if (!fscanf(fp, "%d\n", &table_size)) {
        perror("Reading the initial size of the table failed.\n");
        exit(EXIT_FAILURE);
    }

    table = calloc(table_size, sizeof(HashNode*));

    char command[20], album[150];
    int count;
    while (fscanf(fp, "%s %d %[^\n]s", command, &count, album) == 3) {
        if (strcmp(command, SALE) == 0) {
            int stock = retrieve(album);
            if (stock >= count) {
                update(album, -count);
            } else {
                if (stock > 0) {
                    printf("Not enough stock of %s\n", album);
                } else {
                    printf("No stock of %s\n", album);
                }
            }
        } else if (strcmp(command, RESTOCK) == 0) {
            update(album, count);
        } else if (strcmp(command, SHOW_STOCK) == 0) {
            int stock = retrieve(album);
            if (stock > 0) {
                printf("Current stock of %s: %d\n", album, stock);
            } else {
                printf("No stock of %s\n", album);
            }
        }
    }
    pretty_print_table();

    free_table();
    fclose(fp);

    return 0;
}

